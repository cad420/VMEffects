
#pragma once

#include "VMUtils/ieverything.hpp"
namespace vm
{
namespace fx
{
template<typename Interface,typename DeviceImplType>
class ContextBase : public EverythingBase<Interface>
{
public:
	ContextBase( IRefCnt *cnt,DeviceImplType * device ) :
	  EverythingBase<Interface>( cnt ),m_device( device ) {}
protected:
	DeviceImplType * const m_device;
};
}
}