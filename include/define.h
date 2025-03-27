#ifndef DEFINE_H
#define DEFINE_H

// STL include
#include <algorithm>
#include <string>
#include <list>
#include <unordered_map>

// IO include
#include <iostream>
#include <iomanip>

//
#include "port.h"

//
#define u32  unsigned int
#define u64  unsigned long long
#define u512 unsigned long long // 需要修改 : "unsigned long long" 位宽不足 512 位

// ERROR
#define CPP_ERROR(message) printf(message)

#endif