/**
 * @file
 * @brief Split primitive source file
 * @authors Vorotnikov Andrey
 * @date 19.04.2021
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
static bool _isInside(srm::vec_t point, double width, double height) noexcept {
  if (point.x < 0 || point.y < 0 || point.x > width || point.y > height)
    return false;
  return true;
}

/** \brief Project namespace */
namespace srm {
  /** \brief Split primitives file namespace */
  namespace spf {
    /**
     * @brief Line segment representation type
     *
     * Struct to save line segment with point and dir
     */
    struct line_segm_t {
    public:
      vec_t
        point,  ///< start point
        dir;    ///< direction vector

      /**
       * 2 line segments intersection function
       * @param[in] ls line segment to intersec
       * @return 2 parametres of intersection
       */
      std::pair<double, double> Intersec(const line_segm_t &ls) const {
        double delta = math::det<double>(dir, -ls.dir);
        if (delta == 0)
          throw std::exception("Line segments are parallel");
        vec_t pointDelta = ls.point - point;
        double
          delta1 = math::det<double>(pointDelta, -ls.dir),
          delta2 = math::det<double>(dir, pointDelta);
        return std::pair<double, double>(delta1 / delta, delta2 / delta);
      }
    };

    /**
     * @brief Svg coordinate system representation class
     *
     * Class to save svg coordinate system bar for intersections
     */
    class svg_cs_t {
    private:
      line_segm_t barSegms[4];  ///< line segments of cs bar

    public:
      /**
       * Class constructor by image parametres
       * @param[in] w svg width
       * @param[in] h svg height
       */
      svg_cs_t(double w, double h) {
        if (w <= 0 || h <= 0)
          throw std::exception("Incorrect w and h");
        barSegms[0] = {{0, 0}, {0, h}};
        barSegms[1] = {{0, h}, {w, 0}};
        barSegms[2] = {{w, h}, {0, -h}};
        barSegms[3] = {{w, 0}, {-w, 0}};
      }

      /**
       * Intersection between cs bar and linesegment function
       * @param[in] ls line segment to intersec
       * @return point of intersection (size may be 0, 1, 2)
       */
      std::vector<vec_t> Intersec(const line_segm_t &ls) const {
        bool isFirstPoint = 0;
        bool isParallelY = 0;
        std::pair<double, vec_t> firstPoint;
        // ort j
        try {
          auto [t1, t2] = ls.Intersec(barSegms[0]);
          if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
            isFirstPoint = 1;
            firstPoint = {t1, ls.point + ls.dir * t1};
          }
        }
        catch (std::exception&) {
          isParallelY = 1;
        }
        if (!isParallelY) {
          auto [t1, t2] = ls.Intersec(barSegms[2]);
          if (isFirstPoint && t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
            if (t1 > firstPoint.first)
              return {firstPoint.second, ls.point + ls.dir * t1};
            return {ls.point + ls.dir * t1, firstPoint.second};
          }
          else if (!isFirstPoint && t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
            isFirstPoint = 1;
            firstPoint = {t1, ls.point + ls.dir * t1};
          }
        }
        // ort i
        bool isParallelX = 0;
        try {
          auto [t1, t2] = ls.Intersec(barSegms[1]);
          if (isFirstPoint && t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
            if (t1 > firstPoint.first)
              return {firstPoint.second, ls.point + ls.dir * t1};
            return {ls.point + ls.dir * t1, firstPoint.second};
          }
          else if (!isFirstPoint && t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
            isFirstPoint = 1;
            firstPoint = {t1, ls.point + ls.dir * t1};
          }
        }
        catch (std::exception &) {
          isParallelX = 1;
        }
        if (!isParallelX) {
          auto [t1, t2] = ls.Intersec(barSegms[3]);
          if (isFirstPoint && t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
            if (t1 > firstPoint.first) {
              std::vector<vec_t> res(2);
              res[0] = firstPoint.second;
              res[1] = ls.point + ls.dir * t1;
              return {firstPoint.second, ls.point + ls.dir * t1};
            }
            return {ls.point + ls.dir * t1, firstPoint.second};
          }
          else if (!isFirstPoint && t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
            isFirstPoint = 1;
            firstPoint = {t1, ls.point + ls.dir * t1};
          }
        }
        
        if (isFirstPoint)
          return {firstPoint.second};
        return {};
      }
    };
  }
}

/**
 * Define is point in borders function
 * @param[in] point point to define
 * @param[in] w width of svg
 * @param[in] h height of svg
 * @return true if in, false - otherwise
 */
static bool _isInBorders(srm::vec_t point, double w, double h) {
  return point.x <= w && point.x >= 0 && point.y <= h && point.y >= 0;
}

/**
 * Split primitive to list function
 * @param[in] prim primitive to split
 * @param[out] splitted splitted primitive
 */
static void _splitPrimitive(srm::primitive_t *prim, std::list<srm::primitive_t *> *splitted) {
  auto *trans = srm::translator_t::GetPtr();
  double w = trans->roboConf.GetW(), h = trans->roboConf.GetH();
  srm::spf::svg_cs_t svgBar(w, h);
  splitted->clear();

  enum class point_place {
    not_defined,
    in,
    out
  } firstState = point_place::not_defined, prev = point_place::not_defined, cur = point_place::not_defined;
  srm::vec_t prevP = prim->start;
  if (_isInBorders(prevP, w, h)) {
    prev = point_place::in;
    splitted->push_back(new srm::primitive_t);
    splitted->back()->start = prevP;
  }
  else
    prev = point_place::out;
  firstState = prev;
  for (auto &point : *prim) {
    if (_isInBorders(point.point, w, h))
      cur = point_place::in;
    else
      cur = point_place::out;

    if (cur == point_place::in && prev == point_place::in)
      splitted->back()->push_back(srm::segment_t(point.point.x, point.point.y));
    else if (cur == point_place::in && prev == point_place::out) {
      splitted->push_back(new srm::primitive_t);
      srm::spf::line_segm_t segm;
      segm.point = prevP;
      segm.dir = point.point - prevP;
      auto intersec = svgBar.Intersec(segm);
      splitted->back()->start = intersec[0];
      splitted->back()->push_back(srm::segment_t(point.point.x, point.point.y));
    }
    else if (cur == point_place::out && prev == point_place::in) {
      srm::spf::line_segm_t segm;
      segm.point = prevP;
      segm.dir = point.point - prevP;
      auto intersec = svgBar.Intersec(segm);
      splitted->back()->push_back(srm::segment_t(intersec.back().x, intersec.back().y));
    }
    else {
      srm::spf::line_segm_t segm;
      segm.point = prevP;
      segm.dir = point.point - prevP;
      auto intersec = svgBar.Intersec(segm);

      if (intersec.size() == 2) {
        splitted->push_back(new srm::primitive_t);
        splitted->back()->start = intersec[0];
        splitted->back()->push_back(srm::segment_t(intersec[1].x, intersec[1].y));
      }
    }
    prev = cur;
    prevP = point.point;
  }
  if (prim->fill && firstState == point_place::in && splitted->size() > 1) {
    for (auto point : *splitted->front())
      splitted->back()->push_back(point);
    delete splitted->front();
    splitted->pop_front();
  }
}

/**
 * Unite splitted fill primitive function
 * @param[out] splitted splitted primitive
 */
static void _unitePrimitives(std::list<srm::primitive_t *> *splitted) {
  auto first = splitted->begin();
  auto prim = first;
  prim++;
  while (prim != splitted->end()) {
    (*first)->push_back(srm::segment_t((*prim)->start.x, (*prim)->start.y));
    for (auto point : **prim)
      (*first)->push_back(point);
    prim++;
  }
  prim = first;
  for (prim++; prim != splitted->end(); prim++)
    delete *prim;
  splitted->erase(++first, splitted->end());
  splitted->front()->fill = true;
  srm::vec_t start = splitted->front()->start;
  splitted->back()->push_back(srm::segment_t(start.x, start.y));
}

/**
 * Split primitives in list function
 * @param[out] prims primitive to split
 */
void srm::SplitPrimitives(std::list<primitive_t *> *prims) {
  auto prim = prims->begin();
  while (prim != prims->end()) {
    std::list<primitive_t *> splittedPrim;
    _splitPrimitive(*prim, &splittedPrim);
    if ((*prim)->fill)
      _unitePrimitives(&splittedPrim);
    for (auto &sPrim : splittedPrim)
      prims->insert(prim, sPrim);
    delete *prim;
    prim = prims->erase(prim);
  }
}
