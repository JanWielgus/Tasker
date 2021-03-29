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

#ifdef ARDUINO
    #include <Arduino.h>
#endif


class Task : public IExecutable
{
protected:
    uint32_t interval_us = 0; // [microseconds] time between every execution
    uint32_t nextExecutionTime_us = 0; // time since the beginning when to execute task
    bool isConfigured_flag = false; // set true when setProperties method is called

    static const float Million;


public:
    /**
     * @brief Default ctor. Don't set up the task.
     */
    Task();

    /**
     * @brief Constructor with setting task running frequency.
     * @param frequency Task running frequency.
     */
    Task(float frequency);

    /**
     * @brief Sets the task interval between next executions.
     * @param interval_us interval in microseconds.
     */
    void setInterval_us(uint32_t interval_us);

    /**
     * @return task interval in microseconds.
     */
    uint32_t getInterval_us();

    /**
     * @brief Return this tasks interval in seconds.
     */
    float getInterval_s();

    /**
     * @brief Sets the task running frequency.
     * @param frequency frequency (in Hz).
     */
	void setFrequency_Hz(float frequency_Hz);

    /**
     * @brief Return this tasks frequency in Hz.
     */
    float getFrequency_Hz();

    /**
     * @brief Each concrete Task class have to implement this method.
     */
    virtual void execute() override = 0;

    /**
     * @brief Check if task is ready to add to Tasker.
     * @return true if setProperties method was called at least once.
     */
    bool isConfigured();


    friend class SimpleTasker;
	friend class TaskPlanner;
};


#endif

