/**
 * @file Task.cpp
 * @author Jan Wielgus
 * @date 2020-07-31
 * 
 */

#include <Task.h>


Task::Task()
{
    interval = 0;
    maxDuration = 0;
    nextExecutionTime = 0;
}


Task::Task(float frequency, uint16_t maxDuration)
{
    setProperties(frequency, maxDuration);
    nextExecutionTime = 0;
}


uint32_t Task::getInterval_us()
{
    return interval;
}


float Task::getFrequency_Hz()
{
    return 1000000.0f / interval;
}


void Task::setProperties(float frequency, uint16_t maxDuration)
{
    this->interval = 1000000 / frequency;
    this->maxDuration = maxDuration;
    isConfiguredFlag = true;
}


bool Task::isConfigured()
{
    return isConfiguredFlag;
}

