
#pragma once

#include <VMEffectsVulkan/VulkanWrapper/LogicalDevickVk.h>
#include <VMUtils/traits.hpp>
#include <mutex>
#include <deque>

namespace vm
{
namespace fx
{
class DeviceVkImpl;
using namespace vkwrapper;

class CommandPoolArena:NoCopy,NoMove
{
public:
	CommandPoolArena( DeviceVkImpl &device, 
		uint32_t queueFamilyIndex, 
		VkCommandPoolCreateFlags flags );

	~CommandPoolArena() = default;

	VkCommandPoolWrapper AllocateCommandPool( const char *dbgInfo );

	void ReleaseCommandPool( VkCommandPoolWrapper cmdPool, uint32_t cmdQueueIndex, uint64_t fenceValue );

	void DestroyArena();
private:
	DeviceVkImpl & m_device;
	const uint32_t m_queueFamilyIndex;
	const VkCommandPoolCreateFlags m_flags;
	std::mutex m_mtx;
	std::deque<VkCommandPoolWrapper> m_cmdPools;
};
}
}
