/**
 * @file
 * @brief source file for tagsToPrimitives functions
 * @authors Vorotnikov Andrey, Pavlov Ilya, Chevykalov Grigory
 * @date 17.04.2021
 *
 * Contains tagsToPrimitives realisation and support static functions for each of tags
 */

#include <srm.h>

#include <cmath>
#include <sstream>

/**
 * Transform svg polyline to primitive
 * @param[in] tag pointer to polyline node in xml DOM
 * @param[out] polylinePrimitive the primitive representations of polyline
 */
static void _polylineToPrimitive(const rapidxml::xml_node<> *tag, srm::primitive_t *polylinePrimitive) noexcept {
  srm::translator_t *trans = srm::translator_t::GetPtr();
  
  if (!tag->last_attribute("points")) {
    trans->WriteLog("Warning: attribute points in polyline is not set");
    return;
  }

  std::string points(tag->last_attribute("points")->value());
  for (auto &sym : points) {
    if (!isdigit(sym) && sym != ',' && sym != ' ') {
      trans->WriteLog("Warning: invalid symbol in attribute points in polyline");
      return;
    }
    if (sym == ',') {
      sym = ' ';
    }
  }
  std::istringstream iss(points);
  double x, y;
  iss >> x >> y;
  if (iss.fail()) {
    trans->WriteLog("Warning: wrong start point in attribute points in polyline");
    return;
  }
  polylinePrimitive->start.x = x;
  polylinePrimitive->start.y = y;
  while (!iss.eof()) {
    iss >> x >> y;
    if (!iss.fail()) {
      polylinePrimitive->push_back(srm::segment_t(x, y));
    }
    else {
      std::string check;
      iss >> check;
      if (check != "") {
        trans->WriteLog("Warning: wrong number of points in attribute points in polyline");
        return;
      }
    }
  }
}

/**
 * Transform svg polygon to primitive
 * @param[in] tag pointer to polygon node in xml DOM
 * @param[out] polygonPrimitive the primitive representations of polygon
 */
static void _polygonToPrimitive(const rapidxml::xml_node<> *tag, srm::primitive_t *polygonPrimitive) noexcept {
  srm::translator_t* trans = srm::translator_t::GetPtr();

  if (!tag->last_attribute("points")) {
    trans->WriteLog("Warning: attribute points in polygon is not set");
    return;
  }

  std::string points(tag->last_attribute("points")->value());
  for (auto& sym : points) {
    if (!isdigit(sym) && sym != ',' && sym != ' ') {
      trans->WriteLog("Warning: invalid symbol in attribute points in polygon");
      return;
    }
    if (sym == ',') {
      sym = ' ';
    }
  }
  std::istringstream iss(points);
  double x, y;
  iss >> x >> y;
  if (iss.fail()) {
    trans->WriteLog("Warning: wrong start point in attribute points in polygon");
    return;
  }
  polygonPrimitive->start.x = x;
  polygonPrimitive->start.y = y;
  while (!iss.eof()) {
    iss >> x >> y;
    if (!iss.fail()) {
      polygonPrimitive->push_back(srm::segment_t(x, y));
    }
    else {
      std::string check;
      iss >> check;
      if (check != "") {
        trans->WriteLog("Warning: wrong number of points in attribute points in polygon");
        return;
      }
    }
  }
  polygonPrimitive->push_back(srm::segment_t(polygonPrimitive->start.x, polygonPrimitive->start.y));
}

 /**
  * Transform svg ellipse to primitive
  * @param[in] tag pointer to ellipse node in xml DOM
  * @param[out] ellipsePrimitive the primitive representations of ellipse
  */
static void _ellipseToPrimitive(const rapidxml::xml_node<> *tag, srm::primitive_t *ellipsePrimitive) noexcept {
  double cx, cy, rx, ry;
  const double defaultCoord = 0;
  srm::translator_t *trans = srm::translator_t::GetPtr();

  if (tag->last_attribute("cx")) {
    try {
      cx = std::stod(tag->last_attribute("cx")->value(), NULL);
      if (std::string(tag->last_attribute("cx")->value()).find("%") != std::string::npos) {
        cx = trans->roboConf.GetW() * cx / 100;
      }
    }
    catch (std::exception) {
      cx = defaultCoord;
      trans->WriteLog("Warning: invalid attribute cx in ellipse");
    }
  }
  else {
    cx = defaultCoord;
    trans->WriteLog("Warning: attribute cx is not set in ellipse");
  }

  if (tag->last_attribute("cy")) {
    try {
      cy = std::stod(tag->last_attribute("cy")->value(), NULL);
      if (std::string(tag->last_attribute("cy")->value()).find("%") != std::string::npos) {
        cy = trans->roboConf.GetH() * cy / 100;
      }
    }
    catch (std::exception) {
      cy = defaultCoord;
      trans->WriteLog("Warning: invalid attribute cy in ellipse");
    }
  }
  else {
    cy = defaultCoord;
    trans->WriteLog("Warning: attribute cy is not set in ellipse");
  }

  if (tag->last_attribute("rx")) {
    try {
      rx = std::stod(tag->last_attribute("rx")->value(), NULL);
      if (std::string(tag->last_attribute("rx")->value()).find("%") != std::string::npos) {
        rx = trans->roboConf.GetW() * rx / 100;
      }
    }
    catch (std::exception) {
      ry = -1;
    }
  }
  else {
    rx = -1;
  }
  
  if (tag->last_attribute("ry")) {
    try {
      ry = std::stod(tag->last_attribute("ry")->value(), NULL);
      if (std::string(tag->last_attribute("ry")->value()).find("%") != std::string::npos) {
        ry = trans->roboConf.GetH() * ry / 100;
      }
    }
    catch (std::exception) {
      ry = -1;
    }
  }
  else {
    ry = -1;
  }

  if (rx > 0 && ry <= 0) {
    trans->WriteLog("Warning: invalid attribute ry in ellipse");
  }
  else if (rx <= 0 && ry > 0) {
    trans->WriteLog("Warning: invalid attribute rx in ellipse");
  }
  else if (rx <= 0 && ry <= 0) {
    trans->WriteLog("Warning: invalid attributes rx and ry in ellipse");
    return;
  }
 
  std::vector<srm::vec_t> discreteEllipse =
    srm::EllipseSampling(srm::vec_t(cx, cy), srm::vec_t(rx, ry), trans->roboConf.GetSvgAcc());
  
  ellipsePrimitive->start.x = discreteEllipse[0].x;
  ellipsePrimitive->start.y = discreteEllipse[0].y;
  ellipsePrimitive->reserve(discreteEllipse.size() - 1);
  for (unsigned int i = 1; i < discreteEllipse.size(); ++i) {
    ellipsePrimitive->push_back(srm::segment_t(discreteEllipse[i].x, discreteEllipse[i].y));
  }
}

/**
 * Transform svg line to primitive
 * @param[in] tag pointer to line node in xml DOM
 * @param[out] linePrimitive the primitive representations of line
 */
static void _lineToPrimitive(const rapidxml::xml_node<> *tag, srm::primitive_t *linePrimitive) noexcept {
  double x1, y1, x2, y2;
  const double defaultCoord = 0;
  srm::translator_t *trans = srm::translator_t::GetPtr();
 
  if (tag->last_attribute("x1")) {
    try {
      x1 = std::stod(tag->last_attribute("x1")->value(), NULL);
      if (std::string(tag->last_attribute("x1")->value()).find("%") != std::string::npos) {
        x1 = trans->roboConf.GetW() * x1 / 100;
      }
    }
    catch (std::exception) {
      x1 = defaultCoord;
      trans->WriteLog("Warning: invalid attribute x1 in line");
    }
  }
  else {
    x1 = defaultCoord;
    trans->WriteLog("Warning: attribute x1 in line is not set");
  }

  if (tag->last_attribute("x2")) {
    try {
      x2 = std::stod(tag->last_attribute("x2")->value(), NULL);
      if (std::string(tag->last_attribute("x2")->value()).find("%") != std::string::npos) {
        x2 = trans->roboConf.GetW() * x2 / 100;
      }
    }
    catch (std::exception) {
      x2 = defaultCoord;
      trans->WriteLog("Warning: invalid attribute x2 in line");
    }
  }
  else {
    x2 = defaultCoord;
    trans->WriteLog("Warning: attribute x2 in line is not set");
  }

  if (tag->last_attribute("y1")) {
    try {
      y1 = std::stod(tag->last_attribute("y1")->value(), NULL);
      if (std::string(tag->last_attribute("y1")->value()).find("%") != std::string::npos) {
        y1 = trans->roboConf.GetH() * y1 / 100;
      }
    }
    catch (std::exception) {
      y1 = defaultCoord;
      trans->WriteLog("Warning: invalid attribute y1 in line");
    }
  }
  else {
    y1 = defaultCoord;
    trans->WriteLog("Warning: attribute y1 in line is not set");
  }

  if (tag->last_attribute("y2")) {
    try {
      y2 = std::stod(tag->last_attribute("y2")->value(), NULL);
      if (std::string(tag->last_attribute("y2")->value()).find("%") != std::string::npos) {
        y2 = trans->roboConf.GetH() * y2 / 100;
      }
    }
    catch (std::exception) {
      y2 = defaultCoord;
      trans->WriteLog("Warning: invalid attribute y2 in line");
    }
  }
  else {
    y2 = defaultCoord;
    trans->WriteLog("Warning: attribute y2 in line is not set");
  }

  if (x1 == x2 && y1 == y2)
    return;

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
static void _circleToPrimitive(const rapidxml::xml_node<> *tag, srm::primitive_t *circlePrimitive) noexcept{
  double cx, cy, r;
  const double defaultCoord = 0;
  srm::translator_t *trans = srm::translator_t::GetPtr();

  if (tag->last_attribute("cx")) {
    try {
      cx = std::stod(tag->last_attribute("cx")->value(), NULL);
      if (std::string(tag->last_attribute("cx")->value()).find("%") != std::string::npos) {
        cx = trans->roboConf.GetW() * cx / 100;
      }
    }
    catch (std::exception) {
      cx = defaultCoord;
      trans->WriteLog("Warning: invalid attribute cx in circle");
    }
  }
  else {
    cx = defaultCoord;
    trans->WriteLog("Warning: attribute cx is not set in circle");
  }

  if (tag->last_attribute("cy")) {
    try {
      cy = std::stod(tag->last_attribute("cy")->value(), NULL);
      if (std::string(tag->last_attribute("cy")->value()).find("%") != std::string::npos) {
        cy = trans->roboConf.GetH() * cy / 100;
      }
    }
    catch (std::exception) {
      cy = defaultCoord;
      trans->WriteLog("Warning: invalid attribute cy in circle");
    }
  }
  else {
    cy = defaultCoord;
    trans->WriteLog("Warning: attribute cy is not set in circle");
  }

  if (tag->last_attribute("r")) {
    try {
      r = std::stod(tag->last_attribute("r")->value(), NULL);
      if (r <= 0) {
        trans->WriteLog("Warning: attribute r in circle must be more than 0");
        return;
      }
      if (std::string(tag->last_attribute("r")->value()).find("%") != std::string::npos) {
        r = trans->roboConf.GetW() * r / 100;
      }
    }
    catch (std::exception) {
      trans->WriteLog("Warning: invalid attribute r in circle");
      return;
    }
  }
  else {
    trans->WriteLog("Warning: attribute r is not set in circle");
    return;
  }

  std::vector<srm::vec_t> discreteCircle =
    srm::EllipseSampling(srm::vec_t(cx, cy), srm::vec_t(r, r), trans->roboConf.GetSvgAcc());

  circlePrimitive->start.x = discreteCircle[0].x;
  circlePrimitive->start.y = discreteCircle[0].y;
  circlePrimitive->reserve(discreteCircle.size() - 1);
  for (unsigned int i = 1; i < discreteCircle.size(); ++i) {
    circlePrimitive->push_back(srm::segment_t(discreteCircle[i].x, discreteCircle[i].y));
  }
}

/**
 * Transform svg rectangle to primitive
 * @param[in] tag pointer to rectangle node in xml DOM
 * @param[out] rectanglePrimitive the primitive representations of rectangle
 */
static void _rectToPrimitive(const rapidxml::xml_node<> *tag, srm::primitive_t *rectanglePrimitive) noexcept {
  double x, y, height, width;
  const double defaultX = 0, defaultY = 0;
  srm::translator_t *trans = srm::translator_t::GetPtr();

  if (tag->last_attribute("x")) {
    try {
      x = std::stod(tag->last_attribute("x")->value(), NULL);
      if (std::string(tag->last_attribute("x")->value()).find("%") != std::string::npos) {
        x = trans->roboConf.GetW() * x / 100;
      }
    }
    catch (std::exception) {
      trans->WriteLog("Warning: invalid attribute x in rect");
      x = defaultX;
    }
  }
  else {
    trans->WriteLog("Warning: attribute x in rect is not set");
    x = defaultX;
  }

  if (tag->last_attribute("y")) {
    try {
      y = std::stod(tag->last_attribute("y")->value(), NULL);
      if (std::string(tag->last_attribute("y")->value()).find("%") != std::string::npos) {
        y = trans->roboConf.GetH() * y / 100;
      }
    }
    catch (std::exception) {
      trans->WriteLog("Warning: invalid attribute y in rect");
      y = defaultY;
    }
  }
  else {
    trans->WriteLog("Warning: attribute y in rect is not set");
    y = defaultY;
  }

  if (tag->last_attribute("height")) {
    height = strtod(tag->last_attribute("height")->value(), NULL);
    if (height <= 0) {
      trans->WriteLog("Warning: attribute height in rect must be more than 0");
      return;
    }
    if (std::string(tag->last_attribute("height")->value()).find("%") != std::string::npos) {
      height = trans->roboConf.GetH() * height / 100;
    }
  }
  else {
    trans->WriteLog("Warning: attribute height in rect is not set");
    return;
  }

  if (tag->last_attribute("width")) {
    width = std::stod(tag->last_attribute("width")->value(), NULL);
    if (width <= 0) {
      trans->WriteLog("Warning: attribute widtht in rect is less or equal to zero");
      return;
    }
    if (std::string(tag->last_attribute("width")->value()).find("%") != std::string::npos) {
      width = trans->roboConf.GetW() * width / 100;
    }
  }
  else {
    trans->WriteLog("Warning: attribute width in rect is not set");
    return;
  }

  double rx = 0;
  double ry = 0;

  if (tag->last_attribute("rx")) {
    try { 
      rx = std::stod(tag->last_attribute("rx")->value(), NULL); 
      if (std::string(tag->last_attribute("rx")->value()).find("%") != std::string::npos) {
        rx = trans->roboConf.GetW() * rx / 100;
      }
    }
    catch (std::exception) {
      rx = -1;
    }
  }
  if (tag->last_attribute("ry")) {
    try {
      ry = std::stod(tag->last_attribute("ry")->value(), NULL);
      if (std::string(tag->last_attribute("ry")->value()).find("%") != std::string::npos) {
        ry = trans->roboConf.GetH() * ry / 100;
      }
    }
    catch (std::exception) {
      ry = -1;
    }
  }
  if (rx >= 0 && ry < 0) {
    trans->WriteLog("Warning: wrong attribute ry in rect");
    ry = rx;
  }
  else if (rx < 0 && ry >= 0) {
    trans->WriteLog("Warning: wrong attribute rx in rect");
    rx = ry;
  }
  else if (rx < 0 && ry < 0) {
    trans->WriteLog("Warning: wrong attributes rx and ry in rect");
    rx = ry = 0;
  }

  if (rx > width / 2) {
    rx = width / 2;
    trans->WriteLog("attribute rx in rect is more than half of width");
  }
  if (ry > height / 2) {
    rx = height/ 2;
    trans->WriteLog("attribute ry in rect is more than half of height");
  }

  // TODO: realise processing rx and ry attributes in rect

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
 * Process and save svg width and height
 * @param[in] tag pointer to rectangle node in xml DOM
 */
static void _processSvgParams(const rapidxml::xml_node<> *tag) noexcept {
  const double defaultWidth = 300, defaultHeight = 150;
  double width, height;
  if (tag->last_attribute("width")) {
    width = strtod(tag->last_attribute("width")->value(), NULL);
    if (width <= 0) {
      width = defaultWidth;
      srm::translator_t::GetPtr()->WriteLog("Warning: svg width must be more than 0");
    }
  }
  else {
    width = defaultWidth;
    srm::translator_t::GetPtr()->WriteLog("Warning: svg width is not set");
  }
  if (tag->last_attribute("height")) {
    height = strtod(tag->last_attribute("height")->value(), NULL);
    if (height <= 0) {
      height = defaultHeight;
      srm::translator_t::GetPtr()->WriteLog("Warning: svg height must be more than 0");
    }
  }
  else {
    height = defaultHeight;
    srm::translator_t::GetPtr()->WriteLog("Warning: svg height is not set");
  }
  srm::translator_t::GetPtr()->roboConf.SetWH(width, height);
}

/**
 * Transform svg tags to primitives
 * @param[in] tags the list of tags in DOM
 * @param[out] primitives the list of primitive representations of tags
 */
void srm::TagsToPrimitives(const std::list<srm::tag_t *> &tags, std::list<srm::primitive_t*> *primitives) noexcept {
  std::string tagName;
  std::list<transform_t> transformations;
  transform_t transformCompos; ///< composition of all transformations
  unsigned prevLevel = 0; ///< previoust level in svg tree

  for (auto tag : tags) {
    tagName.assign(tag->node->name(), tag->node->name_size());

    if (tag->level < prevLevel) {
      for (unsigned i = prevLevel; i > tag->level; --i)
        transformations.pop_back();
      
      transformCompos.Clear();
      for (const auto& transform : transformations)
        transformCompos *= transform;

      prevLevel = tag->level;
    }

    if (tagName == "g" && tag->level == prevLevel) {
      transformations.pop_back();
      transformCompos.Clear();
      for (const auto& transform : transformations)
        transformCompos *= transform;
    }

    if (tagName == "svg") {
      _processSvgParams(tag->node);
      transform_t transform;
      auto attr = tag->node->first_attribute("transform");      
      while (attr) {
        transform *= transform_t(attr->value());
        attr = attr->next_attribute("transform");
      }
      transformations.push_back(transform);

      if (tag->node->first_attribute("transform")) {
        transformCompos.Clear();
        for (const auto& transform : transformations)
          transformCompos *= transform;
      }

      prevLevel = tag->level;
    }
    else if (tagName == "g") {
      transform_t transform;
      auto attr = tag->node->first_attribute("transform");
      while (attr) {
        transform *= transform_t(attr->value());
        attr = attr->next_attribute("transform");
      }

      transformations.push_back(transform);

      if (tag->node->first_attribute("transform")) {
        transformCompos.Clear();
        for (const auto& transform : transformations)
          transformCompos *= transform;
      }
      
      prevLevel = tag->level;
    }
    else if (tagName == "path") {
      srm::path_t path(primitives, transformCompos);
      path.ParsePath(tag->node);
    }
    else {
      srm::primitive_t *primitive = new srm::primitive_t();
      if (tagName == "rect") {
        _rectToPrimitive(tag->node, primitive);
      }
      else if (tagName == "circle") {
        _circleToPrimitive(tag->node, primitive);
      }
      else if (tagName == "ellipse") {
        _ellipseToPrimitive(tag->node, primitive);
      }
      else if (tagName == "line") {
        _lineToPrimitive(tag->node, primitive);
      }
      else if (tagName == "polyline") {
        _polylineToPrimitive(tag->node, primitive);
      }
      else if (tagName == "polygon") {
        _polygonToPrimitive(tag->node, primitive);
      }
      else if (tagName == "text") {
        // TODO: realise text processing
      }

      if (primitive->size() > 0) {
        transform_t transform;
        auto attr = tag->node->first_attribute("transform");
        while(attr) {
          transform *= transform_t(attr->value());
          attr = attr->next_attribute("transform");
        }
        if (tag->node->first_attribute("transform")) {
          transform.Apply(primitive);
        }
        transformCompos.Apply(primitive);

        primitive->fill = IsFill(tag->node);

        primitives->push_back(primitive);
      }
      else
        delete primitive;
    }
  }
}