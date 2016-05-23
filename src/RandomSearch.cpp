//
// Created by karl on 01.05.16.
//

#include <ctime>
#include <iostream>

#include "RandomSearch.h"
#include "SolutionGenerator.h"

namespace jobShopSolver
{

/// Constructor
RandomSearch::RandomSearch(const double t_limit)
{
    time_limit = t_limit;
}

/// search best solution by random tries
Solution RandomSearch::findSolution(const std::vector<Job>& jobs) const
{
    std::shared_ptr<SolutionGenerator> solution_handler = std::make_shared<SolutionGenerator>(jobs);

    Solution best_solution = solution_handler->generateRandomSolution();
    const clock_t start_time = clock();

    uint64_t num_steps = 0;
    while ((double)(clock() - start_time) / CLOCKS_PER_SEC < time_limit)
    {
        const Solution solution = solution_handler->generateRandomSolution();
        if (solution.total_op_time < best_solution.total_op_time)
            best_solution = solution;

        ++num_steps;
    }

    std::cout << "Found best solution with operation time of " << best_solution.total_op_time << " after " << num_steps << " trials!" << std::endl;
    solution_handler->printSolution(best_solution);

    return best_solution;
}

} // end namespace