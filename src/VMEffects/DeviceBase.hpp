
#pragma once

#include <VMUtils/ieverything.hpp>

namespace vm
{
namespace fx
{
template<typename Interface>
class Device :public EverythingBase<Interface>
{
public:
	Device( IRefCnt *cnt ) :
	  EverythingBase<Interface>( cnt ) {}
};
}
}