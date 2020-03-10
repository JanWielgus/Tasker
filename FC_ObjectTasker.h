// FC_ObjectTasker.h
/*
    Created:	06/03/2020
    Author:     Jan Wielgus
*/

#ifndef _FC_OBJECTTASKER_h
#define _FC_OBJECTTASKER_h

#include "arduino.h"
#include <FC_Task.h>


class FC_ObjectTasker
{
public:
    FC_ObjectTasker(uint8_t maxTaskAmt);
    virtual ~FC_ObjectTasker();
    bool addTask(FC_Task* task, uint32_t interval, uint16_t maxDuration);
	
	// This should be the only method inside loop()
    virtual void run(); // this method is implemented but can be overridden


protected:
    const uint8_t MaxAmtOfTasks;
    FC_Task** tasksArray; // static array of MaxAmtOfTasks size
    uint8_t amtOfTasks = 0; // current amount of tasks (at most MaxAmtOfTasks)

    uint32_t curTime = 0; // current time (used in run() method)
};


#endif

