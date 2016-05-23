#include "SearchAlgorithm.h"

using namespace jss;

SearchAlgorithm::SearchAlgorithm(const std::string& file_name, unsigned seed)
    : m_random_engine{ seed }
{
    (void)file_name;

    // generate Dummy jobs
    Operation op11 = { 1, 1, 1, 20 };
    Operation op12 = { 1, 2, 2, 15 };
    Operation op13 = { 1, 3, 1, 30 };

    Operation op21 = { 2, 1, 2, 10 };
    Operation op22 = { 2, 2, 2, 45 };
    Operation op23 = { 2, 3, 1, 5 };

    Job job1{ 1 };
    for (auto op : { op11, op12, op13 })
        job1.addOperation(op);
    Job job2{ 2 };
    for (auto op : { op21, op22, op23 })
        job2.addOperation(op);

    m_jobs = { job1, job2 };
}
