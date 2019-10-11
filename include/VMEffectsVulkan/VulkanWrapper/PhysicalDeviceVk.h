#pragma once
#include <memory>
#include <VMUtils/concepts.hpp>
#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>

namespace vm
{
namespace fx
{
namespace vkwrapper
{
class PhysicalDeviceVk : NoMove, NoCopy
{
public:
	static std::unique_ptr<PhysicalDeviceVk> CreatePhysicalDevice( VkPhysicalDevice device );
	VkPhysicalDevice GetPhysicalDeviceHandle() const;
	uint32_t FindMemoryType( uint32_t typeFilter, VkMemoryPropertyFlags properties )const;
	uint32_t FindQueueFamily( VkQueueFlags QueueFlags ) const;
private :

	  PhysicalDeviceVk( VkPhysicalDevice device );

	const VkPhysicalDevice m_device = VK_NULL_HANDLE;
	VkPhysicalDeviceProperties m_properties = {};
	VkPhysicalDeviceFeatures m_features = {};
	VkPhysicalDeviceMemoryProperties m_memoryProperties = {};

	std::vector<VkQueueFamilyProperties> m_queueFamiliyProperties;
	std::vector<VkExtensionProperties> m_extensions;
};
}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm