
#pragma once
#include <cstdint>

namespace vm
{
namespace fx
{
enum EValueType : uint8_t
{
	VM_VT_UNNKOWN = 0,

	VM_VT_INT8,
	VM_VT_INT16,
	VM_VT_INT32,
	VM_VT_UINT8,
	VM_VT_UINT16,
	VM_VT_UINT32,
	VM_VT_FLOAT16,
	VM_VT_FLOAT32,
};

enum EBindFlagBits : uint32_t
{
	VM_BT_UNKNOWN = 0,
	VM_BT_VERTEX_BUFFER = 0x1L,
	VM_BT_INDEX_BUFFER = 0x2L,
	VM_BT_UNIFORM_BUFFER = 0x4L,
	VM_BT_SHADER_RESOURCE=0x8L,
	VM_BT_STREAM_OUTPUT = 0x10L,
	VM_BT_RENDER_TARGET = 0x20L,
	VM_BT_DEPTH_STENCIL = 0x40L,
	VM_BT_UNORDERED_ACCESS = 0x80,
	VM_BT_INDIRECT_DRAW_ARGS=0x100L
};
using BindFlags = uint32_t;

enum EUsage : uint8_t
{
	VM_USG_STATIC = 0,
	VM_USG_DEFAULT,
	VM_USG_DYNAMIC,
	VM_USG_STAGING
};

enum ECPUAccessBits
{
	VM_CA_NONE = 0x0,
	VM_CA_READ = 0x1,
	VM_CA_WRITE = 0x2
};
using CPUAccessFlags = uint32_t;

enum EBufferMode
{
	VM_BM_UNKNOWN=0,
	VM_BM_FORMATTED,
	VM_BM_STRUCTURED,
	VM_BM_RAW,
	VM_BM_MAX_NUM
};

enum ETextureDim : uint8_t
{
	VM_TD_UNKNOWN = 0,
	VM_TD_BUFFER,
	VM_TD_1D,
	VM_TD_1D_ARRAY,
	VM_TD_2D,
	VM_TD_2D_ARRAY,
	VM_TD_3D,
	VM_TD_CUBE,
	VM_TD_CUBE_ARRAY,
	VM_TD_MAX_NUM
};

enum ETextureFormat : uint16_t
{
	VM_TF_UNKNOWN = 0,
	VM_TF_RGB8,
	VM_TF_RGB16,
	VM_TF_RGBA,
	VM_TF_RGB5_A1,
	VM_TF_RGBA8,
	VM_TF_RGB10_A2,
	VM_TF_RGBA12,
	VM_TF_RGBA16,

	// ARB_texture_float / OpenGL 3
	VM_TF_RGBA32F,
	VM_TF_RGB32F,
	VM_TF_RGBA16F,
	VM_TF_RGB16F,

	// EXT_packed_depth_stencil / GL_ARB_framebuffer_object
	VM_TF_DEPTH_STENCIL,
	VM_TF_DEPTH24_STENCIL8,

	// ARB_depth_buffer_float
	VM_TF_DEPTH_COMPONENT32F,
	VM_TF_DEPTH32F_STENCIL8,

	// ARB_depth_texture
	VM_TF_DEPTH_COMPONENT,
	VM_TF_DEPTH_COMPONENT16,
	VM_TF_DEPTH_COMPONENT24,
	VM_TF_DEPTH_COMPONENT32,

	// GL_ARB_texture_rg
	VM_TF_RED,
	VM_TF_COMPRESSED_RED,  // = GL_COMPRESSED_RED,
	VM_TF_COMPRESSED_RG,   // = GL_COMPRESSED_RG,
	VM_TF_RG,			   //= GL_RG,
	VM_TF_R8,			   //= GL_R8,
	VM_TF_R16,			   //= GL_R16,
	VM_TF_RG8,			   //= GL_RG8,
	VM_TF_RG16,			   //= GL_RG16,
	VM_TF_R16F,			   //= GL_R16F,
	VM_TF_R32F,			   //= GL_R32F,
	VM_TF_RG16F,		   // = GL_RG16F,
	VM_TF_RG32F,		   // = GL_RG32F,
	VM_TF_R8I,			   // = GL_R8I,
	VM_TF_R8UI,			   // = GL_R8UI,
	VM_TF_R16I,			   // = GL_R16I,
	VM_TF_R16UI,		   // = GL_R16UI,
	VM_TF_R32I,			   // = GL_R32I,
	VM_TF_R32UI,		   // = GL_R32UI,
	VM_TF_RG8I,			   // = GL_RG8I,
	VM_TF_RG8UI,		   // = GL_RG8UI,
	VM_TF_RG16I,		   // = GL_RG16I,
	VM_TF_RG16UI,		   // = GL_RG16UI,
	VM_TF_RG32I,		   // = GL_RG32I,
	VM_TF_RG32UI,		   // = GL_RG32UI,

	// sRGB OpenGL 2.1
	//TF_SLUMINANCE_ALPHA = GL_SLUMINANCE_ALPHA,
	//TF_SLUMINANCE8_ALPHA8 = GL_SLUMINANCE8_ALPHA8,
	//TF_SLUMINANCE = GL_SLUMINANCE,
	//TF_SLUMINANCE8 = GL_SLUMINANCE8,
	//TF_COMPRESSED_SLUMINANCE = GL_COMPRESSED_SLUMINANCE,
	//TF_COMPRESSED_SLUMINANCE_ALPHA = GL_COMPRESSED_SLUMINANCE_ALPHA,

	// sRGB OpenGL 2.1 / 3.x
	VM_TF_SRGB,					  // = GL_SRGB,
	VM_TF_SRGB8,				  // = GL_SRGB8,
	VM_TF_SRGB_ALPHA,			  // = GL_SRGB_ALPHA,
	VM_TF_SRGB8_ALPHA8,			  // = GL_SRGB8_ALPHA8,
	VM_TF_COMPRESSED_SRGB,		  // = GL_COMPRESSED_SRGB,
	VM_TF_COMPRESSED_SRGB_ALPHA,  // = GL_COMPRESSED_SRGB_ALPHA,

	// from table 3.12 opengl api specs 4.1

	VM_TF_R8_SNORM,		 // = GL_R8_SNORM,
	VM_TF_R8_UNORM,		 //
	VM_TF_R16_SNORM,	 // = GL_R16_SNORM,
	VM_TF_R16_UNORM,	 //
	VM_TF_RG8_SNORM,	 // = GL_RG8_SNORM,
	VM_TF_RG8_UNORM,	 //
	VM_TF_RG16_SNORM,	// = GL_RG16_SNORM,
	VM_TF_RG16_UNORM,	//
	VM_TF_RGB8_SNORM,	// = GL_RGB8_SNORM,
	VM_TF_RGB8_UNORM,	//
	VM_TF_RGBA8_SNORM,   // = GL_RGBA8_SNORM,
	VM_TF_RGBA8_UNORM,   //
	VM_TF_RGBA16_SNORM,  // = GL_RGBA16_SNORM,
	VM_TF_RGBA16_UNORM,
	VM_TF_RGB16_SNORM,  // = GL_RGBA16_SNORM,
	VM_TF_RGB16_UNORM,

	VM_TF_RGB10_A2UI,  // = GL_RGB10_A2UI,

	VM_TF_RGB8I,	 // = GL_RGB8I,
	VM_TF_RGB8UI,	// = GL_RGB8UI,
	VM_TF_RGB16I,	// = GL_RGB16I,
	VM_TF_RGB16UI,   //= GL_RGB16UI,
	VM_TF_RGB32I,	// = GL_RGB32I,
	VM_TF_RGB32UI,   // = GL_RGB32UI,
	VM_TF_RGBA8I,	//= GL_RGBA8I,
	VM_TF_RGBA8UI,   // = GL_RGBA8UI,
	VM_TF_RGBA16I,   // = GL_RGBA16I,
	VM_TF_RGBA16UI,  // = GL_RGBA16UI,
	VM_TF_RGBA32I,   // = GL_RGBA32I,
	VM_TF_RGBA32UI,  // = GL_RGBA32UI

	VM_TF_MAX_NUM
};

enum EComponentType : uint8_t
{
	VM_CT_UNKNOWN = 0,
	VM_CT_FLOAT,
	VM_CT_SNORM,
	VM_CT_UNORM,
	VM_CT_UINT,
	VM_CT_INT,
	VM_CT_DEPTH,
	VM_CT_DEPTH_STENCIL,
	VM_CT_COMPRESSED,
	VM_CT_MAX_NUM
};

struct TextureFormatDesc
{
	const char *Name = nullptr;
	ETextureFormat Format = VM_TF_UNKNOWN;
	uint8_t ComponentSize = 0;
	uint8_t ComponentCount = 0;
	EComponentType ComponentType = VM_CT_UNKNOWN;
	TextureFormatDesc() = default;
	TextureFormatDesc(
	  const char *name,
	  ETextureFormat format,
	  uint8_t componentSize,
	  uint8_t componentCount,
	  EComponentType componentType
	  ) :
	  Name( name ),
	  Format( format ),
	  ComponentSize( componentSize ),
	  ComponentCount( componentCount ),
	  ComponentType( componentType )
	{
	}
};


enum ETextureViewType : uint8_t
{
	VM_TVT_UNKNOWN = 0,
	VM_TVT_SHADER_RESOURCES,
	VM_TVT_RENDER_TARGET,
	VM_TVT_DEPTH_STENCIL,
	VM_TVT_UNORDERER_ACCESS,
	VM_TVT_MAX_NUM
};

enum EBufferViewType : uint8_t
{
	VM_BVT_UNKNOWN = 0,
	VM_BVT_SHADER_RESOURCES,
	VM_BVT_UNORDERED_ACCESS,
	VM_BVT_MAX_NUM
};

};  // namespace fx

}  // namespace vm
