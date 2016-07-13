#include "Schedule.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <string>
#include <tuple>
#include <vector>

#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <cairommconfig.h>

namespace jss
{

Schedule::Schedule(
    std::vector<unsigned>&& job_schedule, unsigned machine_count, std::vector<Job>& jobs)
    : m_job_schedule{ job_schedule }
    , m_machine_count{ machine_count }
    , m_jobs{ jobs }
    , m_machine_schedules{ machine_count, std::queue<Operation>() }
{
    generatePlan();
}

Schedule::Schedule(const Schedule& other, bool copy_machine_schedule)
    : m_job_schedule{ other.m_job_schedule }
    , m_machine_count{ other.m_machine_count }
    , m_jobs{ other.m_jobs }
    , m_machine_schedules{ m_machine_count, std::queue<Operation>() }
{
    if (copy_machine_schedule)
    {
        m_machine_schedules = other.m_machine_schedules;
    }
}

unsigned Schedule::execTime() const { return m_exec_time; }

unsigned Schedule::relativeFitnessToWorst(unsigned worstExecTime)
{
    return worstExecTime - m_exec_time;
}

bool Schedule::swapJobSchedulePositions(unsigned index1, unsigned index2)
{
    // no invalid indices and useless swaps
    if (index1 >= m_job_schedule.size() or index2 >= m_job_schedule.size() or index1 == index2
        or m_job_schedule[index1] == m_job_schedule[index2])
    {
        return false;
    }

    std::swap(m_job_schedule[index1], m_job_schedule[index2]);
    generatePlan();

    return true;
}

std::shared_ptr<Schedule> Schedule::precedencePreservingCrossover(
    const std::vector<bool>& randDecs, const Schedule& sol1, const Schedule& sol2)
{
    std::vector<unsigned> job_schedule;
    job_schedule.reserve(sol1.m_job_schedule.size());

    std::list<unsigned> sched1{ sol1.m_job_schedule.begin(), sol1.m_job_schedule.end() };
    std::list<unsigned> sched2{ sol2.m_job_schedule.begin(), sol2.m_job_schedule.end() };

    unsigned jobIdx;
    for (bool dec : randDecs)
    {
        if (dec)
        {
            jobIdx = sched1.front();
            sched1.pop_front();
            sched2.erase(std::find(sched2.begin(), sched2.end(), jobIdx));
        }
        else
        {
            jobIdx = sched2.front();
            sched2.pop_front();
            sched1.erase(std::find(sched1.begin(), sched1.end(), jobIdx));
        }
        job_schedule.push_back(jobIdx);
    }

    return std::make_shared<Schedule>(std::move(job_schedule), sol1.m_machine_count, sol1.m_jobs);
}

void Schedule::inverseRangeMove(unsigned beginIdx, unsigned endIdx, unsigned newBeginIdx)
{
    std::vector<unsigned> reverseRange;
    reverseRange.reserve(endIdx - beginIdx);

    for (int i = endIdx - 1; i >= static_cast<int>(beginIdx); --i)
    {
        reverseRange.push_back(m_job_schedule[i]);
    }
    m_job_schedule.erase(m_job_schedule.begin() + beginIdx, m_job_schedule.begin() + endIdx);
    m_job_schedule.insert(
        m_job_schedule.begin() + newBeginIdx, reverseRange.begin(), reverseRange.end());

    generatePlan();
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
    const double space_height = 10;
    const double time_line_height = 30;
    const double machine_height = 40;
    const double total_height = m_machine_count * machine_height
        + (m_machine_count + 2) * space_height + time_line_height;
    const double space_width = 20;
    const double total_width = m_exec_time + 2.0 * space_width;
    Cairo::RefPtr<Cairo::ImageSurface> surface
        = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, total_width, total_height);
    Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);
    // background
    cr->save();
    cr->set_source_rgb(0.8, 0.8, 0.8);
    cr->paint();
    cr->restore();

    for (std::queue<Operation> maschine_queue : m_machine_schedules)
    {
        // operation frame
        while (not maschine_queue.empty())
        {
            Operation op = maschine_queue.front();
            maschine_queue.pop();

            const double x = space_width
                + (op.startTime() / static_cast<double>(m_exec_time))
                    * (total_width - 2.0 * space_width);
            const double width = (op.operationTime() / static_cast<double>(m_exec_time))
                * (total_width - 2.0 * space_width);
            const double y = op.machine() * machine_height + (op.machine() + 1) * space_height;
            const double height = machine_height;

            double r, g, b;
            std::tie(r, g, b) = hsv2rgb((360.0 / m_jobs.size()) * op.jobNum(), 0.8, 1.0);

            // colored time rectangle
            cr->save();
            cr->set_source_rgb(r, g, b);
            cr->rectangle(x, y, width, height);
            cr->fill();
            cr->restore();

            // outer time frame
            cr->save();
            cr->set_line_width(1.0);
            cr->rectangle(x, y, width, height);
            cr->stroke();
            cr->restore();

            // inner time frame
            cr->save();
            cr->set_line_width(1.0);
            cr->rectangle(x + 1, y + 1, width - 2, height - 2);
            cr->set_source_rgb(1.0, 1.0, 1.0);
            cr->stroke();
            cr->restore();
        }
    }

    // text over everything else
    for (std::queue<Operation> maschine_queue : m_machine_schedules)
    {
        // operation text
        while (not maschine_queue.empty())
        {
            Operation op = maschine_queue.front();
            maschine_queue.pop();

            const double x = space_width
                + (op.startTime() / static_cast<double>(m_exec_time))
                    * (total_width - 2.0 * space_width);
            const double width = (op.operationTime() / static_cast<double>(m_exec_time))
                * (total_width - 2.0 * space_width);
            const double y = op.machine() * machine_height + (op.machine() + 1) * space_height;
            const double height = machine_height;

            cr->save();
            cr->select_font_face("sans serif", Cairo::FontSlant::FONT_SLANT_NORMAL,
                Cairo::FontWeight::FONT_WEIGHT_BOLD);
            cr->set_font_size(10.0);
            std::string op_num_str = std::to_string(op.operationNum());
            Cairo::TextExtents text_extents;
            cr->get_text_extents(op_num_str, text_extents);
            cr->move_to(
                x + 0.5 * (width - text_extents.height), y + 0.5 * (height - text_extents.width));
            cr->rotate_degrees(90.0);
            cr->show_text(op_num_str);
            cr->restore();
        }
    }
    const double time_line_thickness = 2.0;
    // time line top
    cr->save();
    cr->set_line_width(time_line_thickness);
    cr->move_to(space_width - 2, total_height - time_line_height - space_height);
    cr->line_to(total_width - space_width + 2, total_height - time_line_height - space_height);
    cr->stroke();
    cr->restore();
    const double marker_font_size = 8.0;
    const unsigned small_step = 10;
    const unsigned big_step = 10 * small_step;
    for (unsigned marker = 0.0; marker <= m_exec_time; marker += small_step)
    {
        const double marker_pos = space_width
            + static_cast<double>(marker - (marker & 1)) / m_exec_time
                * (total_width - 2.0 * space_width);
        bool big = (marker % big_step) == 0;
        // time line marker
        cr->save();
        cr->move_to(marker_pos, total_height - time_line_height - space_height);
        if (big)
        {
            cr->line_to(marker_pos, total_height - time_line_height - space_height + 8);
            cr->set_line_width(time_line_thickness);
        }
        else
        {
            cr->line_to(marker_pos, total_height - time_line_height - space_height + 4);
            cr->set_line_width(time_line_thickness / 2.0);
        }
        cr->stroke();
        cr->restore();

        if (not big)
            continue;
        // time lime legend
        cr->save();
        cr->select_font_face(
            "sans serif", Cairo::FontSlant::FONT_SLANT_NORMAL, Cairo::FontWeight::FONT_WEIGHT_BOLD);
        cr->set_font_size(marker_font_size);
        std::string marker_str = std::to_string(marker);
        Cairo::TextExtents text_extents;
        cr->get_text_extents(marker_str, text_extents);
        cr->move_to(marker_pos - 0.5 * text_extents.width,
            total_height - time_line_height - space_height + text_extents.height + 10);
        cr->show_text(marker_str);
        cr->restore();
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
    for (std::queue<Operation> machine_schedule : sched.m_machine_schedules)
    {
        os << "Machine " << machine_counter << ":" << std::endl;
        while (!machine_schedule.empty())
        {
            Operation op = machine_schedule.front();
            os << "Job: " << op.jobNum() << ", Operation: " << op.operationNum()
               << ", Start time: " << op.startTime() << ", Duration: " << op.operationTime()
               << std::endl;
            machine_schedule.pop();
        }
        ++machine_counter;
    }
    os << std::endl << "Total Duration: " << sched.m_exec_time << std::endl;
    return os;
}

bool Schedule::operator<(const Schedule& other) { return this->m_exec_time < other.m_exec_time; }

void Schedule::generatePlan()
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
        Job& job = m_jobs.at(job_index);
        Operation op = job.currentOperation();
        job.popOperation();
        auto start_time = std::max(machine_times.at(op.machine()), job_times.at(op.jobNum()));

        // set start time in operation and push to machine schedule
        op.setStartTime(start_time);
        m_machine_schedules.at(op.machine()).push(op);

        // update time track arrays
        end_time = start_time + op.operationTime();
        machine_times.at(op.machine()) = end_time;
        job_times.at(op.jobNum()) = end_time;
    }

    for (Job& job : m_jobs)
    {
        job.resetToBeginning();
    }

    std::vector<std::queue<Operation> >::iterator minEl, maxEl;
    std::tie(minEl, maxEl)
        = std::minmax_element(m_machine_schedules.begin(), m_machine_schedules.end(),
            [](std::queue<Operation> const& s1, std::queue<Operation> const& s2) {
                return s1.back().startTime() + s1.back().operationTime()
                    < s2.back().startTime() + s2.back().operationTime();
            });

    m_exec_time = maxEl->back().startTime() + maxEl->back().operationTime();
}
}
