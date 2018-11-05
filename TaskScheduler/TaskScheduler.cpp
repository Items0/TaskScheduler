#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "task.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <algorithm>

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

int main()
{
	srand(time(NULL));

	fstream handler("../Instances/sch10.txt", ios::in);
	fstream results;
	int n;
	int totalTime;
	float h = 0.8;
	handler >> n;
	vector <task> tabEarliness;
	vector <task> tabTardiness;
	vector <task> schedule;

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
		int dueDate = floor(totalTime * h);
		cout << "#" << i + 1 << " Total time: " << totalTime << " Due date: " << dueDate << "\n";
		
		//sort (by earliness and tardiness) scheduling
		sort(tabEarliness.begin(), tabEarliness.end(), compareEarliness);
		sort(tabTardiness.rbegin(), tabTardiness.rend(), compareTardiness); //descending sort
		schedule = tabEarliness;
		schedule.insert(schedule.end(), tabTardiness.begin(), tabTardiness.end()); // copy vector

		//r setting
		int r = 0;
		schedule[0].start = r;

		//set start time
		for (int j = 1; j < schedule.size(); j++)
		{
			schedule[j].start = schedule[j - 1].start + schedule[j - 1].time;
		}

		//calculate value of target function
		int target = 0;
		int endTime;
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

		
		
		//display values
		/*
		for (int i = 0; i < tab.size(); i++)
		{
			cout << tab[i].id << " " << tab[i].time << " " << tab[i].earliness << " " << tab[i].tardiness << "\n";
		}
		cout << "Target function: " << target << "\n\n";
		*/

		//save results
		results.open("../Results/sch" + to_string(operationNum) + "_" + to_string(i+1) + "_" + to_string(static_cast<int>(h*10)) + ".txt", ios::out);
		results << h * 10 << "\n" << target << "\n" << operationNum << "\n" << r << "\n";
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
	

	//validate("sch500_1_2.txt");
}


