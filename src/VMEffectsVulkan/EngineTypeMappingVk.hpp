
#pragma once

#include <vulkan/vulkan.h>
#include <VMEffects/EngineType.h>

namespace vm
{
namespace fx
{

VkFormat ETextureFormatToVkType( ETextureFormat format );

VkFormat CompsitionTypeToVkType( EValueType valueType, uint32_t numComp, bool normalized );


}
}