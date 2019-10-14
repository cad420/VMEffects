
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

VkQueue VkLogicalDeviceWrapper::GetQueue( uint32_t queueFamilyIndex ) const
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

VkLogicalDeviceWrapper::VkLogicalDeviceWrapper( VkPhysicalDevice device, const VkDeviceCreateInfo &ci, VkAllocationCallbacks *allocator ) :
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