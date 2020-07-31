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


bool SimpleTasker::addTask(Task* task)
{
    if (!task->isConfigured())
        return false;
    
    if (amtOfTasks >= MaxAmtOfTasks)
        return false;
    

    tasksArray[amtOfTasks] = task;
    amtOfTasks++;

    return true;
}


bool SimpleTasker::addTask(Task* task, float frequency, uint16_t maxDuration)
{
    task->setProperties(frequency, maxDuration);

    return addTask(task);
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
        if (currentTime >= tasksArray[i]->nextExecutionTime)
        {
            tasksArray[i]->nextExecutionTime = currentTime + tasksArray[i]->interval;
            tasksArray[i]->execute();
        }
    }
}


void SimpleTasker::execute()
{
    runLoop();
}
