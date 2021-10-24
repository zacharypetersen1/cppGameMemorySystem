#pragma once

namespace GameMemorySystem
{

// Contains metadata for a memory block
class MemoryBlock
{
private:
	// Rightmost bit contains isFree flag.
	// All other bits contain size.
	size_t m_header = 0;

public:
	// Initializes member data
	void init(bool isFree, size_t size);

	// Returns true if this block is free
	inline bool isFree() const { return (m_header & 1) > 0; }

	// Returns the size of this block
	inline size_t getSize() const { return m_header >> 1; }
};

}
