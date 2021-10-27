/** -*-c++-*-
 *  \class  exsp
 *  \file   exsp.cpp
 *  \author Ken Sewell

 swgLib is used for the parsing and exporting SWG models.
 Copyright (C) 2009-2021 Ken Sewell

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

#include <swgLib/exsp.hpp>
#include <swgLib/base.hpp>

using namespace ml;

exsp::exsp() :
	_exspCenter(0.0f, 0.0f, 0.0f),
	_exspRadius(0.0f) {
}

exsp::~exsp() {}

std::size_t exsp::readOld(std::istream& file) {
	std::size_t size;
	std::size_t	total = base::readRecordHeader(file, "CNTR", size);
		total += base::read(file, _exspCenter);
		total += base::readRecordHeader(file, "RADI", size);
		total += base::read(file, _exspRadius);
	return total;
}

std::size_t exsp::read(std::istream& file) {
	std::size_t exspSize;
	std::size_t total = base::readFormHeader(file, "EXSP", exspSize);
	exspSize += 8;
	std::cout << "Found EXSP form: " << exspSize - 12 << " bytes\n";

	std::string form, type;
	std::size_t size;
	total += base::readFormHeader(file, form, size, type);
	_exspVersion = base::tagToVersion(type);

	if (_exspVersion > 1) {
		std::cout << "Expected form [0000..0001] not " << type << "\n";
		exit(0);
	}
	std::cout << "Found form " << type << "\n";

	if (0 == _exspVersion) {
		total += readOld(file);
	}
	else {
		total += base::readRecordHeader(file, "SPHR", size);
		total += base::read(file, _exspCenter);
		total += base::read(file, _exspRadius);
	}

	std::cout
		<< "EXSP sphere center: " << _exspCenter << "\n"
		<< "EXSP sphere radius: " << _exspRadius << "\n";

	if (total == exspSize)
	{
		std::cout << "Finished reading EXSP.\n";
	}
	else
	{
		std::cout << "Error reading EXSP!\n"
			<< "Read " << total << " out of " << exspSize << "\n";
		exit(0);
	}

	return total;
}
