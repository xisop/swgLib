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

std::size_t exsp::read(std::istream& file) {
	std::size_t exspSize;
	std::size_t total = base::readFormHeader(file, "exsp", exspSize);
	exspSize += 8;
	std::cout << "Found exsp form: " << exspSize - 12 << " bytes\n";

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
		total += base::readRecordHeader(file, "CNTR", size);
		total += base::read(file, _exspCenter);
		total += base::readRecordHeader(file, "RADI", size);
		total += base::read(file, _exspRadius);
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

std::size_t exsp::read(std::istream& file,
	vector3& exspCenter,
	float& exspRadius,
	vector3& boxCorner1,
	vector3& boxCorner2)
{
	std::size_t exspSize;
	std::size_t total = base::readFormHeader(file, "exsp", exspSize);
	exspSize += 8;
	std::cout << "Found exsp form: " << exspSize - 12 << " bytes\n";

	std::string form, type;
	std::size_t size;
	total += base::readFormHeader(file, form, size, type);

	uint8_t version = base::tagToVersion(type);

	if (version > 1) {
		std::cout << "Expected form [0000..0001] not " << type << "\n";
		exit(0);
	}
	std::cout << "Found form " << type << "\n";

	if (1 == version) {
		// Load EXSP
		std::size_t exspSize;
		total += base::readFormHeader(file, "EXSP", exspSize);
		exspSize += 8;
		std::cout << "Found EXSP form: " << exspSize - 12 << " bytes\n";

		std::string exspType;
		total += base::readFormHeader(file, form, size, exspType);

		uint8_t exspVersion = base::tagToVersion(exspType);
		if (exspVersion > 1) {
			std::cout << "Expected form [0000..0001] not " << exspType << "\n";
			exit(0);
		}
		std::cout << "Found form " << exspType << "\n";

		if (0 == exspVersion) {
			total += base::readRecordHeader(file, "CNTR", size);
			total += base::read(file, exspCenter);
			total += base::readRecordHeader(file, "RADI", size);
			total += base::read(file, exspRadius);
		}
		else {
			total += base::readRecordHeader(file, "SPHR", size);
			total += base::read(file, exspCenter);
			total += base::read(file, exspRadius);
		}
	}

	// Load BOX
	total += base::readRecordHeader(file, "BOX ", size);
	total += base::read(file, boxCorner1);
	total += base::read(file, boxCorner2);

	if (0 == version) {
		total += base::readRecordHeader(file, "CNTR", size);
		total += base::read(file, exspCenter);
		total += base::readRecordHeader(file, "RADI", size);
		total += base::read(file, exspRadius);
	}

	std::cout
		<< "EXSP sphere center: " << exspCenter << "\n"
		<< "EXSP sphere radius: " << exspRadius << "\n"
		<< "exsp corner 1: " << boxCorner1 << "\n"
		<< "exsp corner 2: " << boxCorner2 << "\n";

	if (total == exspSize)
	{
		std::cout << "Finished reading exsp." << std::endl;
	}
	else
	{
		std::cout << "Error reading exsp!" << std::endl;
		std::cout << "Read " << total << " out of " << exspSize << "\n";
		exit(0);
	}

	return total;
}
