#include "../include/quadtree.hpp"

Quadtree::Quadtree(const Boundery &boundery, std::size_t max_capacity) : is_devided_(false),
                                                                         boundery_(boundery),
                                                                         capacity_(max_capacity),
                                                                         sub_quads_({nullptr, nullptr, nullptr, nullptr})
{
    points_.reserve(capacity_);
}

Quadtree::~Quadtree()
{
    if (is_devided_)
    {
        for (auto ptr : sub_quads_)
            delete ptr;
    }
}

const Boundery &Quadtree::boundery() const
{
    return boundery_;
}

std::size_t Quadtree::capacity() const
{
    return capacity_;
}

std::size_t Quadtree::nr_of_points() const
{
    return points_.size();
}

const std::vector<Point2D> &Quadtree::points() const
{
    return points_;
}

bool Quadtree::insert(const Point2D &p)
{
    if (!does_contain(boundery_, p))
        return false;

    if (is_devided_)
    {
        for (auto quad : sub_quads_)
            if (quad->insert(p))
                return true;
    }
    else if (points_.size() < capacity_)
    {
        points_.emplace_back(p);
        return true;
    }
    else
    {
        subdevide();
        points_.emplace_back(p);
        for (auto &point : points_)
        {
            for (auto quad : sub_quads_)
            {
                if (quad->insert(point))
                {
                    break;
                }
            }
        }

        //points_.clear();
        return true;
    }

    return false;
}

void Quadtree::query(const Boundery &range, std::vector<Point2D *>& points_in_range)
{
    if (do_overlap(boundery_, range))
    {
        if (is_devided_)
        {
            for (auto quad : sub_quads_)
                quad->query(range, points_in_range);
        }
        else {
            for (auto &point : points_)
            {
                if (does_contain(range, point))
                    points_in_range.emplace_back(&point);
            }
        }
    }
}

void Quadtree::subdevide()
{
    auto &cx = boundery_.cx;
    auto &cy = boundery_.cy;
    auto hhw = boundery_.hw / 2.0; // half half width
    auto hhh = boundery_.hh / 2.0; // half half height
    sub_quads_[0] = new Quadtree(Boundery(cx - hhw, cy - hhh, hhw, hhh), capacity_);
    sub_quads_[1] = new Quadtree(Boundery(cx + hhw, cy - hhh, hhw, hhh), capacity_);
    sub_quads_[2] = new Quadtree(Boundery(cx + hhw, cy + hhh, hhw, hhh), capacity_);
    sub_quads_[3] = new Quadtree(Boundery(cx - hhw, cy + hhh, hhw, hhh), capacity_);
    is_devided_ = true;
}