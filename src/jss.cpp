//
// Created by karl on 30.04.16.
//

#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>

#include "IteratedHillClimber.h"
#include "RandomSearch.h"
#include "StochasticHillClimber.h"

using namespace jss;

int main(int argc, char** argv)
{
    double time;
    unsigned steps;

    //    (void) time;
    //    steps = 10000;
    (void)steps;
    time = 60;

    std::cout << time << " seconds to solve the problem per algorithm" << std::endl
              << std::endl;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::string file{ "instances/benchmark.jssp" };
    if (argc == 2)
    {
        file = argv[1];
    }

    std::string file_basename{ file };
    auto pos = file_basename.find_last_of('/');
    if (pos != std::string::npos)
        assert(file_basename.length() > pos);
    file_basename.erase(0, pos + 1);

    std::shared_ptr<Schedule> solution;

    //    std::cout << "    -----> IteratedHillClimber:" << std::endl;
    //    IteratedHillClimber ihc{ file, seed };
    //    if (ihc.operation_count() < 2)
    //        return -1;
    //    std::ofstream ihc_solution_file{ "solutions/ihc-" + file_basename + ".sol", std::fstream::out | std::fstream::trunc };
    ////    solution = ihc.findSolutionInTime(time);
    //    solution = ihc.findSolutionInSteps(steps);
    //    ihc_solution_file << *solution;
    //    ihc_solution_file.close();
    //    std::cout << "Solution: " << solution->exec_time() << std::endl;

    //    std::cout << std::endl
    //              << "    -----> RandomSearch:" << std::endl;
    //    RandomSearch rs{ file, seed };
    //    if (rs.operation_count() < 2)
    //        return -1;
    //    std::ofstream rs_file{ "solutions/rs-" + file_basename + ".sol", std::fstream::out | std::fstream::trunc };
    ////    solution = rs.findSolutionInTime(time);
    //    solution = rs.findSolutionInSteps(steps);
    //    rs_file << *solution;
    //    rs_file.close();
    //    std::cout << "Solution: " << solution->exec_time() << std::endl;

    std::cout << std::endl
              << "    -----> StochasticHillClimber:" << std::endl;
    StochasticHillClimber shc{ file, seed };
    if (shc.operation_count() < 2)
        return -1;
    std::ofstream shc_file{ "solutions/shc-" + file_basename + ".sol", std::fstream::out | std::fstream::trunc };
    solution = shc.findSolutionInTime(time);
    //    solution = shc.findSolutionInSteps(steps);
    shc_file << *solution;
    shc_file.close();
    std::cout << "Solution: " << solution->exec_time() << std::endl;

    return 0;
}
