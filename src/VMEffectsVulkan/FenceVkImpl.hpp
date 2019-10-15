
#pragma once

#include <FenceBase.hpp>

#include <VMEffectsVulkan/IFenceVk.h>
#include "DeviceVkImpl.hpp"
#include <VMEffectsVulkan/VulkanWrapper/LogicalDevickVk.h>
#include <VMEffectsVulkan/VulkanWrapper/VulkanObjectWrapper.h>
#include <VMEffectsVulkan/VulkanWrapper/VulkanFencePool.h>

namespace vm
{
namespace fx
{
using namespace vkwrapper;
class FenceVkImpl : public FenceBase<IFenceVk, DeviceVkImpl, FenceDesc>
{
public:
	FenceVkImpl( IRefCnt *cnt, DeviceVkImpl *device, const FenceDesc &desc );

	~FenceVkImpl();

	uint64_t GetCompletedValue() override final;

	void Reset( uint64_t value ) override final;

	VkFenceWrapper GetVkFence()
	{
		return m_fencePool.GetFence();
	}

	void AddPendingFence( VkFenceWrapper &&fence, uint64_t fenceValue )
	{
		m_pendingFences.emplace_back( fenceValue, std::move( fence ) );
	}

	void Wait();

private:
	MyVkFencePool m_fencePool;
	std::deque<std::pair<uint64_t, VkFenceWrapper>> m_pendingFences;
	volatile uint64_t m_lastCompletedFenceValue = 0;
};

}  // namespace fx
}  // namespace vm