
#pragma once

#include "DeviceObjectImpl.hpp"
#include <VMEffects/ITexture.h>
namespace vm
{
namespace fx
{
template<typename Interface,typename DeviceType,typename DescType>
class TextureBase : public DeviceObject<Interface, DeviceType>
{
	
public:
	TextureBase( IRefCnt *cnt, DeviceType *device,const TextureDesc & desc ) :
	  DeviceObject<Interface, DeviceType>( cnt, device ),m_desc( desc ) {}
	const DescType & GetDesc()const
	{
		return m_desc;
	}
protected:
	DescType m_desc;
	
};
}
}