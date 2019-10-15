
#pragma once
#include "LogicalDevickVk.h"
#include <vector>
namespace vm
{
namespace fx
{
namespace vkwrapper
{
class MyVkFencePool : NoCopy, NoMove
{
public:
	MyVkFencePool( std::shared_ptr<const VkLogicalDeviceWrapper> logicalDevice );
	~MyVkFencePool() = default;
	VkFenceWrapper GetFence();
	void DisposeFence( VkFenceWrapper && fence );
private:
	std::shared_ptr<const VkLogicalDeviceWrapper> m_logicalDevice;
	std::vector<VkFenceWrapper> m_fences;
};

}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm