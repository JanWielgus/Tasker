/**
 * @file SimpleTasker.cpp
 * @author Jan Wielgus & Aleksy Walczak
 * @date 2020-07-31
 * 
 */

#include "Tasker.h"


const uint32_t Tasker::MinTaskInterval_us = 52;

Tasker::Tasker(uint8_t maxTasksAmount)
    : MaxTasksAmount(maxTasksAmount)
{
    if (MaxTasksAmount > 0)
        tasks = new Task[MaxTasksAmount];
}


Tasker::~Tasker()
{
    if (MaxTasksAmount > 0)
        delete[] tasks;
}


bool Tasker::addTask_Hz(IExecutable* task, float frequency_Hz)
{
    if (tasksAmount >= MaxTasksAmount || task == nullptr || frequency_Hz <= 0)
        return false;

    uint32_t newInterval_us = 1000000.0 / frequency_Hz + 0.5f;
    return addTask_us(task, newInterval_us);
}


bool Tasker::addTask_us(IExecutable* task, uint32_t interval_us)
{
    if (tasksAmount >= MaxTasksAmount || task == nullptr)
        return false;

    Task newTask;
    newTask.executable = task;
    newTask.interval_us = interval_us < MinTaskInterval_us ? MinTaskInterval_us : interval_us;
    newTask.nextExecutionTime_us = currentTime;

    tasks[tasksAmount++] = newTask;
    calculateNextTask();

    return true;
}


bool Tasker::removeTask(IExecutable* task)
{
    bool isTaskFound = false;
    for (uint8_t i = 0; i < tasksAmount-1; ++i)
    {
        if (tasks[i].executable == task)
            isTaskFound = true;
        
        if (isTaskFound)
            tasks[i] = tasks[i+1];
    }

    if (tasks[tasksAmount-1].executable == task)
        isTaskFound = true;

    if (isTaskFound)
        tasksAmount--;
    
    calculateNextTask();

    return isTaskFound;
}


bool Tasker::setTaskFrequency(IExecutable* task, float frequency_Hz)
{
    return setTaskInterval_us(task, 1000000.0 / frequency_Hz + 0.5f);
}


bool Tasker::setTaskInterval_us(IExecutable* task, uint32_t interval_us)
{
    Task* t = getTask(task);
    if (t == nullptr)
        return false;
    
    interval_us = interval_us < MinTaskInterval_us ? MinTaskInterval_us : interval_us;
    t->nextExecutionTime_us += interval_us - t->interval_us;
    t->interval_us = interval_us;
    calculateNextTask();

    return true;
}


bool Tasker::pauseTask_us(IExecutable* task, uint32_t pauseTime_us)
{
    Task* t = getTask(task);
    if (t == nullptr)
        return false;

    t->nextExecutionTime_us += pauseTime_us;
    calculateNextTask();

    return true;
}


bool Tasker::pauseTask_s(IExecutable* task, float pauseTime_s)
{
    return pauseTask_us(task, pauseTime_s * 1000000.f + 0.5f);
}


float Tasker::getTaskFrequency_Hz(IExecutable* task) 
{
    Task* t = getTask(task);
    if (t == nullptr)
        return -1.f;

    return 1000000.f / t->interval_us;
}


uint32_t Tasker::getTaskInterval_us(IExecutable* task) 
{
    Task* t = getTask(task);
    if (t == nullptr)
        return (uint32_t)-1;

    return t->interval_us;
}

float Tasker::getTaskInterval_s(IExecutable* task) 
{
    Task* t = getTask(task);
    if (t == nullptr)
        return -1.f;

    return t->interval_us / 1000000.f;
}

uint8_t Tasker::getTasksAmount() 
{
    return tasksAmount;
}


#ifdef TASKER_LOAD_CALCULATIONS
float Tasker::getLoad()
{
    return load;
}
#endif


uint32_t Tasker::getCurrentTime_micros()
{
    return currentTime;
}


void Tasker::calculateNextTask()
{
    if (tasksAmount == 0)
    {
        nextTask = nullptr;
        return;
    }
    
    nextTask = tasks;
    for (uint8_t i = 1; i < tasksAmount; ++i)
    {
        if (tasks[i].nextExecutionTime_us < nextTask->nextExecutionTime_us)
            nextTask = &tasks[i];
    }
}


Tasker::Task* Tasker::getTask(IExecutable* task)
{
    for (uint8_t i = 0; i < tasksAmount; ++i)
    {
        if (tasks[i].executable == task)
            return &tasks[i];
    }

    return nullptr;
}
