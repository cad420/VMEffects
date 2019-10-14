
#pragma once

#include <VMUtils/ieverything.hpp>

#include "EngineType.h"
#include <VMEffects/ICommandList.h>
#include <VMEffects/ISwapChain.h>
#include <VMEffects/IPipeline.h>
#include <VMEffects/IBuffer.h>
#include <VMEffects/ICommandList.h>
#include <VMEffects/ISwapChain.h>

namespace vm
{
namespace fx
{

struct DrawDesc
{
	bool IsIndexed = false;
	uint32_t Count = 0;
	EValueType IndexType = VM_VT_UINT32;
	uint32_t InstanceCount = 1;
	
};


class IContext : public IEverything
{
public:
	virtual void SetPipeline(IPipeline * pipline) = 0;
	
	virtual void SetBlendFactors() = 0;
	
	virtual void SetStencilValue( uint32_t value ) = 0;
	
	virtual void SetViewport() = 0;
	
	virtual void SetVertexBuffer( IBuffer * buffer ) = 0;
	
	virtual void SetIndexBuffer( IBuffer *buffer ) = 0;
	
	virtual void Draw( DrawDesc &desc ) = 0;
	
	virtual void ExecCmdList(ICommandList * cmdList) = 0;

	virtual void CopyBuffer() = 0;

	virtual void *MapBuffer() = 0;

	virtual void UnmapBuffer(IBuffer * buffer) = 0;

	virtual void SetSwapChain( ISwapChain *swapChain ) = 0;

	virtual void EndFrame() = 0;
};
}
}
