/** -*-c++-*-
 *  \class  sbot
 *  \file   sbot.cpp
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

/**
   This class still needs work.
 */

#include <swgLib/base.hpp>
#include <swgLib/sbot.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

sbot::sbot() :
	_sbotVersion(0),
	_sbotBaseObjectFilename(""),
	_terrainModificationFileName(""),
	_interiorLayoutFileName("") {
}

sbot::~sbot() {
}

std::size_t sbot::readSBOT(std::istream& file) {
	std::size_t sbotSize;
	std::size_t total = base::readFormHeader(file, "SBOT", sbotSize);
	sbotSize += 8;
	std::cout << "Found SBOT form: " << sbotSize << "\n";

	total += readDERV(file, _sbotBaseObjectFilename);

	std::string type;
	std::size_t size;
	total += base::readFormHeader(file, type, size);
	_sbotVersion = base::tagToVersion(type);
	std::cout << "SBOT version: " << (int)_sbotVersion << "\n";

	int32_t numParameters;
	total += readPCNT(file, numParameters);
	for (int32_t i = 0; i < numParameters; ++i)
	{
		total += readSBOTParameter(file);
	}

	total += readSTOT(file);

	if (sbotSize == total)
	{
		std::cout << "Finished reading SBOT" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading SBOT" << std::endl;
		std::cout << "Read " << total << " out of " << sbotSize
			<< std::endl;
		exit(0);
	}

	return total;
}

const std::string& sbot::getTerrainModificationFilename() const {
	return _terrainModificationFileName;
}

const std::string& sbot::getInteriorLayoutFilename() const {
	return _interiorLayoutFileName;
}

void sbot::print(std::ostream& os) const {
#if 0
	os << "Terrain modification filename: " << _terrainModificationFileName << "\n"
		<< "Interior layout filename: " << _interiorLayoutFileName << "\n";
#endif
}

std::size_t sbot::readSBOTParameter(std::istream& file) {
#if 1
	std::size_t xxxxSize;
	std::size_t total = base::readRecordHeader(file, "XXXX", xxxxSize);
	xxxxSize += 8;
	std::string parameter;
	total += base::read(file, parameter);
	//std::cout << "Parameter: " << parameter << std::endl;

	// 0 - None
	// 1 - Single
	// 2 - Weighted list (count(int32), weight(int32), value)
	// 3 - Range [min, max]
	// 4 - Die Roll [numDice(int32), dieSides(int32), base(int32)
	int8_t dataType;
	total += base::read(file, dataType);

#if 0
	std::cout << "Parameter data type: ";
	switch (dataType) {
	case 1: std::cout << "Single"; break;
	case 2: std::cout << "Weighted list"; break;
	case 3: std::cout << "Range"; break;
	case 4: std::cout << "Die Roll"; break;
	default: std::cout << "None:" << (int)dataType;
	}
	//std::cout << " not handled\n";
	std::cout << "\n";
#endif

#if 0
	const std::size_t valueSize(xxxxSize - total);
	std::cout << "Value size: " << valueSize << "\n";
#endif
	if (parameter == "terrainModificationFileName")
	{
		if (1 == dataType) {
			total += base::read(file, _terrainModificationFileName);
			std::cout << "Terrain modification filename: " << _terrainModificationFileName << "\n";
		}
	}
	else if (parameter == "interiorLayoutFileName")
	{
		if (1 == dataType) {
			total += base::read(file, _interiorLayoutFileName);
			std::cout << "Interior layout filename: " << _interiorLayoutFileName << "\n";
		}
	}
	else
	{
		std::cout << "Unknown: " << parameter << std::endl;
		exit(0);
	}

	if (xxxxSize != total) {
		std::cout << "FAILED in reading XXXX" << std::endl;
		std::cout << "Read " << total << " out of " << xxxxSize
			<< std::endl;
		exit(0);
	}

	return total;
#else
	std::size_t xxxxSize;
	std::string type;
	std::size_t total = base::readRecordHeader(file, type, xxxxSize);
	if (type != "XXXX")
	{
		std::cout << "Expected record of type XXXX: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found " << type << std::endl;

	std::string property;
	total += base::read(file, property);
	std::cout << "Property: " << property << std::endl;

	unsigned char enabled;
	if (property == "terrainModificationFileName")
	{
		total += base::read(file, enabled);
		if (enabled > 0)
		{
			total += base::read(file, terrainModificationFilename);

			std::cout << property << ": "
				<< terrainModificationFilename
				<< std::endl;
		}
	}
	else if (property == "interiorLayoutFileName")
	{
		total += base::read(file, enabled);
		if (enabled > 0)
		{
			total += base::read(file, interiorLayoutFilename);

			std::cout << property << ": "
				<< interiorLayoutFilename
				<< std::endl;
		}
	}
	else if (property == "portalLayoutFilename")
	{
		total += base::read(file, enabled);
		if (enabled > 0)
		{
			total += base::read(file, _portalLayoutFilename);

			std::cout << property << ": "
				<< _portalLayoutFilename
				<< std::endl;
		}
	}
	else
	{
		std::cout << "Unknown: " << property << std::endl;
		exit(0);
	}

	if (xxxxSize == (total - 8))
	{
		std::cout << "Finished reading XXXX" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading XXXX" << std::endl;
		std::cout << "Read " << total << " out of " << xxxxSize
			<< std::endl;
		exit(0);
	}

	return total;
#endif
}
