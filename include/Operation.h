#pragma once

namespace jss
{

class Operation
{
public:
    Operation(unsigned job_num, unsigned op_num, unsigned machine, unsigned op_time)
        : m_job_num{ job_num }
        , m_op_num{ op_num }
        , m_machine{ machine }
        , m_op_time{ op_time }
    {
    }

    void setStartTime(unsigned start_time)
    {
        m_start_time = start_time;
    }

    unsigned job_num() const
    {
        return m_job_num;
    }

    unsigned op_num() const
    {
        return m_op_num;
    }

    unsigned machine() const
    {
        return m_machine;
    }

    unsigned op_time() const
    {
        return m_op_time;
    }

    unsigned start_time() const
    {
        return m_start_time;
    }

    bool isStartTimeSet() const
    {
        return m_start_time == -1;
    }

private:
    unsigned m_job_num;
    unsigned m_op_num;
    unsigned m_machine;
    unsigned m_op_time;
    int m_start_time = -1;
};
}
