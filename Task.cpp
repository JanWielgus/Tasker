/**
 * @file Task.cpp
 * @author Jan Wielgus
 * @date 2020-07-31
 * 
 */

#include <Task.h>


const float Task::Million = 1000000.f;


Task::Task()
{
    interval_us = 0;
    maxDuration_us = 0;
    nextExecutionTime_us = 0;
}


Task::Task(float frequency, uint16_t maxDuration)
{
    setProperties(frequency, maxDuration);
    nextExecutionTime_us = 0;
}


uint32_t Task::getInterval_us()
{
    return interval_us;
}


float Task::getInterval_s()
{
    return interval_us / Million;
}


float Task::getFrequency_Hz()
{
    return Million / interval_us;
}


void Task::setProperties(float frequency, uint16_t maxDuration)
{
    this->interval_us = Million / frequency;
    this->maxDuration_us = maxDuration;
    isConfiguredFlag = true;
}


bool Task::isConfigured()
{
    return isConfiguredFlag;
}

