/**
 * @file
 * @brief Fill class source file
 * @authors Pavlov Ilya
 * @date 18.04.2021
 *
 * Contains realisation of class for filling primitives
 */

#include <srm.h>

 /**
  * Principal component analysis
  * @param[in] primitive for filling
  * @return pair of the first and second principal components vectors
  */
std::pair<srm::vec_t, srm::vec_t> _PCA(srm::primitive_t* primitive) {
  double ex , ey; // expected value of x and y

  ex = primitive->start.x;
  ey = primitive->start.y;
  for (const auto &segment : *primitive) {
    ex += segment.point.x;
    ey += segment.point.y;
  }
  ex /= primitive->size() + 1;
  ey /= primitive->size() + 1;

  double cov00 = 0, 
         cov01 = 0, 
         cov11 = 0; // symmetric covariance matrix

  double dx, dy;
  for (const auto& segment : *primitive) {
    dx = segment.point.x - ex;
    dy = segment.point.y - ey;
    cov00 += dx * dx;
    cov01 += dx * dy;
    cov11 += dy * dy;
  }
  cov00 /= primitive->size() + 1;
  cov01 /= primitive->size() + 1;
  cov11 /= primitive->size() + 1;

  double sqrtD = sqrt((cov00 - cov11) * (cov00 - cov11) + 4 * cov01 * cov01);
  double lambda1 = (cov00 + cov11 + sqrtD) / 2;
  double lambda2 = (cov00 + cov11 - sqrtD) / 2;
  
  double y1 = 1;
  double x1;
  x1 = cov01 > 1e-8 ? (lambda1 - cov11) / cov01 : 0;

  double y2 = 1;
  double x2;
  x2 = cov01 > 1e-8 ? (lambda2 - cov11) / cov01 : 0;

  srm::vec_t e1(x1, y1);
  e1.Normalize();

  srm::vec_t e2(x2, y2);
  e2.Normalize();

  return fabs(lambda1) > fabs(lambda2) ? std::pair(e1, e2) : std::pair(e2, e1);
}

 /**
  * Gen and print code for filling primitive
  * @param[in] out output stream
  * @param[in] primitive for filling
  * @warning primitive can be changed
  */
void srm::fill_t::FillPrim(std::ostream &out, srm::primitive_t *primitive) {
  auto basis =_PCA(primitive);
  vec_t e1 = basis.first;
  vec_t e2 = basis.second;
}
