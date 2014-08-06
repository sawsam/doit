#ifndef DEFINE_H_
#define DEFINE_H_

/*
	 涉及到UUGame工程包含define_inc.h即可
	 其他工程可以直接包含base_inc.h
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <hash_map>
#include <map>
#include <queue>
#include <vector>
#include <list>
#include <numeric>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

using namespace std;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#define LOG_PRINT( format, ... ) printf(format, __VA_ARGS__)
#define LOG(x) cout<<x<<endl;
#define LOG_EX(x,y) cout<<x<<":"<<y<<endl;
#define TO_STRING(x) #x;
#define PRINT_ERROR_INFO cout<<"ERROR="<<__FILE__<<"__the " <<__LINE__<<" line__"<< __FUNCTION__<<"()"<<endl;

#endif /* BASE_INC_H_ */