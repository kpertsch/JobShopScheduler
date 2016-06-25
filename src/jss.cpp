//
// Created by karl on 30.04.16.
//

#include <chrono>
#include <iostream>
#include <string>

#include "IteratedHillClimber.h"
#include "Job.h"
#include "RandomSearch.h"

using namespace jss;

int main(int argc, char** argv)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::string file{ "instances/test.jssp" };
    if (argc == 2)
    {
        file = argv[1];
    }

    std::cout << "    -----> IteratedHillClimber:" << std::endl;
    IteratedHillClimber ihc{ file, seed };
    if (ihc.operation_count() < 2)
        return -1;
    std::cout << *ihc.findSolutionInTime(5) << std::endl;

    std::cout << "    -----> RandomSearch:" << std::endl;
    RandomSearch rs{ file, seed };
    if (rs.operation_count() < 2)
        return -1;
    std::cout << *rs.findSolutionInTime(5) << std::endl;

    return 0;
}
