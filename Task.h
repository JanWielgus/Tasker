/**
 * @file FC_Task.h
 * @author Jan Wielgus
 * @brief 
 * @date 2020-03-06
 * 
 */

#ifndef TASK_H
#define TASK_H

#include <IExecutable.h>
#include <arduino.h>


class Task : public IExecutable
{
protected:
    uint32_t interval = 0; // [microseconds] time between every execution
    uint16_t maxDuration = 0; // task max duration (can be used for task planning)
    uint32_t nextExecutionTime = 0; // time since the beginning when to execute task
    //uint16_t timeShift = 0; // NOT USED CURERNTLY but can be to spread tasks in time
    bool isConfiguredFlag = false; // set true when setProperties method is called


public:
    Task();
    Task(float frequency, uint16_t maxDuration);

    /**
     * @brief Return this tasks interval in microseconds
     */
    uint32_t getInterval_us();

    /**
     * @brief Return this tasks frequency in Hz
     */
    float getFrequency_Hz();

    /**
     * @brief Each concrete Task class have to implement this method
     */
    virtual void execute() override = 0;

    /**
     * @brief Set the Properties object Called by the ObjectTasker when adding new task
     * 
     * @param frequency task running frequency
     * @param maxDuration maximum tested duration of task (if not measured, set 0)
     */
	void setProperties(float frequency, uint16_t maxDuration);

    /**
     * @brief Check if task is ready to add to Tasker
     * 
     * @return true if setProperties method was called at least once
     */
    bool isConfigured();


    friend class SimpleTasker;
};


#endif

