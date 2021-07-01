/**
 * @file TaskerFreqTest.ino
 * @author Jan Wielgus
 * @brief Program used to test if tasks are executed
 * with set frequency.
 * @date 2021-06-25
 * 
 */

#include <Tasker.h>


// Config:
const int TestTasksAmt = 5;
const float TestTasksFreq[] = {5.f, 59.f, 200.f, 107.f, 108.f};  // Frequencies for each test task


static_assert(TestTasksAmt == (sizeof(TestTasksFreq) / sizeof(TestTasksFreq[0])), "Provide frequency for each test task");
float someCalculations(float var);


class TestTask : public IExecutable
{
public:
    long counter = 0;
    float dumb; // does nothing

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
        Serial.print("Results:");
        for (int i = 0; i < TestTasksAmt; i++)
        {
            // Show results
            Serial.print("T");
            Serial.print(i+1);
            Serial.print(": ");
            Serial.print(testTasks[i].counter);
            Serial.print('\t');

            testTasks[i].counter = 0;  // reset the counter
        }

        // Show load
        Serial.print("Load: ");
        Serial.print(tasker.getLoad());
        Serial.print('\t');
        
        Serial.println();
    }
} resultsTask;



void setup()
{
    Serial.begin(115200);
    Serial.println("Program has just started!");

    for (int i = 0; i < TestTasksAmt; i++)
        tasker.addTask_Hz(&testTasks[i], TestTasksFreq[i]);

    tasker.addTask_Hz(&resultsTask, 1.f);  // 1 Hz

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
