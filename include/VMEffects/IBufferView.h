
#pragma once
#include "IDeviceObject.h"

namespace vm
{
namespace fx
{

class IBuffer;

struct BufferFormat
{
	BufferFormat() = default;
};

struct BufferViewDesc
{
	
	BufferViewDesc() = default;
};

class IBufferView : public IDeviceObject
{
public:

	virtual IBuffer * GetBuffer() = 0;

	virtual const BufferViewDesc &GetDesc() = 0;
};


}  // namespace fx
}  // namespace vm