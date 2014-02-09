#include "LRTEncoder.h"

LRTEncoder::LRTEncoder(const char* name, uint32_t sourceA, uint32_t sourceB, double trim) :
	Encoder(sourceA, sourceB, false, CounterBase::k4X),
	Loggable(name),
	trim(trim)
{
}

LRTEncoder::~LRTEncoder()
{
}

void LRTEncoder::Start()
{
	Encoder::Stop();
	Encoder::Start();
	Encoder::SetMinRate(10);
}

double LRTEncoder::GetRate()
{
	// WPILib's GetRate() returns NaN if the rate is 0
	// Karthik Viswanathan and Brian Axelrod, January 29th, 2011
	double rate = Encoder::GetRate();
	if (Encoder::GetStopped() || rate != rate) // Test if rate is NaN because NaN != NaN
		return 0.0;
	return rate;
}

int32_t LRTEncoder::Get()
{
	return (int32_t) (Encoder::Get() * trim);
}

void LRTEncoder::Log()
{
	LogToFile(GetRate(), "Rate");
	LogToFile(Get(), "Count");
}
