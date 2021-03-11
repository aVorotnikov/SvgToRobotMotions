/**
 * @file
 * @brief source file for tagsToPrimitives functions
 * @authors Pavlov Ilya
 * @date 11.03.2021
 *
 * Contains tagsToPrimitives realisation and support static functions for each of tags
 */

#include <srm.h>

/**
 * Transform svg line to primitive
 * @param[in] tag pointer to line node in xml DOM
 * @param[out] linePrimitive the primitive representations of line
 */
static void _lineToPrimitive(const rapidxml::xml_node<>* tag, srm::primitive_t* linePrimitive) {
  double x1, y1, x2, y2;
  try {
    x1 = std::stod(tag->last_attribute("x1")->value(), NULL);
    y1 = std::stod(tag->last_attribute("y1")->value(), NULL);
    x2 = std::stod(tag->last_attribute("x2")->value(), NULL);
    y2 = std::stod(tag->last_attribute("y2")->value(), NULL);
  }
  catch (std::exception error) {
    throw std::exception("Invalid attribute value in tag \"line\"");
  }
  srm::motion::segment_t* p = new srm::motion::segment_t(x2, y2);
  linePrimitive->start.x = x1;
  linePrimitive->start.y = y1;
  linePrimitive->push_back(p);
}

/**
 * Transform svg circle to primitive
 * @param[in] tag pointer to circle node in xml DOM
 * @param[out] circlePrimitive the primitive representations of circle
 */
static void _circleToPrimitive(const rapidxml::xml_node<>* tag, srm::primitive_t* circlePrimitive) {
  double cx, cy, r;
  try {
    cx = std::stod(tag->last_attribute("cx")->value(), NULL);
    cy = std::stod(tag->last_attribute("cy")->value(), NULL);
    r = std::stod(tag->last_attribute("r")->value(), NULL);
  }
  catch (std::exception error) {
    throw std::exception("Invalid attribute value in tag \"circle\"");
  }

  srm::motion::arc_t* arc = new srm::motion::arc_t(cx, cy + r, cx, cy - r);
  circlePrimitive->start.x = cx;
  circlePrimitive->start.y = cy - r;
  circlePrimitive->push_back(arc);
}

/**
 * Transform svg rectangle to primitive
 * @param[in] tag pointer to rectangle node in xml DOM
 * @param[out] rectanglePrimitive the primitive representations of rectangle
 */
static void _rectToPrimitive(const rapidxml::xml_node<>* tag, srm::primitive_t* rectanglePrimitive) {
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
  srm::motion::segment_t* p1 = new srm::motion::segment_t(x + width, y);
  srm::motion::segment_t* p2 = new srm::motion::segment_t(x + width, y + height);
  srm::motion::segment_t* p3 = new srm::motion::segment_t(x, y + height);
  srm::motion::segment_t* p4 = new srm::motion::segment_t(x, y);
  rectanglePrimitive->start.x = x;
  rectanglePrimitive->start.y = y;
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
void TagsToPrimitives(const std::list<rapidxml::xml_node<>*>& tags, std::list<srm::primitive_t*>* primitives) {
  std::string tagName;
  for (auto tag : tags) {
    tagName.assign(tag->name(), tag->name_size());
    if (tagName == "svg") {
      // TODO: get width and height for coordinate system
    }
    if (tagName == "path") {
      srm::path_t path(primitives);
      path.ParsePath(tag);
    }
    else if (tagName == "rect") {
      srm::primitive_t* rectanglePrimitive = new srm::primitive_t();
      try {
        _rectToPrimitive(tag, rectanglePrimitive);
      }
      catch (std::exception e) {
        throw e;
      }
      primitives->push_back(rectanglePrimitive);
    }
    else if (tagName == "circle") {
      srm::primitive_t* circlePrimitive = new srm::primitive_t();
      try {
        _circleToPrimitive(tag, circlePrimitive);
      }
      catch (std::exception e) {
        throw e;
      }
      primitives->push_back(circlePrimitive);
    }
    else if (tagName == "ellipse") {
      // TODO: realise ellipse parsing
    }
    else if (tagName == "line") {
      // TODO: realise line parsing
    }
    else if (tagName == "polyline") {
      // TODO: realise polyline parsing
    }
    else if (tagName == "polygon") {
      // TODO: realise polygon parsing
    }
    else if (tagName == "text") {
      // TODO: realise text processing
    }
    else {
      continue;
    }
  }
}