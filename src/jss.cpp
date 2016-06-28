//
// Created by karl on 30.04.16.
//

#include <chrono>
#include <fstream>
#include <iostream>

#include "IteratedHillClimber.h"
#include "RandomSearch.h"
#include "StochasticHillClimber.h"

using namespace jss;

int main(int argc, char** argv)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::string file{ "instances/benchmark.jssp" };
    if (argc == 2)
    {
        file = argv[1];
    }

    std::string file_basename{ file };
    auto pos = file_basename.find_last_of('/');
    if (pos != std::string::npos)
        file_basename.erase(0, pos);

    std::cout << "    -----> IteratedHillClimber:" << std::endl;
    IteratedHillClimber ihc{ file, seed };
    if (ihc.operation_count() < 2)
        return -1;
    std::ofstream ihc_solution_file{ "solutions/ihc-" + file_basename + ".sol", std::fstream::out | std::fstream::trunc };
    std::shared_ptr<Schedule> solution = ihc.findSolutionInTime(10);
    ihc_solution_file << *solution;
    ihc_solution_file.close();
    std::cout << "Solution: " << solution->exec_time() << std::endl;

    std::cout << std::endl
              << "    -----> RandomSearch:" << std::endl;
    RandomSearch rs{ file, seed };
    if (rs.operation_count() < 2)
        return -1;
    std::ofstream rs_file{ "solutions/rs-" + file_basename + ".sol", std::fstream::out | std::fstream::trunc };
    solution = rs.findSolutionInTime(10);
    rs_file << *solution;
    rs_file.close();
    std::cout << "Solution: " << solution->exec_time() << std::endl;

    std::cout << std::endl
              << "    -----> StochasticHillClimber:" << std::endl;
    StochasticHillClimber shc{ file, seed };
    if (shc.operation_count() < 2)
        return -1;
    std::ofstream shc_file{ "solutions/shc-" + file_basename + ".sol", std::fstream::out | std::fstream::trunc };
    solution = shc.findSolutionInTime(10);
    shc_file << *solution;
    shc_file.close();
    std::cout << "Solution: " << solution->exec_time() << std::endl;

    return 0;
}
