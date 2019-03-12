#ifndef QUAD_TREE_HPP
#define QUAD_TREE_HPP

#include <vector>
#include <array>
#include <memory>
#include <sstream>
#include "aabb.hpp"

using Boundery = AABB;

/**
 * @brief A 2D Quadtree structure for representing points position.
 * 
 */
class Quadtree
{
  private:
    Boundery boundery_;
    std::size_t capacity_;
    std::vector<Point2D> points_;
    bool is_devided_;
    std::array<Quadtree *, 4> sub_quads_;

  public:
    Quadtree(const Boundery &boundery, std::size_t max_capacity = 5);
    ~Quadtree();
    const Boundery &boundery() const;
    bool is_devided() const { return is_devided_; };
    std::size_t capacity() const;
    std::size_t nr_of_points() const;
    const std::vector<Point2D> &points() const;
    const std::array<Quadtree *, 4> sub_quads() const { return sub_quads_;};

    /**
     * @brief Inserts a point into Quadtree.
     * 
     * @param p       The given point to insert.
     * @return true   If the point is within the quad boundery.
     * @return false  If the point is not within the quad boundery.
     */
    bool insert(const Point2D &p);

    /**
     * @brief Finds the neighbor points inside the given range.
     * 
     * @param range           A rectangle to search for neighbor points.
     * @param points_in_range The list of points in range.
     */
    void query(const Boundery &range, std::vector<Point2D *> &points_in_range);

    friend std::ostream &operator<<(std::ostream &stream, const Quadtree &qtree)
    {
        for (auto &p : qtree.points())
            stream << p << std::endl;
        return stream;
    }

  private:
    /**
   * @brief Devides the Quadtree into 4 sub-quads
   * 
   */
    void subdevide();
};

#endif