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


public:
    SimpleTasker(uint8_t maxTaskAmt);
    virtual ~SimpleTasker();
    
    /**
     * @brief Add preconfigured task to tasker
     * 
     * @param task Pointer to concrete class that extend Task abstract class
     * @return false if task haven't got set up frequency and maxDuration
     */
    bool addTask(Task* task) override;

    /**
     * @brief Add task and set its frequency and maxDuration
     * 
     * @param task Pointer to concrete class that extend Task abstract class
     * @param frequency Task running frequency
     * @param maxDuration Measured task maxDuration (if not checked, set 0)
     * @return false if task haven't get set up frequency and maxDuration
     */
    bool addTask(Task* task, float frequency, uint16_t maxDuration) override;

    /**
     * @brief Return (almost) current time. Faster than micros()
     */
    uint32_t getCurrentTime_micros() override;

    /**
     * @brief This should be the only method in loop(). Execute one Task execution time checking.
     * 
     */
    void runLoop() override;


protected:
    /**
     * @brief Enable to add mainTasker to another mainTasker as a task
     * 
     */
    void execute() override;
};


#endif

