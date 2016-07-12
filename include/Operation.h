#pragma once

namespace jss
{

class Operation
{
public:
    Operation(unsigned job_num, unsigned op_num, unsigned machine, unsigned op_time);

    void setStartTime(unsigned start_time);

    unsigned jobNum() const;

    unsigned operationNum() const;

    unsigned machine() const;

    unsigned operationTime() const;

    unsigned startTime() const;

    bool isStartTimeSet() const;

private:
    unsigned m_job_num;
    unsigned m_op_num;
    unsigned m_machine;
    unsigned m_op_time;
    int m_start_time = -1;
};
}
