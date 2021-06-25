/**
 * @file Tasker_example.ino
 * @author Jan Wielgus
 * @brief Example program for Tasker (scheduler) library.
 * @date 2021-06-25
 * 
 */

#include <Tasker.h>


Tasker tasker(5);


// some shared variables between tasks
int task1Counter = 0;
int task2Counter = 0;
int task3Counter = 0;
int output;


float someFunction(float zmienna)
{
    return (zmienna + 2) * 5.2;
}

class Task1 : public IExecutable
{
    void execute() override
    {
        task1Counter++;

        float localVar = 1;
        for (int i = 0; i < 5; i++)
        {
            localVar += someFunction(i);
        }
    }
};

Task1 task1;  // one way to create task instance


class : public IExecutable  // you can skip class name and create instances at the end of the class
{
    void execute() override
    {
        task2Counter++;

        float zmienna = 5 * task1Counter + someFunction((float)task1Counter);
        for (int i = 0; i < 30; i++)
        {
            zmienna += map(zmienna, -1, task3Counter, -5, 100);
        }
    }
} task2;  // another way to create task instance


class Task3 : public IExecutable
{
    void execute() override
    {
        task3Counter++;

        if (task1Counter % 3 == 0)
        {
            float var = 0;

            for (int i = 0; i < 20; i++)
            {
                var += someFunction((float)task2Counter);
                var++;
                var /= 5.5f;
                var -= map(var, 1, 5, -4, 50);
            }

            output = (int)var;
        }
    }
} task3;


class : public IExecutable
{
    void execute() override
    {
        Serial.print("t1: ");
        Serial.println(task1Counter);
        Serial.print("t2: ");
        Serial.println(task2Counter);
        Serial.print("t3: ");
        Serial.println(task3Counter);
        Serial.print("Load: ");
        Serial.println(tasker.getLoad());

        task1Counter = 0;
        task2Counter = 0;
        task3Counter = 0;
    }
} showTask;



void setup()
{
    Serial.begin(115200);
    Serial.println("Program has just started!");

    // Add tasks to the tasker
    tasker.addTask_Hz(&task1, 50.f);
    tasker.addTask_us(&task2, 2000);
    tasker.addTask_Hz(&task3, 17.f);
    tasker.addTask_Hz(&showTask, 1.f);

    Serial.println("End of setup()");
}


void loop()
{
    tasker.loop();
}
