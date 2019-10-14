
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
class MemoryManagerVk;
class MemoryPage;

class MemoryAllocationVk : vm::NoCopy
{
public:
	MemoryAllocationVk() = default;
	MemoryAllocationVk( MemoryPage *page,
						VkDeviceSize size,
						VkDeviceSize offset ) :
	  m_page( page ),
	  m_size( size ),
	  m_offset( offset ) {}

	MemoryAllocationVk( MemoryAllocationVk &&alloc ) noexcept :
	  m_page( alloc.m_page ),
	  m_size( alloc.m_size ),
	  m_offset( alloc.m_offset )
	{
		alloc.m_page = nullptr;
		alloc.m_size = 0;
		alloc.m_offset = 0;
	}

	MemoryAllocationVk &operator=( MemoryAllocationVk &&alloc ) noexcept
	{
		m_page = alloc.m_page;
		m_size = alloc.m_size;
		m_offset = alloc.m_offset;

		alloc.m_page = nullptr;
		alloc.m_size = 0;
		alloc.m_offset = 0;

		return *this;
	}

	~MemoryAllocationVk() = default;

private:
	MemoryPage *m_page = nullptr;
	VkDeviceSize m_offset = 0;
	VkDeviceSize m_size = 0;
};

class MemoryPage : NoCopy
{
public:
	MemoryPage( MemoryManagerVk &memMgr,
				VkDeviceSize size,
				uint32_t memoryTypeIndex,
				bool hostVisible ) :
	  m_memMgr( memMgr )
	{
	}

	MemoryPage( MemoryPage &&page ) noexcept :
	  m_memMgr( page.m_memMgr ),
	  m_gpuMemory( std::move( page.m_gpuMemory ) ),
	  m_cpuMemory( page.m_cpuMemory )
	{
		page.m_cpuMemory = nullptr;
	}

	MemoryPage &operator=( MemoryPage && ) = delete;

	bool Empty() const {}
	bool Full() const {}
	VkDeviceSize GetPageSize() const {}
	VkDeviceSize GetUsedSize() const
	{
	}
	MemoryAllocationVk Allocate( VkDeviceSize size, VkDeviceSize alignment );

	VkDeviceMemory GetGPUMemory() const { return m_gpuMemory; }
	void *GetCPUMemory() const { return m_cpuMemory; }

private:
	friend class MemoryAllocationVk;
	void Free( MemoryAllocationVk alloc );
	MemoryManagerVk &m_memMgr;
	std::mutex m_mtx;
	VkDeviceMemoryWrapper m_gpuMemory;
	void *m_cpuMemory = nullptr;
};

class MemoryManagerVk
{
public:
	MemoryManagerVk( const VkLogicalDeviceWrapper &logicalDevice,
					 const VkPhysicalDeviceWrapper &physicalDevice,
					 VkDeviceSize localPageSize,
					 VkDeviceSize hostVisiblePageSize,
					 VkDeviceSize localReserveSize,
					 VkDeviceSize hostVisibleReserveSize );
	~MemoryManagerVk();

	MemoryAllocationVk Allocate( VkDeviceSize size, VkDeviceSize alignment, uint32_t memoryTypeIndex, bool hostVisible );
	;
	MemoryAllocationVk Allocate( const VkMemoryRequirements &req, VkMemoryPropertyFlags flags );

private:
};
}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm