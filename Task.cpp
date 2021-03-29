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
    nextExecutionTime_us = 0;
}


Task::Task(float frequency)
{
    setFrequency_Hz(frequency);
    nextExecutionTime_us = 0;
}


void Task::setInterval_us(uint32_t interval)
{
    this->interval_us = interval;
    isConfigured_flag = true;
}


uint32_t Task::getInterval_us()
{
    return interval_us;
}


float Task::getInterval_s()
{
    return (double)interval_us / Million;
}


void Task::setFrequency_Hz(float frequency)
{
    uint32_t interval = ((double)Million / frequency) + 0.5;
    setInterval_us(interval);
}


float Task::getFrequency_Hz()
{
    return Million / (double)interval_us;
}


bool Task::isConfigured()
{
    return isConfigured_flag;
}

