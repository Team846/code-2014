#include "../Communication/OffboardCommunication.h"

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
