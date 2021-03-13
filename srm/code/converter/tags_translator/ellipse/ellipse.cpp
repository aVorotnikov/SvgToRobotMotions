/**
 * @file
 * @brief ellipse and arcs sampling functions source file
 * @authors Vorotnikov Andrey
 * @date 13.03.2021
 *
 * Contains ellipse and arcs sampling functions realisation
 */

#include <srm.h>

/**
 * Ellipse sampling function
 * @param[in] center ellipse center point
 * @param[in] radiuses 2 ellipse radiuses by x and y
 * @param[in] accuracy sampling accuracy
 * @return point vector with sampling
 */
std::vector<srm::vec_t> srm::EllipseSampling(vec_t center, vec_t radiuses, double accuracy) {
  const double param0 = pi / 2;
  if (radiuses.x <= 0 || radiuses.y <= 0)
    throw std::exception("Incorrect radius in ellipse");
  if (accuracy <= 0)
    throw std::exception("Incorrect accuracy");

  double accuracy2 = accuracy * accuracy, param = 2 * param0, angle0, dist2;
  unsigned numOfPoints = 2;
  vec_t p0;
  if (radiuses.x > radiuses.y) {
    p0.x += radiuses.x;
    angle0 = 0;
  }
  else {
    p0.y += radiuses.y;
    angle0 = pi / 2;
  }

  do {
    param /= 2;
    numOfPoints *= 2;
    double angle = angle0 + param;
    dist2 = (vec_t(radiuses.x * cos(angle), radiuses.y * sin(angle)) - p0).Len2();
  } while (dist2 > accuracy2);

  std::vector<vec_t> res(numOfPoints);
  for (unsigned i = 0; i < numOfPoints; i++) {
    double angle = 2 * pi * i / numOfPoints;
    res[i] = vec_t(center.x + radiuses.x * cos(angle), center.y + radiuses.y * sin(angle));
  }
  return res;
}
