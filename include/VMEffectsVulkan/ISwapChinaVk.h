
#pragma once
#include <VMEffects/ISwapChain.h>
#include <vulkan/vulkan.h>


namespace vm
{
namespace fx
{
class ISwapChainVk : public ISwapChain
{
public:
	virtual VkSwapchainKHR GetVkSwapChain() = 0;
};
}
}