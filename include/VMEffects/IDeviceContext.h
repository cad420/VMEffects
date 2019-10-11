
#pragma once

#include <VMUtils/ieverything.hpp>


namespace vm
{
namespace fx
{
class IContext : public IEverything
{
public:
	virtual void SetPipeline();
};
}
}
