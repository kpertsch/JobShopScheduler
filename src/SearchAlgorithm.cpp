#include "SearchAlgorithm.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include <omp.h>

using namespace jss;

SearchAlgorithm::SearchAlgorithm(const std::string& file_name, unsigned seed, unsigned counters)
    : m_thread_count{ static_cast<unsigned>(omp_get_max_threads()) }
    , m_random_engines{ m_thread_count, std::default_random_engine(seed) }
    , m_counters((counters > 2) ? counters : 2, std::vector<unsigned>(m_thread_count, 0))
{

    for (unsigned i = 1; i < m_thread_count; ++i)
    {
        m_random_engines[i].discard(i);
    }

    std::ifstream file{ file_name };
    if (not file)
    {
        std::cerr << "j3s: Couldn't open file »" << file_name << "«" << std::endl;
        exit(1);
    }

    // ignore comment line
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // read the number of jobs and machine
    unsigned job_count = 0, mach_count = 0;
    if (not(file >> job_count >> mach_count) or job_count == 0 or mach_count == 0)
    {
        std::cerr << "j3s: Couldn't read the number of jobs and machines in file »" << file_name
                  << "«" << std::endl;
        exit(2);
    }
    // ignore newline
    file.ignore();

    // read all jobs
    std::vector<Job> jobs;
    unsigned op_count = 0;
    for (unsigned job_num = 0; job_num < job_count; ++job_num)
    {
        std::string job_line_str;
        std::getline(file, job_line_str);
        if (not file)
        {
            std::cerr << "j3s: Couldn't read job " << job_num << " in file »" << file_name << "«"
                      << std::endl;
            exit(2);
        }

        std::istringstream job_line{ job_line_str };
        Job job{ job_num };
        unsigned machine_num, op_time;
        unsigned op_num = 0;
        // read all operations of one job at a line
        while (job_line >> machine_num >> op_time)
        {
            if (not file)
            {
                std::cerr << "j3s: Couldn't read operation " << op_num << " of job " << job_num
                          << " in file »" << file_name << "«" << std::endl;
                exit(2);
            }
            if (machine_num >= mach_count)
            {
                std::cerr << "j3s: Wrong machine number in Operation " << op_num << " of job "
                          << job_num << " in file »" << file_name << "«" << std::endl;
                exit(2);
            }
            job.addOperation(Operation{ job_num, op_num, machine_num, op_time });
            ++op_num;
            ++op_count;
        }

        jobs.push_back(job);
    }

    m_jobs = std::vector<std::vector<Job> >{ m_thread_count, jobs };
    m_machine_count = mach_count;
    m_operation_count = op_count;
}

std::shared_ptr<Schedule> SearchAlgorithm::findSolution(double time_limit) const
{
    // reset counters
    for (std::vector<unsigned>& counter : m_counters)
    {
        for (unsigned& count : counter)
        {
            count = 0;
        }
    }

    std::vector<std::shared_ptr<Schedule> > schedules{ m_thread_count,
        std::shared_ptr<Schedule>() };
    startTimer();
#pragma omp parallel
    {
        schedules[omp_get_thread_num()] = findSolutionSerial(time_limit);
    }

    return *std::min_element(schedules.begin(), schedules.end(),
        [](std::shared_ptr<Schedule> first, std::shared_ptr<Schedule> second) {
            return *first < *second;
        });
}

std::string SearchAlgorithm::counterName(unsigned counter_idx) const
{
    switch (counter_idx)
    {
    case static_cast<unsigned>(CounterType::Random):
        return "Random solutions generated";
    case static_cast<unsigned>(CounterType::Neighbour):
        return "Neighbour solutions generated";
    default:
        return "";
    }
}

unsigned SearchAlgorithm::jobCount() const { return m_jobs.size(); }

unsigned SearchAlgorithm::machineCount() const { return m_machine_count; }

unsigned SearchAlgorithm::operationCount() const { return m_operation_count; }

unsigned SearchAlgorithm::threadCount() const { return m_thread_count; }

void SearchAlgorithm::increaseCounter(unsigned counter_idx, unsigned inc) const
{
    m_counters[counter_idx][omp_get_thread_num()] += inc;
}

unsigned SearchAlgorithm::countersCount() const { return m_counters.size(); }

unsigned SearchAlgorithm::counter(unsigned counter_idx) const
{
    return std::accumulate(m_counters.at(counter_idx).begin(), m_counters.at(counter_idx).end(), 0);
}

std::default_random_engine& SearchAlgorithm::currentRandomEngine() const
{
    int id = omp_get_thread_num();
    m_random_engines.at(id).discard(m_thread_count - 1);
    return m_random_engines.at(id);
}

std::shared_ptr<Schedule> SearchAlgorithm::generateRandomSolution() const
{
    std::vector<unsigned> job_schedule;
    job_schedule.reserve(m_operation_count);

    std::vector<Job>& jobs = m_jobs.at(omp_get_thread_num());
    std::uniform_int_distribution<int> uni(0, jobs.size() - 1);
    std::vector<unsigned> job_added_count = std::vector<unsigned>(jobs.size(), 0);

    // generate a random job number and add it to the schedule if the refered job
    // isn't finished
    for (unsigned jobs_added = 0; jobs_added < m_operation_count; ++jobs_added)
    {
        unsigned random_job = uni(currentRandomEngine());
        // can't add jobs that are already contained as many times as they have
        // operations
        while (job_added_count[random_job] == jobs[random_job].operationCount())
        {
            random_job = (random_job + 1) % jobs.size();
        }

        job_schedule.push_back(random_job);
        ++(job_added_count[random_job]);
    }

    ++(m_counters[static_cast<unsigned>(CounterType::Random)][omp_get_thread_num()]);
    return std::make_shared<Schedule>(std::move(job_schedule), m_machine_count, jobs);
}

std::shared_ptr<Schedule> SearchAlgorithm::generateNeighbourSolution(const Schedule& sol) const
{
    std::shared_ptr<Schedule> ret = std::make_shared<Schedule>(sol, false);
    std::uniform_int_distribution<int> uni(0, m_operation_count - 1);
    unsigned random_idx1 = uni(currentRandomEngine());
    unsigned random_idx2 = uni(currentRandomEngine());

    while (!ret->swapJobSchedulePositions(random_idx1, random_idx2))
    {
        random_idx1 = (random_idx1 + 1) % m_operation_count;
    }

    ++(m_counters[static_cast<unsigned>(CounterType::Neighbour)][omp_get_thread_num()]);
    return ret;
}

using namespace std::chrono;

void SearchAlgorithm::startTimer() const { m_start_time = steady_clock::now(); }

bool SearchAlgorithm::isTimeLimitReached(double time_limit) const
{
    return duration_cast<duration<double> >(steady_clock::now() - m_start_time).count()
        >= time_limit;
}
