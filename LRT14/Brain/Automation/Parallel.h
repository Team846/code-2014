#ifndef PARALLEL_H_
#define PARALLEL_H_

#include "Automation.h"
#include <vector>

/*!
 * @brief Container for routines executing in parallel.
 */
class Parallel : public Automation
{
public:
	/*!
	 * @brief Creates a parallel routine.
	 * @param name the name of the routine
     * @param queueIfBlocked whether the routine should be queued if blocked
	 * @param restartable whether the routine can be restarted
	 * @param abortOnFirst whether the routine should abort on the first completed routine
	 */
	Parallel(std::string name, bool queueIfBlocked = false, bool restartable = false, bool abortOnFirst = false);
	
	/*!
	 * @brief Creates a parallel routine initialized with a list of routines.
	 * @param name the name of the routine
	 * @param sequence the list of routines to initialize the parallel routine with
     * @param queueIfBlocked whether the routine should be queued if blocked
	 * @param restartable whether the routine can be restarted
	 * @param abortOnFirst whether the routine should abort on the first completed routine
	 */
	Parallel(std::string name, std::vector<Automation*> sequence, bool queueIfBlocked = false, bool restartable = false, bool abortOnFirst = false);
	virtual ~Parallel();

	virtual bool Start();
	virtual bool Abort();
	virtual bool Run();
	virtual void AllocateResources();
	
	/*!
	 * @brief Adds an automation routine to the parallel routines.
	 * @param automation the routine to add
	 */
	void AddAutomation(Automation *automation);
	
	/*!
	 * @brief Adds a list of automation routine to the parallel routines.
	 * @param automation the list of routines to add
	 */
	void AddAutomation(std::vector<Automation*> automation);
	
	/*!
	 * @brief Clears the list of parallel routines.
	 */
	void ClearAutomation();
	
private:
	std::vector<Automation*> routines;
	std::vector<Automation*> running;
	bool m_abortOnFirst;
};

#endif
