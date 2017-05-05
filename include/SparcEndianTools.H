/*******************************************************************************
 * Copyright (C) 2017, Christopher Hill <ch6574@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *******************************************************************************/

// Tooling for dealing with sparc64 endian issues

#ifndef SPARC_ENDIAN_TOOLS_H
#define SPARC_ENDIAN_TOOLS_H

#include <iostream>
#include <iomanip>

// Object hex dumper
template<typename T>
inline void printAsHex(const T& data, std::ostream& os = std::cout)
{
	const unsigned char* p = reinterpret_cast<const unsigned char*>(&data);

	os << "Bytes: [:";
	size_t j = 0;
	for (size_t i = 0; i < sizeof(T); i++)
	{
		os << std::hex << std::setfill('0') << std::setw(2)
				<< static_cast<unsigned>(p[i]) << " ";
		if (++j % (__WORDSIZE / 8) == 0 && j < sizeof(T))
			os << "| ";
	}
	os << "]" << std::endl;
}

#include <endian.h>
#include <stdint.h>

// The target is sparc64, so we must assert these are true, else we need extra logic!
#if __SIZEOF_INT__ != 4 || __SIZEOF_FLOAT__ != 4 || __SIZEOF_DOUBLE__ != 8
#error "This code only works for 4byte / 32bit integers and floats; 8byte / 64bit doubles"
#endif

// Sun Studio compiler struct alignment
#define SUN_STUDIO_ALIGNMENT 8

// Union to 'cast' the memory
union CONV8
{
	uint64_t i;
	double d;
};

union CONV4
{
	uint32_t i;
	float f;
};

// Swap byte order (on little endian machines)
inline void intToSparc64Endian(int& data)
{
	data = htobe32(data);
}
inline void intFromSparc64Endian(int& data)
{
	data = be32toh(data);
}

inline void floatToSparc64Endian(float& data)
{
	CONV4 conv;
	conv.f = data;
	conv.i = htobe32(conv.i);
	data = conv.f;
}
inline void floatFromSparc64Endian(float& data)
{
	CONV4 conv;
	conv.f = data;
	conv.i = be32toh(conv.i);
	data = conv.f;
}

inline void doubleToSparc64Endian(double& data)
{
	CONV8 conv;
	conv.d = data;
	conv.i = htobe32(conv.i);
	data = conv.d;
}
inline void doubleFromSparc64Endian(double& data)
{
	CONV8 conv;
	conv.d = data;
	conv.i = be32toh(conv.i);
	data = conv.d;
}

#endif
