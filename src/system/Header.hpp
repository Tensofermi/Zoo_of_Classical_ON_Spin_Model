#pragma once
#include <bits/stdc++.h>
#define sprintf_s sprintf

// Some useful tools
#include "Utils/Utils.hpp"

// Input output controller
// (indenpent)
#include "IOControl/IOControl.hpp"

// Get time in real world
// (indenpent)
#include "Clock/Clock.hpp"

// Get random number
// (indenpent)
#include "RandomNumGen/RandomNumGen.hpp"

// Strucuture of Monte Carlo parameters
// (indenpent)
#include "../config/Parameter_config.hpp"

// Read parameter from file
// (depends on IOControl and the Parameter structure.)
#include "Inputor/Inputor.hpp"

// Get data of average
// (depends on IOControl, Clock and the Parameter structure.)
#include "Observable/Observable.hpp"

// Get data of distribution
// (depends on IOControl, Clock and the Parameter structure.)
#include "Histogram/Histogram.hpp"

// Build up and update configuration
// (depends on IOControl, Clock, RandomNumGen and the Parameter structure.)
// (also Observable and Distribution.)
#include "../config/Configuration/Configuration.hpp"