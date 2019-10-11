
#include <VMEffectsVulkan/VulkanWrapper/LogicalDevickVk.h>

namespace vm
{
namespace fx
{
namespace vkwrapper
{
std::shared_ptr<VkLogicalDeviceWrapper> VkLogicalDeviceWrapper::CreateLogicalDevice( VkPhysicalDevice device, const VkDeviceCreateInfo & ci, VkAllocationCallbacks *allocator )
{
	auto pDev = new VkLogicalDeviceWrapper( device, ci, allocator );
	return std::shared_ptr<VkLogicalDeviceWrapper>( pDev );
}

VkDevice VkLogicalDeviceWrapper::GetVkDeviceNativeHandle()
{
	return m_device;
}

VkQueue VkLogicalDeviceWrapper::GetQueue( uint32_t queueFamilyIndex )const
{
	VkQueue queue;
	vkGetDeviceQueue( m_device, queueFamilyIndex, 0, &queue );
	return queue;
}

VkLogicalDeviceWrapper::~VkLogicalDeviceWrapper()
{
	vkDestroyDevice( m_device,m_allocator);
}

VkLogicalDeviceWrapper::VkLogicalDeviceWrapper( VkPhysicalDevice device, const VkDeviceCreateInfo &ci, VkAllocationCallbacks *allocator ):m_allocator( allocator )
{
	auto res = vkCreateDevice( device, &ci, m_allocator, &m_device );
}

}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm