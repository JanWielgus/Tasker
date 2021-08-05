/**
 * @file TaskerFreqTest.ino
 * @author Jan Wielgus
 * @brief Program used to test if tasks are executed with set frequency.
 * @date 2021-06-25
 * 
 */

#include <Tasker.h>


// Config:
const int TestTasksAmt = 5;
const float TestTasksFreq[] = {5.f, 59.f, 200.f, 107.5f, 108.f};  // frequencies for each test task
const float ResultsTaskFreq = 0.5f;

float someCalculations(float var);


class TestTask : public IExecutable
{
public:
    volatile long counter = 0;
    volatile float dumb; // does nothing

private:
    void execute() override
    {
        counter++;
        dumb = someCalculations(counter);  // just to take some time
    }
};


Tasker tasker(TestTasksAmt+1);  // +1 for results show task
TestTask testTasks[TestTasksAmt];


class : public IExecutable
{
    void execute() override
    {
        for (int i = 0; i < TestTasksAmt; i++)
        {
            // Show results
            Serial.print("T");
            Serial.print(i+1);
            Serial.print(": ");
            Serial.print(testTasks[i].counter * ResultsTaskFreq);
            Serial.print('\t');

            testTasks[i].counter = 0;  // reset the counter
        }
        
        Serial.print("Load: ");
        Serial.println(tasker.getLoad());
    }
} resultsTask;



void setup()
{
    Serial.begin(115200);
    Serial.println("Program has just started!");

    for (int i = 0; i < TestTasksAmt; i++)
        tasker.addTask_Hz(&testTasks[i], TestTasksFreq[i]);

    tasker.addTask_Hz(&resultsTask, ResultsTaskFreq);

    Serial.println("End of setup()");
}


void loop()
{
    tasker.loop();
}



float someCalculations(float var)
{
    volatile float temp = (var + 2) * 5.2f;

    for (int i = 0; i < 20; i++)
        temp += (i + var) * var - temp;

    return temp;
}


static_assert(TestTasksAmt <= (sizeof(TestTasksFreq) / sizeof(TestTasksFreq[0])), "Provide frequency for each test task");
