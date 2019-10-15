
#pragma once

#include "PhysicalDeviceVk.h"
#include "LogicalDevickVk.h"
#include <VMFoundation/memoryallocationtracker.h>

namespace vm
{
namespace fx
{
namespace vkwrapper
{
template <typename T>
void HashCombine( std::size_t &Seed, const T &Val )
{
	Seed ^= std::hash<T>()( Val ) + 0x9e3779b9 + ( Seed << 6 ) + ( Seed >> 2 );
}

template <typename FirstArgType, typename... RestArgsType>
void HashCombine( std::size_t &Seed, const FirstArgType &FirstArg, const RestArgsType &... RestArgs )
{
	HashCombine( Seed, FirstArg );
	HashCombine( Seed, RestArgs... );  // recursive call using pack expansion syntax
}

template <typename... ArgsType>
std::size_t ComputeHash( const ArgsType &... Args )
{
	std::size_t Seed = 0;
	HashCombine( Seed, Args... );
	return Seed;
}

class VulkanMemoryArena;

class VulkanMemoryHeap;

class MemoryAllocation : vm::NoCopy
{
public:
	MemoryAllocation() = default;
	MemoryAllocation( VulkanMemoryHeap *page,
					  VkDeviceSize size,
					  VkDeviceSize offset ) :
	  Heap( page ),
	  Size( size ),
	  Offset( offset ) {}

	MemoryAllocation( MemoryAllocation &&alloc ) noexcept :
	  Heap( alloc.Heap ),
	  Size( alloc.Size ),
	  Offset( alloc.Offset )
	{
		alloc.Heap = nullptr;
		alloc.Size = 0;
		alloc.Offset = 0;
	}

	MemoryAllocation &operator=( MemoryAllocation &&alloc ) noexcept
	{
		Heap = alloc.Heap;
		Size = alloc.Size;
		Offset = alloc.Offset;

		alloc.Heap = nullptr;
		alloc.Size = 0;
		alloc.Offset = 0;

		return *this;
	}

	VulkanMemoryHeap *GetHeap() { return Heap; }
	const VulkanMemoryHeap *GetHeap() const { return Heap; }

	~MemoryAllocation() = default;

	VulkanMemoryHeap *Heap = nullptr;
	VkDeviceSize Offset = 0;
	VkDeviceSize Size = 0;
};

class VulkanMemoryHeap : NoCopy
{
public:
    VulkanMemoryHeap( VulkanMemoryArena & memMgr,
					  VkDeviceSize size,
					  uint32_t memoryTypeIndex,
					  bool hostVisible );

	VulkanMemoryHeap( VulkanMemoryHeap &&heap ) noexcept :
	  m_memMgr( heap.m_memMgr ),
	  m_gpuMemory( std::move( heap.m_gpuMemory ) ),
	  m_tracker( std::move( heap.m_tracker ) ),
	  m_cpuMemory( heap.m_cpuMemory )
	{
		heap.m_cpuMemory = nullptr;
	}
//
    VulkanMemoryHeap &operator=( VulkanMemoryHeap && ) = delete;

	bool IsEmpty() const { return m_tracker.IsEmpty(); }
	bool IsFull() const { return m_tracker.IsFull(); }
	VkDeviceSize GetHeapSize() const
	{
		return m_tracker.GetMaxSize();
	}
	VkDeviceSize GetUsedSize() const
	{
		return m_tracker.GetUsedSize();
	}
	MemoryAllocation Allocate( VkDeviceSize size, VkDeviceSize alignment );
//
	VkDeviceMemory GetGPUMemory() const { return m_gpuMemory; }  // m_gpuMemory; }

	void *GetCPUMemory() const { return m_cpuMemory; }

	~VulkanMemoryHeap();

private:
	friend class MemoryAllocation;
	void Free( MemoryAllocation alloc );
	VulkanMemoryArena & m_memMgr;
	std::mutex m_mtx;
    ysl::MemoryAllocationTracker m_tracker;
	VkDeviceMemoryWrapper m_gpuMemory;
	void *m_cpuMemory = nullptr;
};

class VulkanMemoryArena
{
public:
	VulkanMemoryArena( const VkLogicalDeviceWrapper &logicalDevice,
					   const VkPhysicalDeviceWrapper &physicalDevice,
					   VkDeviceSize localPageSize,
					   VkDeviceSize hostVisiblePageSize,
					   VkDeviceSize localReserveSize,
					   VkDeviceSize hostVisibleReserveSize ) :
	  LogicalDevice( logicalDevice ),
	  PhysicalDevice( physicalDevice ),
	  m_deviceLocalHeapSize( localPageSize ),
	  m_hostVisibleVisibleSize( hostVisiblePageSize ),
	  m_deviceLocalReserveSize( localReserveSize ),
	  m_hostVisibleReserveSize( hostVisibleReserveSize )
	{
	}

	VulkanMemoryArena( VulkanMemoryArena &&rhs ) noexcept :
	  LogicalDevice( rhs.LogicalDevice ),
	  PhysicalDevice( rhs.PhysicalDevice ),
	  m_heaps( std::move( rhs.m_heaps ) ),

	  m_deviceLocalHeapSize( rhs.m_deviceLocalHeapSize ),
	  m_hostVisibleVisibleSize( rhs.m_hostVisibleVisibleSize ),
	  m_deviceLocalReserveSize( rhs.m_deviceLocalReserveSize ),
	  m_hostVisibleReserveSize( rhs.m_hostVisibleReserveSize ),

	  //m_CurrUsedSize      (rhs.m_CurrUsedSize),
	  m_peakUsedSize( rhs.m_peakUsedSize ),
	  m_currAllocatedSize( rhs.m_currAllocatedSize ),
	  m_peakAllocatedSize( rhs.m_peakAllocatedSize )
	{
		for ( size_t i = 0; i < m_curUsedSize.size(); ++i )
			m_curUsedSize[ i ].store( rhs.m_curUsedSize[ i ].load() );
	}

	~VulkanMemoryArena();

	MemoryAllocation Allocate( VkDeviceSize size, VkDeviceSize alignment, uint32_t memoryTypeIndex, bool hostVisible );
	MemoryAllocation Allocate( const VkMemoryRequirements &req, VkMemoryPropertyFlags flags );

private:
	friend class VulkanMemoryHeap;
	const VkLogicalDeviceWrapper &LogicalDevice;
	
	const VkPhysicalDeviceWrapper &PhysicalDevice;

	void OnFree( VkDeviceSize size, bool hostVisible );

	std::mutex m_heapMtx;

	struct MemoryHeapIndex
	{
		const uint32_t MemoryIndexType;
		const bool IsHostVisible;
		MemoryHeapIndex( uint32_t memIdxTy, bool visible ) :
		  MemoryIndexType( memIdxTy ),
		  IsHostVisible( visible ) {}

		bool operator==( const MemoryHeapIndex &idx ) const
		{
			return MemoryIndexType == idx.MemoryIndexType &&
				   IsHostVisible == idx.IsHostVisible;
		}

		struct Hasher
		{
			size_t operator()( const MemoryHeapIndex &idx ) const
			{
				return ComputeHash( idx.MemoryIndexType, idx.IsHostVisible );
			}
		};
	};

	std::unordered_multimap<MemoryHeapIndex,
							VulkanMemoryHeap,
							MemoryHeapIndex::Hasher>
	  m_heaps;

	const VkDeviceSize m_deviceLocalHeapSize;
	const VkDeviceSize m_hostVisibleVisibleSize;
	const VkDeviceSize m_deviceLocalReserveSize;
	const VkDeviceSize m_hostVisibleReserveSize;

	// 0 for device, 1 for host
	std::array<std::atomic_int64_t, 2> m_curUsedSize = {};
	std::array<VkDeviceSize, 2> m_peakUsedSize = {};
	std::array<VkDeviceSize, 2> m_currAllocatedSize = {};
	std::array<VkDeviceSize, 2> m_peakAllocatedSize = {};
};
}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm