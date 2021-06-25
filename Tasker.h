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

    const uint8_t MaxAmtOfTasks;
    Task* tasks; // static array of MaxAmtOfTasks size
    uint8_t amountOfTasks = 0; // current amount of tasks (at most MaxAmtOfTasks)
    uint32_t currentTime = 0; // current time (used in run() method)
    Task* nextTask; // next task to check

    const float LoadFilterBeta = 0.997f; // If load changes too rapidly or too slowly, adjust this value (bigger->change is slover)
    const float LoadFilterBetaCofactor = 1 - LoadFilterBeta;
    float load = 0.f; // from 0 to 100 [%]
    float curTaskLoadHelper = 0.f; // helper in loop() method to calculate load


public:
    Tasker(uint8_t maxTasksAmount);
    ~Tasker();

    /**
     * @brief Add task and set it's frequency.
     * 
     * @param task Pointer to concrete class that extend Task abstract class.
     * @param frequency Task running frequency (in Hz).
     * @return false if tasks array is full and this task was not added,
     * true otherwise.
     */
    bool addTask_Hz(IExecutable* task, float frequency_Hz);


    bool addTask_us(IExecutable* task, uint32_t interval_us);


    bool removeTask(IExecutable* task);


    bool setTaskFrequency(IExecutable* task, float frequency_Hz);


    bool setTaskInterval_us(IExecutable* task, uint32_t interval_us);


    bool pauseTask_us(IExecutable* task, uint32_t pauseTime_us);


    bool pauseTask_s(IExecutable* task, float pauseTime_s);


    float getTaskFrequency_Hz(IExecutable* task);


    uint32_t getTaskInterval_us(IExecutable* task);


    float getTaskInterval_s(IExecutable* task);


    uint8_t getTasksAmount();

    
    float getLoad();
    
    /**
     * @brief Return (almost) current time. Faster than micros().
     */
    uint32_t getCurrentTime_micros();

    /**
     * @brief This should be the only method in loop(). Execute one Task execution time checking.
     * 
     */
    void loop();


private:
    void calculateNextTask();
};


#endif
