
#pragma once

#include <VMUtils/ieverything.hpp>
#include <vulkan/vulkan.h>

namespace vm
{
namespace fx
{
class ICommandQueueVk : public IEverything
{
public:
	virtual uint64_t GetNextFence() = 0;
	virtual uint64_t Submit( VkCommandBuffer cmdBuffer ) = 0;
	virtual uint64_t Submit( const VkSubmitInfo &submitInfo ) = 0;
	virtual VkResult Present( const VkPresentInfoKHR &presentInfo ) = 0;
	virtual VkQueue GetVkQueue() = 0;
	virtual uint32_t GetQueueFamilyIndex() = 0;
	virtual uint64_t GetCompletedFence() = 0;
	virtual uint64_t WaitForIdle() = 0;
	virtual void SignalFence( VkFence fence ) = 0;
	
};
}
}