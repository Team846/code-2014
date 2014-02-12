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
	AsyncPrinter::Finalize();
	LRTDriverStation::Finalize();
	RobotState::Finalize();
}

void LRTRobot14::RobotInit()
{
	// Initialize global robot state object
	RobotState::Initialize();
	
	// Initialize Utilities
	AsyncPrinter::Initialize();
	LCD::Instance()->Start();

	// Create the Driver Station
	AsyncPrinter::Println("Initializing Driver Station...");
	LRTDriverStation::Initialize();
	
	// Read port mappings
	AsyncPrinter::Println("Loading Port Mappings...");
	ConfigPortMappings::Instance()->Load();
	
	// Create ComponentData
	AsyncPrinter::Println("Creating ComponentData...");
	ComponentData::Initialize();
	
	// Create all components
	AsyncPrinter::Println("Creating Components...");
	Component::CreateComponents();
	
	// Initialize the Brain
	AsyncPrinter::Println("Initializing Brain...");
	Brain::Initialize();
	
	// Start AsyncCANJaguar tasks
	AsyncPrinter::Println("Starting AsyncCANJaguar Tasks...");
	for (vector<AsyncCANJaguar*>::iterator it = AsyncCANJaguar::jaguar_vector.begin(); it < AsyncCANJaguar::jaguar_vector.end(); it++)
	{
		(*it)->Start();
	}

#if PNEUMATICS
	// Create and start compressor
	AsyncPrinter::Println("Creating Pneumatics Compressor...");
	Pneumatics::CreateCompressor();
#endif

	// Initialize SensorFactory
	AsyncPrinter::Println("Initializing Sensor Factory...");
	SensorFactory::Initialize();
	
	// Initialize localization
	AsyncPrinter::Println("Initializing Robot Localization...");
	RobotLocation::Initialize();
	
	// Initialize offboard communication
	AsyncPrinter::Println("Initializing Offboard Communication...");
	OffboardCommunication::Initialize();

	// Initialize the Logger
	AsyncPrinter::Println("Initializing Logger...");
	Logger::Instance()->Initialize();
	
	// Initialize the LiveNetworkSender
	AsyncPrinter::Println("Initializing LiveNetworkSender...");
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
		AsyncPrinter::RedirectToFile(RobotConfig::PRINT_FILE_PATH.c_str());
	}
	else
	{
		AsyncPrinter::RestoreToConsole();
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
	Dashboard2::Tick();
	
	// Reset ComponentData command fields
	ComponentData::ResetAllCommands();
	
	wdCancel(_watchdog);
}

void maintenance()
{
	if (RobotState::Instance().GameMode() == GameState::DISABLED && !maintenanceMode)
	{
		maintenanceMode = true;
		AsyncPrinter::Println("Maintenance mode entered");
	}
	else if (maintenanceMode)
		AsyncPrinter::Println("Already in maintenance mode");
	else
		AsyncPrinter::Println("Please disable to enter maintenance mode");
}

void exit()
{
	if(RobotState::Instance().GameMode() == GameState::DISABLED && maintenanceMode)
	{
		maintenanceMode = false;
		AsyncPrinter::Println("Maintenance mode exited");
	}
	else if (!maintenanceMode)
		AsyncPrinter::Println("Not in maintenance mode");
	else
		AsyncPrinter::Println("Please disable to exit maintenance mode");
}

