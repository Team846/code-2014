#include "ScriptProvider.h"

#include <fstream>
#include <streambuf>

using namespace Rhesus::Toolkit::Scripting;

ScriptProvider::~ScriptProvider()
{
	
}

bool ScriptProvider::LoadScript(std::string script)
{
	m_script = script;
	
	return true;
}

bool ScriptProvider::LoadFromFile(std::string file)
{
	std::ifstream s(file.c_str());
	
	m_script = std::string((std::istreambuf_iterator<char>(s)), std::istreambuf_iterator<char>());
	
	return true;
}
