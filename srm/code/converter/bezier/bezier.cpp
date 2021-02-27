/**
 * @file
 * @brief Bezier building class source file
 * @authors Vorotnikov Andrey
 * @date 27.02.2021
 *
 * Contains class to build Bezier spline realisation
 */

#include <srm.h>

/**
 * Evaluate point with parameter
 * @param[in] t parameter from 0 to 1 to evaluate point of Bezier spline
 * @return point corresponding to parameter
 */
srm::vec_t srm::build_bezier_t::EvaluatePoint(double t) const {
  if (size() == 0)
    throw std::exception("No points added");
  std::vector<vec_t> segments(*this);
  
  while (segments.size() > 1) {
    auto iterator1 = segments.begin(), iterator2 = iterator1 + 1, iteratorEnd = segments.end();

    while (iterator2 != iteratorEnd) {
      *iterator1 = *iterator1 * (1 - t) + *iterator2 * t;
      iterator1 = iterator2;
      iterator2++;
    }

    segments.pop_back();
  }

  return segments[0];
}

/**
 * Sampling by N line segments
 * @param[in] N number of line segments
 * @return Bezier point vector with N+1 length
 * @warning N mustn't be 0
 */
std::vector<srm::vec_t> srm::build_bezier_t::Sampling(unsigned N) const {
  if (N == 0)
    throw std::exception("Too few points for sampling");

  std::vector<vec_t> res;
  res.reserve(N + 1);

  for (unsigned i = 0; i <= N; i++)
    res.push_back(EvaluatePoint((double)i / N));

  return res;
}

/**
 * Sampling by accuracy by line segments
 * @param[in] accuracy length of elementary line segment
 * @return Bezier point vector
 * @warning accuracy must be greater than 0
 */
std::vector<srm::vec_t> srm::build_bezier_t::Sampling(double accuracy) const {
  const double defaultDelta = 0.1;

  if (accuracy <= 0)
    throw std::exception("Incorrect accuracy for sampling");

  // choose initial params: delta, t param
  double delta = defaultDelta, accuracy2 = accuracy * accuracy;
  double tCur = delta, tPrev = 0;
  std::vector<vec_t> res;

  // add first point
  vec_t prevPos = EvaluatePoint(tPrev);
  res.push_back(prevPos);
  while (tPrev < 1) {
    if (tCur > 1)
      tCur = 1;
    // calculate new point
    vec_t curPos = EvaluatePoint(tCur);
    // case when point is suitable : add point
    if ((curPos - prevPos).Len2() <= accuracy2) {
      res.push_back(curPos);
      prevPos = curPos;
      tPrev = tCur;
    }
    // case when point is unsuitable : decrease delta
    else
      delta /= 2;
    tCur = tPrev + delta;
  }
  return res;
}
