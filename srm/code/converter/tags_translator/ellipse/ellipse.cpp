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

/**
 * Ellipse arc from 'path' tag sampling function
 * @param[in] p1 first arc point
 * @param[in] p2 second arc point
 * @param[in] radiuses 2 ellipse radiuses by x and y
 * @param[in] fA flag for arc angle size
 * @param[in] fS flag for angle delta
 * @param[in] phi angle
 * @param[in] accuracy sampling accuracy
 * @return point vector with sampling
 */
std::vector<srm::vec_t> srm::EllipseArcSampling(vec_t p1, vec_t p2, vec_t radiuses, bool fA, bool fS, double phi, double accuracy) {
  // evaluate center
  vec_t delta2 = (p1 - p2) / 2;
  double si = sin(phi), co = cos(phi);
  vec_t p1s = vec_t(co * delta2.x + si * delta2.y, -si * delta2.x + co * delta2.y);
  double
    tmp = radiuses.x * radiuses.x * p1s.y * p1s.y + radiuses.y * radiuses.y * p1s.x * p1s.x,
    muler = sqrt((radiuses.x * radiuses.x * radiuses.y * radiuses.y - tmp) / tmp);
  if (fA == fS)
    muler = -muler;
  vec_t cs = vec_t(muler * radiuses.x * p1s.y / radiuses.y, muler * radiuses.y * p1s.x / radiuses.x);
  vec_t center = vec_t(co * cs.x - si * cs.y + (p1.x + p2.x) / 2, si * cs.x + co * cs.y + (p1.y + p2.y) / 2);

  // evaluate angles (parametres)
  vec_t
    u = vec_t(1, 0),
    v = vec_t((p1s.x - cs.x) / radiuses.x, (p1s.y - cs.y) / radiuses.y);
  double param1 = acos(u.Dot(v) / u.Len() / v.Len());
  if (u.Cross(v) < 0)
    param1 = -param1;
  u = v;
  v = vec_t(-(p1s.x + cs.x) / radiuses.x, -(p1s.y + cs.y) / radiuses.y);
  double paramDelta = acos(u.Dot(v) / u.Len() / v.Len()), param2 = param1;
  if (u.Cross(v) < 0)
    paramDelta = 2 * pi - paramDelta;
  if (fS)
    param2 += paramDelta;
  else
    param2 += paramDelta - 2 * pi;

  // add fisrt 3 points
  std::list<std::pair<double, vec_t>> sampling;
  sampling.push_back(std::pair<double, vec_t>(param1, vec_t(center.x + radiuses.x * cos(param1), center.y + radiuses.y * sin(param1))));
  double med = (param1 + param2) / 2;
  sampling.push_back(std::pair<double, vec_t>(med, vec_t(center.x + radiuses.x * cos(med), center.y + radiuses.y * sin(med))));
  sampling.push_back(std::pair<double, vec_t>(param2, vec_t(center.x + radiuses.x * cos(param2), center.y + radiuses.y * sin(param2))));

  // sampling
  auto cur = sampling.begin();
  auto next = cur;
  next++;
  double accuracy2 = accuracy * accuracy;
  while (next != sampling.end()) {
    if ((cur->second - next->second).Len2() < accuracy2) {
      cur = next;
      next++;
    }
    else {
      double med = (cur->first + next->first) / 2;
      next = sampling.insert(next, std::pair<double, vec_t>(med, vec_t(center.x + radiuses.x * cos(med), center.y + radiuses.y * sin(med))));
    }
  }

  // create result vector
  std::vector<vec_t> res(sampling.size());
  cur = sampling.begin();
  for (auto &vec : res) {
    vec = cur->second;
    cur++;
  }
  return res;
}
