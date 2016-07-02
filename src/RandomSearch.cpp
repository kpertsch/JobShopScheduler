//
// Created by karl on 01.05.16.
//

#include <cassert>
#include <ctime>
#include <iostream>

#include "RandomSearch.h"

namespace jss
{

/// Constructor
RandomSearch::RandomSearch(const std::string& file_name, unsigned seed)
    : SearchAlgorithm(file_name, seed)
{
}

std::shared_ptr<Schedule> RandomSearch::findSolutionInTime(double time_limit) const
{
    startTimer();

    std::shared_ptr<Schedule> best_schedule = generateRandomSolution();

    while (not isTimeLimitReached(time_limit))
    {
        std::shared_ptr<Schedule> schedule = generateRandomSolution();
        if (schedule->exec_time() < best_schedule->exec_time())
        {
            best_schedule = schedule;
        }
    }

    return best_schedule;
}
std::shared_ptr<Schedule> RandomSearch::findSolutionInSteps(unsigned step_limit) const
{
    std::shared_ptr<Schedule> best_schedule = generateRandomSolution();

    for (unsigned step = 0; step < step_limit; ++step)
    {
        std::shared_ptr<Schedule> schedule = generateRandomSolution();
        if (schedule->exec_time() < best_schedule->exec_time())
        {
            best_schedule = schedule;
        }
    }

    return best_schedule;
}

} // end namespace
