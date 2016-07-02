#pragma once

#include "Operation.h"

#include <vector>

namespace jss
{

class Job
{

public:
    Job(unsigned job_num)
        : m_job_num{ job_num }
    {
    }

    bool addOperation(const Operation& op)
    {
        if (op.job_num() != this->m_job_num or (not m_operations.empty() and m_operations.back().op_num() >= op.op_num()))
        {
            return false;
        }

        m_operations.push_back(op);
        return true;
    }

    bool isDone() const
    {
        return m_index == m_operations.size();
    }

    void resetToBeginning()
    {
        m_index = 0;
    }

    Operation popOperation()
    {
        if (isDone())
        {
            resetToBeginning();
        }
        return m_operations[m_index++];
    }

    unsigned job_num() const
    {
        return m_job_num;
    }

    unsigned operation_count() const
    {
        return m_operations.size();
    }

private:
    std::vector<Operation> m_operations;
    unsigned m_job_num;
    unsigned m_index = 0;
};
}
