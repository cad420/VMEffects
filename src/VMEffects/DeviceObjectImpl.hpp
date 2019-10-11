
#pragma once

#include <VMUtils/ieverything.hpp>

namespace vm
{
namespace fx
{

template<typename Interface,typename DeviceType>
class DeviceObject : public EverythingBase<Interface>
{
	DeviceType * const m_device = nullptr;
public:
	DeviceObject( IRefCnt *cnt ,DeviceType * device):EverythingBase<Interface>( cnt ),m_device( device ) {}
	
};
}
}
