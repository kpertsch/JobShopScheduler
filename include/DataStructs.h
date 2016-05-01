//
// Created by karl on 01.05.16.
//

#ifndef JOBSHOPSCHEDULE_DATASTRUCTS_H
#define JOBSHOPSCHEDULE_DATASTRUCTS_H

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
        uint32_t num_machines;
        uint32_t num_jobs;
        std::vector <MachineSchedule> schedules;
        OpTime total_op_time;
    };

} // end namespace

#endif //JOBSHOPSCHEDULE_DATASTRUCTS_H
