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

std::shared_ptr<Schedule> RandomSearch::findSolutionInTime(double time_limit) const
{
    (void)time_limit;
    return {};
}
std::shared_ptr<Schedule> RandomSearch::findSolutionInSteps(unsigned step_limit) const
{
    (void)step_limit;
    return {};
}

} // end namespace
