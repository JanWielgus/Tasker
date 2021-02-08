/**
 * @file SimpleTasker.cpp
 * @author Jan Wielgus
 * @brief 
 * @date 2020-07-31
 * 
 */

#include <SimpleTasker.h>


SimpleTasker::SimpleTasker(uint8_t maxTaskAmt)
    : MaxAmtOfTasks(maxTaskAmt)
{
    if (MaxAmtOfTasks > 0)
        tasksArray = new Task*[MaxAmtOfTasks];
    
    amtOfTasks = 0;
}


SimpleTasker::~SimpleTasker()
{
    if (MaxAmtOfTasks > 0)
        delete[] tasksArray;
}


bool SimpleTasker::addTask(Task* task, float frequency, uint16_t maxDuration)
{
    if (amtOfTasks >= MaxAmtOfTasks)
        return false;

    task->setProperties(frequency, maxDuration);

    // TODO: handle maxDuration param

    tasksArray[amtOfTasks] = task;
    amtOfTasks++;

    return true;
}


bool SimpleTasker::addTask(Task* task, float frequency)
{
    return addTask(task, frequency, 0);
}


uint32_t SimpleTasker::getCurrentTime_micros()
{
    return currentTime;
}


void SimpleTasker::runLoop()
{
    currentTime = micros();

    for (uint8_t i=0; i < amtOfTasks; i++)
    {
        if (currentTime >= tasksArray[i]->nextExecutionTime_us)
        {
            tasksArray[i]->nextExecutionTime_us = currentTime + tasksArray[i]->interval_us;
            tasksArray[i]->execute();
        }
    }
}


void SimpleTasker::execute()
{
    runLoop();
}
