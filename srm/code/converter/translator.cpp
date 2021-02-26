/**
 * @file
 * @brief Translator class source file
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 24.02.2021
 *
 * Contains main converter class realisatiion
 */

#include <srm.h>
#include <fstream>

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
 * Gen robot code from created tag tree
 * @param[in] codeFileName code file name
 * @see SetSvg
 */
void srm::translator_t::GenCode(const std::string &codeFileName) const {
}

/**
 * Translator class destructor
 */
srm::translator_t::~translator_t(void) noexcept{
  delete[] xmlString;
}
