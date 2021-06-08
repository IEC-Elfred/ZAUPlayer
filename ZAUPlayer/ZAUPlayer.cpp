#include <iostream>
#include <thread>
#include <chrono>
#include "../MThread/MThread.h"
#include "../MMAV/MMAV.h"


class MyMThread : public MThread
{   
    private:
        int a = 0;
    public: 
        virtual void run()
        {
            std::cout<<"MyMThread :"<< a <<std::endl;
        }

        MyMThread(int _a)
        {
            a = _a;
        }
};

int main()
{
    MMAVReader reader;
    int ret = reader.Open("/home/jinlin/projects/zauberflote/tor.mkv");
    if(ret)
    {
        std::cout << "Open File Fail!!!"<<std::endl;
        return -1;
    }
    while (1) {
        MMAVPacket pkt;
        reader.Read(&pkt);
        if (ret)
        {
            break;
        }
        std::cout << "Read Packet Success" << std::endl;
    }
    reader.Close();
    return 0;
}
