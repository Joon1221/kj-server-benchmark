//
//  timer.hpp
//  2019_summer_project_client
//
//  Created by Joon Kang on 2019-08-04.
//  Copyright © 2019 Joon Kang. All rights reserved.
//

#ifndef TIMER_HPP
#define TIMER_HPP

#include <stdio.h>
#include <iostream>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point stopTime;
    double timeTaken;
//    clock_t startTime;
//    clock_t stopTime;

public:
    Timer();
    
    void startTimer();
    void stopTimer();
    
    void displayTime();
    double getTimeTaken();
};

#endif /* timer_hpp */
