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
    enum class CounterType : unsigned
    {
        Random,
        Neighbour,
    };

    SearchAlgorithm(const std::string& file_name, unsigned seed, unsigned counters);

    // time_limit in seconds
    std::shared_ptr<Schedule> findSolution(double time_limit) const;
    unsigned counter(unsigned counter_idx) const;
    virtual std::string counterName(unsigned counter_idx) const;
    unsigned countersCount() const;
    unsigned jobCount() const;
    unsigned machineCount() const;
    unsigned operationCount() const;
    unsigned threadCount() const;

protected:
    void increaseCounter(unsigned counter_idx, unsigned inc = 1) const;

    std::default_random_engine& currentRandomEngine() const;

    virtual std::shared_ptr<Schedule> findSolutionSerial(double time_limit) const = 0;

    std::shared_ptr<Schedule> generateRandomSolution() const;
    std::shared_ptr<Schedule> generateNeighbourSolution(const Schedule& sol) const;

    void startTimer() const;
    bool isTimeLimitReached(double time_limit) const;

    const unsigned m_thread_count;
    mutable std::chrono::steady_clock::time_point m_start_time;
    mutable std::vector<std::default_random_engine> m_random_engines;
    mutable std::vector<std::vector<Job> > m_jobs;
    mutable std::vector<std::vector<unsigned> > m_counters;
    unsigned m_machine_count = 0;
    unsigned m_operation_count = 0;
};

} // end namespace
