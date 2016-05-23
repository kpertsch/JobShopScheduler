#include "Operation.h"

#include <queue>

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

        m_operations.push(op);
        return true;
    }

    Operation popOperation()
    {
        Operation ret = m_operations.front();
        m_operations.pop();
        return ret;
    }

    bool isDone() const
    {
        return m_operations.empty();
    }

private:
    std::queue<Operation> m_operations;
    unsigned m_job_num;
};
}
