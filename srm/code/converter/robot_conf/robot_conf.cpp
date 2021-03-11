/**
 * @file
 * @brief Robot configuration source file
 * @authors Vorotnikov Andrey
 * @date 11.03.2021
 *
 * Contains robot_conf_t class realisation to load and save robot configuration
 */

#include <srm.h>

/**
 * Load robot configuration from file function.
 * @param[in] confFileName robot configuration file name
 */
void srm::robot_conf_t::LoadConf(const std::string &confFileName) {
}

/**
 * Get departure distance function.
 * @return distance of departure
 */
double srm::robot_conf_t::GetDepDist(void) {
  return dist;
}

/**
 * Get robot accuracy function.
 * @return robot accuracy
 */
double srm::robot_conf_t::GetRoboAcc(void) {
  return accuracy;
}

/**
 * Get pouring step value function.
 * @return pouring step
 */
double srm::robot_conf_t::GetPouringStep(void) {
  return pouringStep;
}
