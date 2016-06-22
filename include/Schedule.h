#pragma once

#include "SerializedSchedule.h"

#include <iostream>
#include <queue>
#include <vector>
#include <memory>

namespace jss
{

class Schedule
{

public:
    Schedule(std::shared_ptr<SerializedSchedule> ssched, const unsigned& num_machines, const unsigned& num_jobs);

    unsigned exec_time() const
    {
        return m_exec_time;
    }

    friend std::ostream& operator<<(std::ostream& os, const Schedule& sched);

private:
    std::vector<std::queue<Operation>> m_machine_schedules;
    unsigned m_exec_time;
};

}
