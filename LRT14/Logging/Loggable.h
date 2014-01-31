#ifndef LOGGABLE_H_
#define LOGGABLE_H_

#include "Logger.h"
#include <string>
#include <vector>

/*!
 * @brief Base class for classes that can log to the global log file.
 */
class Loggable
{
public:
	Loggable(std::string name);
	virtual ~Loggable();
	
	/*!
	 * @brief Logs data to the Logger.
	 */
	virtual void Log() = 0;
	
protected:
	/*!
	 * @brief Saves a variable to the Logger for logging.
	 * @param field pointer to the variable
	 * @param name name of the field
	 */
	template<typename T> inline void Loggable::LogToFile(T *field, std::string name)
	{
		m_logger->Log(field, name, m_name);
	}
	/*!
	 * @brief Saves a dynamically allocated array to the Logger for logging.
	 * @param field pointer to the variable
	 * @param count number of elements from the array pointer to log
	 * @param name name of the field
	 */
	template<typename T> inline void Loggable::LogToFile(T *field, int count, std::string name)
	{
		m_logger->Log(field, count * sizeof(*field), name, m_name);
	}

	/*!
	 * @brief Saves a value to the Logger for logging.
	 * @param value value to log
	 * @param name name of the field
	 */
	template<typename T> inline void Loggable::LogToFile(T value, std::string name)
	{
		m_logger->Log(value, name, m_name);
	}

private:
	Logger* m_logger;
	std::string m_name;
};

#endif /* LOGGABLE_H_ */
