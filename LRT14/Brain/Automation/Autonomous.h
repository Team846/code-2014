#ifndef AUTONOMOUS_H_
#define AUTONOMOUS_H_

#include "Sequential.h"

#include <fstream>
#include <string>

#include "../../ComponentData/LauncherAngleData.h"

/*!
 * @brief Performs the autonomous routine.
 */
class Autonomous : public Sequential
{
public:
	Autonomous();
	~Autonomous();
	
	bool Start();
	void AllocateResources();
	
private:
	/*!
	 * @brief Reads the autonomous routine file and loads the routine sequence.
	 * @param path the path of the autonomous routine file
	 */
    void LoadRoutine(std::string path);
    
    std::string FindRoutine(int routineNumber);
    
	double m_autonomousStartTime;
	
	LauncherAngleData* m_angleData;
};

#endif
