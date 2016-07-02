#include "SearchAlgorithm.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include <omp.h>

using namespace jss;

SearchAlgorithm::SearchAlgorithm(const std::string& file_name, unsigned seed)
    : m_thread_count{ static_cast<unsigned>(omp_get_max_threads()) }
    , m_random_engines{ static_cast<std::vector<std::default_random_engine>::size_type>(omp_get_max_threads()),
        std::default_random_engine(seed) }
{

    for (int i = 1; i < m_thread_count; ++i)
    {
        m_random_engines[i].discard(i);
    }

    std::cout << "File: " << file_name << std::endl
              << "Seed: " << seed << std::endl;

    std::ifstream file{ file_name };
    if (not file)
    {
        std::cerr << "Couldn't open file '" << file_name << "'!" << std::endl;
        return;
    }

    // ignore comment line
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // read the number of jobs and machine
    unsigned job_count = 0, mach_count = 0;
    if (not(file >> job_count >> mach_count) or job_count == 0 or mach_count == 0)
    {
        std::cerr << "Couldn't read the number of jobs and machines in file '" << file_name << "'!" << std::endl;
        return;
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
            std::cerr << "Couldn't read job " << job_num << " in file '" << file_name << "'!" << std::endl;
            return;
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
                std::cerr << "Couldn't read operation " << op_num << " of job " << job_num << " in file '" << file_name << "'!" << std::endl;
                return;
            }
            if (machine_num >= mach_count)
            {
                std::cerr << "Wrong machine number in Operation " << op_num << " of job " << job_num << " in file '" << file_name << "'!" << std::endl;
                return;
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

std::shared_ptr<Schedule> SearchAlgorithm::findSolutionParallel(double time_limit) const
{
    std::vector<std::shared_ptr<Schedule> > schedules{ m_thread_count, std::shared_ptr<Schedule>() };
#pragma omp parallel
    {
        schedules[omp_get_thread_num()] = findSolution(time_limit);
    }

    return *std::min_element(schedules.begin(),
        schedules.end(),
        [](std::shared_ptr<Schedule> first, std::shared_ptr<Schedule> second) { return *first < *second; });
}
std::default_random_engine& SearchAlgorithm::current_random_engine() const
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

    // generate a random job number and add it to the schedule if the refered job isn't finished
    for (unsigned jobs_added = 0; jobs_added < m_operation_count; ++jobs_added)
    {
        unsigned random_job = uni(current_random_engine());
        // can't add jobs that are already contained as many times as they have operations
        while (job_added_count[random_job] == jobs[random_job].operation_count())
        {
            random_job = (random_job + 1) % jobs.size();
        }

        job_schedule.push_back(random_job);
        ++(job_added_count[random_job]);
    }

    return std::make_shared<Schedule>(std::move(job_schedule), m_machine_count, jobs);
}

std::shared_ptr<std::vector<std::shared_ptr<Schedule> > >
SearchAlgorithm::generateNeighbours(const Schedule& curr_pos) const
{
    auto ret = std::make_shared<std::vector<std::shared_ptr<Schedule> > >();
    for (unsigned i = 0; i < m_operation_count - 1; ++i)
    {
        auto copy = std::make_shared<Schedule>(curr_pos);
        if (copy->swapJobSchedulePositions(i, i + 1))
        {
            ret->push_back(copy);
        }
    }
    return ret;
}

std::shared_ptr<Schedule> SearchAlgorithm::generateNeighbourSolution(const Schedule& curr_pos) const
{
    auto ret = std::make_shared<Schedule>(curr_pos);
    std::uniform_int_distribution<int> uni(0, m_operation_count - 1);
    unsigned random_idx1 = uni(current_random_engine());
    unsigned random_idx2 = uni(current_random_engine());

    while (!ret->swapJobSchedulePositions(random_idx1, random_idx2))
    {
        random_idx1 = (random_idx1 + 1) % m_operation_count;
    }

    return ret;
}

using namespace std::chrono;

void SearchAlgorithm::startTimer() const
{
    m_start_time = steady_clock::now();
}

bool SearchAlgorithm::isTimeLimitReached(double time_limit) const
{
    return duration_cast<duration<double> >(steady_clock::now() - m_start_time).count() >= time_limit;
}
