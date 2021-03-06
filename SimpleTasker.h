/**
 * @file SimpleTasker.h
 * @author Jan Wielgus
 * @brief Simple tasker class
 * @date 2020-03-06
 * 
 */

#ifndef SIMPLETASKER_H
#define SIMPLETASKER_H

#include <ITasker.h>
#include <Task.h>


class SimpleTasker : public ITasker, public Task
{
protected:
    const uint8_t MaxAmtOfTasks;
    Task** tasksArray; // static array of MaxAmtOfTasks size
    uint8_t amtOfTasks = 0; // current amount of tasks (at most MaxAmtOfTasks)
    uint32_t currentTime = 0; // current time (used in run() method)

    const float LoadFilterBeta = 0.997f; // If load changes too rapidly or too slowly, adjust this value (bigger->change is slover)
    float load = 0; // from 0 to 100 [%]
    bool taskWasExecuted_flag; // helper in loop() method to calculate load


public:
    SimpleTasker(uint8_t maxTaskAmt);
    virtual ~SimpleTasker();

    /**
     * @brief Add task and set it's frequency.
     * 
     * @param task Pointer to concrete class that extend Task abstract class.
     * @param frequency Task running frequency (in Hz).
     * @return false if tasks array is full and this task was not added,
     * true otherwise.
     */
    bool addTask(Task* task, float frequency_Hz) override;

    /**
     * @brief Return (almost) current time. Faster than micros().
     */
    uint32_t getCurrentTime_micros() override;

    /**
     * @brief This should be the only method in loop(). Execute one Task execution time checking.
     * 
     */
    void loop() override;

    // TODO: make brief
    float getLoad() override;


protected:
    /**
     * @brief Enable to add mainTasker to another mainTasker as a task.
     * 
     */
    void execute() override;
};


#endif

