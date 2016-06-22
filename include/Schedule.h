#pragma once

#include "SerializedSchedule.h"

#include <ostream>
#include <queue>
#include <vector>

namespace jss
{

class Schedule
{

public:
    Schedule(const SerializedSchedule& ssched);

    unsigned exec_time() const
    {
        return m_exec_time;
    }

    friend std::ostream& operator<<(std::ostream& os, const Schedule& sched);

private:
    std::vector<std::queue<Operation>> m_machine_schedules;
    unsigned m_exec_time;
};

std::ostream& operator<<(std::ostream& os, const Schedule& sched);
}
