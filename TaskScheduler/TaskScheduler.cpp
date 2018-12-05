#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "task.h"
#include "instance.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <chrono>
#include <ctime>

using namespace std;

void validate(string filename)
{
	fstream handler("../Validator/" + filename, ios::in);
	vector <task> schedule;
	int h;
	int target;
	int n;
	int r;
	int totalTime = 0;
	handler >> h >> target >> n >> r;

	int time, earliness, tardiness;
	for (int k = 0; k < n; k++)
	{
		handler >> time >> earliness >> tardiness;
		schedule.emplace_back(k + 1, time, earliness, tardiness);
		totalTime += time;
	}

	int dueDate = floor(totalTime * 1.0 * h / 10);

	schedule[0].start = r;

	//set start time
	for (int j = 1; j < schedule.size(); j++)
	{
		schedule[j].start = schedule[j - 1].start + schedule[j - 1].time;
	}

	//calculate value of target function
	int checkTarget = 0;
	int endTime;
	for (int j = 0; j < schedule.size(); j++)
	{
		endTime = schedule[j].start + schedule[j].time;
		if (endTime < dueDate)
		{
			checkTarget += (dueDate - endTime) * schedule[j].earliness;
		}
		if (endTime > dueDate)
		{
			checkTarget += (endTime - dueDate) * schedule[j].tardiness;
		}
	}

	if (checkTarget == target)
	{
		cout << "OK!" << "\t" << target << "\n";
	}
	else
	{
		cout << "Error! Different values\n";
	}
	cout << "checkTarget = " << checkTarget << "\t target = " << target << endl;
}

bool compareEarliness(task &a, task &b)
{
	if (a.earliness != b.earliness)
	{
		return (a.earliness < b.earliness);
	}
	else
	{
		return (a.time > b.time);
	}
}

bool compareTardiness(task &a, task &b)
{
	if (a.tardiness != b.tardiness)
	{
		return (a.tardiness < b.tardiness);
	}
	else
	{
		return (a.time > b.time);
	}
}

bool compareInstances(instance &a, instance &b)
{
	return a.target < b.target;
}

void rCalculate(instance &myInstance, int dueDate)
{
	int target;
	int endTime;
	int globalTarget;
	int globalR = 0;
	int step = max(1, static_cast<int>(round(1.0 * dueDate / 100)));
	for (int r = 0; r <= dueDate / 2; r += step)
	{
		myInstance.schedule[0].start = r;

		//set start time
		for (int j = 1; j < myInstance.schedule.size(); j++)
		{
			myInstance.schedule[j].start = myInstance.schedule[j - 1].start + myInstance.schedule[j - 1].time;
		}

		//calculate value of target function
		target = 0;
		for (int j = 0; j < myInstance.schedule.size(); j++)
		{
			endTime = myInstance.schedule[j].start + myInstance.schedule[j].time;
			if (endTime < dueDate)
			{
				target += (dueDate - endTime) * myInstance.schedule[j].earliness;
			}
			if (endTime > dueDate)
			{
				target += (endTime - dueDate) * myInstance.schedule[j].tardiness;
			}
		}
		if (r == 0)
		{
			globalTarget = target;
			//cout << "r=0, " << target << "\t";
		}

		if (target < globalTarget)
		{
			globalTarget = target;
			globalR = r;
		}
	}
	myInstance.r = globalR;
	myInstance.target = globalTarget;
}

vector <instance> generateInitInstances(vector <task> &schedule, int instancesNumber, int dueDate)
{
	vector <instance> instances;
	for (int k = 0; k < instancesNumber; k++)
	{
		instances.push_back(schedule);
	}
	rCalculate(instances[0], dueDate);
	int randMultiply = 2;
	for (int i = 1; i < instances.size(); i++)
	{
		int maxIndex = instances[i].schedule.size();
		for (int k = 0; k < maxIndex * randMultiply; k++)
		{
			int index1 = rand() % maxIndex;
			int index2;
			do
			{
				index2 = rand() % maxIndex;
			} while (index1 == index2);
			swap(instances[i].schedule[index1], instances[i].schedule[index2]);
		}
		rCalculate(instances[i], dueDate);
	}
	return instances;
}

vector<task> mutation(vector <task> schedule)
{
	int index1 = rand() % schedule.size();
	int index2;
	do
	{
		index2 = rand() % schedule.size();
	} while (index1 == index2);
	swap(schedule[index1], schedule[index2]);
	return schedule;
}

vector <task> crossover(vector <task> scheduleA, vector <task> scheduleB)
{
	vector <task> result(scheduleA.begin(), scheduleA.begin() + scheduleA.size() / 2);
	for (int k = 0; k < result.size(); k++)
	{
		for (int j = 0; j < scheduleB.size(); j++)
		{
			if (result[k].id == scheduleB[j].id)
			{
				scheduleB.erase(scheduleB.begin() + j);
				break;
			}
		}
	}

	result.insert(result.end(), scheduleB.begin(), scheduleB.end());
	return result;
}

void selection(vector <instance>&instances, int instancesNumber)
{
	sort(instances.begin(), instances.end(), compareInstances);
	instances.resize(instancesNumber);
}

int main()
{
	srand(time(NULL));

	fstream handler("../Instances/sch1000.txt", ios::in);
	fstream results;
	int n;
	int totalTime;
	float h = 0.6;
	handler >> n;
	vector <task> schedule;
	vector <instance> instances;
	int topResult;

	//ARGS
	int instancesNumber = 20;
	int mutationChance = 20;
	int crossOverChance = 10;
	int equalRounds = 40;
	int roundCounter = equalRounds;
	int timeLimit = 60;

	for (int i = 0; i < n; i++)
	{
		int operationNum;
		handler >> operationNum;
		int time;
		int earliness;
		int tardiness;
		totalTime = 0;

		//load values
		for (int k = 0; k < operationNum; k++)
		{
			handler >> time >> earliness >> tardiness;
			schedule.emplace_back(k + 1, time, earliness, tardiness);
			totalTime += time;
		}

		chrono::time_point <chrono::system_clock> timeStart, timeEnd;
		chrono::duration <double>  processingTime;
		timeStart = chrono::system_clock::now();
		timeEnd = timeStart;
		processingTime = timeEnd - timeStart;
		int dueDate = floor(totalTime * h);
		//cout << "#" << i + 1 << " Total time: " << totalTime << " Due date: " << dueDate << "\n";

		instances = generateInitInstances(schedule, instancesNumber, dueDate);
		topResult = instances[0].target;
		//processing
		//int iterNo = 200;
		for (int iter = 0; ; iter++) // iteration number or processingTime < 1 min
		{
			cout << iter << " / " << processingTime.count() << "\r";
			for (int k = 0; k < instances.size(); k++)
			{
				if (rand() % 100 <= mutationChance)
				{
					//cout << "Mutation" << endl;
					instances.push_back(mutation(instances[k].schedule));
					rCalculate(instances.back(), dueDate);
				}

				if (rand() % 100 <= crossOverChance)
				{
					//cout << "Crossover" << endl;
					int index2;
					do
					{
						index2 = rand() % instances.size();
					} while (k == index2);
					instances.push_back(crossover(instances[k].schedule, instances[index2].schedule));
					rCalculate(instances.back(), dueDate);
				}
			}
			selection(instances, instancesNumber);

			//stop conditions
			timeEnd = chrono::system_clock::now();
			processingTime = timeEnd - timeStart;

			//topResult value update
			if (instances[0].target < topResult)
			{
				topResult = instances[0].target;
				roundCounter = equalRounds;
			}
			else
			{
				roundCounter--;
			}

			// breaks
			if (processingTime.count() > timeLimit)
			{
				cout << "Limit: timeLimit" << endl;
				break;
			}
			if (roundCounter < 0)
			{
				cout << "Limit: roundCounter" << endl;
				break;
			}
		}
		
		//the best instance in instance[0]
		timeEnd = chrono::system_clock::now();
		processingTime = timeEnd - timeStart;

		cout << "#" << i + 1 << " processingTime: " << processingTime.count() << endl;

		//save results
		results.open("../Results/sch" + to_string(operationNum) + "_" + to_string(i + 1) + "_" + to_string(static_cast<int>(h * 10)) + ".txt", ios::out);
		results << h * 10 << "\n" << instances[0].target << "\n" << operationNum << "\n" << instances[0].r << "\n";
		for (int i = 0; i < schedule.size(); i++)
		{
			results << instances[0].schedule[i].time << "\t" << instances[0].schedule[i].earliness << "\t" << instances[0].schedule[i].tardiness << "\n";
		}
		results.close();
		instances.clear();
		schedule.clear();
	}
	handler.close();
	
	//validate("sch50_10_2.txt");
}
