#include "../ZAUPlayer/ZAUPlayer.h"
#include "iostream"
#include <chrono>
#include <thread>


int main()
{
    ZAUPlayer player("/home/jinlin/projects/zauberflote/Reset_position.mp4");
    int ret = player.Open();
    if (ret) {
        std::cout<<"Player Open Fail"<<std::endl;
        return -1;
    }

    std::this_thread::sleep_for(std::chrono::seconds(60));
    player.Play();

    //...wait

    player.Pause();

    //...wait

    player.Play();

    //...wait

    player.Stop();

    return 0;
}