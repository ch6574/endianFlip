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

// Tooling for dealing with endian issues (e.g. x86 to/from sparc64)
#ifndef SPARC_ENDIAN_TOOLS_H
#define SPARC_ENDIAN_TOOLS_H

#if __GNUC__ && __cplusplus
#include <endian.h> // htobeNN() and beNNtoh()
#include <cstdint>  // uintNN_t
#include <cstddef>  // size_t

#include <iostream>
#include <iomanip>

// The target is sparc64, so we must assert these are true, else we need extra logic!
#if __SIZEOF_INT__ != 4 || __SIZEOF_FLOAT__ != 4 || __SIZEOF_DOUBLE__ != 8
#error "This code only works for 4byte / 32bit integers and floats; 8byte / 64bit doubles"
#endif

#if !defined __WORDSIZE && defined __sparcv9
#define __WORDSIZE 64 // Sun Studio doesn't define this
#elif !defined __WORDSIZE && defined __sparc
#define __WORDSIZE 32
#endif

/*
 * Object hex pretty printer / hex dumper. Writes to cout (or supplied stream)
 */
template<typename T>
inline void printAsHex(const T& data, std::ostream& os = std::cout) {
	const unsigned char* p = reinterpret_cast<const unsigned char*>(&data);

	os << "Bytes: [:";
	size_t j = 0;
	for (size_t i = 0; i < sizeof(T); i++) {
		os << std::hex << std::setfill('0') << std::setw(2)
				<< static_cast<unsigned>(p[i]) << " ";
		if (++j % (__WORDSIZE / 8) == 0 && j < sizeof(T))
			os << "| ";
	}
	os << "]" << std::endl;
}

// We will template the conversion code to reduce the repetition
template<size_t N> struct SizeT {
};

/*
 * These are the in-line converters which automatically know the respective size specific converter to call
 */
template<typename T> inline void toSparc64Endian(T& t) {
	htobe(&t, SizeT<sizeof t>());
}

template<typename T> inline void fromSparc64Endian(T& t) {
	betoh(&t, SizeT<sizeof t>());
}

/*
 * Utility code for the different sized primitives
 */
#define BYTESWAPS(bits)\
template<typename T> inline void htobe(T* t, const SizeT<bits/8>) {\
	const uint##bits##_t converted = htobe##bits(*reinterpret_cast<uint##bits##_t*>(t));\
	const char* foo = reinterpret_cast<const char*>(&converted);\
	*t = *reinterpret_cast<const T*>(foo);\
}\
template<typename T> inline void betoh(T* t, const SizeT<bits/8>) {\
	const uint##bits##_t converted = be##bits##toh(*reinterpret_cast<uint##bits##_t*>(t));\
	const char* foo = reinterpret_cast<const char*>(&converted);\
	*t = *reinterpret_cast<const T*>(foo);\
}

BYTESWAPS(16)
BYTESWAPS(32)
BYTESWAPS(64)

#undef BYTESWAPS

#endif

#endif
