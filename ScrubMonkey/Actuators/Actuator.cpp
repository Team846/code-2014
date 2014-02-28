#include "Actuator.h"
#include <ErrorBase.h>
#include <Rhesus/Toolkit/IO/BufferedConsole.h>

using namespace Rhesus::Toolkit::IO;
using namespace std;

vector<Actuator*> Actuator::actuator_vector;

Actuator::Actuator(std::string name)
{
	m_name = name;
	actuator_vector.push_back(this);
}

std::string Actuator::GetName()
{
	return m_name;
}

void Actuator::OutputAll()
{
	for (vector<Actuator*>::iterator it = Actuator::actuator_vector.begin(); it < Actuator::actuator_vector.end(); it++)
	{
		if (dynamic_cast<ErrorBase*>(*it))
		{
			if (dynamic_cast<ErrorBase*>(*it)->StatusIsFatal())
			{
				BufferedConsole::Printf("[ERROR] Fatal error in %s: %s\n", (*it)->GetName().c_str(), dynamic_cast<ErrorBase*>(*it)->GetError().GetMessage());
			}
		}
		(*it)->Output();
	}
}

