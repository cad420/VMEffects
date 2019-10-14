
#pragma once

#include "DeviceObjectImpl.hpp"
#include <VMEffects/IBuffer.h>

namespace vm
{
namespace fx
{
template<typename Interface,typename DeviceType,typename  DescType>
class BufferBase : public DeviceObject<Interface, DeviceType>
{
public:
	BufferBase(IRefCnt * cnt,DeviceType * device,const BufferDesc & desc):
	  DeviceObject<Interface, DeviceType>( cnt, device ),
	  m_desc( desc )
	{}
	
	const DescType &GetDesc() override final
	{
		return m_desc;
	}
private: DescType m_desc;

};
}
}