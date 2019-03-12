#include "../include/solution.h"

double distance_2d(const Point &p1, const Point &p2) { return sqrt(pow(p2.m_x - p1.m_x, 2) + pow(p2.m_y - p1.m_y, 2)); };


void closest_point_finder(const std::vector<Point>& points, std::vector<std::pair<Point, Point>> &result,
                                uint32_t begin, uint32_t end, QuadTree &qtree, uint32_t max_capacity, double points_range)
{
    if(begin < 0 || begin >= points.size() || begin >= end)
    {
        std::cerr << "The given indices (Nr of points= " << points.size() << ", begin=" << begin << ", end=" << end 
                                                                << ") are out of range." << std::endl;
        return;
    }
    if(points.size() != result.size()) {
        std::cerr << "The size of points (" << points.size() << ") is not equal to size of result (" << result.size() << ")" << std::endl;
        return;
    }

    for (auto point_idx = begin; point_idx < end; ++point_idx)
    {
        const auto &p = points[point_idx];
        std::vector<Point *> points_in_range;
        auto search_range = 10.0;
        while (true)
        {
            qtree.query(Boundery(p.m_x, p.m_y, search_range, search_range), points_in_range);
            if (points_in_range.size() > max_capacity)
            {
                break;
            }
            else
            {
                points_in_range.clear();
                search_range += search_range;
            }

            if (search_range > points_range)
                break;
        }

        if (!points_in_range.empty())
        {
            auto shortest_dist = points_range;
            auto shortest_point_id = -1;
            for (auto point_in_range_id = 0; point_in_range_id < points_in_range.size(); ++point_in_range_id)
            {
                if (p == *(points_in_range[point_in_range_id]))
                    continue;

                auto dist = distance_2d(p, *(points_in_range[point_in_range_id]));
                if (shortest_dist > dist)
                {
                    shortest_point_id = point_in_range_id;
                    shortest_dist = dist;
                }
            }

            result[point_idx] = std::make_pair(p, *points_in_range[shortest_point_id]);
        }
        else
        {
            std::cerr << "No neighbor found for " << p.m_name << " with search range of " << search_range << std::endl;
            break;
        }
    }
}

void Solution::execute(const std::vector<Point> &points, std::vector<std::pair<Point, Point>> &result, double points_range, int max_capacity, ProcessType process_type)
{
    result.resize(points.size());
    QuadTree qtree(Boundery(points_range / 2.0, points_range / 2.0, points_range / 2.0, points_range / 2.0), max_capacity);
    {
        Timer timer_init_quad("Initializing Quad tree -> ");
        for (auto &point : points)
            qtree.insert(point);
    }

    switch (process_type)
    {
        case WITHOUT_THREAD:
        {
            Timer timer_execution_with_thread("Calculating closest point without thread -> ");
            closest_point_finder(points, result, 0, points.size(), qtree, max_capacity, points_range);
            break;
        }

        case WITH_THREAD:
        {
            /*
                I could have use my own ThreadPool class, but for the
                ease of simplicity, I just used your method of multithreading.
            */
            Timer timer_execution_without_thread("Calculating closest point with thread -> ");
            const uint32_t THREAD_NUMBER = std::thread::hardware_concurrency();
            std::vector<std::thread> threads(THREAD_NUMBER);
            const uint32_t pointsPerThread = static_cast<uint32_t>(std::ceil(result.size() / float(THREAD_NUMBER)));

            for (uint32_t i = 0; i < THREAD_NUMBER; ++i)
            {
                threads[i] = std::thread(std::bind(closest_point_finder, std::cref(points), std::ref(result), 
                                    i * pointsPerThread, (i + 1) * pointsPerThread, std::ref(qtree), max_capacity, points_range));
            }

            for (uint32_t i = 0; i < THREAD_NUMBER; i++)
            {
                if (threads[i].joinable())
                    threads[i].join();
            }
            break;
        }
    }
}