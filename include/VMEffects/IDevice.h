
#pragma once
#include <VMUtils/ieverything.hpp>
#include "IBuffer.h"
#include "ITexture.h"
#include "ISampler.h"
#include "IPipeline.h"
#include "IShader.h"

namespace vm
{
namespace fx
{
class IDevice : public IEverything
{
public:
	void QueryInterface( const InterfaceID &iid, IEverything **interface ) override = 0;

	virtual IShader * CreateShader( const ShaderDesc &desc ) = 0;

	virtual IBuffer * CreateBuffer( const BufferDesc &desc ) = 0;

	virtual ITexture * CreateTexture( const TextureDesc &desc ) = 0;

	virtual ISampler * CreateSampler( const SamplerDesc &desc ) = 0;

	virtual IPipeline * CreatePipeline( const PipelineDesc &desc ) = 0;

	//Create Shader
	//Create Sampler
	//Create Texture
	//Create pipeline
	//Create Fence
	//
	
};
}  // namespace fx
}  // namespace vm