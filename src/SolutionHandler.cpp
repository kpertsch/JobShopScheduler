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


}



}