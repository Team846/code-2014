#include "Autonomous.h"
#include "../../Config/RobotConfig.h"
#include "../../Config/DriverStationConfig.h"
#include <algorithm>
#include <dirent.h>
#include <Rhesus.Toolkit.h>
#include <Rhesus.Toolkit.Utilities.h>
#include <Rhesus/Toolkit/IO/BufferedConsole.h>

using namespace Rhesus::Toolkit;
using namespace Rhesus::Toolkit::IO;
using namespace Rhesus::Toolkit::Utilities;

#include "Drive.h"
#include "Turn.h"
#include "Arc.h"
#include "Collect.h"
#include "CollectorMove.h"
#include "LoadLauncher.h"
#include "Fire.h"
#include "Dribble.h"
#include "Pause.h"
#include "Print.h"
#include "LuaScript.h"
#include "Repeating.h"
#include "Parallel.h"
#include "FaceHotGoal.h"
#include "HotGoalWait.h"
#include "JitterTurn.h"

Autonomous::Autonomous() :
	Sequential("Autonomous")
{
	m_autonomousStartTime = 0.0;
	m_angleData = LauncherAngleData::Get();
}

Autonomous::~Autonomous()
{

}

bool Autonomous::Start()
{
	int autonRoutine = (int)(DriverStation::GetInstance()->GetAnalogIn(DriverStationConfig::AnalogIns::AUTONOMOUS_SELECT) + 0.5) + 1;
	BufferedConsole::Printf("Starting autonomous routine %d\n", autonRoutine);
	m_autonomousStartTime = Timer::GetFPGATimestamp();
	
	ClearSequence();
	
	double delay = DriverStation::GetInstance()->GetAnalogIn(DriverStationConfig::AnalogIns::AUTONOMOUS_DELAY);
	
	if (delay > 0.0)
	{
		AddAutomation(new Pause(delay));
	}
	
	LoadRoutine(FindRoutine(autonRoutine));
	ConfigRuntime::ConfigureAll();

	m_angleData->SetAngle(LauncherAngleData::LONG);
	
	return Sequential::Start();
}

void Autonomous::AllocateResources()
{
	AllocateResource(ControlResource::DRIVE);
	AllocateResource(ControlResource::TURN);
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::COLLECTOR_ROLLERS);
	AllocateResource(ControlResource::LAUNCHER_ANGLE);
	AllocateResource(ControlResource::LAUNCHER_LOADER);
	AllocateResource(ControlResource::BALL_HOLDER);
}

std::string Autonomous::FindRoutine(int routineNumber)
{
	DIR *dir = opendir("/"); 
	if(dir) 
	{ 
		struct dirent *ent; 
		while((ent = readdir(dir)) != NULL) 
		{ 
			std::string file(ent->d_name);
			if(file.find("routine") != std::string::npos && file.find(lexical_cast(routineNumber)) != std::string::npos)
				return "/" + file;
		} 
	} 
	return ""; 	
}

void Autonomous::LoadRoutine(std::string path)
{
	ifstream fin(path.c_str());
	if (!fin.is_open())
	{
		BufferedConsole::Printf("Cannot open autonomous routine file: %s\n", path.c_str());
		return;
	}

	int lineNumber = 1;
	while (!fin.eof())
	{
		// Go through the file line by line for each command
		string line;
		getline(fin, line);
		stringstream nocomments(line);
		getline(nocomments, line, '#');
		line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end()); // Remove spaces
		if (line.length() == 0)
			continue;
		vector<string> routineList;
		unsigned int pos;
		while ((pos = line.find("),")) != std::string::npos) // Look for parallel commands
		{
			routineList.push_back(line.substr(0, pos + 1));
			line.erase(0, pos + 2);
		}
		routineList.push_back(line);
		vector<Automation*> parallelRoutines;
		for (vector<string>::iterator it = routineList.begin(); it < routineList.end(); it++)
		{
			Automation *current;
			stringstream sstream(*it);
			string command, args;
			getline(sstream, command, '('); // Get command name

			getline(sstream, args, ')');

			stringstream argstream(args);
			string temp;
			vector<string> arglist;

			// Check for commas delimiting arguments
			while (getline(argstream, temp, ','))
			{
				arglist.push_back(temp);
			}

			bool failed = false;

			if (command == "drive")
			{
				if (arglist.size() == 1)
					current = new Drive(lexical_cast<double>(arglist[0]));
				else if (arglist.size() == 2)
					current = new Drive(lexical_cast<double>(arglist[0]),
							lexical_cast<double>(arglist[1]));
				else if (arglist.size() == 3)
					current = new Drive(lexical_cast<double>(arglist[0]),
							lexical_cast<double>(arglist[1]),
							lexical_cast<double>(arglist[2]));
				else if (arglist.size() == 4)
					current = new Drive(lexical_cast<double>(arglist[0]),
							lexical_cast<double>(arglist[1]),
							lexical_cast<double>(arglist[2]),
							lexical_cast<bool>(arglist[3]));
				else
					failed = true;
			}
			else if (command == "turn")
			{
				if (arglist.size() == 1)
					current = new Turn(lexical_cast<double>(arglist[0]));
				else if (arglist.size() == 2)
					current = new Turn(lexical_cast<double>(arglist[0]),
							lexical_cast<double>(arglist[1]));
				else if (arglist.size() == 3)
					current = new Turn(lexical_cast<double>(arglist[0]),
							lexical_cast<double>(arglist[1]),
							lexical_cast<double>(arglist[2]));
				else
					failed = true;
			}
			else if (command == "arc")
			{
				if (arglist.size() == 2)
					current = new Arc(lexical_cast<double>(arglist[0]),
							lexical_cast<double>(arglist[1]));
				else if (arglist.size() == 3)
					current = new Arc(lexical_cast<double>(arglist[0]),
							lexical_cast<double>(arglist[1]),
							lexical_cast<double>(arglist[2]));
				else if (arglist.size() == 4)
					current = new Arc(lexical_cast<double>(arglist[0]),
							lexical_cast<double>(arglist[1]),
							lexical_cast<double>(arglist[2]),
							lexical_cast<double>(arglist[3]));
				else if (arglist.size() == 5)
					current = new Arc(lexical_cast<double>(arglist[0]),
							lexical_cast<double>(arglist[1]),
							lexical_cast<double>(arglist[2]),
							lexical_cast<double>(arglist[3]),
							lexical_cast<double>(arglist[4]));
				else
					failed = true;
			}
			else if (command == "fire")
			{
				if (arglist.size() == 0)
					current = new Fire();
				else if (arglist.size() == 1)
					current = new Fire(lexical_cast<bool>(arglist[0]));
				else
					failed = true;
			}
			else if (command == "collect")
			{
				if (arglist.size() == 0)
					current = new Collect();
				else
					failed = true;
			}
			else if (command == "load")
			{
				if (arglist.size() == 0)
					current = new LoadLauncher();
				else
					failed = true;
			}
			else if (command == "dribble")
			{
				Automation* drive;
				double wait = 1.0;
				if (arglist.size() == 1)
					drive = new Drive(lexical_cast<double>(arglist[0]));
				else if (arglist.size() == 2)
				{
					drive = new Drive(lexical_cast<double>(arglist[0]));
					wait = lexical_cast<double>(arglist[1]);
				}
				else if (arglist.size() == 3)
				{
					drive = new Drive(lexical_cast<double>(arglist[0]),
							lexical_cast<double>(arglist[2]));
					wait = lexical_cast<double>(arglist[1]);
				}
				else if (arglist.size() == 4)
				{
					drive = new Drive(lexical_cast<double>(arglist[0]),
							lexical_cast<double>(arglist[2]),
							lexical_cast<double>(arglist[3]));
					wait = lexical_cast<double>(arglist[1]);
				}
				else if (arglist.size() == 5)
				{
					drive = new Drive(lexical_cast<double>(arglist[0]),
							lexical_cast<double>(arglist[2]),
							lexical_cast<double>(arglist[3]),
							lexical_cast<bool>(arglist[4]));
					wait = lexical_cast<double>(arglist[1]);
				}
				else
					failed = true;
				if (!failed)
				{
					current = new Parallel("DribbleDrive", false, false, true);
					Sequential* dribble = new Sequential("DribbleDriveWait");
					dribble->AddAutomation(new Pause(wait));
					dribble->AddAutomation(drive);
					((Parallel*)current)->AddAutomation(dribble);
					((Parallel*)current)->AddAutomation(new Dribble());
				}
			}
			else if (command == "collect_move")
			{
				if (arglist.size() == 1)
					current = new CollectorMove(lexical_cast<bool>(arglist[0]));
				else
					failed = true;
			}
			else if (command == "wait")
			{
				if (arglist.size() == 1)
					current = new Pause(lexical_cast<double>(arglist[0]));
				else
					failed = true;
			}
			else if (command == "print")
			{
				if (arglist.size() == 1)
					current = new Print(arglist[0]);
				else
					failed = true;
			}
			else if (command == "face_hot_goal")
			{
				if(arglist.size() == 1)
					current = new FaceHotGoal(lexical_cast<double>(arglist[0]), lexical_cast<bool>(arglist[1]), false);
				else if(arglist.size() == 2)
					current = new FaceHotGoal(lexical_cast<double>(arglist[0]), lexical_cast<bool>(arglist[1]), false);
				else if(arglist.size() == 3)
					current = new FaceHotGoal(lexical_cast<double>(arglist[0]), lexical_cast<bool>(arglist[1]), lexical_cast<bool>(arglist[2]));
				else if(arglist.size() == 5)
					current = new FaceHotGoal(lexical_cast<double>(arglist[0]), lexical_cast<bool>(arglist[1]), lexical_cast<bool>(arglist[2]), lexical_cast<double>(arglist[3]), lexical_cast<double>(arglist[4]));
				else
					failed = true;
			}
			else if(command == "clear_hot_goal")
			{
				FaceHotGoal::Reset();
				BufferedConsole::Printfln("Resetting FaceHotGoal...");
			}
			else if(command == "wait_hot_goal")
			{
				if(arglist.size() == 2 && (arglist[0] == "left" || arglist[0] == "right"))
					current = new HotGoalWait(arglist[0],lexical_cast<double>(arglist[1]));
				else
					failed = true;
			}
			else if(command == "jitter_turn")
			{
				if(arglist.size() == 3)
				{
					current = new JitterTurn(lexical_cast<double>(arglist[0]), lexical_cast<double>(arglist[1]), lexical_cast<double>(arglist[2]));
				}
				else
					failed = true;
			}
			else if(command == "lua")
			{
				if(arglist.size() == 1)
				{
					current = new LuaScript(lexical_cast<std::string>(arglist[0]));
				}
				else
					failed = true;
			}
			else
			{
				BufferedConsole::Printf(
						"[WARNING] Unknown routine: %s on line %d, ignoring.\n",
						command.c_str(), lineNumber);
				continue;
			}
			if (failed)
			{
				BufferedConsole::Printf(
						"[WARNING] Incorrect number of arguments for routine: %s on line %d, ignoring.\n",
						command.c_str(), lineNumber);
				continue;
			}
			parallelRoutines.push_back(current);
		}
		if (parallelRoutines.size() > 1)
		{
			AddAutomation(new Parallel("AutonomousParallel", parallelRoutines));
		}
		else if (parallelRoutines.size() == 1)
		{
			AddAutomation(parallelRoutines[0]);
		}
		lineNumber++;
	}
	BufferedConsole::Printf("Done loading autonomous routine file: %s\n", path.c_str());
	fin.close();
}

