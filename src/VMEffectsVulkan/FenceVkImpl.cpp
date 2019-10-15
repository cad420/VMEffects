
#include "FenceVkImpl.hpp"

namespace vm
{
namespace fx
{
FenceVkImpl::FenceVkImpl( IRefCnt *cnt, DeviceVkImpl *device, const FenceDesc &desc ):
	FenceBase<vm::fx::IFenceVk, vm::fx::DeviceVkImpl, vm::fx::FenceDesc>( cnt, device, desc ),
	m_fencePool( device->GetLogicalDeviceVk().shared_from_this() )
{
	
}

FenceVkImpl::~FenceVkImpl()
{
	if ( m_pendingFences.empty() == false ) {
		Wait();
	}
}

uint64_t FenceVkImpl::GetCompletedValue()
{
	return 0;
}

void FenceVkImpl::Reset( uint64_t value )
{
}

void FenceVkImpl::Wait()
{
	
}
}
}