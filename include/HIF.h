#ifndef HIF_H
#define HIF_H

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

// For GNU 7
#include <experimental/filesystem>

#include "El.hpp"
#include "HIF/config.h"
#include "metis.h"

// There is an error about extern "C" here, but I don't know how to fix it.
extern "C" {
    #include "metislib.h"
}

#include "HIF/core/environment.hpp"
#include "HIF/graphpartition.h"
#include "HIF/hifgraph.hpp"
#include "HIF/setoperation.hpp"
#include "HIF/tools.hpp"

// For GNU 7
namespace fs = std::experimental::filesystem;

#endif // ifndef HIF_H
