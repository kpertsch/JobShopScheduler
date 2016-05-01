//
// Created by karl on 01.05.16.
//

#ifndef JOBSHOPSCHEDULE_RANDOMSEARCH_H
#define JOBSHOPSCHEDULE_RANDOMSEARCH_H

#include "DataStructs.h"
#include "SearchAlgorithm.h"


namespace jobShopSolver {

class RandomSearch : public SearchAlgorithm {

public:

    /// Constructor
    RandomSearch(double t_limit);

    /// search best solution by random tries
    Solution findSolution(std::vector<Job>& jobs) const ;

};


} // end namespace

#endif //JOBSHOPSCHEDULE_RANDOMSEARCH_H
