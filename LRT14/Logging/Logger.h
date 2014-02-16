#ifndef LOGGER_H_
#define LOGGER_H_

#ifdef USE_IOLIB
#include <ioLib.h>
#endif
#include <WPILib.h>
#include <vector>
#include <typeinfo>
#include <sstream>
#include <Rhesus.Toolkit.Tasks.h>
#include "../Utils/Util.h"
#include "../Process/SynchronizedProcess.h"

class Loggable;

/*!
 * @brief Logs data from Loggable classes to a global log file in binary format each cycle.
 */
class Logger : public SynchronizedProcess
{
public:
	static Logger* Instance();
	static void Finalize();

	~Logger();

	/*!
	 * @brief Writes the header file and allocates the block of memory to log to.
	 */
	void Initialize();
	
	/*!
	 * @brief Logs all data from one cycle into the log file.
	 */
	void Run();
	
	/*!
	 * @brief Saves a number of bytes from a variable for logging.
	 * @param field pointer to the variable
	 * @param size number of bytes to write from the pointer
	 * @param name name of the field
	 * @param source name of the Loggable object that logged this field
	 */
	template<typename T> void Log(T *field, std::size_t size, std::string name, std::string source)
	{
		if (!initialized)
		{
			int count = size / sizeof(*field); // Check for dynamically allocated arrays
			Field f = {count == 1 ? typeid(*field).name() :
					"A" + Util::ToString(count) + "_" + typeid(*field).name(), source + "/" + name, size}; // Change type to static array
			fields.push_back(f);
			dataSize += size;
		}
		else
			Write(field, size);
	}

	/*!
	 * @brief Saves a variable for logging.
	 * @param field pointer to the variable
	 * @param name name of the field
	 * @param source name of the Loggable object that logged this variable
	 */
	template<typename T> void Log(T *field, std::string name, std::string source)
	{
		Log(field, sizeof(*field), name, source);
	}
	
	/*!
	 * @brief Saves a value for logging.
	 * @param value value to log
	 * @param name name of the field
	 * @param source name of the Loggable object that logged this field
	 */
	template<typename T> void Log(T value, std::string name, std::string source)
	{
		Log(&value, sizeof(value), name, source);
	}

	/*!
	 * @brief Registers a Loggable object for logging.
	 * @param loggable the Loggable object to register
	 */
	static void RegisterLoggable(Loggable* loggable);

protected:
	void Tick();
	
private:
	typedef struct
	{
		std::string type;
		std::string name;
		std::size_t size;
	} Field;
	
	Logger();

	/*!
	 * @brief Writes data to the global log file.
	 * @param field pointer to the data
	 * @param size number of bytes to write
	 */
	void Write(void* field, std::size_t size);
	
	static Logger* m_instance;

	static std::vector<Loggable*> loggables;
	std::vector<Field> fields;
#ifdef USE_IOLIB
	int file;
#else
	FILE* file;
#endif
	bool initialized;
	std::size_t dataSize;
	char* curLoc;
	void* startLoc;
	Rhesus::Toolkit::Tasks::Mutex m_syncRoot;
};

#endif /* LOGGER_H_ */
