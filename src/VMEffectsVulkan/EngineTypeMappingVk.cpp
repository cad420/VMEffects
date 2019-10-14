
#include "EngineTypeMappingVk.hpp"
#include <VMUtils/log.hpp>

namespace vm
{
namespace fx
{
class ETextureFormatToVkMapping
{
public:
	ETextureFormatToVkMapping()
	{
		m_mapping[ VM_TF_UNKNOWN ] = VK_FORMAT_UNDEFINED;

		// 32bit per component type
		m_mapping[ VM_TF_RGBA32F ] = VK_FORMAT_R32G32B32A32_SFLOAT;
		m_mapping[ VM_TF_RGBA32I ] = VK_FORMAT_R32G32B32A32_SINT;
		m_mapping[ VM_TF_RGBA32UI ] = VK_FORMAT_R32G32B32A32_UINT;

		m_mapping[ VM_TF_RGB32F ] = VK_FORMAT_R32G32B32_SFLOAT;
		m_mapping[ VM_TF_RGB32I ] = VK_FORMAT_R32G32B32_SINT;
		m_mapping[ VM_TF_RGB32UI ] = VK_FORMAT_R32G32B32_UINT;

		m_mapping[ VM_TF_RG32F ] = VK_FORMAT_R32G32_SFLOAT;
		m_mapping[ VM_TF_RG32I ] = VK_FORMAT_R32G32_SINT;
		m_mapping[ VM_TF_RG32UI ] = VK_FORMAT_R32G32_UINT;

		m_mapping[ VM_TF_R32F ] = VK_FORMAT_R32_SFLOAT;
		m_mapping[ VM_TF_R32I ] = VK_FORMAT_R32_SINT;
		m_mapping[ VM_TF_R32UI ] = VK_FORMAT_R32_UINT;

		// 16bit per component type

		m_mapping[ VM_TF_RGBA16F ] = VK_FORMAT_R16G16B16A16_SFLOAT;
		m_mapping[ VM_TF_RGBA16I ] = VK_FORMAT_R16G16B16A16_SINT;
		m_mapping[ VM_TF_RGBA16UI ] = VK_FORMAT_R16G16B16A16_UINT;
		m_mapping[ VM_TF_RGBA16_SNORM ] = VK_FORMAT_R16G16B16A16_SNORM;
		m_mapping[ VM_TF_RGBA16_UNORM ] = VK_FORMAT_R16G16B16A16_SNORM;

		m_mapping[ VM_TF_RGB16F ] = VK_FORMAT_R16G16B16_SFLOAT;
		m_mapping[ VM_TF_RGB16I ] = VK_FORMAT_R16G16B16_SINT;
		m_mapping[ VM_TF_RGB16UI ] = VK_FORMAT_R16G16B16_UINT;
		m_mapping[ VM_TF_RGB16_SNORM ] = VK_FORMAT_R16G16B16_SNORM;
		m_mapping[ VM_TF_RGB16_UNORM ] = VK_FORMAT_R16G16B16_SNORM;

		m_mapping[ VM_TF_RG16F ] = VK_FORMAT_R16G16_SFLOAT;
		m_mapping[ VM_TF_RG16I ] = VK_FORMAT_R16G16_SINT;
		m_mapping[ VM_TF_RG16UI ] = VK_FORMAT_R16G16_UINT;
		m_mapping[ VM_TF_RG16_SNORM ] = VK_FORMAT_R16G16_SNORM;
		m_mapping[ VM_TF_RG16_UNORM ] = VK_FORMAT_R16G16_SNORM;

		m_mapping[ VM_TF_R16F ] = VK_FORMAT_R16_SFLOAT;
		m_mapping[ VM_TF_R16I ] = VK_FORMAT_R16_SINT;
		m_mapping[ VM_TF_R16UI ] = VK_FORMAT_R16_UINT;
		m_mapping[ VM_TF_R16_SNORM ] = VK_FORMAT_R16_SNORM;
		m_mapping[ VM_TF_R16_UNORM ] = VK_FORMAT_R16_SNORM;

		// 8bit per component type

		m_mapping[ VM_TF_RGBA8I ] = VK_FORMAT_R8G8B8A8_SINT;
		m_mapping[ VM_TF_RGBA8UI ] = VK_FORMAT_R8G8B8A8_UINT;
		m_mapping[ VM_TF_RGBA8_SNORM ] = VK_FORMAT_R8G8B8A8_SNORM;
		m_mapping[ VM_TF_RGBA8_UNORM ] = VK_FORMAT_R8G8B8A8_UNORM;

		m_mapping[ VM_TF_RGB8I ] = VK_FORMAT_R8G8B8_SINT;
		m_mapping[ VM_TF_RGB8UI ] = VK_FORMAT_R8G8B8_UINT;
		m_mapping[ VM_TF_RGB8_SNORM ] = VK_FORMAT_R8G8B8_SNORM;
		m_mapping[ VM_TF_RGB8_UNORM ] = VK_FORMAT_R8G8B8_UNORM;

		m_mapping[ VM_TF_RG8I ] = VK_FORMAT_R8G8_SINT;
		m_mapping[ VM_TF_RG8UI ] = VK_FORMAT_R8G8_UINT;
		m_mapping[ VM_TF_RG8_SNORM ] = VK_FORMAT_R8G8_SNORM;
		m_mapping[ VM_TF_RG8_UNORM ] = VK_FORMAT_R8G8_UNORM;

		m_mapping[ VM_TF_R8I ] = VK_FORMAT_R8_SINT;
		m_mapping[ VM_TF_R8UI ] = VK_FORMAT_R8_UINT;
		m_mapping[ VM_TF_R8_SNORM ] = VK_FORMAT_R8_SNORM;
		m_mapping[ VM_TF_R8_UNORM ] = VK_FORMAT_R8_UNORM;
	}

	VkFormat operator[]( ETextureFormat format )
	{
		return m_mapping[ format ];
	}

private:
	VkFormat m_mapping[ VM_TF_MAX_NUM ] = {};
};

VkFormat ETextureFormatToVkType( ETextureFormat format )
{
	static ETextureFormatToVkMapping instance;
	return instance[format];
}

VkFormat CompsitionTypeToVkType( EValueType valueType, uint32_t NumComponents, bool bIsNormalized )
{
	switch ( valueType ) {
	case VM_VT_FLOAT16: {
		Debug( "Floating point formats cannot be normalized" );
		switch ( NumComponents ) {
		case 1: return VK_FORMAT_R16_SFLOAT;
		case 2: return VK_FORMAT_R16G16_SFLOAT;
		case 4: return VK_FORMAT_R16G16B16A16_SFLOAT;
		default: Debug( "Unusupported number of components" ); return VK_FORMAT_UNDEFINED;
		}
	}

	case VM_VT_FLOAT32: {
		Debug( "Floating point formats cannot be normalized" );
		switch ( NumComponents ) {
		case 1: return VK_FORMAT_R32_SFLOAT;
		case 2: return VK_FORMAT_R32G32_SFLOAT;
		case 3: return VK_FORMAT_R32G32B32_SFLOAT;
		case 4: return VK_FORMAT_R32G32B32A32_SFLOAT;
		default: Debug( "Unusupported number of components" ); return VK_FORMAT_UNDEFINED;
		}
	}

	case VM_VT_INT32: {
		Debug(  "32-bit UNORM formats are not supported. Use R32_FLOAT instead" );
		switch ( NumComponents ) {
		case 1: return VK_FORMAT_R32_SINT;
		case 2: return VK_FORMAT_R32G32_SINT;
		case 3: return VK_FORMAT_R32G32B32_SINT;
		case 4: return VK_FORMAT_R32G32B32A32_SINT;
		default: Debug( "Unusupported number of components" ); return VK_FORMAT_UNDEFINED;
		}
	}

	case VM_VT_UINT32: {
		Debug(  "32-bit UNORM formats are not supported. Use R32_FLOAT instead" );
		switch ( NumComponents ) {
		case 1: return VK_FORMAT_R32_UINT;
		case 2: return VK_FORMAT_R32G32_UINT;
		case 3: return VK_FORMAT_R32G32B32_UINT;
		case 4: return VK_FORMAT_R32G32B32A32_UINT;
		default: Debug( "Unusupported number of components" ); return VK_FORMAT_UNDEFINED;
		}
	}

	case VM_VT_INT16: {
		if ( bIsNormalized ) {
			switch ( NumComponents ) {
			case 1: return VK_FORMAT_R16_SNORM;
			case 2: return VK_FORMAT_R16G16_SNORM;
			case 4: return VK_FORMAT_R16G16B16A16_SNORM;
			default: Debug( "Unusupported number of components" ); return VK_FORMAT_UNDEFINED;
			}
		} else {
			switch ( NumComponents ) {
			case 1: return VK_FORMAT_R16_SINT;
			case 2: return VK_FORMAT_R16G16_SINT;
			case 4: return VK_FORMAT_R16G16B16A16_SINT;
			default: Debug( "Unusupported number of components" ); return VK_FORMAT_UNDEFINED;
			}
		}
	}

	case VM_VT_UINT16: {
		if ( bIsNormalized ) {
			switch ( NumComponents ) {
			case 1: return VK_FORMAT_R16_UNORM;
			case 2: return VK_FORMAT_R16G16_UNORM;
			case 4: return VK_FORMAT_R16G16B16A16_UNORM;
			default: Debug( "Unusupported number of components" ); return VK_FORMAT_UNDEFINED;
			}
		} else {
			switch ( NumComponents ) {
			case 1: return VK_FORMAT_R16_UINT;
			case 2: return VK_FORMAT_R16G16_UINT;
			case 4: return VK_FORMAT_R16G16B16A16_UINT;
			default: Debug( "Unusupported number of components" ); return VK_FORMAT_UNDEFINED;
			}
		}
	}

	case VM_VT_INT8: {
		if ( bIsNormalized ) {
			switch ( NumComponents ) {
			case 1: return VK_FORMAT_R8_SNORM;
			case 2: return VK_FORMAT_R8G8_SNORM;
			case 4: return VK_FORMAT_R8G8B8A8_SNORM;
			default: Debug( "Unusupported number of components" ); return VK_FORMAT_UNDEFINED;
			}
		} else {
			switch ( NumComponents ) {
			case 1: return VK_FORMAT_R8_SINT;
			case 2: return VK_FORMAT_R8G8_SINT;
			case 4: return VK_FORMAT_R8G8B8A8_SINT;
			default: Debug( "Unusupported number of components" ); return VK_FORMAT_UNDEFINED;
			}
		}
	}

	case VM_VT_UINT8: {
		if ( bIsNormalized ) {
			switch ( NumComponents ) {
			case 1: return VK_FORMAT_R8_UNORM;
			case 2: return VK_FORMAT_R8G8_UNORM;
			case 4: return VK_FORMAT_R8G8B8A8_UNORM;
			default: Debug( "Unusupported number of components" ); return VK_FORMAT_UNDEFINED;
			}
		} else {
			switch ( NumComponents ) {
			case 1: return VK_FORMAT_R8_UINT;
			case 2: return VK_FORMAT_R8G8_UINT;
			case 4: return VK_FORMAT_R8G8B8A8_UINT;
			default: Debug( "Unusupported number of components" ); return VK_FORMAT_UNDEFINED;
			}
		}
	}

	default: Debug( "Unusupported format" ); return VK_FORMAT_UNDEFINED;
	}
}


}  // namespace fx
}  // namespace vm
