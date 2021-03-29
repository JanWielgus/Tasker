/**
 * @file ITasker.h
 * @author Jan Wielgus
 * @brief Interface for Tasker (Scheduler) classes
 * @date 2020-07-31
 * 
 */

#ifndef ITASKER_H
#define ITASKER_H

#include <Task.h>

// TODO: add methods description

class ITasker
{
public:
    virtual ~ITasker() {}
    virtual bool addTask(Task* task, float frequency) = 0;
    virtual uint32_t getCurrentTime_micros() = 0;
    virtual void loop() = 0;
    virtual float getLoad() = 0;
};


#endif
