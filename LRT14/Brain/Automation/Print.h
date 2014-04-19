#ifndef PRINT_H_
#define PRINT_H_

#include "Automation.h"
#include <Rhesus.Toolkit.IO.h>

using namespace Rhesus::Toolkit::IO;

/*!
 * @brief Turns a set angle.
 */
class Print : public Automation
{
public:
	Print(std::string msg);

	virtual bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();
private:
	std::string m_msg;
};

#endif
