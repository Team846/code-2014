#ifndef NETCHANNELDEFINITION_H_
#define NETCHANNELDEFINITION_H_

namespace NetChannelDefinition
{
namespace UNRELIABLE_SEQUENCED
{
	enum Enum
	{
		LOCATOR = 0x00,
		DRIVETRAIN_TICKS = 0x01,
		TELEM_INIT = 0x02,
		TELEM_UPDATE = 0x03,
		LOG = 0x04
	};
}
}

#endif
