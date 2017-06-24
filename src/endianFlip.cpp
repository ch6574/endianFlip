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
#include <SparcEndianTools.H>
#include <iostream>

// A typical use case, a struct dumped out onto the wire as a binary message
typedef struct {
	char foo[6];
	int a;
	float b;
	double c;
} EXAMPLE_DATA;

// Helper methods
void convertToSparcEndian(EXAMPLE_DATA& data) {
	toSparc64Endian(data.a);
	toSparc64Endian(data.b);
	toSparc64Endian(data.c);
}
void convertFromSparcEndian(EXAMPLE_DATA& data) {
	fromSparc64Endian(data.a);
	fromSparc64Endian(data.b);
	fromSparc64Endian(data.c);
}

int main() {
	// Fill up some test data
	EXAMPLE_DATA myData = { { "HI!" }, 123, 12345.6789, 12345.6789, };

	printAsHex(myData);

	convertToSparcEndian(myData);
	printAsHex(myData);

	convertFromSparcEndian(myData);
	printAsHex(myData);

	return 0;
}
