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
    double time = 10.0;

    std::cout << time << " seconds to solve the problem per algorithm" << std::endl;

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

    // std::cout << std::endl << "    -----> IteratedHillClimber:" << std::endl;
    // IteratedHillClimber ihc{ file, seed };
    // if (ihc.operation_count() < 2)
    // return -1;
    // std::ofstream ihc_solution_file{ "solutions/ihc-" + file_basename + ".sol", std::fstream::out | std::fstream::trunc };
    // std::cout << "Threads: " << ihc.thread_count() << std::endl;
    // solution = ihc.findSolution(time);
    // solution->storeAsImage("solutions/ihc-" + file_basename + ".png");
    // ihc_solution_file << *solution;
    // ihc_solution_file.close();
    // std::cout << "Solution: " << solution->exec_time() << std::endl;

    // std::cout << std::endl << "    -----> RandomSearch:" << std::endl;
    // RandomSearch rs{ file, seed };
    // if (rs.operation_count() < 2)
    // return -1;
    // std::ofstream rs_file{ "solutions/rs-" + file_basename + ".sol", std::fstream::out | std::fstream::trunc };
    // std::cout << "Threads: " << rs.thread_count() << std::endl;
    // solution = rs.findSolution(time);
    // solution->storeAsImage("solutions/rs-" + file_basename + ".png");
    // rs_file << *solution;
    // rs_file.close();
    // std::cout << "Solution: " << solution->exec_time() << std::endl;

    std::cout << std::endl << "    -----> StochasticHillClimber:" << std::endl;
    StochasticHillClimber shc{ file, seed };
    if (shc.operation_count() < 2)
        return -1;
    std::ofstream shc_file{ "solutions/shc-" + file_basename + ".sol", std::fstream::out | std::fstream::trunc };
    std::cout << "Threads: " << shc.thread_count() << std::endl;
    solution = shc.findSolution(time);
    solution->storeAsImage("solutions/shc-" + file_basename + ".png");
    shc_file << *solution;
    shc_file.close();
    std::cout << "Solution: " << solution->exec_time() << std::endl;

    return 0;
}
