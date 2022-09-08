#ifndef HIF_HPP
#define HIF_HPP

#include <algorithm>
#include <fstream>
#include <functional>
#include <iomanip>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>

// For GNU 8+
// #include <filesystem>
// For GNU 7
#include <experimental/filesystem>

#include "El.hpp"
#include "HIF/config.h"
#include "metis.h"
extern "C" {
#include "metislib.h"
}

#include "HIF/core/environment.hpp"
#include "HIF/graphpartition.hpp"
#include "HIF/hifgraph.hpp"
#include "HIF/setoperation.hpp"
#include "HIF/tools.hpp"

// For GNU 8+
// namespace fs = std::filesystem;
// For GNU 7
namespace fs = std::experimental::filesystem;

#endif // ifndef HIF_HPP
