
#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace vm
{
namespace fx
{
namespace vkwrapper
{


struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;

	SwapChainSupportDetails() = default;
};

SwapChainSupportDetails querySwapChainSupport( VkPhysicalDevice device,VkSurfaceKHR surface );


}
}
}