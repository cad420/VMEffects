#pragma once

#include <VMUtils/ieverything.hpp>
#include <VMEffects/BasicType.h>

namespace vm
{
namespace fx
{

struct SwapChainDesc
{
	uint32_t Width = 0;
	uint32_t Height = 0;
	TextureFormat ColorBufferFormat = VM_TF_RGBA;
	TextureFormat DepthBufferFormat = VM_TF_DEPTH_COMPONENT32F;
	uint8_t DefaultStencilValue = 0;
	uint32_t BufferCount = 2;
	uint32_t SamplesCount = 1;
	SwapChainDesc(uint32_t width,
		uint32_t height,
		TextureFormat colorBufferFormat,
		TextureFormat depthBufferFormat,
		uint8_t defaultStencilValue,
		uint32_t bufferCount,
		uint32_t samplesCount
	):Width( width ),
	Height( height ),
	ColorBufferFormat( colorBufferFormat ),
	DepthBufferFormat( depthBufferFormat ),
	DefaultStencilValue( defaultStencilValue ),
	BufferCount( bufferCount ),
	SamplesCount( samplesCount )
	{}
};

class ISwapChain : public IEverything
{
public:
	virtual void Present(uint32_t interval) = 0;
	virtual void Resize( uint32_t w, uint32_t h ) = 0;
};

}  // namespace fx
}  // namespace vm