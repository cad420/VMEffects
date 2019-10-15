
#pragma once

#include <VMEffectsVulkan/VulkanWrapper/LogicalDevickVk.h>
#include <deque>

namespace vm
{
namespace fx
{
class DeviceVkImpl;
using namespace vkwrapper;
class CommandPoolManager:NoCopy,NoMove
{
public:
	CommandPoolManager( DeviceVkImpl &device, 
		uint32_t queueFamilyIndex, 
		VkCommandPoolCreateFlags flags );

	~CommandPoolManager() = default;

	VkCommandPoolWrapper AllocateCommandPool( const char *dbgInfo );

	void ReleaseCommandPool( VkCommandPoolWrapper cmdPool, uint32_t cmdQueueIndex, uint64_t fenceValue );

	void DestroyPools();

private:
	DeviceVkImpl & m_device;
	const uint32_t m_queueFamilyIndex;
	const VkCommandPoolCreateFlags m_flags;
	std::mutex m_mtx;
	std::deque<VkCommandPoolWrapper> m_cmdPools;
	
};
}
}
