#include "SerializedSchedule.h"

#include <utility>

using namespace jss;

bool SerializedSchedule::swapOperations(unsigned index1, unsigned index2)
{
    // no invalid indices
    if (index1 >= m_operations.size() or index2 >= m_operations.size() or index1 == index2)
    {
        return false;
    }

    // operations of the same job have to be ordered
    if (m_operations[index1].job_num() == m_operations[index2].job_num() and m_operations[index1].op_num() <= m_operations[index2].op_num())
    {
        return false;
    }

    std::swap(m_operations[index1], m_operations[index2]);

    return true;
}
