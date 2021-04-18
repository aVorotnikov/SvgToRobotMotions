/**
 * @file
 * @brief Fill primitive source file
 * @authors Pavlov Ilya
 * @date 18.04.2021
 *
 * Contains realisation of function for filling primitives
 */

#include <srm.h>

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

static void _getSegmentsList(const srm::primitive_t &primitive, 
  std::list<std::pair<srm::vec_t, srm::vec_t>> *segments) {
  
  segments->push_back(std::pair{primitive.start, primitive[0].point});
  for (int i = 0; i < primitive.size() - 1; ++i) 
    segments->push_back(std::pair{ primitive[i].point, primitive[i + 1].point });

  if ((primitive[primitive.size() - 1].point - primitive.start).Len() > 1e-4) 
    segments->push_back(std::pair{ primitive[primitive.size() - 1].point, primitive.start });

}

 /**
  * Gen and print code for filling primitive
  * @param[in] out output stream
  * @param[in] primitive for filling
  */
void srm::FillPrimitive(std::ostream &out, const srm::primitive_t &primitive) {
  auto basis =_PCA(primitive);
  vec_t e1 = basis.first;
  vec_t e2 = basis.second;

  std::list<std::pair<vec_t, vec_t>> segments;
  _getSegmentsList(primitive, &segments);

}
