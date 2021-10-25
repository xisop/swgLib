/** -*-c++-*-
 *  \class  efct
 *  \file   efct.cpp
 *  \author Ken Sewell

 swgLib is used for the parsing and exporting SWG models.
 Copyright (C) 2006-2021 Ken Sewell

 This file is part of swgLib.

 swgLib is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 swgLib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with swgLib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <swgLib/efct.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

efct::efct()
{
}

efct::~efct()
{
}

std::size_t efct::readEFCT(std::istream& file)
{
	std::size_t efctSize;
	std::size_t total = base::readFormHeader(file, "EFCT", efctSize);
	efctSize += 8;
	std::cout << "Found EFCT form: " << efctSize - 12 << " bytes\n";

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);
	_version = base::tagToVersion(type);
	if ((0 != _version) && (1 != _version)) {
		std::cout << "Expected 0000 or 0001. Found: " << type << "\n";
		exit(0);
	}
	std::cout << "Found form " << type << ": " << size - 4 << " bytes\n";

	// Read DATA record
	total += base::readRecordHeader(file, "DATA", size);
	std::cout << "Found record DATA: " << size << "\n";

	total += base::read(file, _numImplementations);
	std::cout << "Number of implementations: " << (int)_numImplementations << "\n";

	if (1 == _version) {
		total += base::read(file, _precalcVertexLighting);
		std::cout << "Precalculated vertex Lighting: " << std::boolalpha << _precalcVertexLighting << "\n";
	}

	// Read all implementations...
	_implementation.resize(_numImplementations);
	for (auto& i : _implementation) {
		total += i.read(file);
	}

	if (efctSize == total) {
		std::cout << "Finished reading EFCT\n";
	}
	else {
		std::cout << "FAILED in reading EFCT\n";
		std::cout << "Read " << total << " out of " << efctSize << "\n";
		exit(0);
	}

	return total;
}
