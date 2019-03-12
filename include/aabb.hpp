#ifndef AABB_HPP
#define AABB_HPP

#include <glm/glm.hpp>
#include "point.hpp"

/**
 * @brief Axis-Aligned Bounding Box for representing the boundery/rect.
 * 
 */
struct AABB
{
    AABB() : cx(0.0),
             cy(0.0),
             hw(1.0),
             hh(1.0)
    {
    }

    AABB(double center_x, double center_y, double half_width, double half_height)
        : cx(center_x), cy(center_y), hw(half_width), hh(half_height)
    {
    }

    AABB(const AABB &a)
        : cx(a.cx), cy(a.cy), hw(a.hw), hh(a.hh)
    {
    }

    void operator=(const AABB &a)
    {
        cx = a.cx;
        cy = a.cy;
        hw = a.hw;
        hh = a.hh;
    }

    double cx; // center x
    double cy; // center y
    double hw; // half width
    double hh; // half height
};


/**
 * @brief Checks wether the AABB contains the point.
 * 
 * @param a         The AABB
 * @param p         The point
 * @return true     If the point is inside the AABB
 * @return false    If the point is not inside the AABB
 */
inline bool does_contain(const AABB &a, const Point2D &p)
{
    return (double)p.x <= a.cx + a.hw &&
           (double)p.x >= a.cx - a.hw &&
           (double)p.y <= a.cy + a.hh &&
           (double)p.y >= a.cy - a.hh;
}


/**
 * @brief Checks wether the two AABB do overlap
 * 
 * @param a         The first AABB
 * @param b         The second AABB
 * @return true     IF two AABBs do overlap
 * @return false    IF two AABBs do not overlap
 */
inline bool do_overlap(const AABB &a, const AABB &b)
{
    return !(a.cx - a.hw > b.cx + b.hw ||
             a.cx + a.hw < b.cx - b.hw ||
             a.cy - a.hh > b.cy + b.hh ||
             a.cy + a.hh < b.cy - b.hh);
}

#endif // !AABB_HPP