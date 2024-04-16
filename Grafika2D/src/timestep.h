/* Copyright (c) 2022 Szabadi L�szl� Zsolt
 * You should have received a copy of the GNU AGPL v3.0 license with
 * this file. If not, please write to: pliexe, or visit : https://github.com/Pliexe/HeliosEngine/blob/master/LICENSE.txt
 */
#include <chrono>

#pragma once

namespace Grafika
{
    class Time {
    public:
        static float DeltaTime();
        static float FPS() { return FramesPerSecond(); }
        static float FramesPerSecond();
        static std::chrono::high_resolution_clock::time_point LastFrame();
        static double PassedTime();
        static unsigned long long CurrentTimeMicroseconds();

    private:
        static void internal_init();
        static void internal_frame_update();

        friend class App;
    };

}