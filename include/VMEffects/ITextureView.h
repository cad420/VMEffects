#pragma once

#include "IDeviceObject.h"

namespace vm
{
namespace fx
{
struct TextureViewDesc
{
	ETextureViewType TextureViewType = VM_TVT_UNKNOWN;
	ETextureDim TextureDim = VM_TD_UNKNOWN;
	ETextureFormat TextureFormat = VM_TF_UNKNOWN;

	union
	{
		uint32_t ArrayOffset = 0;
		uint32_t DepthOffset;
	};

	union
	{
		uint32_t ArrayCount = 0;
		uint32_t DepthCount;
	};

	TextureViewDesc() = default;
	TextureViewDesc( ETextureViewType textureViewType,
					 ETextureDim textureDim,
					 ETextureFormat textureFormat,
					 uint32_t depthOrArrayOffset,
					 uint32_t depthOrArrayCount ) :
	  TextureViewType( textureViewType ),
	  TextureDim( textureDim ),
	  TextureFormat( textureFormat ),
	  ArrayOffset( depthOrArrayOffset ),
	  ArrayCount( depthOrArrayCount )
	{}
};

class ISampler;
class ITexture;

class ITextureView : public IDeviceObject
{
public:
	virtual const TextureViewDesc &GetDesc() const = 0;
	virtual void SetSampler( ISampler *sampler ) = 0;
	virtual ISampler *GetSampler() = 0;
	virtual ITexture *GetTexture() = 0;
};
}  // namespace fx
}  // namespace vm