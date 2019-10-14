
#pragma once

#include <VMEffects/IBuffer.h>
#include <vulkan/vulkan.h>

namespace vm
{
namespace fx
{
class IBufferVk : public IBuffer
{
public:
	virtual VkBuffer GetVkBuffer() const = 0;
};
}  // namespace fx
}  // namespace vm