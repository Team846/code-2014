#include "IOUtils.h"

using namespace Rhesus::Toolkit::IO;

bool IOUtils::s_file = false;
int IOUtils::fd = 0;
fpos_t IOUtils::pos = 0;
