
#pragma once

#include <VMEffectsVulkan/IContextVk.h>
#include <ContextBase.hpp>
#include "DeviceVkImpl.hpp"
#include <VMEffects/IPipeline.h>
#include <VMEffects/IBuffer.h>
#include <VMEffects/ITexture.h>

namespace vm
{
namespace fx
{
class ContextVkImpl : public ContextBase<IContextVk, DeviceVkImpl>
{
public:
	ContextVkImpl( IRefCnt *cnt ,DeviceVkImpl * device) :
	  ContextBase<vm::fx::IContextVk, vm::fx::DeviceVkImpl>( cnt ,device) {}
	void SetPipeline( IPipeline *pipline ) override
	{
		
	}
	void SetBlendFactors() override
	{
		
	}
	
	void SetStencilValue( uint32_t value ) override
	{
		
	}
	void SetViewport() override
	{
		
	}
	void SetVertexBuffer( IBuffer *buffer ) override
	{
		
	}

	void SetIndexBuffer( IBuffer *buffer ) override
	{
		
	}
	
	void Draw( DrawDesc &desc ) override
	{
		
	}
	void ExecCmdList( ICommandList *cmdList ) override
	{
		
	}

	void CopyBuffer() override
	{
		
	}
	
	void *MapBuffer() override
	{
		return nullptr;
	}
	
	void UnmapBuffer( IBuffer *buffer ) override
	{
		
	}

	void SetSwapChain( ISwapChain *swapChain ) override
	{
		
	}
	void EndFrame() override
	{
		
	}
};
}
}