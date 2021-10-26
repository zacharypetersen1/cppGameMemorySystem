#pragma once
#include <assert.h>
#include <cstdint>

namespace GameMemorySystem
{
// Type defines
typedef uint8_t U8;

// Used to specify alignment when making allocations
class Alignment
{
private:
	int m_value;
public:
	explicit Alignment(int a);
	inline int getValue() const { return m_value; }
};

// The different types of allocators to choose from
enum class Allocator { dynamic, singleFrame, persistant };

// Returns maximum possible payload size required to store
// size bytes with given Alignment
size_t getMaxPayloadSize(size_t size, Alignment align);

// Returns address itself if already aligned or the next aligned address.
uintptr_t Align(uintptr_t addr, Alignment align);
}