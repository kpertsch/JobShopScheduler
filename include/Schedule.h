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
    enum class DispatchRule
    {
        ShortestProcessSequence,
        ShortestProcessingTime,
        LongestTotalProcessingTime,
    };

    Schedule(std::vector<unsigned>&& job_schedule, unsigned machine_count, std::vector<Job>& jobs);
    Schedule(const Schedule& other, bool copy_machine_schedule = true);

    unsigned execTime() const;
    unsigned relativeFitnessToWorst(unsigned worstExecTime);

    bool swapJobSchedulePositions(unsigned index1, unsigned index2);
    void inverseRangeMove(unsigned beginIdx, unsigned endIdx, unsigned newBeginIdx);

    static std::shared_ptr<Schedule> precedencePreservingCrossover(const std::vector<bool>& randDecs, const Schedule& sol1, const Schedule& sol2);

    void storeAsImage(const std::string& file_name) const;

    friend std::ostream& operator<<(std::ostream& os, const Schedule& sched);
    bool operator<(const Schedule& other);

private:
    void generatePlan();

    std::vector<unsigned> m_job_schedule;
    unsigned m_machine_count;
    std::vector<Job>& m_jobs;
    std::vector<std::queue<Operation> > m_machine_schedules;
    unsigned m_exec_time;
};
}
