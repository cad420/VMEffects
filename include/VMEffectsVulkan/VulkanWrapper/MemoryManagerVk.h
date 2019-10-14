
#pragma once

//#include "LogicalDevickVk.h"
#include "PhysicalDeviceVk.h"
#include <VMEffectsVulkan/VulkanWrapper/VulkanObjectWrapper.h>
namespace vm
{
namespace fx
{
namespace vkwrapper
{
class VulkanMemoryArena;

class VulkanMemoryHeap;

class MemoryAllocation : vm::NoCopy
{
public:
	MemoryAllocation() = default;
	MemoryAllocation( VulkanMemoryHeap *page,
						VkDeviceSize size,
						VkDeviceSize offset ) :
	  m_heap( page ),
	  m_size( size ),
	  m_offset( offset ) {}

	MemoryAllocation( MemoryAllocation &&alloc ) noexcept :
	  m_heap( alloc.m_heap ),
	  m_size( alloc.m_size ),
	  m_offset( alloc.m_offset )
	{
		alloc.m_heap = nullptr;
		alloc.m_size = 0;
		alloc.m_offset = 0;
	}

	MemoryAllocation &operator=( MemoryAllocation &&alloc ) noexcept
	{
		m_heap = alloc.m_heap;
		m_size = alloc.m_size;
		m_offset = alloc.m_offset;

		alloc.m_heap = nullptr;
		alloc.m_size = 0;
		alloc.m_offset = 0;

		return *this;
	}

	VulkanMemoryHeap * GetHeap() { return m_heap; }
	const VulkanMemoryHeap *GetHeap() const { return m_heap; }
	
	~MemoryAllocation() = default;

private:
	VulkanMemoryHeap *m_heap = nullptr;
	VkDeviceSize m_offset = 0;
	VkDeviceSize m_size = 0;
};

class VulkanMemoryHeap : NoCopy
{
public:
	VulkanMemoryHeap( VulkanMemoryArena &memMgr,
				VkDeviceSize size,
				uint32_t memoryTypeIndex,
				bool hostVisible ) :
	  m_memMgr( memMgr )
	{
	}

	VulkanMemoryHeap( VulkanMemoryHeap &&page ) noexcept :
	  m_memMgr( page.m_memMgr ),
	  m_gpuMemory( std::move( page.m_gpuMemory ) ),
	  m_cpuMemory( page.m_cpuMemory )
	{
		page.m_cpuMemory = nullptr;
	}

	VulkanMemoryHeap &operator=( VulkanMemoryHeap && ) = delete;

	bool Empty() const {}
	bool Full() const {}
	VkDeviceSize GetPageSize() const {}
	VkDeviceSize GetUsedSize() const
	{
	}
	MemoryAllocation Allocate( VkDeviceSize size, VkDeviceSize alignment );

	VkDeviceMemory GetGPUMemory() const { return m_gpuMemory; }
	void *GetCPUMemory() const { return m_cpuMemory; }

private:
	friend class MemoryAllocation;
	void Free( MemoryAllocation alloc );
	VulkanMemoryArena &m_memMgr;
	std::mutex m_mtx;
	ysl::Memory
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
					 VkDeviceSize hostVisibleReserveSize );
	~VulkanMemoryArena();

	MemoryAllocation Allocate( VkDeviceSize size, VkDeviceSize alignment, uint32_t memoryTypeIndex, bool hostVisible );
	MemoryAllocation Allocate( const VkMemoryRequirements &req, VkMemoryPropertyFlags flags );

private:
};
}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm