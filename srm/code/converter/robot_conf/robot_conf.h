/**
 * @file
 * @brief Robot configuration header file
 * @authors Vorotnikov Andrey
 * @date 11.03.2021
 *
 * Contains robot_conf_t class definition to load and save robot configuration
 */

#pragma once

#ifndef __ROBOT_CONF_H_INCLUDED
#define __ROBOT_CONF_H_INCLUDED

#include "../defs.h"
#include "../robot_conf/cs/cs.h"

/** \brief Project namespace */
namespace srm {
  /**
   * @brief Robot configuration representation class
   *
   * Class to load and save robot configuration
   */
  class robot_conf_t : public cs_t {
  private:
    double
      dist,         ///< distance of departure
      accuracy,     ///< robot accuracy
      pouringStep;  ///< step for pouring

  public:
    /**
     * Load robot configuration from file function.
     * @param[in] confFileName robot configuration file name
     */
    void LoadConf(const std::string &confFileName);

    /**
     * Get departure distance function.
     * @return distance of departure
     */
    double GetDepDist(void);

    /**
     * Get robot accuracy function.
     * @return robot accuracy
     */
    double GetRoboAcc(void);

    /**
     * Get pouring step value function.
     * @return pouring step
     */
    double GetPouringStep(void);
  };
}

#endif /* __ROBOT_CONF_H_INCLUDED */

