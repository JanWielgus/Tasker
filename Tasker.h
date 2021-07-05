/**
 * @file Tasker.h
 * @author Jan Wielgus & Aleksy Walczak
 * @brief Tasker class
 * @date 2020-03-06
 * 
 */

#ifndef TASKER_H
#define TASKER_H

#include "IExecutable.h"
#include "TaskerConfig.h"

#ifdef ARDUINO
    #include <Arduino.h>
#endif


class Tasker
{
    struct Task
    {
        IExecutable* executable;
        uint32_t interval_us;
        uint32_t nextExecutionTime_us;
    };

    const uint8_t MaxTasksAmount;   // max amount of tasks
    Task* tasks;                    // array of MaxTasksAmount length
    uint8_t tasksAmount = 0;        // current amount of tasks (at most MaxTasksAmount)
    uint32_t currentTime = 0;       // current time (used in loop() method)
    Task* nextTask;                 // next task to check if it's triggered

#ifdef TASKER_LOAD_CALCULATIONS
    float load = 0.f;                       // from 0 to 100 [%]
    float curTaskLoadHelper = 0.f;          // helper in loop() method to calculate load
#endif

    static const uint32_t MinTaskInterval_us;  // minimal task interval (in us) - to prevent overloading


public:
    Tasker(uint8_t maxTasksAmount);
    ~Tasker();

    /**
     * @brief Add task and set it's execution frequency
     * @param task Pointer to concrete class that implements IExecutable interface.
     * @param frequency Task running frequency (in Hz).
     * @return true only if the task was successfully added
     */
    bool addTask_Hz(IExecutable* task, float frequency_Hz);

    /**
     * @brief Add task and set it's interval - time between two nearest executions
     * @param task Pointer to concrete class that implements IExecutable interface.
     * @param interval_us Task running interval (in us).
     * @return true if the task was successfully added,
     * false otherwise
     */
    bool addTask_us(IExecutable* task, uint32_t interval_us);

    /**
     * @brief Remove task from the tasker
     * @param task Task to remove
     * @return false if the task not found,
     * otherwise true
     */
    bool removeTask(IExecutable* task);

    /**
     * @brief Set task's frequency (in Hz) 
     * @param task Previously added task which frequency you want to change.
     * @param frequency_Hz New frequency (in Hz)
     * @return false if the task not found,
     * otherwise true
     */
    bool setTaskFrequency(IExecutable* task, float frequency_Hz);

    /**
     * @brief Set task's interval (in us) - the time between two nearest task's executions
     * @param task Previously added task which interval you want to change.
     * @param interval_us New interval (in us)
     * @return false if the task not found,
     * otherwise true
     */
    bool setTaskInterval_us(IExecutable* task, uint32_t interval_us);

    /**
     * @brief Pause task for a specified time (in us)
     * @param task Previously added task you want to pause.
     * @param pauseTime_us Time for task to be paused (in us)
     * @return false if the task not found,
     * otherwise true
     */
    bool pauseTask_us(IExecutable* task, uint32_t pauseTime_us);

    /**
     * @brief Pause task for a specified time (in s)
     * @param task Previously added task you want to pause
     * @param pauseTime_us Time for task to be paused (in s)
     * @return false if the task not found,
     * otherwise true
     */
    bool pauseTask_s(IExecutable* task, float pauseTime_s);

    /**
     * @param task Previously added task whose frequency you want to check
     * @return -1 if task isn't added to the tasker,
     * frequency (in Hz) of given task otherwise  
     */
    float getTaskFrequency_Hz(IExecutable* task);

    /**
     * @param task Previously added task whose interval (in us) you want to check
     * @return -1 (max int) if task isn't added to the tasker,
     * interval (in us) of given task otherwise
     */
    uint32_t getTaskInterval_us(IExecutable* task);

    /**
     * @param task Previously added task whose interval (in s) you want to check
     * @return -1 if task isn't added to the tasker,
     * interval (in s) of given task otherwise 
     */
    float getTaskInterval_s(IExecutable* task);

    /**
     * @return current tasks amount 
     */
    uint8_t getTasksAmount();

#ifdef TASKER_LOAD_CALCULATIONS
    /**
     * @return current tasker load
     */
    float getLoad();
#endif
    
    /**
     * @return (almost) current time. Faster than micros() from arduino standard library
     */
    uint32_t getCurrentTime_micros();

    /**
     * @brief This should be the only method in arduino's loop()
     */
    void loop();


private:
    /**
     * @brief This method calculates which task is going to be firstly executed
     */
    void calculateNextTask();

    /**
     * @param task Previously added task whose Task structure you want to get
     * @return nullptr if task not found
     * Task structure of given task otherwise
     */
    Task* getTask(IExecutable* task);
};


#ifdef TASKER_LOAD_CALCULATIONS
inline void Tasker::loop()
{
    currentTime = micros();
    curTaskLoadHelper = 0.f;

    if (nextTask != nullptr && currentTime >= nextTask->nextExecutionTime_us)
    {
        nextTask->nextExecutionTime_us += nextTask->interval_us;
        nextTask->executable->execute();
        calculateNextTask();
        curTaskLoadHelper = 100.f;
    }

    load = TASKER_LOAD_FILTER_BETA * load + TASKER_LOAD_FILTER_BETA_COFACTOR * curTaskLoadHelper;
}
#else
inline void Tasker::loop()
{
    currentTime = micros();

    if (nextTask != nullptr && currentTime >= nextTask->nextExecutionTime_us)
    {
        nextTask->nextExecutionTime_us += nextTask->interval_us;
        nextTask->executable->execute();
        calculateNextTask();
    }
}
#endif
#endif
