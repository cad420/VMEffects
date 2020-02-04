#pragma once

#include <VMUtils/ieverything.hpp>
#include <VMEffects/EngineType.h>
#include "ITextureView.h"

namespace vm
{
namespace fx
{
struct SwapChainDesc
{
	uint32_t Width = 0;
	uint32_t Height = 0;
	ETextureFormat ColorBufferFormat = VM_TF_RGBA;
	ETextureFormat DepthBufferFormat = VM_TF_DEPTH_COMPONENT32F;
	uint8_t DefaultStencilValue = 0;
	uint32_t BufferCount = 2;
	uint32_t SamplesCount = 1;
	const char * Info = nullptr;
	SwapChainDesc() {}
	SwapChainDesc( uint32_t width,
				   uint32_t height,
				   ETextureFormat colorBufferFormat,
				   ETextureFormat depthBufferFormat,
				   uint8_t defaultStencilValue,
				   uint32_t bufferCount,
				   uint32_t samplesCount,
					const char * info) :
	  Width( width ),
	  Height( height ),
	  ColorBufferFormat( colorBufferFormat ),
	  DepthBufferFormat( depthBufferFormat ),
	  DefaultStencilValue( defaultStencilValue ),
	  BufferCount( bufferCount ),
	  SamplesCount( samplesCount ),
	  Info(info)
	{
	}
};

class ISwapChain : public IEverything
{
public:
	virtual void Present( uint32_t interval ) = 0;
	virtual void Resize( uint32_t w, uint32_t h ) = 0;
	virtual ITextureView *GetCurrentFramebuffer() = 0;
	virtual ITextureView *GetDepthBuffer() = 0;
};

}  // namespace fx
}  // namespace vm