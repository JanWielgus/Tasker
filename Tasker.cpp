/**
 * @file SimpleTasker.cpp
 * @author Jan Wielgus & Aleksy Walczak
 * @date 2020-07-31
 * 
 */

#include "Tasker.h"

const uint32_t Tasker::MinTaskInterval_us = 52;
const uint8_t Tasker::SleepTimeMargin_us = 40;

#ifdef PROCESSOR_OVERLOAD_CALLBACK
const uint32_t Tasker::SystemOverloadedSleepingTime_us = -1000;
#endif

static void defaultSleepingFunction(uint32_t timeToSleep);
static void defaultProcessorOverloadCallback();


Tasker::Tasker(uint8_t maxTasksAmount)
    : MaxTasksAmount(maxTasksAmount)
{
    if (MaxTasksAmount > 0)
        tasks = new Task[MaxTasksAmount];

#ifdef SLEEPING_FUNCTION
    sleepingFunction = defaultSleepingFunction;
#endif

#ifdef PROCESSOR_OVERLOAD_CALLBACK
    processorOverloadCallback = defaultProcessorOverloadCallback;
#endif

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
    newTask.nextExecutionTime_us = lastTaskExecutionTime;

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


void Tasker::setSleepingFunction(SleepingFunction sleepingFunction)
{
#ifdef SLEEPING_FUNCTION
    if (sleepingFunction != nullptr)
        this->sleepingFunction = sleepingFunction;
#endif
}


void Tasker::setProcessorOverloadCallback(VoidFunction processorOverloadCallback)
{
#ifdef PROCESSOR_OVERLOAD_CALLBACK
    if (processorOverloadCallback != nullptr)
        this->processorOverloadCallback = processorOverloadCallback;
#endif
}


float Tasker::getLoad()
{
#ifdef TASKER_LOAD_CALCULATIONS
    return load * 100.f;
#else
    return 0.f;
#endif
}


uint32_t Tasker::getLastTaskExecutionTime_us()
{
    return lastTaskExecutionTime;
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


void defaultSleepingFunction(uint32_t timeToSleep)
{
    uint32_t sleepingEndTime = micros() + timeToSleep;
    while (micros() < sleepingEndTime);
}


void defaultProcessorOverloadCallback()
{
}
