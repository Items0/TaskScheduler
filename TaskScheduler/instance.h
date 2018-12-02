#pragma once
#include <vector>
#include "task.h"

class instance
{
public:
	int r;
	int target;
	std::vector <task> schedule;
	instance();
	instance(std::vector <task> schedule);
	~instance();
};

