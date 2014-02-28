#include "Logger.h"
#include "Loggable.h"
#include "../Config/RobotConfig.h"
#include "../RobotState.h"

#include <Rhesus/Toolkit/IO/BufferedConsole.h>
#include <Rhesus/Toolkit/GameState.h>

using namespace std;
using namespace Rhesus::Toolkit;
using namespace Rhesus::Toolkit::IO;
using namespace Rhesus::Toolkit::Tasks;

Logger *Logger::m_instance = NULL;
vector<Loggable*> Logger::loggables;

Logger* Logger::Instance()
{
	if (m_instance == NULL)
		m_instance = new Logger();
	return m_instance;
}

void Logger::Finalize()
{
	delete m_instance;
	m_instance = NULL;
}

Logger::Logger() :
	SynchronizedProcess("Logger")
{
	curLoc = NULL;
	for (int i = 0; i < NUM_BUFFERS; i++)
		startLoc[i] = NULL;
	dataSize = 0;
	curIndex = 0;
	initialized = false;
}

Logger::~Logger()
{
	free(startLoc);
	for (int i = 0; i < NUM_BUFFERS; i++)
		startLoc[i] = NULL;
	curLoc = NULL;
}

void Logger::Initialize()
{
	if (initialized)
		return;
	for (vector<Loggable*>::iterator it = loggables.begin(); it < loggables.end(); it++)
	{
		(*it)->Log();
	}
	for (int i = 0; i < NUM_BUFFERS; i++)
	{
		free(startLoc[i]);
		startLoc[i] = malloc(dataSize);
	}
	FILE* header = fopen((RobotConfig::LOG_FILE_PATH + ".header").c_str(), "w");
	fprintf(header, "%d\n", fields.size());
	for (vector<Field>::iterator it = fields.begin(); it < fields.end(); it++)
	{
		fprintf(header, "%s %s %d\n", it->type.c_str(), it->name.c_str(), it->size);
	}
	fclose(header);
	fields.clear();
	initialized = true;
	Start();
}

void Logger::Run()
{
	if (!initialized)
	{
		BufferedConsole::Printf("[ERROR] Logger is not initialized\n");
		return;
	}
	if (RobotState::Instance().GameMode() == GameState::DISABLED)
	{
		if (RobotState::Instance().LastGameMode() != GameState::DISABLED)
#ifdef USE_IOLIB
			close(file);
#else
			fclose(file);
#endif
	}
	else
	{
		if (RobotState::Instance().LastGameMode() == GameState::DISABLED)
#ifdef USE_IOLIB
			file = open(RobotConfig::LOG_FILE_PATH.c_str(), O_CREAT | O_WRONLY, 0777);
#else
			file = fopen(RobotConfig::LOG_FILE_PATH.c_str(), "wb");
#endif
		curLoc = (char*)startLoc[curIndex];
		lock_on l(m_syncRoot[curIndex]);
		for (vector<Loggable*>::iterator it = loggables.begin(); it < loggables.end(); it++)
		{
			(*it)->Log();
		}
		curIndex = (curIndex + 1) % NUM_BUFFERS;
		RunOneCycle();
	}
}

void Logger::Tick()
{
	lock_on l(m_syncRoot[curIndex]);
	{
		double start = Timer::GetFPGATimestamp();
	#ifdef USE_IOLIB
		write(file, (char*)startLoc, dataSize);
	#else
		fwrite(startLoc, dataSize, 1, file);
		fflush(file);
	#endif
		double end = Timer::GetFPGATimestamp();
		if (end - start > 0.005)
		{
			BufferedConsole::Printf("Logging time overflow: %f\n", 1000 * (end - start));
		}
	}
}

void Logger::Write(void* field, size_t size)
{
	if (RobotState::Instance().GameMode() != GameState::DISABLED)
	{
		memcpy(curLoc, field, size);
		curLoc += size;
	}
}

void Logger::RegisterLoggable(Loggable *loggable)
{
	if (m_instance == NULL || !m_instance->initialized)
		loggables.push_back(loggable);
	else
		BufferedConsole::Printf("[ERROR] Logger initialized before all Loggable objects were created\n");
}
