
#pragma once
#include <mutex>
#include <memory>

#include <VMEffectsVulkan/ICommandQueueVk.h>
#include <VMEffectsVulkan/VulkanWrapper/LogicalDevickVk.h>
#include <VMUtils/ref.hpp>

#include "FenceVkImpl.hpp"

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

	uint64_t GetNextFence() override final
	{
		return m_nextFence.load();
	}
	uint64_t Submit( VkCommandBuffer cmdBuffer ) override final
	{
		VkSubmitInfo info={};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.pNext = nullptr;
		info.pCommandBuffers = &cmdBuffer;
		info.commandBufferCount = 1;

		info.pSignalSemaphores = nullptr;
		info.signalSemaphoreCount = 0;

		info.waitSemaphoreCount = 0;
		info.pWaitSemaphores = nullptr;

		info.pWaitDstStageMask = nullptr;
		return Submit(info);
	}
	uint64_t Submit( const VkSubmitInfo &submitInfo ) override final
	{
		std::lock_guard<std::mutex> lk( m_mtx );
		auto fenceValue = m_nextFence.load();
		++m_nextFence;
		auto newFence = m_fence->GetVkFence();
		auto res = vkQueueSubmit(m_vkQueue,1,&submitInfo,newFence);
		#ifndef NDEBUG

		if(res != VK_SUCCESS)
		{
			auto what = vm::fmt("failed to submit command queue. {} {}",__FILE__,__LINE__);
			throw std::runtime_error(what);
		}
		#endif
		m_fence->AddPendingFence(std::move(newFence),fenceValue);
		return fenceValue;
	}
	VkResult Present( const VkPresentInfoKHR &presentInfo ) override final
	{
		std::lock_guard<std::mutex> l(m_mtx);
		return vkQueuePresentKHR(m_vkQueue,&presentInfo);
	}

	VkQueue GetVkQueue() override final
	{
		return m_vkQueue;
	}

	uint32_t GetQueueFamilyIndex()const override final
	{
		return m_queueFamilyIndex;
	}

	uint64_t WaitForIdle() override final
	{
		std::lock_guard<std::mutex> l(m_mtx);
		uint64_t lastComplemtedValue = m_nextFence.load();
		++m_nextFence;
		vkQueueWaitIdle(m_vkQueue);

		m_fence->Wait(std::numeric_limits<uint64_t>::max());
		m_fence->Reset(lastComplemtedValue);
		return lastComplemtedValue;
	}

	uint64_t GetCompletedFence() override final
	{
		std::lock_guard<std::mutex> l(m_mtx);
		return m_fence->GetCompletedValue();
	}

	void SignalFence( VkFence fence ) override final
	{
		std::lock_guard<std::mutex> l(m_mtx);
		auto res = vkQueueSubmit(m_vkQueue,0,nullptr,fence);
		#ifndef NDEBUG
		if(res != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to submit");
		}
		#endif
	}

	void SetFence(FenceVkImpl * fence)
	{
		m_fence = fence;
	}

	~CommandQueueVkImpl()
	{
	}
private:
	std::shared_ptr<VkLogicalDeviceWrapper> m_logicalDevice;
	const VkQueue m_vkQueue;
	const uint32_t m_queueFamilyIndex;
	std::atomic_int64_t m_nextFence;
	Ref<FenceVkImpl> m_fence;
	std::mutex m_mtx;
};
}  // namespace fx
}  // namespace vm