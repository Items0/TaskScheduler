#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "task.h"
#include <ctime>
#include <cstdlib>
#include <string>

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


int main()
{
	/*
	srand(time(NULL));

	fstream handler("../Instances/sch10.txt", ios::in);
	fstream results;
	int n;
	int totalTime;
	float h = 0.8;
	handler >> n;
	vector <task> tab;
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
			tab.emplace_back(k + 1, time, earliness, tardiness);
			totalTime += time;
		}
		int dueDate = floor(totalTime * h); // round or floor()?
		cout << "#" << i + 1 << " Total time: " << totalTime << " Due date: " << dueDate << "\n";
		
		//naive scheduling
		schedule = tab;
		for (int z = 0; z < operationNum / 2; z++)
		{
			int selectedIndex1 = rand() % tab.size();
			int selectedIndex2 = rand() % tab.size();
			swap(schedule[selectedIndex1], schedule[selectedIndex2]);
		}

		//r setting
		int r = 0;
		schedule[0].start = r;

		//set start time
		for (int j = 1; j < tab.size(); j++)
		{
			schedule[j].start = schedule[j - 1].start + schedule[j - 1].time;
		}

		//calculate value of target function
		int target = 0;
		int endTime;
		for (int j = 0; j < tab.size(); j++)
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
		for (int i = 0; i < tab.size(); i++)
		{
			cout << tab[i].id << " " << tab[i].time << " " << tab[i].earliness << " " << tab[i].tardiness << "\n";
		}
		cout << "Target function: " << target << "\n\n";

		//save results
		results.open("../Results/sch" + to_string(operationNum) + "_" + to_string(i+1) + "_" + to_string(static_cast<int>(h*10)) + ".txt", ios::out);
		results << h * 10 << "\n" << target << "\n" << operationNum << "\n" << r << "\n";
		for (int i = 0; i < tab.size(); i++)
		{
			results << schedule[i].time << "\t" << schedule[i].earliness << "\t" << schedule[i].tardiness << "\n";
		}
		results.close();
		
		tab.clear();
		schedule.clear();
	}
	handler.close();
	*/

	validate("sch500_1_2.txt");
}


