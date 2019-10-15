
#pragma once

#include "DeviceObjectImpl.hpp"

namespace vm
{
namespace fx
{
template <typename Interface, typename DeviceType, typename DescType>
class FenceBase : DeviceObject<Interface, DeviceType>
{
public:
	FenceBase( IRefCnt *cnt, DeviceType *device ,const DescType & desc) :
	  DeviceObject<Interface, DeviceType>( cnt, device ),m_desc( desc ) {}
	const DescType &GetDesc() const override
	{
		return m_desc;
	}
	~FenceBase() {}

private:
	DescType m_desc;
};
}  // namespace fx
}  // namespace vm