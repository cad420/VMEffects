
#include <VMEffectsVulkan/VulkanWrapper/MemoryManagerVk.h>

namespace vm
{
namespace fx
{
namespace vkwrapper
{
VulkanMemoryHeap::VulkanMemoryHeap( VulkanMemoryArena &memMgr, VkDeviceSize size, uint32_t memoryTypeIndex, bool hostVisible ) :
  m_memMgr( memMgr ),
  m_tracker( size )
{
	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.allocationSize = size;
	allocInfo.memoryTypeIndex = memoryTypeIndex;
	m_gpuMemory = m_memMgr.LogicalDevice.AllocateDeviceMemory( allocInfo, "allocate device memory from device" );

	if ( hostVisible == true ) {
		auto res = m_memMgr.LogicalDevice.MapMemory( m_gpuMemory, 0, size, 0, &m_cpuMemory );
		if ( res != VK_SUCCESS ) {
			throw std::runtime_error( "Failed to map device memory" );
		}
	}
}

MemoryAllocation VulkanMemoryHeap::Allocate( VkDeviceSize size, VkDeviceSize alignment )
{
	std::lock_guard<std::mutex> lk( m_mtx );
	const auto alloc = m_tracker.Allocate( size, alignment );
	if ( alloc.IsValid() ) {
		return { this, alloc.Size, alloc.UnalignedOffset };
	}
	return MemoryAllocation{};
}

VulkanMemoryHeap::~VulkanMemoryHeap()
{
	if ( m_cpuMemory ) {
		m_memMgr.LogicalDevice.UnmapMemory( m_gpuMemory );
	}
	if ( IsEmpty() == false ) {
		Debug( "Release a non-empty memory buffer" );
	}
}

void VulkanMemoryHeap::Free( MemoryAllocation alloc )
{
	m_memMgr.OnFree( alloc.Size, alloc.Offset );
	std::lock_guard<std::mutex> lk( m_mtx );
	m_tracker.Free( alloc.Offset, alloc.Size );
}

VulkanMemoryArena::~VulkanMemoryArena()
{
	// do nothing
}

MemoryAllocation VulkanMemoryArena::Allocate( VkDeviceSize size,
											  VkDeviceSize alignment,
											  uint32_t memoryTypeIndex,
											  bool hostVisible )
{
	MemoryAllocation allocation;

	MemoryHeapIndex heapIndex{ memoryTypeIndex, hostVisible };
	std::lock_guard<std::mutex> lk( m_heapMtx );
	auto range = m_heaps.equal_range( heapIndex );
	for ( auto it = range.first; it != range.second; ++it ) {
		allocation = it->second.Allocate( size, alignment );
		if ( allocation.Heap != nullptr )
			break;  // find a proper memory heap
	}

	size_t idx = hostVisible ? 1 : 0;

	if ( allocation.Heap == nullptr ) {  // There is no proper heap, allocating a new one
		auto heapSize = hostVisible ? m_hostVisibleVisibleSize : m_deviceLocalHeapSize;
		while ( heapSize < size )
			heapSize *= 2;

		m_currAllocatedSize[ idx ] += heapSize;
		m_peakAllocatedSize[ idx ] = std::max( m_peakAllocatedSize[ idx ], m_currAllocatedSize[ idx ] );

		auto it = m_heaps.emplace( heapIndex, VulkanMemoryHeap( *this, heapSize, memoryTypeIndex, hostVisible ) );
		// callback
		allocation = it->second.Allocate( size, alignment );
	}

	//if ( allocation.Heap != nullptr ) {
	//
	//}

	m_curUsedSize[ idx ].fetch_add( allocation.Size );
	m_peakUsedSize[ idx ] = std::max( m_peakUsedSize[ idx ], static_cast<VkDeviceSize>( m_curUsedSize[ idx ].load() ) );

	return allocation;
}

MemoryAllocation VulkanMemoryArena::Allocate( const VkMemoryRequirements &req,
											  VkMemoryPropertyFlags flags )
{
	const auto memoryTypeIndex = PhysicalDevice.FindMemoryType( req.memoryTypeBits, flags );
	const bool hostVisible = ( flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) != 0;
	return Allocate( req.size, req.alignment, memoryTypeIndex, hostVisible );
}

void VulkanMemoryArena::OnFree( VkDeviceSize size, bool hostVisible )
{
	m_curUsedSize[ hostVisible ? 1 : 0 ].fetch_add( -static_cast<int64_t>( size ) );
}
}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm