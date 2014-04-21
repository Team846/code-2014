#include "LRTRobot14.h"

#include "Brain/Brain.h"
#include "Components/Component.h"
#include "Actuators/Actuator.h"
#include "Actuators/AsyncCANJaguar.h"
#include "Actuators/Pneumatics.h"
#include "RobotState.h"
#include "DriverStation/LRTDriverStation.h"
#include "ComponentData/ComponentData.h"
#include "Sensors/RobotLocation.h"
#include "Sensors/SensorFactory.h"
#include "Sensors/CheesyVisionServer.h"
#include "Maintenance.h"

#include "Config/ConfigRuntime.h"
#include "Config/ConfigPortMappings.h"
#include "Config/RobotConfig.h"
#include "Config/DriverStationConfig.h"
#include "Utils/LCD.h"
#include "Logging/Logger.h"
#include "Communication/Dashboard2.h"
#include "Communication/DashboardTelemetryID.h"
#include "Communication/LiveNetworkSender.h"
#include "Communication/OffboardCommunication.h"

#include "Brain/Automation/FaceHotGoal.h"

#include <Rhesus.Toolkit.h>
#include <Rhesus.Toolkit.Diagnostics.h>
#include <Rhesus.Toolkit.IO.h>
#include <Rhesus.Toolkit.Scripting.h>
#include <Rhesus.Toolkit.Tasks.h>

using namespace Rhesus::Toolkit;
using namespace Rhesus::Toolkit::Diagnostics;
using namespace Rhesus::Toolkit::IO;
using namespace Rhesus::Toolkit::Scripting;

bool maintenanceMode = false;
bool showProfiler = false;
int tickCount = 0;

LRTRobot14::LRTRobot14()
{
	_watchdog = wdCreate();
	
	printf("LRTRobot14 Constructed\n");
}

LRTRobot14::~LRTRobot14()
{
	printf("LRTRobot14 Destructing\n");
	
	Dashboard2::Close();
	
	// Stop all tasks
	for (vector<AsyncCANJaguar*>::iterator it = AsyncCANJaguar::jaguar_vector.begin(); it < AsyncCANJaguar::jaguar_vector.end(); it++)
	{
		(*it)->Abort();
	}
	
	// Delete all singletons
	Component::DestroyComponents();
	ComponentData::Finalize();
	ConfigPortMappings::Finalize();
	ConfigRuntime::Finalize();
	Logger::Finalize();
	LiveNetworkSender::Finalize();
	OffboardCommunication::Finalize();
#if PNEUMATICS
	Pneumatics::DestroyCompressor();
#endif
	Brain::Finalize();
	SensorFactory::Finalize();
	LCD::Finalize();
	LRTDriverStation::Finalize();
	RobotState::Finalize();
}

void LRTRobot14::RobotInit()
{
	// Initialize global robot state object
	RobotState::Initialize();
	
	// Initialize Utilities
	LCD::Instance()->Start();

	// Create the Driver Station
	BufferedConsole::Printfln("Initializing Driver Station...");
	LRTDriverStation::Initialize();
	
	// Read port mappings
	BufferedConsole::Printfln("Loading Port Mappings...");
	ConfigPortMappings::Instance()->Load();
	
	// Create ComponentData
	BufferedConsole::Printfln("Creating ComponentData...");
	ComponentData::Initialize();
	
	// Create all components
	BufferedConsole::Printfln("Creating Components...");
	Component::CreateComponents();
	
	// Initialize the Brain
	BufferedConsole::Printfln("Initializing Brain...");
	Brain::Initialize();
	
	CheesyVisionServer::GetInstance()->StartListening();
	
	
	// Start AsyncCANJaguar tasks
	BufferedConsole::Printfln("Starting AsyncCANJaguar Tasks...");
	for (vector<AsyncCANJaguar*>::iterator it = AsyncCANJaguar::jaguar_vector.begin(); it < AsyncCANJaguar::jaguar_vector.end(); it++)
	{
		(*it)->Start();
	}

#if PNEUMATICS
	// Create and start compressor
	BufferedConsole::Printfln("Creating Pneumatics Compressor...");
	Pneumatics::CreateCompressor();
#endif

	// Initialize SensorFactory
	BufferedConsole::Printfln("Initializing Sensor Factory...");
	SensorFactory::Initialize();
	
	// Initialize localization
	BufferedConsole::Printfln("Initializing Robot Localization...");
	RobotLocation::Initialize();
	
	// Initialize offboard communication
	BufferedConsole::Printfln("Initializing Offboard Communication...");
	OffboardCommunication::Initialize();

	// Initialize the Logger
	BufferedConsole::Printfln("Initializing Logger...");
	Logger::Instance()->Initialize();
	
	// Initialize the LiveNetworkSender
	BufferedConsole::Printfln("Initializing LiveNetworkSender...");
	LiveNetworkSender::Initialize();
	
	// Initialize dashboard telemetry types
	BufferedConsole::Printfln("Setting up dashboard telemetry...");
	
	// Apply runtime configuration
	ConfigRuntime::ConfigureAll();
	
	Dashboard2::Create();
	
	Dashboard2::InitializeTelemetry();
	
	Dashboard2::LogToConsole(true);
}

static int TimeoutCallback(...)
{
	Dashboard2::LogW("Main loop execution time > 20 ms");
	
	BufferedConsole::Printfln("======================================");
	BufferedConsole::Printfln("PROFILED TIMES (over 20ms):");
	BufferedConsole::Printfln("");
	BufferedConsole::Printfln("Activity\tLast Time (ms)");
	std::hash_map<std::string, double> times = Profiler::CloneLastTimes();
	
	for(std::hash_map<std::string, double>::iterator it = times.begin(); it != times.end(); ++it)
	{
		std::string name = it->first;
		double time = it->second;
		
		BufferedConsole::Printfln("%s:\t\t%lf ms", name.c_str(), time);
	}
	
	BufferedConsole::Printfln("======================================");
	return 0;
}

void LRTRobot14::Tick()
{
	wdStart(_watchdog, sysClkRateGet() / RobotConfig::LOOP_RATE,
			TimeoutCallback, 0);

	static int tickCount = 0;
	
	Profiler::StartActivity("LRTRobot14::Tick()");

	Profiler::StartActivity("RobotState::Update()");
	// Update global robot state object
	RobotState::Instance().Update();
	Profiler::End("RobotState::Update()");

	// Redirect all prints to a file when console is blocked during matches
	if (RobotState::Instance().FMSAttached() && RobotState::Instance().GameMode() != GameState::DISABLED)
	{
		IOUtils::RedirectOutputToFile(RobotConfig::PRINT_FILE_PATH.c_str());
	}
	else
	{
		IOUtils::RedirectOutputToConsole();
	}

	Profiler::StartActivity("RobotLocation::Zero()");
	// Zero robot location if enabled
	if (RobotState::Instance().GameMode() != GameState::DISABLED && RobotState::Instance().LastGameMode() == GameState::DISABLED)
	{
		RobotLocation::Instance()->Zero();
	}
	Profiler::End("RobotLocation::Zero()");

	Profiler::StartActivity("LRTDriverStation::Update()");
	// Update the Driver Station
	LRTDriverStation::Update();
	Profiler::End("LRTDriverStation::Update()");

	// Update offboard
//	OffboardCommunication::Instance()->Update();
	
	if (maintenanceMode)
	{
		// Run maintenance mode
		Maintenance::Update();		
	}
	else
	{
		Profiler::StartActivity("Brain::Update()");
		// Update the Brain
		Brain::Instance()->Update();
		Profiler::End("Brain::Update()");

		Profiler::StartActivity("Component::UpdateAll()");
		// Update all components
		Component::UpdateAll();	
		Profiler::End("Component::UpdateAll()");
	}

	Profiler::StartActivity("Actuator::OutputAll()");
	// Flush outputs to all actuators
	Actuator::OutputAll();
	Profiler::End("Actuator::OutputAll()");

#if PNEUMATICS
	// Toggle compressor based on Driver Station switches
	if (DriverStation::GetInstance()->GetDigitalIn(DriverStationConfig::DigitalIns::COMPRESSOR))
	{
		Pneumatics::SetCompressor(true);
	}
	else
	{
		Pneumatics::SetCompressor(false);
	}
#endif

	Profiler::StartActivity("ConfigRuntime::CheckForFileUpdates()");
	// Check for runtime configuration file changes
	if (RobotState::Instance().GameMode() == GameState::DISABLED)
	{
		ConfigRuntime::Instance()->CheckForFileUpdates();
	}
	Profiler::End("ConfigRuntime::CheckForFileUpdates()");
	
//	HotGoal::Side side = FaceHotGoal::LastHotGoalSide();
//	std::string sideStr = "???";
//	
//	if(side == HotGoal::LEFT) sideStr = "LEFT";
//	else if(side == HotGoal::NONE_ACTIVE) sideStr = "NONE_ACTIVE";
//	else if(side == HotGoal::RIGHT) sideStr = "RIGHT";
//	else sideStr = "???";
//	
//	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::AUTON_HOT_GOAL_LAST_SIDE, sideStr);

	Profiler::StartActivity("LCD::RunOneCycle()");
	// Utilities
	LCD::Instance()->RunOneCycle();
	Profiler::End("LCD::RunOneCycle()");
//	Logger::Instance()->Run();
	Profiler::StartActivity("LiveNetworkSender::Run()");
	if (DriverStation::GetInstance()->GetDigitalIn(DriverStationConfig::DigitalIns::NETWORK))
	{
		LiveNetworkSender::Instance()->Run();
		Dashboard2::Tick();
	}
	Profiler::End("LiveNetworkSender::Run()");

	Profiler::StartActivity("ComponentData::ResetAllCommands()");
	// Reset ComponentData command fields
	ComponentData::ResetAllCommands();
	Profiler::End("ComponentData::ResetAllCommands()");
	
	tickCount++;
	
	Profiler::End("LRTRobot14::Tick()");
	
	wdCancel(_watchdog);
	
	if(showProfiler && ((tickCount - 1) % (RobotConfig::LOOP_RATE * 5)) == 0) // -1 because tickCount is incremented before here
	{
		BufferedConsole::Printfln("===========================================================================");
		BufferedConsole::Printfln("PROFILER:");
		BufferedConsole::Printfln("Current Tick: %d", (tickCount - 1));
		BufferedConsole::Printfln("");
		BufferedConsole::Printfln("Activity\tLast Time (ms)\tAverage Time (ms)\tGreatest Time (ms)");
		BufferedConsole::Printfln("---------------------------------------------------------------------------");
		std::hash_map<std::string, double> times = Profiler::CloneLastTimes();
		std::hash_map<std::string, double> avgTimes = Profiler::CloneAverageTimes();
		std::hash_map<std::string, double> maxTimes = Profiler::CloneMaxTimes();
		
		for(std::hash_map<std::string, double>::iterator it = times.begin(); it != times.end(); ++it)
		{
			std::string name = it->first;
			double time = it->second;
			
			std::hash_map<std::string, double>::iterator avgIt = avgTimes.find(name);
			double avgTime = (avgIt == times.end()) ? -1.0 : avgIt->second;
			
			std::hash_map<std::string, double>::iterator maxIt = maxTimes.find(name);
			double maxTime = (maxIt == times.end()) ? -1.0 : maxIt->second;
			
			BufferedConsole::Printfln("%s:\t\t%lf ms\t\t%lf ms\t\t\t%lf ms", name.c_str(), time, avgTime, maxTime);
		}
		
		BufferedConsole::Printfln("===========================================================================\n\n");
	}
}

void maintenance()
{
	if (RobotState::Instance().GameMode() == GameState::DISABLED && !maintenanceMode)
	{
		maintenanceMode = true;
		BufferedConsole::Printfln("Maintenance mode entered");
	}
	else if (maintenanceMode)
		BufferedConsole::Printfln("Already in maintenance mode");
	else
		BufferedConsole::Printfln("Please disable to enter maintenance mode");
}

void exit()
{
	if(RobotState::Instance().GameMode() == GameState::DISABLED && maintenanceMode)
	{
		maintenanceMode = false;
		BufferedConsole::Printfln("Maintenance mode exited");
	}
	else if (!maintenanceMode)
		BufferedConsole::Printfln("Not in maintenance mode");
	else
		BufferedConsole::Printfln("Please disable to exit maintenance mode");
}

void profiler()
{
	showProfiler = !showProfiler;
	
	BufferedConsole::Printfln("Display profiler: %s", (showProfiler ? "true" : "false"));
}

void running()
{
	Brain::Instance()->PrintRunningAutomation();
}
