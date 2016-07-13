#include "Job.h"

namespace jss
{

Job::Job(unsigned job_num)
    : m_job_num{ job_num }
{
}

bool Job::addOperation(const Operation& op)
{
    if (op.jobNum() != this->m_job_num
        or (not m_operations.empty() and m_operations.back().operationNum() >= op.operationNum()))
    {
        return false;
    }

    m_operations.push_back(op);
    return true;
}

bool Job::isDone() const { return m_index == m_operations.size(); }

void Job::resetToBeginning() { m_index = 0; }

const Operation& Job::currentOperation() const { return m_operations[m_index]; }

void Job::popOperation()
{
    ++m_index;
    if (isDone())
    {
        resetToBeginning();
    }
}

unsigned Job::jobNum() const { return m_job_num; }

unsigned Job::operationCount() const { return m_operations.size(); }
}
