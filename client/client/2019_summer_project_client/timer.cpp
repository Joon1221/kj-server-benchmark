//
//  timer.cpp
//  2019_summer_project_client
//
//  Created by Joon Kang on 2019-08-04.
//  Copyright © 2019 Joon Kang. All rights reserved.
//

#include "timer.h"

using namespace std;

Timer::Timer() {
    
}

void Timer::startTimer() {
    startTime = std::chrono::high_resolution_clock::now();
}

void Timer::stopTimer() {
    stopTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(stopTime - startTime);
    timeTaken = time_span.count();
}

void Timer::displayTime() {
    std::cout << "Time taken: " << timeTaken << " seconds" << endl;
}

double Timer::getTimeTaken() {
    return timeTaken;
}
