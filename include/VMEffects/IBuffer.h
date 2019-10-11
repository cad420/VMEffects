
#pragma once

#include "IDeviceObject.h"

namespace vm
{
namespace fx
{

class IBufferView;

struct BufferDesc
{
	uint32_t BufferSize = 0;
	BufferDesc(
	  uint32_t bufferSize ) :
	  BufferSize( bufferSize ) {}
};

class IBuffer : public IDeviceObject
{
public:
	void QueryInterface( const InterfaceID &iid, IEverything **interface ) override = 0;
	virtual IBufferView *CreateBufferView() = 0;
};
}  // namespace fx
}  // namespace vm