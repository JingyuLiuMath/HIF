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

#include "HIF/core/environment.hpp"
#include "HIF/hifgraph.hpp"
#include "GraphPart.hpp"
#include "SetOperation.hpp"

// For GNU 8+
// namespace fs = std::filesystem;
// For GNU 7
namespace fs = std::experimental::filesystem;

#endif // ifndef HIF_HPP
