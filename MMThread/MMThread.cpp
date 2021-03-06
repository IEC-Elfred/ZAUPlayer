#include "MMThread.h"
#include <iostream>
#include <ostream>
#include <thread>

int MMThread::Start()
{
	if(t == nullptr)
	{
		stopFlag = 0;
		t = new std::thread(&MMThread::run,this);
	}
	return 0;
}

int MMThread::Stop()
{
	if(t != nullptr)
	{
		stopFlag = 1;
		std::cout<<"flag"<<stopFlag<<std::endl;
		t->join();
		delete t;
		t = nullptr;
	}
	return 0;
}