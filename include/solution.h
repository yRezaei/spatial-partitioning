#ifndef SOLUTION_HPP
#define SOLUTION_HPP
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <cmath>
#include <thread>
#include "../include/Timer.h"
#include "../include/quad_tree.h"

/**
 * @brief A class for representing closest point solution. Please use the static function "execute" to proceed.
 * 
 */
class Solution
{
public:
  enum ProcessType
  {
    WITHOUT_THREAD,
    WITH_THREAD
  };

public:
  /**
   * @brief Handels the calculation of the closest points with regard of signe/multi threaded process.
   * 
   * @param points        List of 2D points.
   * @param result        List of found closest points.
   * @param points_range  Maximum with of area that the points are destributed in.
   * @param max_capacity  Maximum capacity of the points for each quad. 
   * @param process_type  Signle thread or multithread
   */
  static void execute(const std::vector<Point> &points, std::vector<std::pair<Point, Point>> &result,
                      double points_range, int max_capacity, ProcessType process_type);
};

#endif // !SOLUTION_HPP
