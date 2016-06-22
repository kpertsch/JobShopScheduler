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

    std::shared_ptr<RandomSearch> search_alg = std::make_shared<RandomSearch>("test.jssp", 42);
    return 0;
}
