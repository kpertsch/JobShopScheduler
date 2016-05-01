//
// Created by karl on 01.05.16.
//

#include <algorithm>

#include "Evaluator.h"


namespace jobShopSolver {

    bool Evaluator::checkDone(const std::vector<std::queue<Operation>>& machine_schedules) const
    {
        for(auto& schedule : machine_schedules)
        {
            if(!schedule.empty())
                return false;
        }
        return true;
    }


    void Evaluator::updateOccupation(std::vector<bool>& machine_occupied, std::vector<bool>& job_occupied,
                                     const OpTime& current_time, const std::vector<OpTime>& machine_times,
                                     const uint32_t& num_machines, std::vector<std::queue<Operation>>& machine_schedules,
                                     std::vector<uint32_t>& ops_finished) const
    {
        for(auto machine = 0; machine < num_machines; ++machine)
        {
            if(current_time >= machine_times.at(machine) && machine_occupied.at(machine))
            {
                machine_occupied[machine] = false;
                const auto& finished_op = machine_schedules.at(machine).front();
                job_occupied[finished_op.job_num - 1] = false;
                ++ops_finished[finished_op.job_num - 1];
                machine_schedules.at(machine).pop();            // delete first queue element -> job is done
            }
        }
    }


    void Evaluator::evaluateSolution(Solution &solution) const
    {
        std::vector<bool> machine_occupied (solution.num_machines, false);
        std::vector<bool> job_occupied (solution.num_jobs, false);
        std::vector<uint32_t> ops_finished (solution.num_jobs, 0);
        std::vector<OpTime> machine_times (solution.num_machines, 0);

        std::vector<std::queue<Operation>> machine_schedules;
        for(auto machine_schedule : solution.schedules)
        {
            std::queue<Operation> schedule;
            for(auto op : machine_schedule)
            {
                schedule.push(op);
            }
            machine_schedules.push_back(schedule);
        }

        OpTime current_time = 0;        // track current time to check for free machines
        bool changed = false;           // tracks whether a change was made in the current loop iteration
        uint32_t ignore_counter = 0;    // track how many 'smallest' times have been ignored and nothing changed
        while(!checkDone(machine_schedules))
        {
            for(auto machine = 0; machine < solution.num_machines; ++machine)
            {
                if(machine_occupied[machine] || machine_schedules.at(machine).empty())
                    continue;

                auto next_op = machine_schedules.at(machine).front();
                if(!job_occupied[next_op.job_num - 1] &&
                        ops_finished.at(next_op.job_num - 1) == next_op.op_num - 1)
                {
                    machine_times[next_op.machine - 1] = current_time + next_op.op_time;
                    machine_occupied[next_op.machine - 1] = true;
                    job_occupied[next_op.job_num - 1] = true;
                    changed = true;
                }
            }

            // fast forward time until next operation is finished
            auto min_time = *(std::min_element(std::begin(machine_times), std::end(machine_times)));
            if(changed)
            {
                changed = false;        // change was made -> use smallest time
                ignore_counter = 0;     // set back ignore_counter
            }
            else    // no change in last step -> one/multiple machine(s) waiting for another -> use second smallest time
            {
                auto temp_vec = machine_times;
                temp_vec.erase(std::remove(temp_vec.begin(), temp_vec.end(), min_time), temp_vec.end());
                min_time = *(std::min_element(std::begin(temp_vec), std::end(temp_vec)));
                for(auto i = 0; i < ignore_counter; ++i)
                {
                    temp_vec.erase(std::remove(temp_vec.begin(), temp_vec.end(), min_time), temp_vec.end());
                    min_time = *(std::min_element(std::begin(temp_vec), std::end(temp_vec)));
                }
                ++ignore_counter;
            }
            current_time = min_time;

            // free machines who's operation is finished
            updateOccupation(machine_occupied, job_occupied, current_time,
                             machine_times, solution.num_machines, machine_schedules, ops_finished);
        }

        // solution eval time = max machine time
        solution.total_op_time = *(std::max_element(std::begin(machine_times), std::end(machine_times)));
    }

} // end namespace