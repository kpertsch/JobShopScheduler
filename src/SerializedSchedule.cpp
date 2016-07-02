#include "SerializedSchedule.h"

#include <algorithm>
#include <tuple>
#include <utility>

using namespace jss;

bool SerializedSchedule::swapOperations(unsigned index1, unsigned index2)
{
    // no invalid indices
    if (index1 >= m_operations.size() or index2 >= m_operations.size() or index1 == index2)
    {
        return false;
    }

    unsigned idx1_job = m_operations[index1].job_num();
    unsigned idx2_job = m_operations[index2].job_num();

    // swaped operations can't be from the same job
    if (idx1_job == idx2_job)
    {
        return false;
    }

    // result has to stay ordered, therefore we can't have operations of the same job in the middle
    unsigned min_idx, max_idx;
    std::tie(min_idx, max_idx) = std::minmax(index1, index2);
    for (int i = min_idx + 1; i < max_idx; ++i)
    {
        unsigned i_job = m_operations[i].job_num();
        if (i_job == idx1_job or i_job == idx2_job)
        {
            return false;
        }
    }

    std::swap(m_operations[index1], m_operations[index2]);

    return true;
}
