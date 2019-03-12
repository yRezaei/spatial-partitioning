#ifndef POINT_HPP
#define POINT_HPP

#include <iomanip>
#include <glm/glm.hpp>


struct Point2D : public glm::f32vec2
{
    Point2D() {}
    Point2D(float X, float Y) : glm::f32vec2(X, Y) {}
    Point2D(const Point2D& p) : glm::f32vec2(p.x, p.y) {}
    void operator=(const Point2D& p) { this->x = p.x, this->y = p.y; }
    bool operator==(const Point2D& p) { return (x == p.x && y == p.y); }
    bool operator!=(const Point2D& p) { return (x != p.x || y != p.y); }

    friend std::ostream& operator << (std::ostream& stream, const Point2D& p)
    {
        stream << "[" << std::left << std::setfill(' ') << std::setw(15) << p.x << "," << std::setfill(' ') << std::setw(15) << p.y << "]";
        return stream;
    }
};


#endif // !POINT_HPP