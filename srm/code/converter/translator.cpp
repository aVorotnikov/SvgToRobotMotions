/**
 * @file
 * @brief Translator class source file
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 02.03.2021
 *
 * Contains main converter class realisatiion
 */

#include <srm.h>
#include <fstream>
#include <list>

/**
 * Set svg image to convert function function. Check file abd create tag tree
 * @param[in] svgFileName svg image file name
 */
void srm::translator_t::SetSvg(const std::string &svgFileName) {
  std::ifstream fin(svgFileName);
  if (!fin.is_open()) {
    throw std::exception("Open file error");
  }

  std::string buf;
  std::string line;
  while (std::getline(fin, line)) {
    buf += line;
  }

  // this char string have to be in the heap during rapidxml works
  char *xmlString = new char[buf.length() + 1];

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
 * Gen robot code from created tag tree
 * @param[in] codeFileName code file name
 * @see SetSvg, _getTags
 */
void srm::translator_t::GenCode(const std::string &codeFileName) const {
  std::list<primitive_t> primitives;
  
  std::list<rapidxml::xml_node<> *> tags;
  std::string tagName;
  _getTags(xmlTree.first_node(), &tags);

 
  for (auto tag : tags) {
    tagName.assign(tag->name(), tag->name_size());
    if (tagName == "svg") {
      // TODO: get width and height for coordinate system
    }
    if (tagName == "path") {
      // TODO: realise path parsing
    }
    else if (tagName == "rect") {
      // (x, y) is left top point
      int x = atoi(tag->last_attribute("x")->value());
      int y = atoi(tag->last_attribute("y")->value());
      int height = atoi(tag->last_attribute("height")->value());
      int width = atoi(tag->last_attribute("width")->value());
      // TODO: realise rx and ry processing
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
 * Translator class destructor
 */
srm::translator_t::~translator_t(void) noexcept{
  delete[] xmlString;
}
