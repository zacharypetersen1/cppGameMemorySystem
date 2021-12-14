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

// Returns maximum possible payload size required to store
// "size" bytes with given Alignment
size_t getMaxPayloadSize(size_t size, Alignment align);

// Returns address itself if already aligned or the next aligned address.
U8* Align(U8* pAddr, Alignment align);

// Same as Align but ensures there is space for prefix to store alignment offset
U8* AlignWithSpaceForPrefix(U8* pAddr, Alignment align);
}