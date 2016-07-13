#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>

#include "GeneticAlgorithm.h"
#include "SimulatedAnnealing.h"
#include "StochasticHillClimber.h"

using namespace jss;

void printHelp()
{
    std::cout << "Usage: j3s [OPTION]..." << std::endl
              << "Job schop scheduling problem solver" << std::endl
              << std::endl
              << "The following options are available:" << std::endl
              << "    -h, --help: displays this help and exists" << std::endl
              << "    -f, --file <file> : solves the problem specified in <file>" << std::endl
              << "    -s, --seed <seed> : uses a specified unsigned integer as a seed" << std::endl
              << "    -t, --time <time> : stops the solver after <time> seconds and reports the "
                 "best solution"
              << std::endl
              << "    -e, --exclude <algorithm> : excludes one of the following algorithms,"
              << std::endl
              << "                  " << GeneticAlgorithm::shortName << " - Genetic algorithm"
              << std::endl
              << "                  " << StochasticHillClimber::shortName
              << " - Stochastic hill climber" << std::endl
              << "                  " << SimulatedAnnealing::shortName << " - Simulated annealing"
              << std::endl;
}

int main(int argc, char** argv)
{
    double time = 60;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::string file{ "instances/benchmark.jssp" };
    bool useGeneticAlgorithm = true;
    bool useStochasticHillClimber = true;
    bool useSimulatedAnnealing = true;

    std::vector<std::string> args;
    if (argc < 0)
        argc = 0;
    args.reserve(static_cast<unsigned>(argc));
    for (int i = 0; i < argc; ++i)
    {
        std::string arg{ argv[i] };
        if (arg.size() == 0)
            continue;
        args.emplace_back(std::move(arg));
    }

    for (unsigned i = 1; i < args.size(); ++i)
    {
        std::string arg = args[i];
        std::string nextArg;
        if (i + 1 < args.size())
        {
            nextArg = args[i + 1];
        }

        if (arg == "--help" or arg == "-h")
        {
            printHelp();
            return 0;
        }
        else if (arg == "--file" or arg == "-f")
        {
            if (nextArg.empty() or nextArg[0] == '-')
            {
                std::cerr << "j3s: No file specified for " << arg << std::endl;
                return 1;
            }
            file = nextArg;
            ++i;
        }
        else if (arg == "--seed" or arg == "-s")
        {
            if (nextArg.empty() or nextArg[0] == '-')
            {
                std::cerr << "j3s: No seed specified for " << arg << std::endl;
                return 1;
            }
            try
            {
                seed = std::stoul(nextArg);
            }
            catch (const std::exception& ex)
            {
                std::cerr << "j3s: Specified seed wasn't valid" << std::endl;
                return 1;
            }
            ++i;
        }
        else if (arg == "--time" or arg == "-t")
        {
            if (nextArg.empty() or nextArg[0] == '-')
            {
                std::cerr << "j3s: No time specified for " << arg << std::endl;
                return 1;
            }
            try
            {
                time = std::stod(nextArg);
                if (time <= 0.0)
                    throw std::exception();
            }
            catch (const std::exception& ex)
            {
                std::cerr << "j3s: Specified time wasn't valid" << std::endl;
                return 1;
            }
            ++i;
        }
        else if (arg == "--exclude" or arg == "-e")
        {
            if (nextArg.empty() or nextArg[0] == '-')
            {
                std::cerr << "j3s: No algorithm specified for " << arg << std::endl;
                return 1;
            }
            if (nextArg == GeneticAlgorithm::shortName)
                useGeneticAlgorithm = false;
            else if (nextArg == StochasticHillClimber::shortName)
                useStochasticHillClimber = false;
            else if (nextArg == SimulatedAnnealing::shortName)
                useSimulatedAnnealing = false;
            else
            {
                std::cerr << "j3s: Unknown algorithm »" << nextArg << "«" << std::endl;
                return 1;
            }
            ++i;
        }
        else
        {
            std::cerr << "j3s: Unknown option »" << arg << "«" << std::endl
                      << "„j3s --help“ gives for more information." << std::endl;
            return 1;
        }
    }

    std::vector<std::tuple<std::shared_ptr<SearchAlgorithm>, std::string, std::string> > algorithms;

    if (useGeneticAlgorithm)
    {
        algorithms.push_back(std::make_tuple(std::make_shared<GeneticAlgorithm>(file, seed),
            GeneticAlgorithm::longName, GeneticAlgorithm::shortName));
    }

    if (useStochasticHillClimber)
    {
        algorithms.push_back(std::make_tuple(std::make_shared<StochasticHillClimber>(file, seed),
            StochasticHillClimber::longName, StochasticHillClimber::shortName));
    }

    if (useSimulatedAnnealing)
    {
        algorithms.push_back(std::make_tuple(std::make_shared<SimulatedAnnealing>(file, seed),
            SimulatedAnnealing::longName, SimulatedAnnealing::shortName));
    }

    if (algorithms.empty())
    {
        std::cerr << "j3s: Can't exclude all algorithms" << std::endl;
        return 1;
    }

    std::string file_basename{ file };
    auto pos = file_basename.find_last_of('/');
    file_basename.erase(0, pos + 1);

    std::cout << time << " seconds to solve the problem per algorithm" << std::endl
              << std::endl
              << "File: " << file << std::endl
              << "Seed: " << seed << std::endl
              << "Threads: " << std::get<0>(algorithms[0])->threadCount() << std::endl;

    std::shared_ptr<Schedule> solution;

    for (auto& algorithm_tuple : algorithms)
    {
        std::shared_ptr<SearchAlgorithm> algorithm;
        std::string long_name, short_name;
        std::tie(algorithm, long_name, short_name) = algorithm_tuple;
        std::cout << std::endl << "    -----> " + long_name + ':' << std::endl;
        if (algorithm->operationCount() < 2)
            return -1;
        std::ofstream solution_file{ "solutions/" + short_name + '-' + file_basename + ".sol",
            std::fstream::out | std::fstream::trunc };
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
