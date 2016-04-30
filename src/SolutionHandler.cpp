//
// Created by karl on 30.04.16.
//

#include "SolutionHandler.h"
#include <iostream>
#include <algorithm>
#include <tuple>
#include <iterator>


namespace jobShopSolver {

SolutionHandler::SolutionHandler(std::vector<Job> jobs)
{
    procedure = jobs;
    num_machines = getNumberMachines(procedure);
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

void SolutionHandler::generateRandomSolution()
{
    std::cout << "Now I am generating a random solution!" << std::endl;
    auto num_jobs = procedure.size();
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

    std::cout << "Generated random schedules!" << std::endl;

    std::cout << "Now start evaluating random schedules!" << std::endl;


}



}