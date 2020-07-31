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
#include <arduino.h>


class ITasker
{
public:
    virtual ~ITasker() {}

    virtual bool addTask(Task* task) = 0;
    virtual bool addTask(Task* task, float frequency, uint16_t maxDuration) = 0;
    virtual uint32_t getCurrentTime_micros() = 0;
    virtual void runLoop() = 0;
};


#endif
