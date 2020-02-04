
#pragma once

#include <VMUtils/ieverything.hpp>

namespace vm
{
namespace fx
{
class IEngineFactory :public IEverything
{
	void QueryInterface( const InterfaceID &iid, IEverything **interface ) override = 0;
};

}
}