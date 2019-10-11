#pragma once

#include <VMUtils/ieverything.hpp>


namespace vm
{
namespace fx
{

class IDeviceObject : public IEverything
{
public:
	void QueryInterface( const InterfaceID &iid, IEverything **interface ) override = 0;
	
};

}
}
