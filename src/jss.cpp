//
// Created by karl on 30.04.16.
//

#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>

#include "GeneticAlgorithm.h"
#include "StochasticHillClimber.h"

using namespace jss;

int main(int argc, char** argv)
{
    double time = 60;

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

    std::vector<std::tuple<std::shared_ptr<SearchAlgorithm>, std::string, std::string> > algorithms{
        std::make_tuple(std::make_shared<StochasticHillClimber>(file, seed), "StochasticHillClimber", "shc"),
        std::make_tuple(std::make_shared<GeneticAlgorithm>(file, seed), "GeneticAlgorithm", "ga")
    };

    std::cout << std::endl
              << "File: " << file << std::endl
              << "Seed: " << seed << std::endl
              << "Threads: " << std::get<0>(algorithms[0])->threadCount() << std::endl;

    for (auto& algorithm_tuple : algorithms)
    {
        std::shared_ptr<SearchAlgorithm> algorithm;
        std::string long_name, short_name;
        std::tie(algorithm, long_name, short_name) = algorithm_tuple;
        std::cout << std::endl << "    -----> " + long_name + ':' << std::endl;
        if (algorithm->operationCount() < 2)
            return -1;
        std::ofstream solution_file{ "solutions/" + short_name + '-' + file_basename + ".sol", std::fstream::out | std::fstream::trunc };
        solution = algorithm->findSolution(time);
        for (unsigned i = 0; i < algorithm->countersCount(); ++i)
        {
            unsigned counter;
            if ((counter = algorithm->counter(i)) > algorithm->threadCount())
            {
                std::cout << algorithm->counterName(i) << ": " << counter << std::endl;
            }
        }
        solution->storeAsImage("solutions/" + short_name + '-' + file_basename + ".png");
        solution_file << *solution;
        solution_file.close();
        std::cout << "Solution: " << solution->execTime() << std::endl;
    }

    return 0;
}
