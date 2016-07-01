#include "Schedule.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <tuple>
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
    std::vector<int> job_times = std::vector<int>(num_jobs, 0);

    // declare end time in function scope to use in end for execution time
    unsigned end_time = 0;

    // sort operations to respective machine schedules
    for (auto& op : *ssched)
    {
        auto start_time = std::max(machine_times[op.machine()], job_times[op.job_num()]);

        // set start time in operation and push to machine schedule
        op.setStartTime(start_time);
        m_machine_schedules.at(op.machine()).push(op); // wird hier auf Originaldaten geschrieben?

        // update time track arrays
        end_time = start_time + op.op_time();
        machine_times[op.machine()] = end_time;
        job_times[op.job_num()] = end_time;
    }

    std::vector<std::queue<Operation> >::iterator minEl, maxEl;
    std::tie(minEl, maxEl) = std::minmax_element(begin(m_machine_schedules), end(m_machine_schedules),
        [](std::queue<Operation> const& s1, std::queue<Operation> const& s2) {
            return s1.back().start_time() + s1.back().op_time() < s2.back().start_time() + s2.back().op_time();
        });

    m_exec_time = maxEl->back().start_time() + maxEl->back().op_time();
}

std::ostream& operator<<(std::ostream& os, const Schedule& sched)
{
    os << "********** SCHEDULE ***********" << std::endl;
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
