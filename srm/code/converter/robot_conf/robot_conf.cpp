/**
 * @file
 * @brief Robot configuration source file
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 13.03.2021
 *
 * Contains robot_conf_t class realisation to load and save robot configuration
 */

#include <srm.h>
#include <functional>
#include <map>
#include <fstream>
#include <sstream>
#include <iterator>
#include <iomanip>

/** \brief Project namespace */
namespace srm {
  /**
   * @brief Robot configuration file parameter saving struct
   *
   * Struct to save parsed lines in robot configuration file
   */
  struct robot_file_t {
    std::pair<bool, srm::vec3_t>
      p1,           ///< main board point
      p2,           ///< ort i board point
      p3;           ///< ort j board point
    std::pair<bool, double>
      dist,         ///< distance of departure
      accuracy,     ///< robot accuracy
      pouringStep;  ///< step for pouring
  };

  /**
   * @brief Robot configuration file parsing struct
   *
   * Struct to save possible lines in robot configuration file
   */
  struct line_t {
    std::function<void (robot_file_t *, const std::vector<double> &)> func;  ///< function to parse
    int numOfParams;                                                         ///< number of parametres
  };
}

/**
 * p1 command parser function
 * @param[out] rConf robot configuration file variable
 * @param[in] params line param
 */
static void _p1Func(srm::robot_file_t *rConf, const std::vector<double> &params) {
  rConf->p1.first = true;
  rConf->p1.second = srm::vec3_t(params[0], params[1], params[2]);
}

/**
 * p2 command parser function
 * @param[out] rConf robot configuration file variable
 * @param[in] params line param
 */
static void _p2Func(srm::robot_file_t *rConf, const std::vector<double> &params) {
  rConf->p2.first = true;
  rConf->p2.second = srm::vec3_t(params[0], params[1], params[2]);
}

/**
 * p3 command parser function
 * @param[out] rConf robot configuration file variable
 * @param[in] params line param
 */
static void _p3Func(srm::robot_file_t *rConf, const std::vector<double> &params) {
  rConf->p3.first = true;
  rConf->p3.second = srm::vec3_t(params[0], params[1], params[2]);
}

/**
 * dist command parser function
 * @param[out] rConf robot configuration file variable
 * @param[in] params line param
 */
static void _distFunc(srm::robot_file_t *rConf, const std::vector<double> &params) {
  rConf->dist.first = true;
  rConf->dist.second = params[0];
}

/**
 * accu command parser function
 * @param[out] rConf robot configuration file variable
 * @param[in] params line param
 */
static void _accuFunc(srm::robot_file_t *rConf, const std::vector<double> &params) {
  rConf->accuracy.first = true;
  rConf->accuracy.second = params[0];
}

/**
 * step command parser function
 * @param[out] rConf robot configuration file variable
 * @param[in] params line param
 */
static void _stepFunc(srm::robot_file_t *rConf, const std::vector<double> &params) {
  rConf->pouringStep.first = true;
  rConf->pouringStep.second = params[0];
}

static std::map<const std::string, srm::line_t> s_Lines = {
  {"p1", {_p1Func, 3}},
  {"p2", {_p2Func, 3}},
  {"p3", {_p3Func, 3}},
  {"dist", {_distFunc, 1}},
  {"accu", {_accuFunc, 1}},
  {"step", {_stepFunc, 1}}
};

/**
 * Load robot configuration from file function.
 * @param[in] confFileName robot configuration file name
 */
void srm::robot_conf_t::LoadConf(const std::string &confFileName) {
  std::ifstream iFStream(confFileName);

  int lineNum = 0;
  robot_file_t roboFile = {};

  while (!iFStream.eof()) {
    lineNum++;

    std::string line;
    std::getline(iFStream, line);

    std::vector<std::string> splitedLine;
    std::istringstream strStream(line);
    std::istream_iterator<std::string> iter(strStream), eos;
    std::copy(iter, eos, std::back_inserter(splitedLine));

    auto lineStruct = s_Lines.find(splitedLine[0]);
    if (lineStruct == s_Lines.end())
      continue;
    if (lineStruct->second.numOfParams != splitedLine.size() - 1)
      throw std::exception((std::string("Incorrect number of parameters in '") + splitedLine[0] + "' in line #" + std::to_string(lineNum)).c_str());
    std::vector<double> args(lineStruct->second.numOfParams);
    try {
      for (size_t i = 0; i < lineStruct->second.numOfParams; i++)
        args[i] = std::atof(splitedLine[i + 1].c_str());
    }
    catch (std::exception &) {
      throw std::exception((std::string("Incorrect argument in '") + splitedLine[0] + "' in line #" + std::to_string(lineNum)).c_str());
    }

    lineStruct->second.func(&roboFile, args);
  }
  if (!roboFile.p1.first || !roboFile.p2.first || !roboFile.p3.first ||
    !roboFile.dist.first || !roboFile.accuracy.first || !roboFile.pouringStep.first)
    throw std::exception("Not enough lines");
  SetPlane(roboFile.p1.second, roboFile.p2.second, roboFile.p3.second);
  dist = roboFile.dist.second;
  accuracy = roboFile.accuracy.second;
  pouringStep = roboFile.pouringStep.second;
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
 * Get accuracy in svg cs function.
 * @return svg accuracy
 */
double srm::robot_conf_t::GetSvgAcc(void) {
  return SvgToRobotAcc(accuracy);
}

/**
 * Get pouring step value function.
 * @return pouring step
 */
double srm::robot_conf_t::GetPouringStep(void) {
  return pouringStep;
}
