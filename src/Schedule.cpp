#include "Schedule.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>

namespace jss
{

Schedule::Schedule(std::vector<unsigned>&& job_schedule, unsigned machine_count, std::vector<Job>& jobs)
    : m_job_schedule{ job_schedule }
    , m_jobs{ jobs }
    , m_machine_schedules{ machine_count, std::queue<Operation>() }
    , m_machine_count{ machine_count }
{
    generate_plan();
}

bool Schedule::swapJobSchedulePositions(unsigned index1, unsigned index2)
{
    // no invalid indices and useless swaps
    if (index1 >= m_job_schedule.size() or index2 >= m_job_schedule.size() or index1 == index2 or m_job_schedule[index1] == m_job_schedule[index2])
    {
        return false;
    }

    std::swap(m_job_schedule[index1], m_job_schedule[index2]);
    generate_plan();

    return true;
}

std::ostream& operator<<(std::ostream& os, const Schedule& sched)
{
    unsigned machine_counter = 0;
    for (auto machine_schedule : sched.m_machine_schedules)
    {
        os << "Machine " << machine_counter << ":" << std::endl;
        while (!machine_schedule.empty())
        {
            auto op = machine_schedule.front();
            os << "Job: " << op.job_num() << ", Operation: " << op.op_num() << ", Start time: " << op.start_time()
               << ", Duration: " << op.op_time() << std::endl;
            machine_schedule.pop();
        }
        ++machine_counter;
    }
    os << std::endl
       << "Total Duration: " << sched.m_exec_time << std::endl;
    return os;
}

bool Schedule::operator<(const Schedule& other)
{
    return this->m_exec_time < other.m_exec_time;
}

void Schedule::generate_plan()
{
    for (auto& machine_sched : m_machine_schedules)
    {
        machine_sched = std::queue<Operation>();
    }
    // initialize time track arrays
    std::vector<int> machine_times = std::vector<int>(m_machine_count, 0);
    std::vector<int> job_times = std::vector<int>(m_jobs.size(), 0);

    // declare end time in function scope to use in end for execution time
    unsigned end_time = 0;

    // sort operations to respective machine schedules
    for (unsigned job_index : m_job_schedule)
    {
        assert(not m_jobs.at(job_index).isDone());
        Operation op = m_jobs.at(job_index).popOperation();
        auto start_time = std::max(machine_times.at(op.machine()), job_times.at(op.job_num()));

        // set start time in operation and push to machine schedule
        op.setStartTime(start_time);
        m_machine_schedules.at(op.machine()).push(op);

        // update time track arrays
        end_time = start_time + op.op_time();
        machine_times.at(op.machine()) = end_time;
        job_times.at(op.job_num()) = end_time;
    }

    for (Job& job : m_jobs)
    {
        job.resetToBeginning();
    }

    std::vector<std::queue<Operation> >::iterator minEl, maxEl;
    std::tie(minEl, maxEl) = std::minmax_element(m_machine_schedules.begin(), m_machine_schedules.end(),
        [](std::queue<Operation> const& s1, std::queue<Operation> const& s2) {
            return s1.back().start_time() + s1.back().op_time() < s2.back().start_time() + s2.back().op_time();
        });

    m_exec_time = maxEl->back().start_time() + maxEl->back().op_time();
}
}
