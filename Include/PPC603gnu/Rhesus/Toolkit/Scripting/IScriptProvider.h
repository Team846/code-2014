#ifndef RHESUS_ISCRIPTPROVIDER_H_
#define RHESUS_ISCRIPTPROVIDER_H_

#include <string>

namespace Rhesus 
{
namespace Toolkit 
{
namespace IO 
{

class IScriptProvider
{
	virtual bool RunScript(std::string script) = 0;
	virtual bool RunFromFile(std::string file) = 0;
};

}
}
}

#endif
