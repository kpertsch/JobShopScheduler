//
// Created by karl on 30.04.16.
//

#include <memory>

#include "SolutionHandler.h"

using namespace jobShopSolver;

std::vector<Job> generateDummyJobs()
{
    Operation op11 = {1, 1, 1, 20.0};
    Operation op12 = {1, 2, 2, 15.0};
    Operation op13 = {1, 3, 1, 30.0};

    Operation op21 = {2, 1, 2, 10.0};
    Operation op22 = {2, 2, 2, 45.0};
    Operation op23 = {2, 3, 1, 5.0};

    Job job1 = {op11, op12, op13};
    Job job2 = {op21, op22, op23};

    std::vector<Job> jobs = {job1, job2};
    return jobs;
}


int main() {

    std::vector<Job> jobs = generateDummyJobs();

    std::shared_ptr<SolutionHandler> solution_handler = std::make_shared<SolutionHandler>(SolutionHandler(jobs));
    solution_handler->generateRandomSolution();

    return 0;
}