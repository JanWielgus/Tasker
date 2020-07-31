/*
    Created:	18.04.2020
    Author:     Jan Wielgus

    This program shows how to immitate tasks with priority in order to offload processor
    when there are some tasks that have lower update rate.
    This lower priority tasks can be checked by Tasker in slower rate.
    Create two taskers and add one as a task to the other one.
*/

#include <Task.h>
#include <SimpleTasker.h>

SimpleTasker mainTasker(5);
SimpleTasker lowPriorityTasker(5);



class ImportantTask1 : public Task
{
    void execute() override
    {
        // important task code
    }
};


class OccasionalTask1 : public Task
{
    void execute() override
    {
        // occasional task 1 code
    }
};


class OccasionalTask2 : public Task
{
    void execute() override
    {
        // occasional task 2 code
    }
};




void setup()
{
    // Add important tasks (leave one place for low priority tasker, it will be added just like other tasks)
    mainTasker.addTask(new ImportantTask1, 250.0f, 0); // 250Hz
    // other tasks that need high execution accuracy ...

    
    // Add lower priority tasks to the second tasker
    lowPriorityTasker.addTask(new OccasionalTask1, 1.0f, 0); // 1Hz
    lowPriorityTasker.addTask(new OccasionalTask2, 5.0f, 0); // 5Hz
    // other tasks which executin time could be checked less times per second ...


    // Add second (low priority) tasker as a task to the main tasker
    // check low priority tasks with precision of 1000 microseconds (1 ms) (this can be changed)
    // it is good to this interval be a divisor of all low priority tasks
    mainTasker.addTask(&lowPriorityTasker, 100.0f, 0); // 100Hz (check if need to update low priority tasks 100 times per second)
}

void loop()
{
    // execute only main tasker
    mainTasker.runLoop();
}

