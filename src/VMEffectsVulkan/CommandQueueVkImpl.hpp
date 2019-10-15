
#pragma once
#include <VMEffectsVulkan/ICommandQueueVk.h>
#include <VMEffectsVulkan/VulkanWrapper/LogicalDevickVk.h>
#include <memory>

namespace vm
{
namespace fx
{
using namespace vkwrapper;
class CommandQueueVkImpl final : public EverythingBase<ICommandQueueVk>
{
public:
	CommandQueueVkImpl( IRefCnt *cnt,
						std::shared_ptr<VkLogicalDeviceWrapper> logicalDevice,
						uint32_t queueFamily );
	~CommandQueueVkImpl() = default;

	uint64_t GetNextFence() override
	{
		return 0;
	}
	uint64_t Submit( VkCommandBuffer cmdBuffer ) override
	{
		return 0;
	}
	uint64_t Submit( const VkSubmitInfo &submitInfo ) override
	{
		std::lock_guard<std::mutex> lk( m_mtx );
		std::atomic_int64_t fence = m_nextFence.load();

		++m_nextFence;

		return 0;
	}
	VkResult Present( const VkPresentInfoKHR &presentInfo ) override
	{
		return VkResult{};
	}

	VkQueue GetVkQueue() override
	{
		return VkQueue{};
	}

	uint32_t GetQueueFamilyIndex() override
	{
		return 0;
	}

	uint64_t WaitForIdle() override
	{
		return 0;
	}

	uint64_t GetCompletedFence() override
	{
		return 0;
	}

	void SignalFence( VkFence fence ) override
	{
	}

private:
	std::shared_ptr<VkLogicalDeviceWrapper> m_logicalDevice;
	const VkQueue m_queue;
	const uint32_t m_queueFamilyIndex;
	std::atomic_int64_t m_nextFence;
	std::mutex m_mtx;
};
}  // namespace fx
}  // namespace vm