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


bool SimpleTasker::addTask(Task* task, float frequency)
{
    if (amtOfTasks >= MaxAmtOfTasks)
        return false;

    task->setFrequency_Hz(frequency);
    task->nextExecutionTime_us = micros();

    tasksArray[amtOfTasks] = task;
    amtOfTasks++;

    return true;
}


uint32_t SimpleTasker::getCurrentTime_micros()
{
    return currentTime;
}


void SimpleTasker::loop()
{
    currentTime = micros();
    taskWasExecuted_flag = false;

    for (uint8_t i=0; i < amtOfTasks; i++)
    {
        if (currentTime >= tasksArray[i]->nextExecutionTime_us)
        {
            // Calculate nextExecutionTime based on the previous nextExecutionTime
            tasksArray[i]->nextExecutionTime_us += tasksArray[i]->interval_us; // This way of time calculation results in better precision
            //tasksArray[i]->nextExecutionTime_us = currentTime + tasksArray[i]->interval_us;

            tasksArray[i]->execute();

            // Indicate that at least one task was executed
            taskWasExecuted_flag = true;
        }
    }

    // Update load
    load = LoadFilterBeta*load + (1-LoadFilterBeta)*(taskWasExecuted_flag ? 100.f : 0.f);
}


float SimpleTasker::getLoad()
{
    return load;
}


void SimpleTasker::execute()
{
    loop();
}
