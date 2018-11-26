#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "task.h"
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

vector <vector <task>> generateInitInstances(vector <task> &schedule, int instancesNumber)
{
	vector <vector <task>> instances(instancesNumber, schedule);
	int randMultiply = 2;
	for (int i = 0; i < instances.size(); i++)
	{
		int maxIndex = instances[i].size();
		for (int k = 0; k < maxIndex * randMultiply; k++)
		{
			int index1 = rand() % maxIndex;
			int index2;
			do
			{
				index2 = rand() % maxIndex;
			} while (index1 == index2);
			swap(instances[i][index1], instances[i][index2]);
		}
	}
	return instances;
}

void mutation(vector <task> &schedule)
{
	int index1 = rand() % schedule.size();
	int index2;
	do
	{
		index2 = rand() % schedule.size();
	} while (index1 == index2);
	swap(schedule[index1], schedule[index2]);
}

vector <task> crossover(vector <task> scheduleA, vector <task> scheduleB)
{
	return scheduleA;
}

vector <vector <task> >selection(vector <vector <task> >&instances, int intancesNumber)
{
	return instances;
}

int main()
{
	srand(time(NULL));

	fstream handler("../Instances/sch500.txt", ios::in);
	fstream results;
	int n;
	int totalTime;
	float h = 0.6;
	handler >> n;
	vector <task> tabEarliness;
	vector <task> tabTardiness;
	vector <task> schedule;

	//ARGS
	int instancesNumber = 30;
	int mutationChance = 20;
	int crossOverChance = 10;
	int improveRate = 10;

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
			if (earliness < tardiness)
			{
				tabEarliness.emplace_back(k + 1, time, earliness, tardiness);
			}
			else
			{
				tabTardiness.emplace_back(k + 1, time, earliness, tardiness);
			}
			totalTime += time;
		}

		chrono::time_point <chrono::system_clock> timeStart, timeEnd;
		timeStart = chrono::system_clock::now();
		int dueDate = floor(totalTime * h);
		//cout << "#" << i + 1 << " Total time: " << totalTime << " Due date: " << dueDate << "\n";

		//sort (by earliness and tardiness) scheduling
		sort(tabEarliness.begin(), tabEarliness.end(), compareEarliness);
		sort(tabTardiness.rbegin(), tabTardiness.rend(), compareTardiness); //descending sort
		schedule = tabEarliness;
		schedule.insert(schedule.end(), tabTardiness.begin(), tabTardiness.end()); //copy vector

		//r setting
		int target;
		int endTime;
		int globalTarget;
		int globalR = 0;
		int step = max(1, static_cast<int>(round(1.0 * dueDate / 100)));
		for (int r = 0; r <= dueDate / 2; r += step)
		{
			schedule[0].start = r;

			//set start time
			for (int j = 1; j < schedule.size(); j++)
			{
				schedule[j].start = schedule[j - 1].start + schedule[j - 1].time;
			}

			//calculate value of target function
			target = 0;
			for (int j = 0; j < schedule.size(); j++)
			{
				endTime = schedule[j].start + schedule[j].time;
				if (endTime < dueDate)
				{
					target += (dueDate - endTime) * schedule[j].earliness;
				}
				if (endTime > dueDate)
				{
					target += (endTime - dueDate) * schedule[j].tardiness;
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

		timeEnd = chrono::system_clock::now();
		chrono::duration <double>  processingTime = timeEnd - timeStart;

		//cout << "r=" << globalR << ", " << globalTarget << endl;
		cout << "#" << i + 1 << " processingTime: " << processingTime.count() << endl;
		//save results
		results.open("../Results/sch" + to_string(operationNum) + "_" + to_string(i + 1) + "_" + to_string(static_cast<int>(h * 10)) + ".txt", ios::out);
		results << h * 10 << "\n" << globalTarget << "\n" << operationNum << "\n" << globalR << "\n";
		for (int i = 0; i < schedule.size(); i++)
		{
			results << schedule[i].time << "\t" << schedule[i].earliness << "\t" << schedule[i].tardiness << "\n";
		}
		results.close();

		tabEarliness.clear();
		tabTardiness.clear();
		schedule.clear();
	}
	handler.close();


	//validate("sch100_10_6.txt");
}


