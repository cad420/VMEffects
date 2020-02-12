
#pragma once
#include "LogicalDevickVk.h"
#include <vector>
namespace vm
{
namespace fx
{
namespace vkwrapper
{
class FenceArena: NoCopy, NoMove
{
public:
	FenceArena( std::shared_ptr<const VkLogicalDeviceWrapper> logicalDevice );
	~FenceArena() = default;
	VkFenceWrapper GetFence();
	void Collect( VkFenceWrapper && fence );
private:
	std::shared_ptr<const VkLogicalDeviceWrapper> m_logicalDevice;
	std::vector<VkFenceWrapper> m_fences;
};

}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm