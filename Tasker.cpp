/**
 * @file SimpleTasker.cpp
 * @author Jan Wielgus & Aleksy Walczak
 * @date 2020-07-31
 * 
 */

#include <stdint.h>
#include "Tasker.h"
#include "TaskerConfig.h"

const uint32_t Tasker::MinTaskInterval_us = 100;

static void defaultSleepFunction(uint32_t timeToSleep_us);


Tasker::Tasker(uint8_t maxTasksAmount)
    : MaxTasksAmount(maxTasksAmount)
{
    if (MaxTasksAmount > 0)
        tasks = new Task[MaxTasksAmount];

    #ifdef TASKER_SLEEP_FUNCTION
        sleepFunction = defaultSleepFunction;
    #endif
}


Tasker::~Tasker()
{
    if (MaxTasksAmount > 0)
        delete[] tasks;
}


bool Tasker::addTask_Hz(ExecutableType task, float frequency_Hz, TaskType type)
{
    if (tasksAmount >= MaxTasksAmount || task == nullptr || frequency_Hz <= 0)
        return false;

    uint32_t newInterval_us = 1000000.0 / frequency_Hz + 0.5f;
    return addTask_us(task, newInterval_us, type);
}


bool Tasker::addTask_us(ExecutableType task, uint32_t interval_us, TaskType type)
{
    if (tasksAmount >= MaxTasksAmount || task == nullptr)
        return false;

    for (uint8_t i = 0; i < tasksAmount; ++i)
        if (tasks[i].executable == task)
            return false;

    Task newTask;
    newTask.executable = task;
    newTask.interval_us = interval_us < MinTaskInterval_us ? MinTaskInterval_us : interval_us;
    newTask.nextExecutionTime_us = micros();
    newTask.taskType = type;

    tasks[tasksAmount++] = newTask;
    calculateNextTask();

    return true;
}


bool Tasker::removeTask(ExecutableType task)
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


bool Tasker::setTaskFrequency(ExecutableType task, float frequency_Hz)
{
    return setTaskInterval_us(task, 1000000.0 / frequency_Hz + 0.5f);
}


bool Tasker::setTaskInterval_us(ExecutableType task, uint32_t interval_us)
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


bool Tasker::pauseTask_us(ExecutableType task, uint32_t pauseTime_us)
{
    Task* t = getTask(task);
    if (t == nullptr)
        return false;

    t->nextExecutionTime_us += pauseTime_us;
    calculateNextTask();

    return true;
}


bool Tasker::pauseTask_s(ExecutableType task, float pauseTime_s)
{
    if (pauseTime_s > 0)
        return pauseTask_us(task, pauseTime_s * 1000000.f + 0.5f);
    else
        return false;
}


float Tasker::getTaskFrequency_Hz(ExecutableType task) 
{
    Task* t = getTask(task);
    if (t == nullptr)
        return -1.f;

    return 1000000.f / t->interval_us;
}


uint32_t Tasker::getTaskInterval_us(ExecutableType task) 
{
    Task* t = getTask(task);
    if (t == nullptr)
        return (uint32_t)-1;

    return t->interval_us;
}


float Tasker::getTaskInterval_s(ExecutableType task) 
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


void Tasker::setSleepFunction(SleepFunction sleepFunction)
{
    #ifdef TASKER_SLEEP_FUNCTION
        if (sleepFunction != nullptr)
            this->sleepFunction = sleepFunction;
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
    
    nextTask = &tasks[0];
    for (uint8_t i = 1; i < tasksAmount; ++i)
    {
        if ((long)(tasks[i].nextExecutionTime_us - nextTask->nextExecutionTime_us) < 0)
        {
            nextTask = &tasks[i];
        }
    }
}


Tasker::Task* Tasker::getTask(ExecutableType task)
{
    for (uint8_t i = 0; i < tasksAmount; ++i)
    {
        if (tasks[i].executable == task)
            return &tasks[i];
    }

    return nullptr;
}


void defaultSleepFunction(uint32_t timeToSleep_us)
{
    uint32_t sleepEndTime_us = micros() + timeToSleep_us;
    while (micros() < sleepEndTime_us);
}
