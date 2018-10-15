#pragma once
class task
{
public:
	int id;
	int time;
	int earliness;
	int tardiness;

	task();
	task(int id, int time, int earliness, int tardiness);
	~task();
};

