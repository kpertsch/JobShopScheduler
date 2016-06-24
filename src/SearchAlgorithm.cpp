#include "SearchAlgorithm.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using namespace jss;

SearchAlgorithm::SearchAlgorithm(const std::string& file_name, unsigned seed)
    : m_random_engine{ seed }
{
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

    m_jobs = std::move(jobs);
    m_machine_count = mach_count;
    m_operation_count = op_count;

#ifndef NDEBUG
    std::cout << m_jobs.size() << " jobs and " << m_machine_count << " machines" << std::endl;
    for (auto job : m_jobs)
    {
        std::cout << "job " << job.job_num() << ':' << std::endl;
        while (not job.isDone())
        {
            Operation op = job.popOperation();
            std::cout << "    o_{" << op.job_num() << ',' << op.op_num() << "} = (" << op.machine() << ',' << op.op_time() << ')' << std::endl;
        }
    }
    std::cout << "with a total of " << m_operation_count << " operations" << std::endl;
#endif
}

std::shared_ptr<std::vector<SerializedSchedule> >
SearchAlgorithm::generateNeighbours(const SerializedSchedule& curr_pos) const
{
    std::shared_ptr<std::vector<SerializedSchedule> > ret = std::make_shared<std::vector<SerializedSchedule> >();
    assert(m_operation_count);
    for (unsigned i = 0; i < m_operation_count - 1; ++i)
    {
        SerializedSchedule copy{ curr_pos };
        if (copy.swapOperations(i, i + 1))
        {
            ret->push_back(copy);
        }
    }
    return ret;
}
