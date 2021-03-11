/**
 * @file
 * @brief source file for tagsToPrimitives functions
 * @authors Pavlov Ilya
 * @date 11.03.2021
 *
 * Contains tagsToPrimitives realisation and support static functions for each of tags
 */

#include <srm.h>

#include <cmath>

/**
 * Transform primitive by attribute transform
 *
 */
static void _transformPrimitive(srm::primitive_t *primitive) {
  // TODO: add required parametres and realise _transformPrimitive
}

/**
 * Transform svg polyline to primitive
 * @param[in] tag pointer to polyline node in xml DOM
 * @param[out] polylinePrimitive the primitive representations of polyline
 */
static void _polylineToPrimitive(const rapidxml::xml_node<> *tag, srm::primitive_t *polylinePrimitive) {
  // TODO: realise _polylineToPrimitive
}

/**
 * Transform svg polygon to primitive
 * @param[in] tag pointer to polygon node in xml DOM
 * @param[out] polygonPrimitive the primitive representations of polygon
 */
static void _polygonToPrimitive(const rapidxml::xml_node<> *tag, srm::primitive_t *polygonPrimitive) {
  // TODO: realise _polygonToPrimitive
}

 /**
  * Transform svg ellipse to primitive
  * @param[in] tag pointer to ellipse node in xml DOM
  * @param[out] ellipsePrimitive the primitive representations of ellipse
  */
static void _ellipseToPrimitive(const rapidxml::xml_node<> *tag, srm::primitive_t *ellipsePrimitive) {
  double cx, cy, rx, ry;
  try {
    if (tag->last_attribute("cx"))
      cx = std::stod(tag->last_attribute("cx")->value(), NULL);
    else
      throw std::exception("No required attribute in tag \"ellipse\"");
    if(tag->last_attribute("cx"))
      cy = std::stod(tag->last_attribute("cy")->value(), NULL);
    else
      throw std::exception("No required attribute in tag \"ellipse\"");
    if(tag->last_attribute("cx"))
      rx = std::stod(tag->last_attribute("rx")->value(), NULL);
    else
      throw std::exception("No required attribute in tag \"ellipse\"");
    if(tag->last_attribute("cx"))
      ry = std::stod(tag->last_attribute("ry")->value(), NULL);
    else
      throw std::exception("No required attribute in tag \"ellipse\"");
  }
  catch (std::exception error) {
    throw std::exception("Invalid attribute value in tag \"ellipse\"");
  }

  ellipsePrimitive->start.x = cx + rx;
  ellipsePrimitive->start.y = cy;

  const double PI = 3.14159265358979323846;
  double delta = PI / 20;
  double t = delta;
  double x, y;
  while (t < 2 * PI) {
    x = cx + rx * cos(t);
    y = cy + ry * sin(t);
    // TODO: check accuracy
    srm::segment_t p(x, y);
    ellipsePrimitive->push_back(p);
    t += delta;
  }
}

/**
 * Transform svg line to primitive
 * @param[in] tag pointer to line node in xml DOM
 * @param[out] linePrimitive the primitive representations of line
 */
static void _lineToPrimitive(const rapidxml::xml_node<> *tag, srm::primitive_t *linePrimitive) {
  double x1, y1, x2, y2;
  try {
    if (tag->last_attribute("x1"))
      x1 = std::stod(tag->last_attribute("x1")->value(), NULL);
    else
      throw std::exception("No required attribute in tag \"line\"");
    if (tag->last_attribute("y1"))
      y1 = std::stod(tag->last_attribute("y1")->value(), NULL);
    else
      throw std::exception("No required attribute in tag \"line\"");
    if (tag->last_attribute("x2"))
      x2 = std::stod(tag->last_attribute("x2")->value(), NULL);
    else
      throw std::exception("No required attribute in tag \"line\"");
    if (tag->last_attribute("y2"))
      y2 = std::stod(tag->last_attribute("y2")->value(), NULL);
    else
      throw std::exception("No required attribute in tag \"line\"");
  }
  catch (std::exception error) {
    throw std::exception("Invalid attribute value in tag \"line\"");
  }
  srm::segment_t p(x2, y2);
  linePrimitive->start.x = x1;
  linePrimitive->start.y = y1;
  linePrimitive->push_back(p);
}

/**
 * Transform svg circle to primitive
 * @param[in] tag pointer to circle node in xml DOM
 * @param[out] circlePrimitive the primitive representations of circle
 */
static void _circleToPrimitive(const rapidxml::xml_node<> *tag, srm::primitive_t *circlePrimitive) {
  double cx, cy, r;
  try {
    if (tag->last_attribute("cx"))
      cx = std::stod(tag->last_attribute("cx")->value(), NULL);
    else
      throw std::exception("No required attribute in tag \"circle\"");
    if(tag->last_attribute("cy"))
      cy = std::stod(tag->last_attribute("cy")->value(), NULL);
    else
      throw std::exception("No required attribute in tag \"circle\"");
    if(tag->last_attribute("r"))
      r = std::stod(tag->last_attribute("r")->value(), NULL);
    else
      throw std::exception("No required attribute in tag \"circle\"");
  }
  catch (std::exception error) {
    throw std::exception("Invalid attribute value in tag \"circle\"");
  }
  /*
  srm::motion::arc_t* arc = new srm::motion::arc_t(cx, cy + r, cx, cy - r);
  circlePrimitive->start.x = cx;
  circlePrimitive->start.y = cy - r;
  circlePrimitive->push_back(arc);*/

  // TODO: Fix circle translating with segment_t 
}

/**
 * Transform svg rectangle to primitive
 * @param[in] tag pointer to rectangle node in xml DOM
 * @param[out] rectanglePrimitive the primitive representations of rectangle
 */
static void _rectToPrimitive(const rapidxml::xml_node<> *tag, srm::primitive_t *rectanglePrimitive) {
  double x, y, height, width;
  try {
    if (tag->last_attribute("x"))
      x = std::stod(tag->last_attribute("x")->value(), NULL);
    else
      throw std::exception("No required attribute in tag \"rect\"");
    if (tag->last_attribute("y"))
      y = std::stod(tag->last_attribute("y")->value(), NULL);
    else
      throw std::exception("No required attribute in tag \"rect\"");
    if (tag->last_attribute("height"))
      height = std::stod(tag->last_attribute("height")->value(), NULL);
    else
      throw std::exception("No required attribute in tag \"rect\"");
    if (tag->last_attribute("width"))
      width = std::stod(tag->last_attribute("width")->value(), NULL);
    else
      throw std::exception("No required attribute in tag \"rect\"");
  }
  catch (std::exception error) {
    throw std::exception("Invalid attribute value in tag \"rect\"");
  }
  double rx = 0;
  double ry = 0;

  if (tag->last_attribute("rx")) {
    rx = strtod(tag->last_attribute("rx")->value(), NULL);
  }
  if (tag->last_attribute("ry")) {
    ry = strtod(tag->last_attribute("ry")->value(), NULL);
  }

  // Transform to primitive
  rectanglePrimitive->start.x = x;
  rectanglePrimitive->start.y = y;

  srm::segment_t p1(x + width, y);
  srm::segment_t p2(x + width, y + height);
  srm::segment_t p3(x, y + height);
  srm::segment_t p4(x, y);
  rectanglePrimitive->push_back(p1);
  rectanglePrimitive->push_back(p2);
  rectanglePrimitive->push_back(p3);
  rectanglePrimitive->push_back(p4);
}

/**
 * Transform svg tags to primitives
 * @param[in] tags the list of tags in DOM
 * @param[out] primitives the list of primitive representations of tags
 */
void TagsToPrimitives(const std::list<rapidxml::xml_node<>*> &tags, std::list<srm::primitive_t*> *primitives) {
  std::string tagName;
  for (auto tag : tags) {
    try {
      tagName.assign(tag->name(), tag->name_size());

      if (tagName == "svg") {
        srm::translator_t::GetPtr();
        // TODO: Set width and height for cs
      }
      if (tagName == "path") {
        srm::path_t path(primitives);
        path.ParsePath(tag);
      }
      else if (tagName == "rect") {
        srm::primitive_t* rectanglePrimitive = new srm::primitive_t();
        _rectToPrimitive(tag, rectanglePrimitive);
        primitives->push_back(rectanglePrimitive);
      }
      else if (tagName == "circle") {
        srm::primitive_t* circlePrimitive = new srm::primitive_t();
        _circleToPrimitive(tag, circlePrimitive);
        primitives->push_back(circlePrimitive);
      }
      else if (tagName == "ellipse") {
        srm::primitive_t* ellipsePrimitive = new srm::primitive_t();
        _ellipseToPrimitive(tag, ellipsePrimitive);
        primitives->push_back(ellipsePrimitive);
      }
      else if (tagName == "line") {
        srm::primitive_t* linePrimitive = new srm::primitive_t();
        _lineToPrimitive(tag, linePrimitive);
        primitives->push_back(linePrimitive);
      }
      else if (tagName == "polyline") {
        srm::primitive_t* polylinePrimitive = new srm::primitive_t();
        _polylineToPrimitive(tag, polylinePrimitive);
        primitives->push_back(polylinePrimitive);
      }
      else if (tagName == "polygon") {
        srm::primitive_t* polygonPrimitive = new srm::primitive_t();
        _polygonToPrimitive(tag, polygonPrimitive);
        primitives->push_back(polygonPrimitive);
      }
      else if (tagName == "text") {
        // TODO: realise text processing
      }
      else {
        continue;
      }
    }
    catch(std::exception error) {
      throw error;
    }
  }
}