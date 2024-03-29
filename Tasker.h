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


enum class TaskType : uint8_t
{
    CATCHING_UP = true,
    NO_CATCHING_UP = false
};


class Tasker
{
    #ifdef TASKER_IEXECUTABLE_POINTER_TASK
        typedef IExecutable* ExecutableType;
    #else
        typedef void (*ExecutableType)();
    #endif

    struct Task
    {
        ExecutableType executable;
        uint32_t interval_us;
        uint32_t nextExecutionTime_us;
        TaskType taskType;
    };

    const uint8_t MaxTasksAmount;
    Task* tasks;                            // array of tasks (MaxTasksAmount length)
    uint8_t tasksAmount = 0;                // current amount of tasks in array (at most MaxTasksAmount)
    uint32_t lastTaskExecutionTime = 0;     // execution time of the last executed task
    Task* nextTask = nullptr;               // task that will be executed next or nullptr

    #ifdef TASKER_LOAD_CALCULATIONS
        float load = 0.f;                           // from 0 to 1
    #endif

    typedef void (*SleepFunction)(uint32_t);    // function with one argument (max time [in us] after which that function should return)
    #ifdef TASKER_SLEEP_FUNCTION
        SleepFunction sleepFunction;                // function that make processor sleep specified amount of time
    #endif

    static const uint32_t MinTaskInterval_us;   // minimum task interval (in us) - to prevent overloading

public:
    Tasker(uint8_t maxTasksAmount);
    ~Tasker();

    /**
     * @brief Add task and set it's execution frequency.
     * @param task Pointer to instance of class that implements IExecutable interface.
     * @param frequency Task running frequency (in Hz).
     * @return true only if the task was successfully added,
     * false otherwise.
     */
    bool addTask_Hz(ExecutableType task, float frequency_Hz, TaskType type = TaskType::CATCHING_UP);

    /**
     * @brief Add task and set it's interval (time between two nearest executions).
     * @param task Pointer to instance of class that implements IExecutable interface.
     * @param interval_us Task running interval (in us).
     * @return true if the task was successfully added, false otherwise.
     */
    bool addTask_us(ExecutableType task, uint32_t interval_us, TaskType type = TaskType::CATCHING_UP);

    /**
     * @brief Remove task from the tasker.
     * @param task Task to remove.
     * @return false if the task not found, otherwise true.
     */
    bool removeTask(ExecutableType task);

    /**
     * @brief Change frequency of currently running task.
     * @param task Previously added task which frequency you want to change.
     * @param frequency_Hz New frequency (in Hz).
     * @return false if the task not found,
     * otherwise true.
     */
    bool setTaskFrequency(ExecutableType task, float frequency_Hz);

    /**
     * @brief Change interval (time between two nearest executions)
     * of currently running task.
     * @param task Previously added task which interval you want to change.
     * @param interval_us New interval (in us).
     * @return false if the task not found, otherwise true.
     */
    bool setTaskInterval_us(ExecutableType task, uint32_t interval_us);

    /**
     * @brief Pause currently running task for a specified time (in us).
     * @param task Previously added task you want to pause.
     * @param pauseTime_us Time (in us) after which task execution
     * will be continued.
     * @return false if the task not found, otherwise true.
     */
    bool pauseTask_us(ExecutableType task, uint32_t pauseTime_us);

    /**
     * @brief Pause currentlly running task for a specified time (in seconds).
     * @param task Previously added task you want to pause.
     * @param pauseTime_us Time (in seconds) after which task execution
     * will be continued.
     * @return false if the task not found, otherwise true.
     */
    bool pauseTask_s(ExecutableType task, float pauseTime_s);

    /**
     * @param task Previously added task whose frequency (in Hz) you want to check.
     * @return frequency (in Hz) of given task or
     * -1 if task hasn't been added to the tasker.
     */
    float getTaskFrequency_Hz(ExecutableType task);

    /**
     * @param task Previously added task whose interval (in us) you want to check.
     * @return interval (in us) of given task or
     * -1 (max uint32_t) if task hasn't been added to the tasker.
     */
    uint32_t getTaskInterval_us(ExecutableType task);

    /**
     * @param task Previously added task whose interval (in s) you want to check.
     * @return interval (in s) of given task or
     * -1 if task hasn't been added to the tasker.
     */
    float getTaskInterval_s(ExecutableType task);

    /**
     * @return Get current amount of tasks.
     */
    uint8_t getTasksAmount();

    /**
     * @brief Set your own function that will return after specified time
     * (intention is to sleep processor to use less power)
     * (this feature could be enabled or disabled in TaskerConfig.h file)
     * @param sleepFunction Pointer to function with one uint32_t parameter
     * (time in microseconds to wait) that return void. It is much better
     * for this function to wait too short than too long.
     */
    void setSleepFunction(SleepFunction sleepFunction);

    /**
     * @return Check current tasker load (from 0 to 100 [%])
     * (this feature could be enabled or disabled in TaskerConfig.h file)
     */
    float getLoad();
    
    /**
     * @return Execution start time of the last executed task
     * (almost current time). Faster than Arduino micros().
     */
    uint32_t getLastTaskExecutionTime_us();

    /**
     * @brief This should be the only method in arduino's loop().
     * Organizes all tasker work.
     */
    void loop();


private:
    /**
     * @brief Calculates which task is going to be executed next.
     */
    void calculateNextTask();

    /**
     * @param task Previously added task whose information you want to get.
     * @return Task mathing the argument or nullptr if was not found.
     */
    Task* getTask(ExecutableType task);
};



inline void Tasker::loop()
{
    uint32_t loopStartTime = micros();

    if (nextTask != nullptr && loopStartTime >= nextTask->nextExecutionTime_us)
    {
        lastTaskExecutionTime = loopStartTime;
        #ifdef TASKER_IEXECUTABLE_POINTER_TASK
            nextTask->executable->execute();
        #else
            nextTask->executable();
        #endif // TASKER_IEXECUTABLE_POINTER_TASK
        #ifdef TASKER_LOAD_CALCULATIONS
            uint32_t lastExecEndTime = micros();
        #endif // TASKER_LOAD_CALCULATIONS

        if (nextTask->taskType == TaskType::CATCHING_UP)
            nextTask->nextExecutionTime_us += nextTask->interval_us;
        else
            nextTask->nextExecutionTime_us = loopStartTime + nextTask->interval_us;

        calculateNextTask();

        #if defined(TASKER_SLEEP_FUNCTION) || defined(TASKER_LOAD_CALCULATIONS)
            int32_t timeToSleep = nextTask->nextExecutionTime_us - micros();

            if (timeToSleep > 0)
            {
                #ifdef TASKER_LOAD_CALCULATIONS
                    float taskExecTime = lastExecEndTime - lastTaskExecutionTime;
                    load = TASKER_LOAD_FILTER_BETA * load + TASKER_LOAD_FILTER_BETA_COFACTOR * (taskExecTime / (float(timeToSleep) + taskExecTime));
                #endif

                #ifdef TASKER_SLEEP_FUNCTION
                    sleepFunction(timeToSleep);
                #endif
            }
            else // timeToSleep <= 0
            {
                #ifdef TASKER_LOAD_CALCULATIONS
                    load = TASKER_LOAD_FILTER_BETA * load + TASKER_LOAD_FILTER_BETA_COFACTOR/* *1.f */; // 100% load
                #endif
            }
        #endif
    }
}


#endif
