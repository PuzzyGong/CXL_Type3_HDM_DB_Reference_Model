#ifndef GLOBAL_CONF_H
#define GLOBAL_CONF_H

// ----------------------------- ERROR AND WARNING -----------------------------
#include "./error_and_warning.h"

// ------------------------------ CPP INCLUDE ------------------------------
// STRING INCLUDE
#include <string>

// STL INCLUDE
#include <vector>
#include "./limited_list.h"
#include "./sram.h"

// OTHERS INCLUDE
#include <tuple>
#include <memory>  // std::unique_ptr

// IO INCLUDE
#include <iostream>
#include <iomanip>

// ------------------------------ BASE TYPE ------------------------------
#define u32 unsigned int
#define u64 unsigned long long
#include "./u512.h"


#define HDM_PID 123


#endif
