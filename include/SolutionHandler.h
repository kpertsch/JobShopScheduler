//
// Created by karl on 30.04.16.
//

#ifndef JOBSHOPSCHEDULE_SOLUTIONHANDLER_H
#define JOBSHOPSCHEDULE_SOLUTIONHANDLER_H

#include <vector>
#include <stdint.h>

namespace jobShopSolver {

    using JobNum = uint32_t;
    using OperationNum = uint32_t;
    using Machine = uint32_t;
    using OpTime = double;

    struct Operation {
        JobNum job_num;
        OperationNum op_num;
        Machine machine;
        OpTime op_time;
    };

    using Job = std::vector<Operation>;
    using MachineSchedule = std::vector<Operation>;

    struct Solution {
        std::vector<MachineSchedule> schedules;
        OpTime total_op_time;
    };

class SolutionHandler {

public:

    /// Constructor
    SolutionHandler(std::vector<Job> jobs);

    /// generate random solution
    void generateRandomSolution();

    /// return best solution
    Solution getBestSolution() const {return best_solution;}

private:

    /// Retrieve number of machines
    Machine getNumberMachines(std::vector<Job> jobs) const;

    /// Store jobs to complete
    std::vector<Job> procedure;

    /// store number of machines
    Machine num_machines;

    /// Member for storing best solution so far
    Solution best_solution;

};

} // end namespace

#endif //JOBSHOPSCHEDULE_SOLUTIONHANDLER_H
