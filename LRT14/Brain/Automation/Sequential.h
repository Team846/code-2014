#ifndef SEQUENTIAL_H_
#define SEQUENTIAL_H_

#include "Automation.h"
#include <queue>
#include <vector>

/*!
 * @brief Container for sequentially executing automation routines.
 */
class Sequential : public Automation
{
public:
	/*!
	 * @brief Creates a sequential routine.
	 * @param name the name of the routine
     * @param queueIfBlocked whether the routine should be queued if blocked
	 * @param restartable whether the routine can be restarted
	 */
	Sequential(std::string name, bool queueIfBlocked = false, bool restartable = false);
	
	/*!
	 * @brief Creates a sequential routine initialized with a sequence of routines.
	 * @param name the name of the routine
	 * @param sequence the list of routines to initialize the sequential routine with
     * @param queueIfBlocked whether the routine should be queued if blocked
	 * @param restartable whether the routine can be restarted
	 */
	Sequential(std::string name, std::vector<Automation*> sequence, bool queueIfBlocked = false, bool restartable = false);
	virtual ~Sequential();

	virtual bool Start();
	virtual bool Abort();
	virtual bool Run();
	virtual void AllocateResources();
	
	/*!
	 * @brief Adds an automation routine to the sequence of routines.
	 * @param automation the routine to add
	 */
	void AddAutomation(Automation *automation);
	
	/*!
	 * @brief Adds a list of automation routine to the sequence of routines.
	 * @param automation the list of routines to add
	 */
	void AddAutomation(std::vector<Automation*> automation);
	
	/*!
	 * @brief Clears the sequence of routines.
	 */
	void ClearSequence();
	
protected:
	/*!
	 * @brief Checks whether the next step in the sequence can be started.
	 * @return whether the next step in the sequence can be started
	 */
	virtual bool ContinueNextStep();
	
	/*!
	 * @brief Gets the currently running routine.
	 * @return the currently running routine
	 */
	Automation* GetCurrentAutomation();
	
private:
	std::vector<Automation*> routines;
	std::queue<Automation*> queued;
    bool started;
};

#endif
