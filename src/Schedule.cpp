#include "Schedule.h"

#include <iostream>
#include <vector>

namespace jss
{

Schedule::Schedule(std::shared_ptr<SerializedSchedule> ssched, const unsigned& num_machines, const unsigned& num_jobs)
{
    // initialize machine schedule vector
    for (auto machine = 0; machine < num_machines; ++machine)
    {
        m_machine_schedules.push_back(std::queue<Operation>());
    }

    // initialize time track arrays
    std::vector<int> machine_times = std::vector<int>(num_machines, 0);
    std::vector<int> job_times = std::vector<int>(num_machines, 0);

    // declare end time in function scope to use in end for execution time
    unsigned end_time = 0;

    // sort operations to respective machine schedules
    for (auto op_it = ssched->begin(); op_it != ssched->end(); ++op_it)
    {
        auto start_time = std::max(machine_times[op_it->machine()], job_times[op_it->job_num()]);

        // set start time in operation and push to machine schedule
        auto op = *op_it;
        op.setStartTime(start_time);
        m_machine_schedules.at(op_it->machine()).push(op); // wird hier auf Originaldaten geschrieben?

        // update time track arrays
        end_time = start_time + op_it->op_time();
        machine_times[op_it->machine()] = end_time;
        job_times[op_it->job_num()] = end_time;
    }
    m_exec_time = end_time;
}

std::ostream& operator<<(std::ostream& os, const Schedule& sched)
{
    os << std::endl
       << "********** SCHEDULE ***********" << std::endl;
    unsigned machine_counter = 0;
    for (auto machine_schedule : sched.m_machine_schedules)
    {
        os << "MACHINE " << machine_counter << ":" << std::endl;
        while (!machine_schedule.empty())
        {
            auto op = machine_schedule.front();
            os << "Job: " << op.job_num() << ", OP: " << op.op_num() << ", Start time: " << op.start_time()
               << ", Duration: " << op.op_time() << std::endl;
            machine_schedule.pop();
        }
        ++machine_counter;
    }
    os << std::endl
       << "Total Duration: " << sched.m_exec_time << std::endl;
    return os;
}
}
