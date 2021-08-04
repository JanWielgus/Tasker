/**
 * @file Tasker_example.ino
 * @author Jan Wielgus
 * @brief Example program for Tasker (scheduler) library.
 * @date 2021-06-25
 * 
 */

#include <Tasker.h>

const int MaxTasksAmt = 5;

Tasker tasker(MaxTasksAmt);


class ExampleTask1 : public IExecutable
{
    void execute() override  // override execute() method in each task
    {
        Serial.println("Executed example task 1!");
    }
};

ExampleTask1 exampleTask1;  // example way to create a task instance


class : public IExecutable
{
    void execute() override
    {
        Serial.println("Greetings from example task 2");
    }
} exampleTask2;  // another example way to create a task instance



void setup()
{
    Serial.begin(115200);
    Serial.println("Program has just started!");

    tasker.addTask_Hz(&exampleTask1, 2.f);  // [2Hz -> two times per second] add a periodic task and set it's execution frequency
    tasker.addTask_us(&exampleTask2, 2000000L);  // [0.5Hz = 100000us / 200000us -> every two seconds] add a periodic task and set interval in microseconds between each task execution
    // you can add up to MaxTasksAmt (number that you passed in the tasket ctor)

    Serial.println("End of setup()");
}


void loop()
{
    // This should be the only thing in the loop() function.
    // Divide all functionalities into tasks and add them to the tasker.
    // Your tasks will be executed periodically simulating a multithreaded system.
    tasker.loop();
}
