#include "MThread.h"
#include <thread>
int MThread::Start()
{
    std::thread t(&MThread::run,this);
    t.detach();
    return 0;
}