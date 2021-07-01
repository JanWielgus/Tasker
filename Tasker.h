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

    const uint8_t MaxAmtOfTasks;    //  max amount of tasks - set once in the constructor
    Task* tasks;                    // static array of MaxAmtOfTasks size
    uint8_t amountOfTasks = 0;      // current amount of tasks (at most MaxAmtOfTasks)
    uint32_t currentTime = 0;       // current time (used in loop() method)
    Task* nextTask;                 // next task to check if it's triggered

    const float LoadFilterBeta = 0.997f;    // If load changes too rapidly or too slowly, adjust this value (bigger->change is slover)
    const float LoadFilterBetaCofactor = 1 - LoadFilterBeta;
    float load = 0.f;                       // from 0 to 100 [%]
    float curTaskLoadHelper = 0.f;          // helper in loop() method to calculate load


public:
    Tasker(uint8_t maxTasksAmount);
    ~Tasker();

    /**
     * @brief Add task with it's frequency.
     * @param task Pointer to concrete class that implements IExecutable interface.
     * @param frequency Task running frequency (in Hz).
     * @return true if the task was successfully added
     * @return false otherwise
     */
    bool addTask_Hz(IExecutable* task, float frequency_Hz);

    /**
     * @brief Add task with it's interval.
     * @param task Pointer to concrete class that implements IExecutable interface.
     * @param interval_us Task running interval (in us).
     * @return true if the task was successfully added
     * @return false otherwise
     */
    bool addTask_us(IExecutable* task, uint32_t interval_us);

    /**
     * @brief Remove task from the tasker
     * @param task Task to remove
     * @return true if the task was found and therefore removed
     * @return false otherwise
     */
    bool removeTask(IExecutable* task);

    /**
     * @brief Set task's frequency (in Hz) 
     * @param task Task which fequency is to change
     * @param frequency_Hz New frequency (in Hz)
     * @return true if the task was found and therefore its frequency was changed
     * @return false otherwise
     */
    bool setTaskFrequency(IExecutable* task, float frequency_Hz);

    /**
     * @brief Set task's interval (in us)
     * @param task Task which interval is to change
     * @param interval_us New interval (in us)
     * @return true if the task was found and therefore its interval was changed
     * @return false otherwise
     */
    bool setTaskInterval_us(IExecutable* task, uint32_t interval_us);

    /**
     * @brief Pause task for a specified time (in us)
     * @param task Task to pause
     * @param pauseTime_us Time for task to be stopped (in us)
     * @return true if the task was found and therefore paused
     * @return false otherwise
     */
    bool pauseTask_us(IExecutable* task, uint32_t pauseTime_us);

    /**
     * @brief Pause task for a specified time (in s)
     * @param task Task to pause
     * @param pauseTime_us Time for task to be stopped (in s)
     * @return true if the task was found and therefore paused
     * @return false otherwise
     */
    bool pauseTask_s(IExecutable* task, float pauseTime_s);

    /**
     * @param task Task whose frequency (in Hz) is returned
     * @return frequency (in Hz) of given task or -1 if task isn't added to the tasker 
     */
    float getTaskFrequency_Hz(IExecutable* task);

    /**
     * @param task Task whose interval (in us) is returned
     * @return interval (in us) of given task or -1 (max int) if task isn't added to the tasker 
     */
    uint32_t getTaskInterval_us(IExecutable* task);

    /**
     * @param task Task whose interval (in s) is returned
     * @return interval (in s) of given task or -1 if task isn't added to the tasker 
     */
    float getTaskInterval_s(IExecutable* task);

    /**
     * @return current tasks amount 
     */
    uint8_t getTasksAmount();

    /**
     * @return current tasker load
     */
    float getLoad();
    
    /**
     * @return (almost) current time. Faster than micros() from arduino standard library.
     */
    uint32_t getCurrentTime_micros();

    /**
     * @brief This should be the only method in arduino's loop(). Execute one Task execution time checking.
     */
    void loop();


private:
    void calculateNextTask();
};


#endif
