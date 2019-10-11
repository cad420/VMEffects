
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
	SwapChain( IRefCnt *cnt ,IDevice * device,IContext * context):EverythingBase<Interface>( cnt ),m_device( device ),m_context( context )
	{}
	~SwapChain() = default;
private:
	Ref<IDevice> m_device;
	IContext * m_context = nullptr;		// weak reference temporary
};

}  // namespace fx
}  // namespace vm
