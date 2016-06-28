//
// Created by karl on 01.05.16.
//

#pragma once

#include "Job.h"
#include "Schedule.h"
#include "SerializedSchedule.h"

#include <chrono>
#include <memory>
#include <random>
#include <string>
#include <vector>

namespace jss
{

class SearchAlgorithm
{
public:
    SearchAlgorithm(const std::string& file_name, unsigned seed);

    // time_limit in seconds
    virtual std::shared_ptr<Schedule> findSolutionInTime(double time_limit) const = 0;
    virtual std::shared_ptr<Schedule> findSolutionInSteps(unsigned step_limit) const = 0;

    unsigned job_count() const
    {
        return m_jobs.size();
    }

    unsigned machine_count() const
    {
        return m_machine_count;
    }

    unsigned operation_count() const
    {
        return m_operation_count;
    }

protected:
    std::shared_ptr<SerializedSchedule> generateRandomSolution() const;
    std::shared_ptr<std::vector<std::shared_ptr<SerializedSchedule> > > generateNeighbours(const SerializedSchedule& curr_pos) const;
    std::shared_ptr<SerializedSchedule> generateNeighbourSolution(SerializedSchedule& curr_pos) const; /// generate one neighbour solution at random

    void startTimer() const;
    bool isTimeLimitReached(double time_limit) const;

    mutable std::chrono::steady_clock::time_point m_start_time;
    mutable std::default_random_engine m_random_engine;
    std::vector<Job> m_jobs;
    unsigned m_machine_count = 0;
    unsigned m_operation_count = 0;
};

} // end namespace
