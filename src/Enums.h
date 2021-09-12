#pragma once
#include "Types.h"
#include <assert.h>

enum class Allocator { dynamic, oneFrame, persistant };
uintptr_t Align(uintptr_t addr, U8 align);