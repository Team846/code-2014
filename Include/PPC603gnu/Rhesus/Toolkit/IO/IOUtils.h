#ifndef RHESUS_IOUTILS_H_
#define RHESUS_IOUTILS_H_

namespace Rhesus 
{
namespace Toolkit 
{
namespace IO 
{

	class IOUtils
	{
	private:
		static bool s_file;
		static int fd;
		static fpos_t pos;
		
	public:
		static void RedirectOutputToFile(const char* file)
		{
			if (s_file)
			{
				printf("\n--------------------Redirecting print stream to file--------------------\n");
				fflush(stdout);
				fgetpos(stdout, &pos);
				fd = dup(fileno(stdout));
				freopen(file, "a", stdout);
				printf("\n--------------------Print stream redirected--------------------\n");
				s_file = true;
			}
		}
		
		static void RedirectOutputToConsole()
		{
			if (s_file)
			{
				printf("\n--------------------Restoring print stream to console--------------------\n");
				fflush(stdout);
				dup2(fd, fileno(stdout));
				close(fd);
				clearerr(stdout);
				fsetpos(stdout, &pos); 
				printf("\n--------------------Print stream restored--------------------\n");			                
				s_file = false;
			}
		}

		static bool IsFile()
		{
			return s_file;
		}
	};

	bool IOUtils::s_file = false;
	int IOUtils::fd = 0;
	fpos_t IOUtils::pos = 0;
}
}
}

#endif
