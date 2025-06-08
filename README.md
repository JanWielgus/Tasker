# Tasker (Scheduler)

_Tasker_ is an Arduino library that enables execution of multiple fragments of code recurrently with different intervals.
 
## What is the purpose?

The purpose of this library is to simplify writing Arduino programs that simulate working on a multi-core environment. It can be achieved by dividing different functionalities into separate tasks (functions) and telling _Tasker_ how often they should be executed.
It enables you to easily make complex programs that will be very transparent (see [examples](https://github.com/JanWielgus/Tasker/tree/master/examples) and check how easy it is).

## How to use it?

### Installation

If you don't know how to add libraries to Arduino check out this website: https://www.arduino.cc/en/guide/libraries

### Use in code

To use this library include Tasker to your code

```cpp
#include <Tasker.h>
```

Then create your `Tasker` instance

```cpp
Tasker tasker(MaxTasksAmt);
```

MaxTasksAmt is the maximum amount of tasks added to the tasker at the same time ("concurrent" tasks).

Then it is time to create your tasks. As you can see it is very simple

```cpp
class ExampleTask1 : public IExecutable
{
    void execute() override
    {
        // Your actions inside
        Serial.println("Executed example task 1!");
    }
};
```

Each task is a new class that extends `IExecutable` class (interface). Put your actions inside the execute() method. This is just a normal class so for example you can add some private variables and even other methods.
Then create your task instance

```cpp
ExampleTask1 exampleTask1;
```

If you don't want to write your own constructor and you just want to create a task this is the simplest way

```cpp
class : public IExecutable
{
    void execute() override
    {
        // Your actions inside
        Serial.println("Executed example task 1!");
    }
} exampleTask1;
```

Of course you can create multiple classes that represent different tasks.
The next step is to add your tasks to the `Tasker` instance for example inside the `setup()` method.
In order to do this use `addTask_Hz()` (and tell Tasker execution frequency of your task) or `addTask_us()` (and tell Tasker interval between next executions in microseconds).

```cpp
void setup()
{
    tasker.addTask_Hz(&exampleTask1, 2.f);  // 2Hz -> two times per second
    // or
    tasker.addTask_us(&exampleTask1, 500000L);  // (2Hz) 500000us = 1000000us / 2 -> also two times per second
}
```

Now the only thing that left you is to call the `Tasker` loop() method inside the arduino `loop()`. This should be the only thing there.

```cpp
void loop()
{
    tasker.loop();
}
```

That's all.
**As you can see, this is really simple.**

This is part of the Tasker_example from [examples](https://github.com/JanWielgus/Tasker/tree/master/examples). Run it on your arduino and play around!





