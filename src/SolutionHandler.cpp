//
// Created by karl on 30.04.16.
//

#include <iostream>
#include <algorithm>
#include <iterator>

#include "SolutionHandler.h"


namespace jobShopSolver {

SolutionHandler::SolutionHandler(std::vector<Job> jobs)
{
    procedure = jobs;
    num_machines = getNumberMachines(procedure);
    evaluator = std::make_shared<Evaluator>();
}

Machine SolutionHandler::getNumberMachines(std::vector<Job> jobs) const
{
    // find max number of machines
    Machine num_machines = 0;
    for (auto& job : jobs)
    {
        std::vector<Operation>::iterator minEl, maxEl;
        std::tie(minEl, maxEl) = std::minmax_element(begin(job), end(job),
                                                     [] (Operation const& j1, Operation const& j2)
                                                     {
                                                         return j1.machine < j2.machine;
                                                     });

        Machine temp_num_machines = maxEl->machine;
        if(temp_num_machines > num_machines)
            num_machines = temp_num_machines;
    }
    return num_machines;
}

Solution SolutionHandler::generateRandomSolution()
{
    auto num_jobs = static_cast<uint32_t>(procedure.size());
    auto num_ops = procedure[0].size();
    std::vector<int> job_indices (num_jobs, 0);
    std::vector<MachineSchedule> schedules (num_machines);

    // initialize random number generator
    std::mt19937 rng(rd());

    for(auto i = 0; i < num_jobs * num_ops; i++)
    {
        auto num_finished_jobs = std::count(job_indices.begin(), job_indices.end(), num_ops);
        std::uniform_int_distribution<int> uni(1, num_jobs - num_finished_jobs);
        auto random_integer = uni(rng);

        // find random_integer'th job that is not finished so far
        std::vector<int>::iterator next_job_it;
        {
            int count = 0;
            next_job_it = std::find_if(job_indices.begin(), job_indices.end(), [&count, &num_ops, &random_integer](decltype(job_indices)::value_type const& index) {
                return (index != num_ops && ++count == random_integer);
            });
        }
        auto current_job_idx = next_job_it - job_indices.begin();
        auto current_op_idx = *next_job_it;
        auto current_machine_idx = procedure.at(current_job_idx).at(current_op_idx).machine - 1;    // machine numbers start at 1, not 0!

        // add next operation to appropriate machine schedule
        schedules.at(current_machine_idx).push_back(procedure.at(current_job_idx).at(current_op_idx));

        // increment index
        ++job_indices[current_job_idx];
    }

    OpTime op_time(0);
    Solution solution {num_machines, num_jobs, schedules, op_time};

    evaluator->evaluateSolution(solution);

    return solution;
}

void SolutionHandler::printSolution(const Solution& solution) const
{
    for(auto machine = 0; machine < num_machines; ++machine)
    {
        std::cout << "Machine " << machine << ":" << std::endl;
        for(const auto& operation : solution.schedules.at(machine))
        {
            std::cout << "Job: " << operation.job_num << ", Operation: " << operation.op_num << ", OperationTime: " << operation.op_time << std::endl;
        }
        std::cout << std::endl;
    }

}



}