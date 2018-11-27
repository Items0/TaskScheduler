#pragma once
class instance
{
public:
	int r;
	int target;
	std::vector <task> schedule;
	instance();
	instance(std::vector <task>);
	~instance();
};

