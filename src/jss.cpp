//
// Created by karl on 30.04.16.
//

#include <iostream>
#include <memory>

#include "Job.h"
#include "RandomSearch.h"

using namespace jss;

int main()
{

    std::shared_ptr<RandomSearch> search_alg = std::make_shared<RandomSearch>("dummy_file", 42);

    std::shared_ptr<Schedule> solution = search_alg->findSolutionInSteps(10);
    std::cout << solution << std::endl;

    return 0;
}
