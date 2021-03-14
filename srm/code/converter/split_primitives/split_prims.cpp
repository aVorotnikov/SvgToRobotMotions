/**
 * @file
 * @brief Split primitive source file
 * @authors Vorotnikov Andrey
 * @date 14.03.2021
 *
 * Contains function realisation to split primitives by points out of svg cs
 */

#include <srm.h>

/**
 * Is point inside coordinate system evaluation function
 * @param[in] point point to evaluate
 * @param[in] width svg width
 * @param[in] height svg height
 * @return true if is inside, false - otherwise
 */
static bool _isInside(srm::vec_t point, double width, double height) {
  if (point.x < 0 || point.y < 0 || point.x > width || point.y > height)
    return false;
  return true;
}

/**
 * Split primitives in list function
 * @param[out] prims primitive to split
 */
void srm::SplitPrimitives(std::list<primitive_t *> *prims) {
}
