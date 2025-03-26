#include <list>
#include <stack>
#include <unordered_map>


#include <iostream>
#include <iomanip>
#include <string>

#define u32         unsigned int
#define u64         unsigned long long
#define u512        unsigned long long  //unsigned long long位宽不足512位，暂定如此

#define HOST_NUM    7                   //此段共享内存有 HOST_NUM 个主机共享
#define LD_PID      9                   //LD(Logic Device) PID

#define CPP_ERROR(message)        printf(message)
#define DONT_PRINT_LD_PID
