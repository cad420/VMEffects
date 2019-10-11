
#include <VMEffectsVulkan/VulkanWrapper/LogicalDevickVk.h>

namespace vm
{
namespace fx
{
namespace vkwrapper
{
std::shared_ptr<LogicalDeviceVk> LogicalDeviceVk::CreateLogicalDevice( VkPhysicalDevice device, const VkDeviceCreateInfo & ci, VkAllocationCallbacks *allocator )
{
	auto pDev = new LogicalDeviceVk( device, ci, allocator );
	return std::shared_ptr<LogicalDeviceVk>( pDev );
}

VkDevice LogicalDeviceVk::GetVkDevice()
{
	return m_device;
}

VkQueue LogicalDeviceVk::GetQueue( uint32_t queueFamilyIndex )const
{
	VkQueue queue;
	vkGetDeviceQueue( m_device, queueFamilyIndex, 0, &queue );
	return queue;
}

LogicalDeviceVk::~LogicalDeviceVk()
{
	vkDestroyDevice( m_device,m_allocator);
}

LogicalDeviceVk::LogicalDeviceVk( VkPhysicalDevice device, const VkDeviceCreateInfo &ci, VkAllocationCallbacks *allocator ):m_allocator( allocator )
{
	auto res = vkCreateDevice( device, &ci, m_allocator, &m_device );
}

}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm