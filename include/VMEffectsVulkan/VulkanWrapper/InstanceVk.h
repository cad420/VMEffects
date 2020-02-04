
#pragma once

#include <vulkan/vulkan.h>
#include <VMUtils/concepts.hpp>
#include <VMUtils/log.hpp>
#include <memory>
#include <vector>
#include <set>

namespace vm
{
namespace fx
{
namespace vkwrapper
{
class InstanceVkWrapper final : public std::enable_shared_from_this<InstanceVkWrapper>,
								NoCopy,
								NoMove
{
public:
	~InstanceVkWrapper();
	static std::shared_ptr<InstanceVkWrapper> CreateInstance();
	VkInstance GetVkInstanceNativeHandle();
	VkPhysicalDevice GetAPhysicalDevice();

private:
	InstanceVkWrapper();
	void SetupDebugCallback();
	static VkResult CreateDebugUtilsMessengerEXT(
	  VkInstance instance,
	  const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
	  const VkAllocationCallbacks *pAllocator,
	  VkDebugUtilsMessengerEXT *pCallback );
	static void DestroyDebugUtilsMessengerEXT( VkInstance instance,
											   VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks *pAllocator );

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	  VkDebugUtilsMessageTypeFlagsEXT messageType,
	  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
	  void *pUserData );

	struct QueueFamiliyIndices
	{
		uint32_t graphicsFamiliy = -1;
		uint32_t presentFamiliy = -1;
		bool isComplete() const
		{
			return graphicsFamiliy != -1;
		}
	};

	QueueFamiliyIndices m_queueFamilyIndex;
	bool IsRequired( VkPhysicalDevice device );
	bool CheckExtensionSupport( VkPhysicalDevice device );

	VkInstance m_instance = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT m_callback = VK_NULL_HANDLE;
	std::vector<VkPhysicalDevice> m_physicalDevices;
};
}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm
