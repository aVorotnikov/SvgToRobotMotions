/**
 * @file
 * @brief Fill primitive source file
 * @authors Pavlov Ilya
 * @date 19.04.2021
 *
 * Contains realisation of function for filling primitives
 */

#include <srm.h>

#include <algorithm>
#include <sstream>

 /**
  * Principal component analysis
  * @param[in] primitive for filling
  * @return pair of the first and second principal components vectors
  */
static std::pair<srm::vec_t, srm::vec_t> _PCA(const srm::primitive_t &primitive)  noexcept {
  const double eps = 1e-4; // defines when primitive is considered closed
  bool isClosed = true;

  double size = primitive.size();
  if ((primitive[primitive.size() - 1].point - primitive.start).Len() > eps) {
    isClosed = false;
    ++size;
  }

  double ex, ey; // expected value of x and y
  ex = 0;
  ey = 0;
  for (const auto &segment : primitive) {
    ex += segment.point.x;
    ey += segment.point.y;
  }
  if (!isClosed) {
    ex += primitive.start.x;
    ey += primitive.start.y;
  }
  ex /= size;
  ey /= size;

  double cov00 = 0, 
         cov01 = 0, 
         cov11 = 0; // symmetric covariance matrix

  double dx, dy;
  for (const auto &segment : primitive) {
    dx = segment.point.x - ex;
    dy = segment.point.y - ey;
    cov00 += dx * dx;
    cov01 += dx * dy;
    cov11 += dy * dy;
  }
  if (!isClosed) {
    dx = primitive.start.x - ex;
    dy = primitive.start.y - ey;
    cov00 += dx * dx;
    cov01 += dx * dy;
    cov11 += dy * dy;
  }
  cov00 /= size;
  cov01 /= size;
  cov11 /= size;

  double sqrtD = sqrt((cov00 - cov11) * (cov00 - cov11) + 4 * cov01 * cov01);
  double lambda1 = (cov00 + cov11 + sqrtD) / 2;
  double lambda2 = (cov00 + cov11 - sqrtD) / 2;
  
  double y1 = 1;
  double x1;
  x1 = fabs(cov01) > 1e-8 ? (lambda1 - cov11) / cov01 : 0;

  double y2 = 1;
  double x2;
  x2 = fabs(cov01) > 1e-8 ? (lambda2 - cov11) / cov01 : 0;

  if (x1 == 0 && x2 == 0) {
    return cov00 >= cov11 ? std::pair(srm::vec_t(1, 0), srm::vec_t(0, 1)) : std::pair(srm::vec_t(0, 1), srm::vec_t(1, 0));
  }

  srm::vec_t e1(x1, y1);
  e1.Normalize();

  srm::vec_t e2(x2, y2);
  e2.Normalize();

  return fabs(lambda1) > fabs(lambda2) ? std::pair(e1, e2) : std::pair(e2, e1);
}

/**
 * Get list of primitive's segments
 * @param[out] primitive primitive
 * @param[out] segments list of primitive's segments
 */
static void _getSegmentsList(const srm::primitive_t &primitive, 
  std::list<std::pair<srm::vec_t, srm::vec_t>> *segments) {

  segments->clear();

  const double eps = 1e-4; // defines when primitive is considered closed
  
  segments->push_back(std::pair{primitive.start, primitive[0].point});
  for (int i = 0; i < primitive.size() - 1; ++i) 
    segments->push_back(std::pair{ primitive[i].point, primitive[i + 1].point });

  if ((primitive[primitive.size() - 1].point - primitive.start).Len() > eps) // is not closed
    segments->push_back(std::pair{ primitive[primitive.size() - 1].point, primitive.start });

}

/**
 * Find intersection points of line with primitive's segments
 * @param[in] h height of line along the axis e2
 * @param[in] e1 the first basis vector
 * @param[in] e2 the second basis vector
 * @param[in] segments list of primitive's segments
 * @param[out] interPoints intersection points of line with primitive's segments
 */
static void _getIntersectionPoints(double h, srm::vec_t e1, srm::vec_t e2, 
  std::list<std::pair<srm::vec_t, srm::vec_t>> *segments, 
  std::list<srm::vec_t> *interPoints) noexcept {

  interPoints->clear();

  auto it = segments->begin();
  double A1, B1, C1; // line equation coefs Ax + By + C = 0 for painting line
  double A2, B2, C2; // line equation coefs Ax + By + C = 0 for line includes segment
  double x, y; // intersection coords
  double det; // determinant
  const double eps = 1e-8; // 0 with error
  A1 = e2.x;
  B1 = e2.y;
  while (it != segments->end() && std::min(it->first.Dot(e2), it->second.Dot(e2)) < h) {
    if (std::max(it->first.Dot(e2), it->second.Dot(e2)) < h) {
      auto tmp = ++it;
      --it;
      segments->erase(it);
      it = tmp;
      continue;
    }
    C1 = -h;
    A2 = it->second.y - it->first.y;
    B2 = it->first.x - it->second.x;
    C2 = it->first.y * it->second.x - it->first.x * it->second.y;
    det = A1 * B2 - A2 * B1;
    if (fabs(det) > eps) {
      x = (-C1 * B2 + C2 * B1) / det;
      y = (-A1 * C2 + A2 * C1) / det;
      if (x >= std::min(it->first.x, it->second.x) - eps && x <= std::max(it->first.x, it->second.x + eps)
        && y >= std::min(it->first.y, it->second.y) - eps && y <= std::max(it->first.y, it->second.y) + eps) {
        interPoints->push_back(srm::vec_t(x, y));
      }
    }
    ++it;
  }
}

/**
 * Print code for filling primitive in one line
 * @param[in] out output stream
 * @param[in] interPoints intersection points of line with primitive's segments
 */
static void _writeCode(std::ostream &out, const std::list<srm::vec_t> &interPoints) noexcept {
  auto it1 = interPoints.begin();
  auto tmp = it1;
  auto it2 = it1;

  srm::vec3_t delta, delta2;
  srm::translator_t* trans = srm::translator_t::GetPtr();
  while (it1 != interPoints.end()) {
    tmp = it1;
    it2 = ++it1;
    it1 = tmp;

    delta = trans->roboConf.SvgToRobotDelta(*it1);
    delta2 = trans->roboConf.SvgToRobotDelta(*it2);
    out << "\tLAPPRO SHIFT (p1 BY " +
      std::to_string(delta.x) + ", " +
      std::to_string(delta.y) + ", " +
      std::to_string(delta.z) + "), " << std::to_string(trans->roboConf.GetDepDist()) << "\n";
    out << "BREAK\n";

    out << "\tLMOVE SHIFT (p1 BY " +
      std::to_string(delta.x) + ", " +
      std::to_string(delta.y) + ", " +
      std::to_string(delta.z) + ")\n";;
    out << "BREAK\n";

    out << "\tLMOVE SHIFT (p1 BY " +
      std::to_string(delta2.x) + ", " +
      std::to_string(delta2.y) + ", " +
      std::to_string(delta2.z) + ")\n";;
    out << "BREAK\n";


    out << "\tLDEPART " << std::to_string(trans->roboConf.GetDepDist()) << "\n";
    out << "BREAK\n";

    ++it1;
    ++it1;
  }
}

/**
  * Gen and print code for filling primitive
  * @param[in] out output stream
  * @param[in] primitive for filling
  */
void srm::FillPrimitive(std::ostream &out, const srm::primitive_t &primitive) noexcept {
  auto basis =_PCA(primitive);
  vec_t e1 = basis.first;
  vec_t e2 = basis.second;

  std::list<std::pair<vec_t, vec_t>> segments;
  _getSegmentsList(primitive, &segments);
  segments.sort([e2](const std::pair<vec_t, vec_t>& lhs, const std::pair<vec_t, vec_t>& rhs) -> bool {
    return std::min(lhs.first.Dot(e2), lhs.second.Dot(e2)) < std::min(rhs.first.Dot(e2), rhs.second.Dot(e2));
    });

  double y = std::min(segments.front().first.Dot(e2), segments.front().second.Dot(e2));
  double finish = -1e60;
  double tmp;
  for (const auto &segment : segments) {
    tmp = std::max(segment.first.Dot(e2), segment.second.Dot(e2));
    if (tmp > finish)
      finish = tmp;
  }

  translator_t *trans = translator_t::GetPtr();
  // TODO: step from robot to svg
  std::list<vec_t> interPoints;
  double step = trans->roboConf.GetPouringStep();
  bool directionFlag = false;
  while (y < finish) {
    _getIntersectionPoints(y, e1, e2, &segments, &interPoints);
    if (directionFlag) {
      interPoints.sort([e1](const vec_t& lhs, const vec_t& rhs) {
        return lhs.Dot(e1) < rhs.Dot(e1);
        });
    }
    else {
      interPoints.sort([e1](const vec_t& lhs, const vec_t& rhs) {
        return lhs.Dot(e1) > rhs.Dot(e1);
        });
    }

    _writeCode(out, interPoints);

    directionFlag = directionFlag ? false : true;

    y += step;
  }
}

/**
 *Check if tag must be filled
 * @param[in] tag tag for checking
 * @return true if must, false if not
 */
bool srm::IsFill(const rapidxml::xml_node<>* tag) noexcept {
  std::string tagName;
  tagName.assign(tag->name(), tag->name_size());
  if (tagName == "line" || tagName == "text")
    return false;

  auto attr = tag->last_attribute("fill");
  if (!attr || attr->value() == "")
    return true;
   
  std::string fillAttr(attr->value());
  std::istringstream iss(fillAttr);
  std::string fillVal;
  std::string check;
  iss >> fillVal;
  iss >> check;

  return check != "" || (fillVal != "none" && fillVal != "white" && fillVal != "#fff");
}
