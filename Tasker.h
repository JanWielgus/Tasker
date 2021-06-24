/**
 * @file Tasker.h
 * @author Jan Wielgus & Aleksy Walczak
 * @brief Tasker class
 * @date 2020-03-06
 * 
 */

#ifndef TASKER_H
#define TASKER_H

#include  "IExecutable.h"


class Tasker
{
    struct Task
    {
        IExecutable* executable;
        uint32_t interval_us;
        uint32_t nextExecutionTime_us;
    };

    const uint8_t MaxAmtOfTasks;
    Task* tasksArray; // static array of MaxAmtOfTasks size
    uint8_t amtOfTasks = 0; // current amount of tasks (at most MaxAmtOfTasks)
    uint32_t currentTime = 0; // current time (used in run() method)
    Task* nextTask; // next task to check

    const float LoadFilterBeta = 0.997f; // If load changes too rapidly or too slowly, adjust this value (bigger->change is slover)
    float load = 0; // from 0 to 100 [%]
    bool taskWasExecuted_flag; // helper in loop() method to calculate load


public:
    Tasker(uint8_t maxTaskAmt);
    ~Tasker();

    /**
     * @brief Add task and set it's frequency.
     * 
     * @param task Pointer to concrete class that extend Task abstract class.
     * @param frequency Task running frequency (in Hz).
     * @return false if tasks array is full and this task was not added,
     * true otherwise.
     */
    bool addTask(IExecutable* task, float frequency_Hz);


    bool addTaskInterval_us(IExecutable* task, uint32_t interval_us);


    bool removeTask(IExecutable* task);


    bool setTaskFrequency(IExecutable* task, float frequency_Hz);


    bool setTaskInterval_us(IExecutable* task, uint32_t interval_us);


    bool pauseTaskFor_us(IExecutable* task, uint32_t pause_us);


    bool pauseTaskFor_s(IExecutable* task, float pause_s);


    uint32_t getTaskInterval_us(IExecutable* task);


    float getTaskInterval_s(IExecutable* task);


    float getTaskFrequency_Hz(IExecutable* task);


    uint8_t getAmtOfTasks();
    
    /**
     * @brief Return (almost) current time. Faster than micros().
     */
    uint32_t micros() override;

    /**
     * @brief This should be the only method in loop(). Execute one Task execution time checking.
     * 
     */
    void loop() override;

    // TODO: make brief
    float getLoad() override;
};


#endif

