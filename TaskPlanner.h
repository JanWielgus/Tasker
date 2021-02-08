/**
 * @file TaskPlanner.h
 * @author Jan Wielgus
 * @brief Can be used to plan executing tasks for one time
 * @date 2020-12-03
 * 
 */

#ifndef TASKPLANNER_H
#define TASKPLANNER_H

#include "Task.h"


class TaskPlanner : public Task
{
private:
    const uint8_t MaxPlannedTasks; // At the same time
    Task** tasksArray;
    uint8_t plannedTasks = 0;
    uint32_t timeNow; // helper variable to store micros() value in execute


public:
    /**
     * @param maxPlannedTasks Maximum planned tasks at the same time.
     */
    TaskPlanner(uint8_t maxPlannedTasks);
    ~TaskPlanner();

    /**
     * @brief Schedule task to execute once.
     * @param task Pointer to class that extends Task class.
     * @param callIn Time in milliseconds to call passed task.
     * @return false if MaxPlannedTasks has been already reached
     * and task was not added.
     */
    bool scheduleTask_ms(Task* task, uint32_t callIn);

    /**
     * @brief Schedule task to execute once.
     * @param task Pointer to class that extends Task class.
     * @param callIn Time in microseconds to call passed task.
     * @return false if MaxPlannedTasks has been already reached
     * and task was not added.
     */
    bool scheduleTask_us(Task* task, uint32_t callIn);

    /**
     * @brief Check if execute scheduled tasks.
     * Have to be called as fast as possible
     * (faster -> better accuracy).
     */
    void execute() override;

    /**
     * @brief Get how many tasks are scheduled.
     */
    uint8_t getAmtOfScheduledTasks();

private:
    /**
     * @brief Remove one task from the array and shift remaining tasks
     * to fill the gap. Don't check if index is out of bounds.
     */
    void removeTaskFromArray(uint8_t index);
};


#endif

