
#pragma once
#include "EngineType.h"
#include "IDeviceObject.h"
#include "IBufferView.h"

namespace vm
{
namespace fx
{
class IBufferView;

struct BufferDesc
{
	const char * Info = nullptr;
	uint32_t BufferSize = 0;
	BindFlags BindFlag = VM_BT_UNKNOWN;
	EUsage Usage = VM_USG_DEFAULT;
	CPUAccessFlags CPUAccessFlag = VM_CA_NONE;
	EBufferMode Mode = VM_BM_UNKNOWN;
	uint64_t CommandQueueMask = 0;
	BufferDesc(
	  const char * info ,
	  uint32_t bufferSize,
	  BindFlags bindFlag,
	  EUsage usage,
	  CPUAccessFlags cpuAccessFlag,
	  EBufferMode mode,
	  uint64_t commandQueueMask ) :
	  Info(info),
	  BufferSize( bufferSize ),
	  BindFlag( bindFlag ),
	  Usage( usage ),
	  CPUAccessFlag( cpuAccessFlag ),
	  Mode( mode ),
	  CommandQueueMask( commandQueueMask ) {}
};

struct BufferData
{
	const void *Data = nullptr;
	uint64_t DataSize = 0;

	BufferData() = default;
	BufferData( const void *data, uint64_t dataSize ) :
	  Data( data ),
	  DataSize( dataSize ) {}
};

class IBuffer : public IDeviceObject
{
public:
	void QueryInterface( const InterfaceID &iid, IEverything **interface ) override = 0;
	virtual IBufferView *CreateBufferView( const BufferViewDesc &desc ) = 0;
	virtual const BufferDesc &GetDesc() = 0;
	virtual void *GetNativeHandle() = 0;
};
}  // namespace fx
}  // namespace vm