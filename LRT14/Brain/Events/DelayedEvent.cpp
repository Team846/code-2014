#include "DelayedEvent.h"

DelayedEvent::DelayedEvent(Event* event, int delayCycles)
{
	m_event = event;
	m_delay = delayCycles;
	m_count = 0;
}


DelayedEvent::~DelayedEvent()
{
}

bool DelayedEvent::CheckCondition()
{
	return m_count >= m_delay;
}

void DelayedEvent::Update()
{
	Event::Update();
	if (m_event->CheckCondition())
	{
		m_count++;
	}
	else
	{
		m_count = 0;
	}
}
