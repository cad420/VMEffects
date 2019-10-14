
#pragma once
#include <memory>
#include <vulkan/vulkan.h>
#include <VMUtils/log.hpp>
#include <stdexcept>

namespace vm
{
namespace fx
{
namespace vkwrapper
{
template <typename T>
class VkObjectWrapper;

using VkImageWrapper = VkObjectWrapper<VkImage>;
using VkImageViewWrapper = VkObjectWrapper<VkImageView>;
using VkBufferWrapper = VkObjectWrapper<VkBuffer>;
using VkBufferViewWrapper = VkObjectWrapper<VkBufferView>;
using VkDeviceMemoryWrapper = VkObjectWrapper<VkDeviceMemory>;

class VkLogicalDeviceWrapper : public std::enable_shared_from_this<VkLogicalDeviceWrapper>
{
public:
	static std::shared_ptr<VkLogicalDeviceWrapper> CreateLogicalDevice( VkPhysicalDevice device,
																		const VkDeviceCreateInfo &,
																		VkAllocationCallbacks *allocator );
	VkDevice GetVkDeviceNativeHandle();
	VkQueue GetQueue( uint32_t queueFamilyIndex ) const;
	~VkLogicalDeviceWrapper();

	template <typename VulkanObjectType, typename CreateFuncName, typename CreateInfoType>
	VkObjectWrapper<VulkanObjectType> CreateVkObject(
	  CreateFuncName func,
	  const CreateInfoType &createInfo,
	  const char *dbgInfo )const
	{
		VulkanObjectType vkObject = VK_NULL_HANDLE;
		auto res = func( m_device, &createInfo, m_allocator, &vkObject );
		if ( res != VK_SUCCESS ) {
			Log( "Failed to create Vulkan object: {}", dbgInfo );
			throw std::runtime_error( "Failed to create Vulkan object" );
		}

		return VkObjectWrapper<VulkanObjectType>( shared_from_this(), std::move( vkObject ) );
	}

	VkImageWrapper CreateVkImage( const VkImageCreateInfo &createInfo, const char *dbgInfo ) const;
	void ReleaseVkObject( VkImageWrapper &&vkImageWrapper ) const;

	VkImageViewWrapper CreateVkImageView( const VkImageViewCreateInfo &createInfo, const char *dbgInfo ) const;
	void ReleaseVkObject( VkImageViewWrapper &&vkImageViewWrapper ) const;

	VkBufferWrapper CreateVkBuffer( const VkBufferCreateInfo &createInfo, const char *dbgInfo ) const;
	void ReleaseVkObject( VkBufferWrapper &&vkBufferWrapper ) const;

	VkBufferViewWrapper CreateVkBufferView( const VkBufferViewCreateInfo &createInfo, const char *dbgInfo ) const;
	void ReleaseVkObject( VkBufferViewWrapper &&vkBufferViewWrapper ) const;

	VkMemoryRequirements GetImageMemoryRequirements( VkImage image )const;

	VkMemoryRequirements GetBufferMemoryRRequirements( VkBuffer buffer)const;

private:
	VkLogicalDeviceWrapper( VkPhysicalDevice device, const VkDeviceCreateInfo &ci, VkAllocationCallbacks *allocator );

	VkDevice m_device = VK_NULL_HANDLE;
	VkAllocationCallbacks *m_allocator = nullptr;
};

}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm
