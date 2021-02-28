/**
 * @file
 * @brief Bezier building class source file
 * @authors Vorotnikov Andrey
 * @date 28.02.2021
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
 * @param[in] startDelta delta to evaluate first point (default value - 1)
 * @return Bezier point vector
 * @warning accuracy must be greater than 0
 */
std::vector<srm::vec_t> srm::build_bezier_t::Sampling(double accuracy, double startDelta) const {
  if (accuracy <= 0)
    throw std::exception("Incorrect accuracy for sampling");

  // choose initial params: delta, t param
  double delta = startDelta, accuracy2 = accuracy * accuracy;
  double tCur[] = {0, 1}, tPrev[] = {0, 1};
  std::list<vec_t> resList[2];

  // add first point
  vec_t prevPos[] = {EvaluatePoint(tPrev[0]), EvaluatePoint(tPrev[1])};
  resList[0].push_back(prevPos[0]);
  resList[1].push_back(prevPos[1]);

  bool toFirst = 0;
  if (startDelta >= 1 && (prevPos[0] - prevPos[1]).Len2() > accuracy2)
    delta = 0.5;
  while (tCur[0] < tCur[1] - delta) {
    if (toFirst)
      tCur[toFirst] = tPrev[toFirst] - delta;
    else
      tCur[toFirst] = tPrev[toFirst] + delta;
    // calculate new point
    vec_t curPos = EvaluatePoint(tCur[toFirst]);
    // case when point is suitable : add point
    if ((curPos - prevPos[toFirst]).Len2() <= accuracy2) {
      resList[toFirst].push_back(curPos);
      prevPos[toFirst] = curPos;
      tPrev[toFirst] = tCur[toFirst];
      toFirst = !toFirst;
    }
    // case when point is unsuitable : decrease delta
    else {
      delta /= 2;
      tCur[toFirst] = tPrev[toFirst];
    }
  }
  // create res vector from 2 lists
  std::vector<vec_t> res;
  res.reserve(resList[0].size() + resList[1].size());
  for (auto &point : resList[0])
    res.push_back(point);
  const auto &listStartIt = resList[1].cbegin();
  for (auto it = --resList[1].end(); it != listStartIt; it--)
    res.push_back(*it);
  res.push_back(*listStartIt);
  return res;
}
