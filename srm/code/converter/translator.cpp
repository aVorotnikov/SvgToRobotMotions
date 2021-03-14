/**
 * @file
 * @brief Translator class source file
 * @authors Vorotnikov Andrey, Pavlov Ilya, Chevykalov Grigory
 * @date 14.03.2021
 *
 * Contains main converter class realisatiion
 */

#include <srm.h>

#include <fstream>
#include <list>
#include <string>
#include <iostream>

srm::translator_t srm::translator_t::singleToneVar;  ///< tranlator singletone variable

/**
 * Private constructor for single tone
 */
srm::translator_t::translator_t(void) noexcept : logStream(&std::cout) {
}

/**
 * Get 'translator_t' class single tone variable
 * @return pointer to single tone variable
 */
srm::translator_t * srm::translator_t::GetPtr(void) noexcept {
  return &singleToneVar;
}

/**
 * Write string in log stream function
 * @param[in] str string to write
 */
void srm::translator_t::WriteLog(const std::string &str) noexcept {
  *logStream << str << std::endl;
}

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
    tags->push_back(node);
    _getTags(node->first_node(), tags);
    node = node->next_sibling();
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
  try {
    TagsToPrimitives(tags, &primitives);
  }
  catch (std::exception e) {
    throw e;
  }
  
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
  fout << ".PROGRAM " << roboConf.GetProgramName() << "()" <<std::endl;
  
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
