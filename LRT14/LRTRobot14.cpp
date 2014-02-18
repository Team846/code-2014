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
#include "Maintenance.h"

#include "Config/ConfigRuntime.h"
#include "Config/ConfigPortMappings.h"
#include "Config/RobotConfig.h"
#include "Config/DriverStationConfig.h"
#include "Utils/LCD.h"
#include "Logging/Logger.h"
#include "Communication/Dashboard2.h"
#include "Communication/LiveNetworkSender.h"
#include "Communication/OffboardCommunication.h"

#include "Rhesus/Toolkit/GameState.h"
#include "Rhesus/Toolkit/Tasks/Rhesus.Toolkit.Tasks.h"

#include <Rhesus/Toolkit/IO/IOUtils.h>
#include <Rhesus/Toolkit/IO/BufferedConsole.h>

using namespace Rhesus::Toolkit::IO;

bool maintenanceMode = false;

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
	
	// Apply runtime configuration
	ConfigRuntime::ConfigureAll();
}

static int TimeoutCallback(...)
{
	printf("Main loop execution time > 20 ms\n");
	return 0;
}

void LRTRobot14::Tick()
{
	wdStart(_watchdog, sysClkRateGet() / RobotConfig::LOOP_RATE,
			TimeoutCallback, 0);
	
	// Update global robot state object
	RobotState::Instance().Update();
	
	// Redirect all prints to a file when console is blocked during matches
	if (RobotState::Instance().FMSAttached() && RobotState::Instance().GameMode() != GameState::DISABLED)
	{
		IOUtils::RedirectOutputToFile(RobotConfig::PRINT_FILE_PATH.c_str());
	}
	else
	{
		IOUtils::RedirectOutputToConsole();
	}
	
	// Zero robot location if enabled
	if (RobotState::Instance().GameMode() != GameState::DISABLED && RobotState::Instance().LastGameMode() == GameState::DISABLED)
	{
		RobotLocation::Instance()->Zero();
	}
	
	// Update the Driver Station
	LRTDriverStation::Update();

	if (maintenanceMode)
	{
		// Run maintenance mode
		Maintenance::Update();		
	}
	else
	{
		// Update the Brain
		Brain::Instance()->Update();

		// Update all components
		Component::UpdateAll();	
	}
	
	// Flush outputs to all actuators
	Actuator::OutputAll();

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
	
	// Check for runtime configuration file changes
	if (RobotState::Instance().GameMode() == GameState::DISABLED)
	{
		ConfigRuntime::Instance()->CheckForFileUpdates();
	}
	
	// Utilities
	LCD::Instance()->RunOneCycle();
//	Logger::Instance()->Run();
	LiveNetworkSender::Instance()->Run();
	//Dashboard2::Tick();
	
	// Reset ComponentData command fields
	ComponentData::ResetAllCommands();
	
	wdCancel(_watchdog);
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
