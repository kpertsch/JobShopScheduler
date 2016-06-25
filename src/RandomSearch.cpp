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

    auto best_schedule = std::make_shared<Schedule>(generateRandomSolution(), m_machine_count, m_jobs.size());

    while (not isTimeLimitReached(time_limit))
    {
        auto serialized_schedule = generateRandomSolution();
        auto schedule = std::make_shared<Schedule>(serialized_schedule, m_machine_count, m_jobs.size());
        if (schedule->exec_time() < best_schedule->exec_time())
        {
            best_schedule = schedule;
        }
    }

    return best_schedule;
}
std::shared_ptr<Schedule> RandomSearch::findSolutionInSteps(unsigned step_limit) const
{
    auto best_schedule = std::make_shared<Schedule>(generateRandomSolution(), m_machine_count, m_jobs.size());

    for (unsigned step = 0; step < step_limit; ++step)
    {
        auto serialized_schedule = generateRandomSolution();
        auto schedule = std::make_shared<Schedule>(serialized_schedule, m_machine_count, m_jobs.size());
        if (schedule->exec_time() < best_schedule->exec_time())
        {
            best_schedule = schedule;
        }
    }

    return best_schedule;
}

} // end namespace
