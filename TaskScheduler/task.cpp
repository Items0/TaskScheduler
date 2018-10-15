#include "stdafx.h"
#include "task.h"


task::task()
{

}

task::task(int time, int earliness, int tardiness)
{
	this->time = time;
	this->earliness = earliness;
	this->tardiness = tardiness;
}


task::~task()
{
}

