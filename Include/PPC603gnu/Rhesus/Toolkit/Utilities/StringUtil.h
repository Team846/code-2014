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
		
		/*!
		 * Trims the string from the beginning
		 */
		static std::string TrimLeft(std::string s)
		{
			s.erase(
					s.begin(),
					std::find_if(s.begin(), s.end(),
							std::not1(std::ptr_fun<int, int>(std::isspace))));
			
			return s;
		}
	
		/*!
		 * trims the string from the end
		 */
		static std::string TrimRight(std::string s)
		{
			s.erase(
					std::find_if(s.rbegin(), s.rend(),
							std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
					s.end());
			
			return s;
		}
	
		/*!
		 * @return the input string with all whitespace removed
		 * @param s the original string
		 */
		static std::string Trim(std::string s)
		{
			return TrimLeft(TrimRight(s));
		}
	
		/*!
		 * splits the string by the specified character
		 * @return a vector containing the split strings
		 * @param s the original string
		 * @param d the delimeter
		 */
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
	};

}
}
}

#endif
