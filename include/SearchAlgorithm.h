//
// Created by karl on 01.05.16.
//

#ifndef JOBSHOPSCHEDULE_SEARCHALGORITHM_H
#define JOBSHOPSCHEDULE_SEARCHALGORITHM_H

#include <vector>
#include "DataStructs.h"

namespace jobShopSolver {

class SearchAlgorithm {

public:

    virtual Solution findSolution(const std::vector<Job>& jobs) const = 0;

protected:

    /// store time limit
    double time_limit;

};

} // end namespace

#endif //JOBSHOPSCHEDULE_SEARCHALGORITHM_H
