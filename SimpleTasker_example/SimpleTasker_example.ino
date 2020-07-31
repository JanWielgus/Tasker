/*
    Name:       FC_ObjectTasker.ino
    Created:	07.03.2020
    Author:     JanWielgus

    Example program of object-oriented tasker (scheduler)
    library for the drone project
*/

#include <Task.h>
#include <SimpleTasker.h>

ITasker* tasker = new SimpleTasker(5);
//SimpleTasker tasker(5);


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// test variables, functions and tasks
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

int varTask1 = 0;
int varTask2 = 0;
int varTask3 = 0;
int output;

float otherFunciton(float zmienna)
{
    return (zmienna + 2) * 5.2;
}

class Task1 : public Task
{
    void execute() override
    {
        varTask1++;

        float localVar = 1;
        for (int i = 0; i < 5; i++)
        {
            localVar += otherFunciton(i);
        }
    }
};

class Task2 : public Task
{
    void execute() override
    {
        varTask2++;

        float zmienna = 5 * varTask1 + otherFunciton((float)varTask1);
        for (int i = 0; i < varTask1; i++)
        {
            zmienna += map(zmienna, 0, varTask3, -5, 100);
        }
    }
};

class Task3 : public Task
{
    void execute() override
    {
        varTask3++;

        if (varTask1 % 3 == 0)
        {
            float var = 0;

            for (int i = 0; i < 20; i++)
            {
                var += otherFunciton((float)varTask2);
                var++;
                var /= 5.5f;
                var -= map(var, 1, 5, -4, 50);
            }

            output = (int)var;
        }
    }
};

class ShowTask : public Task
{
    void execute() override
    {
        Serial.print("t1: ");
        Serial.println(varTask1);
        Serial.print("t2: ");
        Serial.println(varTask2);
        Serial.print("t3: ");
        Serial.println(varTask3);

        varTask1 = 0;
        varTask2 = 0;
        varTask3 = 0;
    }
};



// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

void setup()
{
    Serial.begin(115200);
    Serial.println("Program has just started.");

    // Add new instance of Task1 to the tasker
    tasker->addTask(new Task1, 50.0f, 0); // 50Hz
    tasker->addTask(new Task2, 500.0f, 0); // 500Hz
    tasker->addTask(new Task3, 17.0f, 0); // 17Hz
    tasker->addTask(new ShowTask, 1.0f, 0); // 1Hz

    Serial.println("End of setup()");
}

void loop()
{
    tasker->runLoop();
}
