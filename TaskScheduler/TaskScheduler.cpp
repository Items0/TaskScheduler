#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "task.h"
using namespace std;

int main()
{
	fstream handler("../Instances/sch10.txt", ios::in);
	fstream results("../results.txt", ios::out);
	int n;
	int totalTime = 0;
	float h = 0.6;
	handler >> n;
	vector <task> tab;
	for (int i = 0; i < n; i++)
	{
		int operationNum;
		handler >> operationNum;
		int time;
		int earliness;
		int tardiness;
		 
		//load values
		for (int k = 0; k < operationNum; k++)
		{
			handler >> time >> earliness >> tardiness;
			task tempTask(time, earliness, tardiness);
			tab.push_back(tempTask);
			totalTime += time;
		}
		int dueDate = floor(totalTime * h); // round or floor()?
		cout << "#" << i + 1 << " Total time: " << totalTime << " Due date: " << dueDate << "\n";


		//results << i + 1 << "\n" << operationNum << "\n" << totalTime << "\n";
		totalTime = 0;
		
		//display values
		for (int i = 0; i < tab.size(); i++)
		{
			cout << i+ 1 << " " << tab[i].time << " " << tab[i].earliness << " " << tab[i].tardiness << "\n";
		}
		tab.clear();
	}
	handler.close();
}

