
#include "FenceVkImpl.hpp"
#include <limits>

namespace vm
{
namespace fx
{
FenceVkImpl::FenceVkImpl( IRefCnt *cnt, DeviceVkImpl *device, const FenceDesc &desc ):EverythingBase<IFenceVk>(cnt),
	m_fencePool( device->GetLogicalDeviceVk().shared_from_this() ),
	m_device(device),
	m_desc(desc),
	m_lastCompletedFenceValue(1)
{

}

FenceVkImpl::~FenceVkImpl()
{
	if ( m_pendingFences.empty() == false ) 
	{
		Wait(std::numeric_limits<uint64_t>::max());
	}
}

uint64_t FenceVkImpl::GetCompletedValue()
{	
	return m_lastCompletedFenceValue;
}

void FenceVkImpl::Reset( uint64_t value )
{
	if(value > m_lastCompletedFenceValue)
	{
		m_lastCompletedFenceValue = value;
	}
}

void FenceVkImpl::Wait(uint64_t fenceValue)
{
	const auto & logicalDevice = m_device->GetLogicalDeviceVk();
	while(!m_pendingFences.empty()){
		auto & p = m_pendingFences.front();
		if(p.first > m_lastCompletedFenceValue){
			break;
		}
		auto s = logicalDevice.GetFenceStatus(p.second);
		if(s == VK_NOT_READY ){
			VkFence fence = p.second;
			logicalDevice.WaitForFence(1,&fence,VK_TRUE,std::numeric_limits<uint64_t>::max());
		}
		assert(s == VK_SUCCESS);
		if(p.first >m_lastCompletedFenceValue)
		{
			m_lastCompletedFenceValue = p.first;
		}
		m_fencePool.Collect(std::move(p.second));

		m_pendingFences.pop_front();
	}
}

}
}