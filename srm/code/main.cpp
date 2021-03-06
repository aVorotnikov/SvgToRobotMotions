/**
 * @file
 * @brief Main program source file
 * @authors Vorotnikov Andrey
 * @date 12.03.2021
 *
 * Contains main function to start program
 */

/**
 * @mainpage Introduction
 * @author Baev Daniil, Vorotnikov Andrey, Pavlov Ilya, Chevykalov Grigory
 *
 * Converter from svg image format to Kawasaki robot motions for robowizard.
 */

#include <iostream>
#include <srm.h>

/**
 * Main program function
 * @param[in] argC number of arguments in command line
 * @param[in] argV parsed arguments of command line
 * @return system code (0 if success)
 */
int main(int argC, char *argV[]) {
  if (argC != 3 && argC != 4) {
    std::cout << "Error: wrong params" << std::endl;
    return 0;
  }

  try {
    srm::translator_t *trans = srm::translator_t::GetPtr();

    if (argC == 4)
      trans->roboConf.LoadConf(argV[3]);
    trans->SetSvg(argV[1]);
    trans->GenCode(argV[2]);
  }
  catch (std::exception &e) {
    std::cout << "Error: " << e.what() << std::endl;
    return 0;
  }

  std::cout << "Success" << std::endl;
 
  return 0;
}
