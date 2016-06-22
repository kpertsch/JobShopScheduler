//
// Created by karl on 01.05.16.
//

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

std::shared_ptr<Schedule> RandomSearch::findSolutionInTime(double time_limit)
{
    (void)time_limit;

    auto serialized_schedule = generateRandomSolution();

    std::cout << "Successfully generated random solution!" << std::endl;

    auto schedule = std::make_shared<Schedule>(serialized_schedule, m_machine_count, m_jobs.size());

    return {};
}
std::shared_ptr<Schedule> RandomSearch::findSolutionInSteps(unsigned step_limit) const
{
    (void)step_limit;
    return {};
}

} // end namespace
