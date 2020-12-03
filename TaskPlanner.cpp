/**
 * @file TaskPlanner.cpp
 * @author Jan Wielgus
 * @date 2020-12-03
 * 
 */

#include "TaskPlanner.h"


TaskPlanner::TaskPlanner(uint8_t maxPlannedTasks)
    : MaxPlannedTasks(maxPlannedTasks)
{
    tasksArray =  new Task*[MaxPlannedTasks];
    plannedTasks = 0;
}


TaskPlanner::~TaskPlanner()
{
    delete[] tasksArray;
}


bool TaskPlanner::scheduleTask_ms(Task* task, uint32_t callIn)
{
    if (plannedTasks >= MaxPlannedTasks)
        return false;

    task->nextExecutionTime_us = micros() + (callIn * 1000);
    tasksArray[plannedTasks] = task;
    plannedTasks++;

    return true;
}


bool TaskPlanner::scheduleTask_us(Task* task, uint32_t callIn)
{
    if (plannedTasks >= MaxPlannedTasks)
        return false;

    task->nextExecutionTime_us = micros() + callIn;
    tasksArray[plannedTasks] = task;
    plannedTasks++;

    return true;
}


void TaskPlanner::execute()
{
    timeNow = micros();

    for (uint8_t i = 0; i < plannedTasks; i++)
    {
        if (timeNow >= tasksArray[i]->nextExecutionTime_us)
        {
            tasksArray[i]->execute();
            removeTaskFromArray(i);
        }
    }
}


uint8_t TaskPlanner::getAmtOfScheduledTasks()
{
    return plannedTasks;
}


void TaskPlanner::removeTaskFromArray(uint8_t index)
{
    for (uint8_t i = index + 1; i < plannedTasks; i++)
        tasksArray[i-1] = tasksArray[i];

    --plannedTasks;
}

