//
// Created by karl on 30.04.16.
//

#include <iostream>
#include <memory>
#include <string>

#include "Job.h"
#include "RandomSearch.h"

using namespace jss;

int main(int argc, char** argv)
{
    unsigned seed = 42;
    if (argc == 2)
    {
        seed = static_cast<unsigned>(std::stoi(std::string(argv[1])));
    }
    std::shared_ptr<RandomSearch> search_alg = std::make_shared<RandomSearch>("test.jssp", seed);

    std::cout << std::endl;

    if (search_alg->operation_count() < 2)
    {
        return -1;
    }

    search_alg->findSolutionInTime(5.5);

    return 0;
}
