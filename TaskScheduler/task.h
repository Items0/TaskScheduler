#pragma once
class task
{
public:
	int time;
	int earliness;
	int tardiness;

	task();
	task(int time, int earliness, int tardiness);
	~task();
};

