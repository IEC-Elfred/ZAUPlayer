#pragma once
#include <cstdio>
#include <thread>

class MMThread
{
public:
	virtual void run() = 0;
	int Start();
	int Stop();
	int stopFlag = 0;//信号量
private:
	std::thread* t = nullptr;
};