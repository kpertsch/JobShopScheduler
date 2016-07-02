//
// Created by karl on 01.05.16.
//

#pragma once

#include "SearchAlgorithm.h"

namespace jss
{

class RandomSearch : public SearchAlgorithm
{

public:
    /// Constructor
    RandomSearch(const std::string& file_name, unsigned seed);

    /// search best solution by random tries
    std::shared_ptr<Schedule> findSolution(double time_limit) const override;
};

} // end namespace
