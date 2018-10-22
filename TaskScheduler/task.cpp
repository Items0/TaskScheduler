#include "stdafx.h"
#include "task.h"


task::task()
{

}

task::task(int id, int time, int earliness, int tardiness)
{
	this->id = id;
	this->time = time;
	this->earliness = earliness;
	this->tardiness = tardiness;
	this->start = 0;
}


task::~task()
{
}

