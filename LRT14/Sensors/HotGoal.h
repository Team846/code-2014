#ifndef HOTGOAL_H_
#define HOTGOAL_H_

#include "../Communication/OffboardCommunication.h"
#include <vector>
 
class HotGoal
{
public:	
	enum Side
	{
		LEFT = 0x00,
		RIGHT = 0x01,
		NONE_ACTIVE = 0x02
	};
	
	HotGoal();
	~HotGoal();
	
	Side GetActiveSide();
	
private:
	OffboardCommunication* m_offboard;
};

#endif
