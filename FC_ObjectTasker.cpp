// 
// 
// 

#include <FC_ObjectTasker.h>


FC_ObjectTasker::FC_ObjectTasker(uint8_t maxTaskAmt)
	: MaxAmtOfTasks(maxTaskAmt)
{
	tasksArray = new FC_Task* [MaxAmtOfTasks];
	amtOfTasks = 0;
}


FC_ObjectTasker::~FC_ObjectTasker()
{
	delete[] tasksArray;
}


bool FC_ObjectTasker::addTask(FC_Task* task)
{
	if (amtOfTasks >= MaxAmtOfTasks) // array is full
		return false;

	tasksArray[amtOfTasks] = task;
	amtOfTasks++;

	return true;
}


void FC_ObjectTasker::run()
{
	curTime = micros();
	for (uint8_t i = 0; i < amtOfTasks; i++)
	{
		/*curTime = micros();*/ // measuring time there cause worse efficiency than before for loop

		//if time has elapsed -> execute the task
		if (curTime >= tasksArray[i]->nextExecutionTime)
		{
			tasksArray[i]->nextExecutionTime = curTime + tasksArray[i]->interval;
			tasksArray[i]->execute();
		}
	}
}


