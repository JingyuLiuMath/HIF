#pragma once
#ifndef HIF_HPP
#define HIF_HPP

#include <algorithm>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <stack>
#include <functional>
#include <iomanip>

// For GNU 8+
// #include <filesystem>
// For GNU 7
#include <experimental/filesystem>


#include "HIF/config.h"
#include "El.hpp"
#include "metis.h"
extern "C"
{
	#include "metislib.h"
}

#include "HIF/core/environment.hpp"
#include "HIF/hifgraph.hpp"
#include "HIF/graphpartition.hpp"
#include "HIF/setoperation.hpp"
#include "HIF/tools.hpp"

// For GNU 8+
// namespace fs = std::filesystem;
// For GNU 7
namespace fs = std::experimental::filesystem;

#endif // ifndef HIF_HPP
