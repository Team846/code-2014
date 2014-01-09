#ifndef RHESUS_STRINGUTIL_H_
#define RHESUS_STRINGUTIL_H_

#include <string>
#include <sstream>
#include <vector>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

namespace Rhesus
{
namespace Toolkit
{
namespace Utilities
{

	/*
	 * @brief Contains functions to perform string operations
	 * @author Varun Parthasarathy
	 */
	class StringUtil
	{
	public:
		static std::string ltrim(std::string s)
		{
			s.erase(
					s.begin(),
					std::find_if(s.begin(), s.end(),
							std::not1(std::ptr_fun<int, int>(std::isspace))));
			
			return s;
		}
	
		// trim from end
		static std::string rtrim(std::string s)
		{
			s.erase(
					std::find_if(s.rbegin(), s.rend(),
							std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
					s.end());
			
			return s;
		}
	
		// trim from both ends
		static std::string Trim(std::string s)
		{
			return ltrim(rtrim(s));
		}
	
		static std::vector<std::string> Split(std::string s, char d)
		{
			std::vector<std::string> ret;
	
			std::string buff;
	
			for (UINT32 i = 0; i < s.length(); i++)
			{
	
				if (s.at(i) == d)
				{
					ret.push_back(buff);
					buff.clear();
				} else
				{
					buff.push_back(s.at(i));
				}
	
			}
			return ret;
		}
	
		template<class T>
		static std::string ValToString(T val)
		{
			std::stringstream ss;
			ss << std::boolalpha << val;
			
			return ss.str();
		}
	};

}
}
}

#endif
