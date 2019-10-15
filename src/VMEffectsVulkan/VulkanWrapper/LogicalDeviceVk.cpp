
#include <vulkan/vulkan.h>
#include <VMEffectsVulkan/VulkanWrapper/LogicalDevickVk.h>
#include <VMEffectsVulkan/VulkanWrapper/VulkanObjectWrapper.h>
#include <assert.h>

namespace vm
{
namespace fx
{
namespace vkwrapper
{
std::shared_ptr<VkLogicalDeviceWrapper> VkLogicalDeviceWrapper::CreateLogicalDevice( VkPhysicalDevice device, const VkDeviceCreateInfo &ci, VkAllocationCallbacks *allocator )
{
	auto pDev = new VkLogicalDeviceWrapper( device, ci, allocator );
	return std::shared_ptr<VkLogicalDeviceWrapper>( pDev );
}

VkDevice VkLogicalDeviceWrapper::GetVkDeviceNativeHandle()
{
	return m_device;
}

VkQueue VkLogicalDeviceWrapper::GetQueue( uint32_t queueFamilyIndex, 
	uint32_t queueIndex ) const
{
	VkQueue queue;
	vkGetDeviceQueue( m_device, queueFamilyIndex, 0, &queue );
	return queue;
}

VkLogicalDeviceWrapper::~VkLogicalDeviceWrapper()
{
	vkDestroyDevice( m_device, m_allocator );
}

VkImageWrapper VkLogicalDeviceWrapper::CreateVkImage( const VkImageCreateInfo &createInfo, const char *dbgInfo ) const
{
	return CreateVkObject<VkImage>( vkCreateImage, createInfo, dbgInfo );
}

void VkLogicalDeviceWrapper::ReleaseVkObject( VkImageWrapper &&vkImageWrapper ) const
{
	vkDestroyImage( m_device, vkImageWrapper.m_object, m_allocator );
	vkImageWrapper.m_object = VK_NULL_HANDLE;
}

VkImageViewWrapper VkLogicalDeviceWrapper::CreateVkImageView( const VkImageViewCreateInfo &createInfo, const char *dbgInfo ) const
{
	return CreateVkObject<VkImageView>( vkCreateImageView, createInfo, dbgInfo );
}

void VkLogicalDeviceWrapper::ReleaseVkObject( VkImageViewWrapper &&vkImageViewWrapper ) const
{
	vkDestroyImageView( m_device, vkImageViewWrapper.m_object, m_allocator );

	vkImageViewWrapper.m_object = VK_NULL_HANDLE;
}

VkBufferWrapper VkLogicalDeviceWrapper::CreateVkBuffer( const VkBufferCreateInfo &createInfo, const char *dbgInfo ) const
{
	return CreateVkObject<VkBuffer>( vkCreateBuffer, createInfo, dbgInfo );
}

void VkLogicalDeviceWrapper::ReleaseVkObject( VkBufferWrapper &&vkBufferWrapper ) const
{
	vkDestroyBuffer( m_device, vkBufferWrapper.m_object, m_allocator );
	vkBufferWrapper.m_object = VK_NULL_HANDLE;
}

VkBufferViewWrapper VkLogicalDeviceWrapper::CreateVkBufferView( const VkBufferViewCreateInfo &createInfo, const char *dbgInfo ) const
{
	return CreateVkObject<VkBufferView>( vkCreateBufferView, createInfo, dbgInfo );
}

void VkLogicalDeviceWrapper::ReleaseVkObject( VkBufferViewWrapper &&vkBufferViewWrapper ) const
{
	vkDestroyBufferView( m_device, vkBufferViewWrapper.m_object, m_allocator );
	vkBufferViewWrapper.m_object = VK_NULL_HANDLE;
}

VkFenceWrapper VkLogicalDeviceWrapper::CreateFence( const VkFenceCreateInfo &createInfo, const char *dbgInfo ) const
{
	return CreateVkObject<VkFence>( vkCreateFence, createInfo, dbgInfo );
}

void VkLogicalDeviceWrapper::ReleaseVkObject( VkFenceWrapper &&vk_fence_wrapper ) const
{
	vkDestroyFence( m_device, vk_fence_wrapper, m_allocator );
	vk_fence_wrapper.m_object = VK_NULL_HANDLE;
}

void VkLogicalDeviceWrapper::ReleaseVkObject( VkCommandPoolWrapper &&vkCommandPoolWrapper ) const
{
	vkDestroyCommandPool( m_device, vkCommandPoolWrapper.m_object, m_allocator );
	vkCommandPoolWrapper.m_object = VK_NULL_HANDLE;
}

void VkLogicalDeviceWrapper::ReleaseVkObject( VkDeviceMemoryWrapper &&vkDeviceMemoryWrapper )const
{
	vkFreeMemory( m_device, vkDeviceMemoryWrapper.m_object, m_allocator );
	vkDeviceMemoryWrapper.m_object = VK_NULL_HANDLE;
}

VkCommandPoolWrapper VkLogicalDeviceWrapper::CreateCommandPool( const VkCommandPoolCreateInfo &createInfo, const char *dbgInfo )const
{
	return CreateVkObject<VkCommandPool>( vkCreateCommandPool, createInfo, dbgInfo );
}

VkResult VkLogicalDeviceWrapper::ResetCommandPool( VkCommandPool cmdPool, VkCommandPoolResetFlags flags ) const
{
	auto res = vkResetCommandPool( m_device, cmdPool, flags );
	if ( res != VK_SUCCESS ) {
		throw std::runtime_error( "Failed to reset command pool" );
	}
	return res;
}

VkMemoryRequirements VkLogicalDeviceWrapper::GetImageMemoryRequirements( VkImage image )const
{
	VkMemoryRequirements req = {};
	vkGetImageMemoryRequirements( m_device, image, &req );
	return req;
}

VkMemoryRequirements VkLogicalDeviceWrapper::GetBufferMemoryRRequirements( VkBuffer buffer )const
{
	VkMemoryRequirements req = {};
	vkGetBufferMemoryRequirements( m_device, buffer, &req );
	return req;
}

VkResult VkLogicalDeviceWrapper::BindBufferMemory( VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize offset ) const
{
	return vkBindBufferMemory( m_device, buffer, memory, offset );
}

VkResult VkLogicalDeviceWrapper::BindImageMemory( VkImage image, VkDeviceMemory memory, VkDeviceSize offset ) const
{
	return vkBindImageMemory( m_device, image, memory, offset );
}

VkDeviceMemoryWrapper VkLogicalDeviceWrapper::AllocateDeviceMemory( const VkMemoryAllocateInfo &allocInfo, const char *dbgInfo )const
{
	VkDeviceMemory deviceMemory = VK_NULL_HANDLE;
	auto res = vkAllocateMemory( m_device, &allocInfo, m_allocator, &deviceMemory );
	if ( res != VK_SUCCESS ) 
	{
		throw std::runtime_error( "Failed to allocate device memory\n" );
	}
	return VkDeviceMemoryWrapper( shared_from_this(), std::move( deviceMemory ) );
}

VkCommandBuffer VkLogicalDeviceWrapper::AllocateCommandBuffer( const VkCommandBufferAllocateInfo &allocInfo, const char *dbgInfo ) const
{
	VkCommandBuffer buffer;
	auto res = vkAllocateCommandBuffers( m_device, &allocInfo, &buffer );
	if ( res != VK_SUCCESS ) {
		throw std::runtime_error( "Failed to allocate command buffer" );
	}
	return buffer;
}

VkResult VkLogicalDeviceWrapper::ResetFence( VkFence fence ) const
{
	const auto res = vkResetFences( m_device, 1, &fence );
	if ( res != VK_SUCCESS ) {
		throw std::runtime_error( "Failed to reset fence" );
	}
	return res;
}

VkResult VkLogicalDeviceWrapper::MapMemory( VkDeviceMemory devMemory, VkDeviceSize size, VkDeviceSize offset, VkMemoryMapFlags flags, void **data ) const
{
	return vkMapMemory( m_device, devMemory, offset, size, flags, data );
}

void VkLogicalDeviceWrapper::UnmapMemory( VkDeviceMemory devMemory ) const
{
	vkUnmapMemory( m_device, devMemory );
}


VkLogicalDeviceWrapper::VkLogicalDeviceWrapper( VkPhysicalDevice device,
	const VkDeviceCreateInfo &ci, 
	VkAllocationCallbacks *allocator ) :
  m_allocator( allocator )
{
	auto res = vkCreateDevice( device, &ci, m_allocator, &m_device );
	if ( res != VK_SUCCESS ) {
		Debug( "Faild to create Vulkan logical device" );
		throw std::runtime_error( "Failed to create Vulkan logical device" );
	}
}

}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm