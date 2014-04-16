#ifndef RHESUS_SCRIPTPROVIDER_H_
#define RHESUS_SCRIPTPROVIDER_H_

#include <string>

#include "ExecutionState.h"

#include "../Utilities/Generic.h"

namespace Rhesus 
{
namespace Toolkit 
{
namespace Scripting 
{

class ScriptProvider
{
public:
	virtual ~ScriptProvider();
	
	virtual bool LoadScript(std::string script);
	virtual bool LoadFromFile(std::string file);
	
	virtual void CreateContext() = 0;
	
	virtual ExecutionState::Enum Step() = 0;
	
protected:
	std::string m_script;
};

}
}
}

#endif
