#include <OpenNI.h>
#include "Viewer.h"

int main(int argc, char** argv)
{
	openni::Status rc = openni::STATUS_OK;

	openni::Device device;
	openni::VideoStream depth;
	const char* deviceURI = openni::ANY_DEVICE;
	if (argc > 1)
	{
		deviceURI = argv[1];
	}

	rc = openni::OpenNI::initialize();

	rc = device.open(deviceURI);
	if (rc != openni::STATUS_OK)
	{
		printf("Device open failed: %s\n", openni::OpenNI::getExtendedError());
		openni::OpenNI::shutdown();
		return 1;
	}

	rc = depth.create(device, openni::SENSOR_DEPTH);
	if (rc == openni::STATUS_OK)
	{
		rc = depth.start();
		if (rc != openni::STATUS_OK)
		{
			printf("Couldn't start depth stream: %s\n", openni::OpenNI::getExtendedError());
			depth.destroy();
		}
	}
	else
	{
		printf("Couldn't find depth stream: %s\n", openni::OpenNI::getExtendedError());
	}

	if (!depth.isValid())
	{
		printf("No valid streams. Exiting\n");
		openni::OpenNI::shutdown();
		return 2;
	}

	Viewer sampleViewer("Viewer", depth);

	rc = sampleViewer.init(argc, argv);
	if (rc != openni::STATUS_OK)
	{
		openni::OpenNI::shutdown();
		return 3;
	}
	sampleViewer.run();
}
