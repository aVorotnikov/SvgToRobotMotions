/**
 * @file
 * @brief Bezier building class source file
 * @authors Vorotnikov Andrey
 * @date 27.02.2021
 *
 * Contains class to build Bezier spline realisation
 */

#include <srm.h>

#include <list>

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
std::vector<srm::vec_t> srm::build_bezier_t::Sampling(double accuracy, double startDelta) const {
  if (accuracy <= 0)
    throw std::exception("Incorrect accuracy for sampling");

  // choose initial params: delta, t param
  double delta = startDelta, accuracy2 = accuracy * accuracy;
  double tCur1 = 0, tPrev1 = 0, tCur2 = 1, tPrev2 = 1;
  std::list<vec_t> resList1, resList2;

  // add first point
  vec_t
    prevPos1 = EvaluatePoint(tPrev1),
    prevPos2 = EvaluatePoint(tPrev2);
  resList1.push_back(prevPos1);
  resList2.push_front(prevPos2);

  bool toFirst = 1;
  while (tCur1 < tCur2) {
    // case to use first list
    if (toFirst) {
      tCur1 = tPrev1 + delta;
      // calculate new point
      vec_t curPos = EvaluatePoint(tCur1);
      // case when point is suitable : add point
      if ((curPos - prevPos1).Len2() <= accuracy2) {
        resList1.push_back(curPos);
        prevPos1 = curPos;
        tPrev1 = tCur1;
        toFirst = 0;
      }
      // case when point is unsuitable : decrease delta
      else {
        delta /= 2;
        tCur1 = tPrev1;
      }
    }
    // case to use second list
    else {
      tCur2 = tPrev2 - delta;
      // calculate new point
      vec_t curPos = EvaluatePoint(tCur2);
      // case when point is suitable : add point
      if ((curPos - prevPos2).Len2() <= accuracy2) {
        resList2.push_front(curPos);
        prevPos2 = curPos;
        tPrev2 = tCur2;
        toFirst = 1;
      }
      // case when point is unsuitable : decrease delta
      else {
        delta /= 2;
        tCur2 = tPrev2;
      }
    }
  }
  // create res vector from 2 lists
  std::vector<vec_t> res;
  res.reserve(resList1.size() + resList2.size());
  for (auto &point : resList1)
    res.push_back(point);
  for (auto &point : resList2)
    res.push_back(point);
  return res;
}
