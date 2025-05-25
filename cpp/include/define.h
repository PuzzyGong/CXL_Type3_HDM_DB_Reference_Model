#ifndef DEFINE_H
#define DEFINE_H

// String include
#include <string>
#include <sstream>
#include <regex>

// STL include
#include <algorithm>
#include <vector>
#include <list>
#include <unordered_map>

// IO include
#include <iostream>
#include <iomanip>

//
#define u32  unsigned int
#define u64  unsigned long long
#define u512 unsigned long long // 需要修改 : "unsigned long long" 位宽不足 512 位

//   
#define MAX_NUM_HOST  8
#define BITAG_AUTO_ALLOC
#define CPP_WARNING(message) printf(message)

Flit_Mode flit_mode;
LDFAM GFAM
//
#include "port.h"

#endif