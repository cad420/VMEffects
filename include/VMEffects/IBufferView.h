
#pragma once
#include "IDeviceObject.h"
#include "IBuffer.h"

namespace vm
{
namespace fx
{


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
	virtual IBuffer *GetBuffer() = 0;
};


}  // namespace fx
}  // namespace vm