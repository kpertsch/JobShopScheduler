//
// Created by karl on 01.05.16.
//

#ifndef JOBSHOPSCHEDULE_EVALUATOR_H
#define JOBSHOPSCHEDULE_EVALUATOR_H

#include <queue>

#include "DataStructs.h"

namespace jobShopSolver {


class Evaluator {

public:

    /// calculate Schedule Time and store to solution variable
    void evaluateSolution(Solution& solution) const;

private:

    /// check if all schedules done
    bool checkDone(const std::vector<std::queue<Operation>>& machine_schedules) const;

    /// update values of occupation vectors
    void updateOccupation(std::vector<bool>& machine_occupied, std::vector<bool>& job_occupied,
                          const OpTime& current_time, const std::vector<OpTime>& machine_times,
                          const uint32_t& num_machines, std::vector<std::queue<Operation>>& machine_schedules,
                          std::vector<uint32_t>& ops_finished) const;

};


} // end namespace

#endif //JOBSHOPSCHEDULE_EVALUATOR_H
