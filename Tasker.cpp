/**
 * @file SimpleTasker.cpp
 * @author Jan Wielgus
 * @brief 
 * @date 2020-07-31
 * 
 */

#include "Tasker.h"


Tasker::Tasker(uint8_t maxTasksAmount)
    : MaxAmtOfTasks(maxTasksAmount)
{
    if (MaxAmtOfTasks > 0)
        tasks = new Task[MaxAmtOfTasks];
}


Tasker::~Tasker()
{
    if (MaxAmtOfTasks > 0)
        delete[] tasks;
}


bool Tasker::addTask_Hz(IExecutable* task, float frequency_Hz)
{
    if (amountOfTasks >= MaxAmtOfTasks || task == nullptr || frequency_Hz <= 0)
        return false;

    Task newTask;
    newTask.executable = task;
    newTask.interval_us = 1000000.0 / frequency_Hz + 0.5f;
    newTask.nextExecutionTime_us = currentTime;

    tasks[amountOfTasks++] = newTask;

    calculateNextTask();
    return true;
}


bool Tasker::addTask_us(IExecutable* task, uint32_t interval_us)
{
    if (amountOfTasks >= MaxAmtOfTasks || task == nullptr)
        return false;

    Task newTask;
    newTask.executable = task;
    newTask.interval_us = interval_us;
    newTask.nextExecutionTime_us = currentTime;

    tasks[amountOfTasks++] = newTask;

    calculateNextTask();
    return true;
}


bool Tasker::removeTask(IExecutable* task)
{
    bool isTaskFound = false;
    for (uint8_t i = 0; i < amountOfTasks-1; ++i)
    {
        if (tasks[i].executable == task)
            isTaskFound = true;
        
        if (isTaskFound)
            tasks[i] = tasks[i+1];
    }

    if (tasks[amountOfTasks-1].executable == task)
        isTaskFound = true;

    if (isTaskFound)
        amountOfTasks--;
    
    calculateNextTask();

    return isTaskFound;
}


void Tasker::calculateNextTask()
{
    if (amountOfTasks == 0)
    {
        nextTask = nullptr;
        return;
    }
    
    Task* nextTask = tasks;
    for (uint8_t i = 1; i < amountOfTasks; ++i)
    {
        if (tasks[i].nextExecutionTime_us < nextTask->nextExecutionTime_us)
            nextTask = &tasks[i];
    }
}


bool Tasker::setTaskFrequency(IExecutable* task, float frequency_Hz)
{
    for (uint8_t i = 0; i < amountOfTasks-1; ++i)
    {
        if (tasks[i].executable == task)
        {
            uint32_t newInterval_us = 1000000.0 / frequency_Hz + 0.5f;
            tasks[i].nextExecutionTime_us += newInterval_us - tasks[i].interval_us;
            tasks[i].interval_us = newInterval_us;
            calculateNextTask();
            return true;
        }
    }
    
    return false;
}


bool Tasker::setTaskInterval_us(IExecutable* task, uint32_t interval_us)
{
    for (uint8_t i = 0; i < amountOfTasks; ++i)
    {
        if (tasks[i].executable == task)
        {
            tasks[i].nextExecutionTime_us += interval_us - tasks[i].interval_us;
            tasks[i].interval_us = interval_us;
            calculateNextTask();
            return true;
        }
    }
    
    return false;
}


bool Tasker::pauseTask_us(IExecutable* task, uint32_t pauseTime_us)
{
    for (uint8_t i = 0; i < amountOfTasks; ++i)
    {
        if (tasks[i].executable == task)
        {
            tasks[i].nextExecutionTime_us += pauseTime_us;
            calculateNextTask();
            return true;
        }
    }
    
    return false;
}


bool Tasker::pauseTask_s(IExecutable* task, float pauseTime_s)
{
    for (uint8_t i = 0; i < amountOfTasks; ++i)
    {
        if (tasks[i].executable == task)
        {
            tasks[i].nextExecutionTime_us += pauseTime_s * 1000000.f + 0.5f;
            calculateNextTask();
            return true;
        }
    }
    
    return false;
}

float Tasker::getTaskFrequency_Hz(IExecutable* task) 
{
    for (uint8_t i = 0; i < amountOfTasks; ++i)
    {
        if (tasks[i].executable == task)
            return 1000000.f / tasks[i].interval_us;
    }
    
    return -1.f;
}


uint32_t Tasker::getTaskInterval_us(IExecutable* task) 
{
    for (uint8_t i = 0; i < amountOfTasks; ++i)
    {
        if (tasks[i].executable == task)
            return tasks[i].interval_us;
    }
    
    return (uint32_t)-1;
}

float Tasker::getTaskInterval_s(IExecutable* task) 
{
    for (uint8_t i = 0; i < amountOfTasks; ++i)
    {
        if (tasks[i].executable == task)
            return 1000000.f * tasks[i].interval_us;
    }
    
    return -1.f;
}

uint8_t Tasker::getTasksAmount() 
{
    return amountOfTasks;
}



float Tasker::getLoad()
{
    return load;
}


uint32_t Tasker::getCurrentTime_micros()
{
    return currentTime;
}


void Tasker::loop()
{
    currentTime = micros();
    lastTaskLoad = 0.f;

    if (currentTime >= nextTask->nextExecutionTime_us)
    {
        nextTask->nextExecutionTime_us += nextTask->interval_us;
        nextTask->executable->execute();
        calculateNextTask();
        lastTaskLoad = 100.f;
    }

    load = LoadFilterBeta*load + LoadFilterBetaCofactor*lastTaskLoad;
}
