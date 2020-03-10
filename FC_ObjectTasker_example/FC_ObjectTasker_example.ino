/*
    Name:       FC_ObjectTasker.ino
    Created:	07.03.2020
    Author:     JanWielgus

    Example program of object-oriented tasker (scheduler)
    library for the drone project
*/

#include <FC_ObjectTasker.h>
#include <FC_Task.h>

FC_ObjectTasker* tasker = new FC_ObjectTasker(5);
//FC_ObjectTasker tasker(5);


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// test variables, functions and tasks
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

volatile int varTask1 = 0;
volatile int varTask2 = 0;
volatile int varTask3 = 0;
volatile int output;

float otherFunciton(float zmienna)
{
    return (zmienna + 2) * 5.2;
}

class Task1 : public FC_Task
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

class Task2 : public FC_Task
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

class Task3 : public FC_Task
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

class ShowTask : public FC_Task
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
    tasker->addTask(new Task1, 20000L, 0); // 50Hz
    tasker->addTask(new Task2, 555L, 0); // 2500Hz
    tasker->addTask(new Task3, 10000L, 0); // 15Hz
    tasker->addTask(new ShowTask, 1000000L, 0); // 1Hz

    Serial.println("End of setup()");
}

void loop()
{
    tasker->run();
}
