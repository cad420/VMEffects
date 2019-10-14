
#pragma once

#include <VMUtils/ieverything.hpp>

namespace vm
{
namespace fx
{

template<typename Interface,typename DeviceType>
class DeviceObject : public EverythingBase<Interface>
{
public:
	DeviceObject( IRefCnt *cnt ,DeviceType * device):EverythingBase<Interface>( cnt ),m_device( device ) {}
protected:
	DeviceType * const m_device = nullptr;
};
}
}
