// FC_Task.h
/*
    Created:	06/03/2020
    Author:     Jan Wielgus
*/

#ifndef _FC_TASK_h
#define _FC_TASK_h

#include "arduino.h"


class FC_Task
{
public:
	// Each Task have to override this method (and put there actual task code)
	virtual void execute() = 0;

    uint32_t getInterval()
    {
        return interval;
    }
	
private:
	// this method have to be called by the FC_ObjectTasker when adding a new task
	void setProperties(uint32_t interval, uint16_t maxDuration);


private:
    uint32_t interval = 0; // [in microseconds] time between every execution
    uint16_t maxDuration = 0; // task max duration (can be used for task planning)
    uint32_t nextExecutionTime = 0; // time since the beginning when to execute task
    //uint16_t timeShift = 0; // NOT USED CURERNTLY but can be to spread tasks in time

    friend class FC_ObjectTasker; // Allow FC_ObjectTasker to use private components of this class
};


#endif

