
#include "HotGoal/HotGoal.h"

int main (int argc, char** argv)
{
	HotGoal hotgoal;

	while(1)
	{
		hotgoal.Tick();
		if(waitKey(30) >= 0) break;
		usleep((1/LOOP_RATE)*1000);
	}
}




