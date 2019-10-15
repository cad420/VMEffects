#pragma once

#include "IDeviceObject.h"
#include "ITextureView.h"
#include "EngineType.h"
#include "IBuffer.h"

namespace vm
{
namespace fx
{
struct TextureDesc
{
	uint32_t Width = 0;
	uint32_t Height = 0;
	union
	{
		uint32_t ArraySize = 1;
		uint32_t Depth;
	};
	ETextureFormat TextureFormat = VM_TF_UNKNOWN;
	ETextureDim TextureDim = VM_TD_UNKNOWN;
	uint32_t MipMapLevels = 1;
	uint32_t SampleCount = 1;
	EUsage Usage = VM_USG_DEFAULT;
	BindFlags BindFlag;
	TextureDesc() = default;
	TextureDesc(
	  uint32_t width,
	  uint32_t height,
	  uint32_t depthOrArraySize,
	  ETextureFormat textureFormat,
	  ETextureDim textureDim,
	  uint32_t mipMapLevels,
	  uint32_t sampleCount,
	  EUsage usage,
	 BindFlags bindFlags) :
	  Width( width ),
	  Height( height ),
	  ArraySize( depthOrArraySize ),
	  TextureFormat( textureFormat ),
	  TextureDim( textureDim ),
	  MipMapLevels( mipMapLevels ),
	  SampleCount( sampleCount ),
	  Usage( usage ),
	  BindFlag(bindFlags)
	{
	}
};

struct TextureResource
{
	const void *DataPtr = nullptr;

	IBuffer *DataBuffer = nullptr;
	uint64_t Offset = 0;
	uint64_t Column = 0;
	uint64_t Depth = 0;

	TextureResource() = default;
	TextureResource( const void *dataPtr,
					 IBuffer *dataBuffer,
					 uint64_t offset,
					 uint64_t column,
					 uint64_t depth ) :
	  DataPtr( dataPtr ),
	  DataBuffer( dataBuffer ),
	  Offset( offset ),
	  Column( column ),
	  Depth( depth ) {}
};

struct TextureData
{
	TextureResource *TexResPtr = nullptr;
	uint32_t ResCount = 0;
	TextureData() = default;
	TextureData( TextureResource *texResPtr,
				 uint32_t resCount ) :
	  TexResPtr( texResPtr ),
	  ResCount( resCount ) {}
};

class ITexture : public IDeviceObject
{
public:
	virtual const TextureDesc &GetDesc() const = 0;
	virtual ITextureView *CreateTextureView( const TextureViewDesc &desc ) = 0;
	virtual void *GetNativeHandle() = 0;
};
}  // namespace fx
}  // namespace vm