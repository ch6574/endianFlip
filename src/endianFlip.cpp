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

// Demo of tooling for dealing with sparc64 endian issues

#include <iostream>
#include "SparcEndianTools.H"

// A typical use case, a struct dumped out onto the wire as a binary message
// Note the GCC alignment attribute regarding minimum memory layout
typedef struct
{
	char foo[6];
	int a;
	float b;
	double c;
}
#if __GNUC__
__attribute__ ((aligned(SUN_STUDIO_ALIGNMENT)))
#endif
EXAMPLE_DATA;

// Helper methods
void convertToSparcEndian(EXAMPLE_DATA& data)
{
	intToSparc64Endian(data.a);
	floatToSparc64Endian(data.b);
	doubleToSparc64Endian(data.c);
}
void convertFromSparcEndian(EXAMPLE_DATA& data)
{
	intFromSparc64Endian(data.a);
	floatFromSparc64Endian(data.b);
	doubleFromSparc64Endian(data.c);
}

int main()
{
	// Fill up some test data
	EXAMPLE_DATA myData = {
			{"HI!"},
			123,
			12345.6789,
			12345.6789,
	};

	printAsHex(myData);

	convertToSparcEndian(myData);
	printAsHex(myData);

	convertFromSparcEndian(myData);
	printAsHex(myData);

	return 0;
}
