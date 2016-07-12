#include "Operation.h"

namespace jss
{

Operation::Operation(unsigned job_num, unsigned op_num, unsigned machine, unsigned op_time)
    : m_job_num{ job_num }
    , m_op_num{ op_num }
    , m_machine{ machine }
    , m_op_time{ op_time }
{
}

void Operation::setStartTime(unsigned start_time) { m_start_time = start_time; }

unsigned Operation::jobNum() const { return m_job_num; }

unsigned Operation::operationNum() const { return m_op_num; }

unsigned Operation::machine() const { return m_machine; }

unsigned Operation::operationTime() const { return m_op_time; }

unsigned Operation::startTime() const { return m_start_time; }

bool Operation::isStartTimeSet() const { return m_start_time == -1; }
}
