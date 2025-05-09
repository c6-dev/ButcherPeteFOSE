#pragma once

// 4018 - signed/unsigned mismatch
// 4244 - loss of data by assignment
// 4267 - possible loss of data (truncation)
// 4305 - truncation by assignment
// 4288 - disable warning for crap microsoft extension screwing up the scope of variables defined in for loops
// 4311 - pointer truncation
// 4312 - pointer extension
#pragma warning(disable: 4018 4244 4267 4305 4288 4312 4311 4996)

// win2k and above
#define _WIN32_WINNT	0x0500

#include <cstdio>
#include "ITypes.h"
#include "IErrors.h"
#include "IDebugLog.h"
#include <Windows.h>
#include <string>
#include <cmath>