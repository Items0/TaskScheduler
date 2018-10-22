#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "task.h"
#include <ctime>
#include <cstdlib>

using namespace std;

int main()
{
	srand(time(NULL));

	fstream handler("../Instances/sch10.txt", ios::in);
	fstream results("../results.txt", ios::out);
	int n;
	int totalTime;
	float h = 0.6;
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
		for (int i = 0; i < tab.size(); i++)
		{
			results << schedule[i].id << " " << schedule[i].time << " " << schedule[i].earliness << " " << schedule[i].tardiness << "\n";
		}
		results << "Target function: " << target << "\n\n";
		tab.clear();
		schedule.clear();
	}
	handler.close();
}

