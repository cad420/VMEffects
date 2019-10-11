
#pragma once

#include <VMUtils/ieverything.hpp>
#include <VMUtils/ref.hpp>
#include <VMEffects/IDevice.h>
#include <VMEffects/IContext.h>

namespace vm
{
namespace fx
{
template <typename Interface>
class SwapChain : public EverythingBase<Interface>
{
public:
	SwapChain( IRefCnt *cnt ):EverythingBase<Interface>( cnt )
	{}
	~SwapChain() = default;
private:
	Ref<IDevice> m_device;
	IContext * m_context;		// weak reference temporary
};

}  // namespace fx
}  // namespace vm
