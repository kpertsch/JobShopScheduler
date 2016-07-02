#pragma once

#include "Job.h"

#include <iostream>
#include <memory>
#include <queue>
#include <vector>

namespace jss
{

class Schedule
{

public:
    Schedule(std::vector<unsigned>&& job_schedule, unsigned machine_count, std::vector<Job>& jobs);

    unsigned exec_time() const
    {
        return m_exec_time;
    }

    bool swapJobSchedulePositions(unsigned index1, unsigned index2);
    friend std::ostream& operator<<(std::ostream& os, const Schedule& sched);

private:
    void generate_plan();

    std::vector<unsigned> m_job_schedule;
    unsigned m_machine_count;
    std::vector<Job>& m_jobs;
    std::vector<std::queue<Operation> > m_machine_schedules;
    unsigned m_exec_time;
};
}
