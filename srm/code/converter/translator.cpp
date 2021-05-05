/**
 * @file
 * @brief Translator class source file
 * @authors Vorotnikov Andrey, Pavlov Ilya, Chevykalov Grigory
 * @date 05.05.2021
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
 * Set svg image for converting. Check file and create tag tree.
 * @param[in] svgFileName path to file with svg image
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
static void _getTags(rapidxml::xml_node<> *node, std::list<srm::tag_t *> *tags, unsigned int lvl) noexcept {
  std::string nodeName;
  while (node) {
    srm::tag_t *tag = new srm::tag_t(node);
    nodeName.assign(node->name());
    if (nodeName == "g" || nodeName == "svg") {
      tag->level = lvl + 1;
      tags->push_back(tag);
      _getTags(node->first_node(), tags, lvl + 1);
    }
    else {
      tag->level = lvl;
      tags->push_back(tag);
      _getTags(node->first_node(), tags, lvl);
    }
    node = node->next_sibling();
  }
}

/**
 * Gen robot code from created tag tree
 * @param[in] codeFileName code file name
 * @see SetSvg
 */
void srm::translator_t::GenCode(const std::string &codeFileName) const {
  if (!xmlTree.first_node())
    throw std::exception("Svg file is not set or empty");
  std::list<srm::tag_t *> tags;
  _getTags(xmlTree.first_node(), &tags, 0);
  
  std::list<srm::primitive_t *> primitives;
  srm::TagsToPrimitives(tags, &primitives);
  srm::SplitPrimitives(&primitives);

  for (auto tag : tags)
    delete tag;

  std::ofstream fout(codeFileName);
  if (!fout.is_open()) {
    for (auto primitive : primitives)
      delete primitive;
    throw std::exception("Failed to open or create output file");
  }

  fout << ".PROGRAM " << roboConf.GetProgramName()  << "()" << std::endl;
  fout << "\tHERE .#start" << std::endl;
  fout << "\tSPEED " << roboConf.GetVelocity() << " MM/S ALWAYS" << std::endl;
  fout << "\tACCURACY " << roboConf.GetRoboAcc() << std::endl;
  fout << "\tCP off" << std::endl;
  fout << "\tPOINT frm = FRAME(p1, p2, p3, p1)" << std::endl;

  for (auto primitive : primitives) {
    fout << *primitive << ";\n";
    if (primitive->fill)
      FillPrimitive(fout, *primitive);
  }

  fout << "\tJMOVE .#start" << std::endl;
  fout << ".END";


  for (auto primitive : primitives)
    delete primitive;
}

/**
 * Translator class destructor
 */
srm::translator_t::~translator_t(void) noexcept{
  delete[] xmlString;
}
