#ifndef DELAYED_EVENT_H_
#define DELAYED_EVENT_H_

#include "Event.h"

/*!
 * @brief Event that fires when the conditions of an event are true for a number of cycles.
 */
class DelayedEvent : public Event
{
public:
	DelayedEvent(Event *event, int delayCycles);
	virtual ~DelayedEvent();
	
	virtual bool CheckCondition();
	virtual void Update();
	
private:
	Event *m_event;
	int m_delay;
	int m_count;
};

#endif
