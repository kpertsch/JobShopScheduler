#pragma once

#include "Operation.h"

#include <vector>

namespace jss
{

class Job
{

public:
    Job(unsigned job_num);

    bool addOperation(const Operation& op);

    bool isDone() const;

    void resetToBeginning();

    const Operation& currentOperation() const;

    void popOperation();

    unsigned jobNum() const;

    unsigned operationCount() const;

private:
    std::vector<Operation> m_operations;
    unsigned m_job_num;
    unsigned m_index = 0;
};
}
