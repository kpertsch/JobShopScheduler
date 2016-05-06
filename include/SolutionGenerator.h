//
// Created by karl on 30.04.16.
//

#ifndef JOBSHOPSCHEDULE_SOLUTIONHANDLER_H
#define JOBSHOPSCHEDULE_SOLUTIONHANDLER_H


#include <random>
#include <memory>

#include "DataStructs.h"
#include "Evaluator.h"

namespace jobShopSolver {

class SolutionGenerator {

public:

    /// Constructor
    SolutionGenerator(const std::vector<Job>& jobs);

    /// generate random solution
    Solution generateRandomSolution();

    void printSolution(const Solution& solution) const ;

private:

    /// Retrieve number of machines
    Machine getNumberMachines(std::vector<Job>& jobs) const;

    /// Store jobs to complete
    std::vector<Job> procedure;

    /// store number of machines
    Machine num_machines;

    /// random number generator
    std::random_device rd;

    /// solution evaluator
    std::shared_ptr<Evaluator> evaluator;

};

} // end namespace

#endif //JOBSHOPSCHEDULE_SOLUTIONHANDLER_H
