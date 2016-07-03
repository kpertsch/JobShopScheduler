#include "Schedule.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <cairommconfig.h>

namespace jss
{

Schedule::Schedule(std::vector<unsigned>&& job_schedule, unsigned machine_count, std::vector<Job>& jobs)
    : m_job_schedule{ job_schedule }
    , m_machine_count{ machine_count }
    , m_jobs{ jobs }
    , m_machine_schedules{ machine_count, std::queue<Operation>() }
{
    generate_plan();
}

bool Schedule::swapJobSchedulePositions(unsigned index1, unsigned index2)
{
    // no invalid indices and useless swaps
    if (index1 >= m_job_schedule.size() or index2 >= m_job_schedule.size() or index1 == index2 or m_job_schedule[index1] == m_job_schedule[index2])
    {
        return false;
    }

    std::swap(m_job_schedule[index1], m_job_schedule[index2]);
    generate_plan();

    return true;
}

std::tuple<double, double, double> hsv2rgb(double h, double s, double v)
{
    double hh, p, q, t, ff, r, g, b;
    long i;

    if (s <= 0.0)
    { // < is bogus, just shuts up warnings
        r = v;
        g = v;
        b = v;
        return std::make_tuple(r, g, b);
    }
    hh = h;
    if (hh >= 360.0)
        hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = v * (1.0 - s);
    q = v * (1.0 - (s * ff));
    t = v * (1.0 - (s * (1.0 - ff)));

    switch (i)
    {
    case 0:
        r = v;
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = v;
        b = p;
        break;
    case 2:
        r = p;
        g = v;
        b = t;
        break;

    case 3:
        r = p;
        g = q;
        b = v;
        break;
    case 4:
        r = t;
        g = p;
        b = v;
        break;
    case 5:
    default:
        r = v;
        g = p;
        b = q;
        break;
    }
    return std::make_tuple(r, g, b);
}

void Schedule::storeAsImage(const std::string& file_name) const
{
    const double machine_width = 30;
    const double space_width = 10;
    const double total_width = m_exec_time * 0.5;
    Cairo::RefPtr<Cairo::ImageSurface> surface
        = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, total_width, m_machine_count * machine_width + (m_machine_count + 1) * space_width);
    Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);
    // background
    cr->save();
    cr->set_source_rgb(0.2, 0.2, 0.2);
    cr->paint();
    cr->restore();

    for (std::queue<Operation> maschine_queue : m_machine_schedules)
    {
        // operation
        while (not maschine_queue.empty())
        {
            Operation op = maschine_queue.front();
            maschine_queue.pop();

            double x = (op.start_time() / static_cast<double>(m_exec_time)) * (total_width - 2.0 * space_width) + space_width;
            double width = (op.op_time() / static_cast<double>(m_exec_time)) * (total_width - 2.0 * space_width);
            double y = op.machine() * machine_width + (op.machine() + 1) * space_width;
            double height = machine_width;

            double r, g, b;
            std::tie(r, g, b) = hsv2rgb((360.0 / m_jobs.size()) * op.job_num(), 1.0, 1.0);

            // time rectangle
            cr->save();
            cr->set_source_rgba(r, g, b, 0.5);
            cr->rectangle(x, y, width, height);
            cr->fill();
            cr->restore();

            // operation number text
            double font_size = 12.0;
            cr->save();
            cr->select_font_face("sans serif", Cairo::FontSlant::FONT_SLANT_NORMAL, Cairo::FontWeight::FONT_WEIGHT_BOLD);
            cr->set_font_size(font_size);
            std::string op_num_str = std::to_string(op.op_num());
            Cairo::TextExtents text_extents;
            cr->get_text_extents(op_num_str, text_extents);
            cr->move_to(x + 0.5 * (width - text_extents.width), y + 0.5 * (height - text_extents.height) + text_extents.height);
            cr->show_text(op_num_str);
            cr->restore();
        }
    }

#ifdef CAIRO_HAS_PNG_FUNCTIONS
    surface->write_to_png(file_name);
#else
    std::cerr << "Cairo with PNG support is required to store a Schedule as an Image!" << std::endl;
#endif
}

std::ostream& operator<<(std::ostream& os, const Schedule& sched)
{
    unsigned machine_counter = 0;
    for (auto machine_schedule : sched.m_machine_schedules)
    {
        os << "Machine " << machine_counter << ":" << std::endl;
        while (!machine_schedule.empty())
        {
            auto op = machine_schedule.front();
            os << "Job: " << op.job_num() << ", Operation: " << op.op_num() << ", Start time: " << op.start_time() << ", Duration: " << op.op_time()
               << std::endl;
            machine_schedule.pop();
        }
        ++machine_counter;
    }
    os << std::endl << "Total Duration: " << sched.m_exec_time << std::endl;
    return os;
}

bool Schedule::operator<(const Schedule& other) { return this->m_exec_time < other.m_exec_time; }

void Schedule::generate_plan()
{
    for (auto& machine_sched : m_machine_schedules)
    {
        machine_sched = std::queue<Operation>();
    }
    // initialize time track arrays
    std::vector<int> machine_times = std::vector<int>(m_machine_count, 0);
    std::vector<int> job_times = std::vector<int>(m_jobs.size(), 0);

    // declare end time in function scope to use in end for execution time
    unsigned end_time = 0;

    // sort operations to respective machine schedules
    for (unsigned job_index : m_job_schedule)
    {
        assert(not m_jobs.at(job_index).isDone());
        Operation op = m_jobs.at(job_index).popOperation();
        auto start_time = std::max(machine_times.at(op.machine()), job_times.at(op.job_num()));

        // set start time in operation and push to machine schedule
        op.setStartTime(start_time);
        m_machine_schedules.at(op.machine()).push(op);

        // update time track arrays
        end_time = start_time + op.op_time();
        machine_times.at(op.machine()) = end_time;
        job_times.at(op.job_num()) = end_time;
    }

    for (Job& job : m_jobs)
    {
        job.resetToBeginning();
    }

    std::vector<std::queue<Operation> >::iterator minEl, maxEl;
    std::tie(minEl, maxEl) = std::minmax_element(
        m_machine_schedules.begin(), m_machine_schedules.end(), [](std::queue<Operation> const& s1, std::queue<Operation> const& s2) {
            return s1.back().start_time() + s1.back().op_time() < s2.back().start_time() + s2.back().op_time();
        });

    m_exec_time = maxEl->back().start_time() + maxEl->back().op_time();
}
}
