//
// Created by karl on 01.05.16.
//

#pragma once

#include "Job.h"
#include "Schedule.h"

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
    std::shared_ptr<Schedule> findSolution(double time_limit) const;

    unsigned job_count() const { return m_jobs.size(); }

    unsigned machine_count() const { return m_machine_count; }

    unsigned operation_count() const { return m_operation_count; }

    unsigned thread_count() const { return m_thread_count; }

    unsigned swap_count() const { return std::accumulate(m_swap_counts.begin(), m_swap_counts.end(), 0); }

    unsigned random_count() const { return std::accumulate(m_random_counts.begin(), m_random_counts.end(), 0); }

protected:
    virtual std::shared_ptr<Schedule> findSolutionSerial(double time_limit) const = 0;
    std::default_random_engine& current_random_engine() const;

    std::shared_ptr<Schedule> generateRandomSolution() const;
    std::shared_ptr<std::vector<std::shared_ptr<Schedule> > > generateNeighbours(const Schedule& curr_pos) const;
    std::shared_ptr<Schedule> generateNeighbourSolution(const Schedule& curr_pos) const; /// generate one neighbour solution at random

    void startTimer() const;
    bool isTimeLimitReached(double time_limit) const;

    unsigned m_thread_count = 0;
    mutable std::chrono::steady_clock::time_point m_start_time;
    mutable std::vector<std::default_random_engine> m_random_engines;
    mutable std::vector<std::vector<Job> > m_jobs;
    unsigned m_machine_count = 0;
    unsigned m_operation_count = 0;
    mutable std::vector<unsigned> m_swap_counts;
    mutable std::vector<unsigned> m_random_counts;
};

} // end namespace
