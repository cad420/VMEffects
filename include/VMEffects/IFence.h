
#pragma once

#include <VMUtils/ieverything.hpp>
#include "IDeviceObject.h"

namespace vm
{
namespace fx
{


struct FenceDesc
{

};

class IFence : public IDeviceObject
{
public:
	virtual const FenceDesc &GetDesc() const= 0;

	virtual uint64_t GetCompletedValue() = 0;

	virtual void Reset( uint64_t value ) = 0;
};
}
}