
#pragma once
#include <memory>
#include <vulkan/vulkan.h>

namespace vm
{
namespace fx
{
namespace vkwrapper
{
class LogicalDeviceVk : public std::enable_shared_from_this<LogicalDeviceVk>
{
public:
	static std::shared_ptr<LogicalDeviceVk> CreateLogicalDevice( VkPhysicalDevice device,
																 const VkDeviceCreateInfo &,
																 VkAllocationCallbacks *allocator );
	VkDevice GetVkDevice();
	VkQueue GetQueue( uint32_t queueFamilyIndex ) const;
	~LogicalDeviceVk();

private:
	LogicalDeviceVk( VkPhysicalDevice device, const VkDeviceCreateInfo &ci, VkAllocationCallbacks *allocator );

	VkDevice m_device = VK_NULL_HANDLE;
	VkAllocationCallbacks *m_allocator = nullptr;
};

}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm
