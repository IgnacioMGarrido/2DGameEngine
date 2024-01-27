#pragma once

namespace Core
{
class IService
{
	virtual bool Init() = 0;
	virtual void Shutdown() = 0;
};
} // namespac core
