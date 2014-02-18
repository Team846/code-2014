#ifndef RHESUS_LEXICALCAST_H_
#define RHESUS_LEXICALCAST_H_

#include <sstream>

namespace Rhesus
{
namespace Toolkit
{

template<typename T>
inline T lexical_cast(std::string value)
{
    T temp;
    std::stringstream sstream(value);
    sstream >> std::boolalpha >> temp;
    return temp;
}

template<typename T>
inline std::string lexical_cast(T value)
{
	return std::string(static_cast<std::ostringstream*>(&(std::ostringstream() << value))->str());
}

}
}

#endif
