#include "stdafx.h"
#include "instance.h"
#include "task.h"
#include <vector>

instance::instance()
{
}

instance::instance(std::vector<task> schedule)
{
	this->r = 0;
	this->schedule = schedule;
	this->target = 0;
}


instance::~instance()
{
}
