/**
 * @file
 * @brief Robot configuration header file
 * @authors Vorotnikov Andrey
 * @date 19.03.2021
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
      vel,                    ///< robot velocity
      dist,                   ///< distance of departure
      accuracy,               ///< robot accuracy
      pouringStep;            ///< step for pouring
    std::string programName;  ///< name of robot program

  public:
    /**
     * Load robot configuration from file function.
     * @param[in] confFileName robot configuration file name
     */
    void LoadConf(const std::string &confFileName);

    /**
    * Get robot velocity function.
    * @return distance of departure
    */
    double GetVelocity(void) const noexcept;

    /**
     * Get departure distance function.
     * @return distance of departure
     */
    double GetDepDist(void) const noexcept;

    /**
     * Get accuracy in robot cs function.
     * @return robot accuracy
     */
    double GetRoboAcc(void) const noexcept;

    /**
     * Get accuracy in svg cs function.
     * @return svg accuracy
     */
    double GetSvgAcc(void) const noexcept;

    /**
     * Get pouring step value function.
     * @return pouring step
     */
    double GetPouringStep(void) const noexcept;

    /**
     * Get program name function.
     * @return string wirh program name
     */
    std::string GetProgramName(void) const noexcept;
  };
}

#endif /* __ROBOT_CONF_H_INCLUDED */

