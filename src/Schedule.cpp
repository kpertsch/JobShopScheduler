#include "Schedule.h"

#include <iostream>

namespace jss{

Schedule::Schedule(std::shared_ptr<SerializedSchedule> ssched)
{
    // sort operations to respective machine schedules
    for(auto op_it = ssched->begin(); op_it != ssched->end(); ++op_it)
    {
        std::cout << "Executing Job " << op_it->job_num()
                << ", operation number " << op_it->op_num()
                << " on machine " << op_it->machine() << std::endl;
    }
}

}