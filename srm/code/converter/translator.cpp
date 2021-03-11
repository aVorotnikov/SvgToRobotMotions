/**
 * @file
 * @brief Translator class source file
 * @authors Vorotnikov Andrey, Pavlov Ilya, Chevykalov Grigory
 * @date 10.03.2021
 *
 * Contains main converter class realisatiion
 */

#include <srm.h>
#include <fstream>
#include <list>
#include <iostream>

/**
 * Set svg image to convert function function. Check file abd create tag tree
 * @param[in] svgFileName svg image file name
 */
void srm::translator_t::SetSvg(const std::string &svgFileName) {
  std::ifstream fin(svgFileName);
  if (!fin.is_open()) {
    throw std::exception("Failed to open intput file");
  }

  std::string buf;
  std::string line;
  while (std::getline(fin, line)) {
    buf += line;
  }

  // this char string have to be in the heap during rapidxml works
  xmlString = new char[buf.length() + 1];

  strcpy_s(xmlString, (buf.length() + 1) * sizeof(char), buf.c_str());

  try {
    xmlTree.parse<rapidxml::parse_full>(xmlString);
  }
  catch (rapidxml::parse_error err) {
    throw std::exception("Error in the svg file");
  }
}

/**
 * Collect all tags from DOM to list
 * @param[in] node starting node in the xml DOM
 * @param[out] tags pointer to list of pointers to tags
 * @see GenCode
 */
static void _getTags(rapidxml::xml_node<> *node, std::list<rapidxml::xml_node<> *> *tags) noexcept {
  while (node) {
    _getTags(node->first_node(), tags);
    tags->push_back(node);
    node = node->next_sibling();
  }
}

/**
 * Transform svg rectangle to primitive
 * @param[in] tag pointer to rectangle node in xml DOM
 * @param[out] rectanglePrimitive the primitive representations of rectangle
 */
static void _rectToPrimitive(const rapidxml::xml_node<>* tag, srm::primitive_t *rectanglePrimitive) noexcept {
  // Get attributes
      // (x, y) is left top point
  double x = strtod(tag->last_attribute("x")->value(), NULL);
  double y = strtod(tag->last_attribute("y")->value(), NULL);
  double height = strtod(tag->last_attribute("height")->value(), NULL);
  double width = strtod(tag->last_attribute("width")->value(), NULL);
  double rx = 0;
  double ry = 0;

  if (tag->last_attribute("rx")) {
    rx = strtod(tag->last_attribute("rx")->value(), NULL);
  }
  if (tag->last_attribute("ry")) {
    ry = strtod(tag->last_attribute("ry")->value(), NULL);
  }

  // Transform to primitive
  srm::motion::segment_t *p1 = new srm::motion::segment_t(x + width, y);
  srm::motion::segment_t *p2 = new srm::motion::segment_t(x + width, y + height);
  srm::motion::segment_t *p3 = new srm::motion::segment_t(x, y + height);
  srm::motion::segment_t *p4 = new srm::motion::segment_t(x, y);
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
static void _tagsToPrimitives(const std::list<rapidxml::xml_node<> *> &tags, std::list<srm::primitive_t *> *primitives) noexcept {
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
      srm::primitive_t *rectanglePrimitive = new srm::primitive_t();
      _rectToPrimitive(tag, rectanglePrimitive);
      primitives->push_back(rectanglePrimitive);
    }
    else if (tagName == "circle") {
      // TODO: realise circle parsing
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

/**
 * Gen robot code from created tag tree
 * @param[in] codeFileName code file name
 * @see SetSvg
 */
void srm::translator_t::GenCode(const std::string &codeFileName) const {
  if (!xmlTree.first_node()) {
    throw std::exception("Svg file is not set or empty");
  }
  std::list<rapidxml::xml_node<> *> tags;
  _getTags(xmlTree.first_node(), &tags);
  
  std::list<srm::primitive_t *> primitives;
  _tagsToPrimitives(tags, &primitives);
  
  vec3_t p1(0, 0, 1), p2(1, 0, 1), p3(0, 0, 0); // ! only for check base_t.GenCode !
  cs_t cs(100, 100, p1, p2, p3); // ! only for check base_t.GenCode !

  std::ofstream fout(codeFileName);
  if (!fout.is_open()) {
    for (auto primitive : primitives) {
      delete primitive;
    }
    throw std::exception("Failed to open or create output file");
  }

  // NOT REAL PROGRAM, JUST TEST
  fout << ".PROGRAM " << codeFileName  << "()" <<std::endl;
  
  for (auto primitive : primitives) {
    fout << *primitive << ";\n";
  }
  fout << ".END";

  for (auto primitive : primitives) {
    delete primitive;
  }
}

/**
 * Translator class destructor
 */
srm::translator_t::~translator_t(void) noexcept{
  delete[] xmlString;
}
