/** -*-c++-*-
 *  \class  cmp
 *  \file   cmp.cpp
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

#include <swgLib/cmp.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

cmp::cmp() : appr::appr(), _hasRadar(false)
{
}

cmp::~cmp()
{
}

uint32_t cmp::getNumParts() const { return (uint32_t)_parts.size(); }

const cmp::part& cmp::getPart(const uint32_t& index) const {
	return _parts.at(index);
}

const std::vector<cmp::part>& cmp::getParts() const {
	return _parts;
}

std::size_t cmp::readCMP(std::istream& file)
{
	std::size_t cmpSize;
	std::size_t total = base::readFormHeader(file, "CMPA", cmpSize);
	std::cout << "Found form CMPA: " << cmpSize << "\n";
	cmpSize -= 8;

	std::string type;
	std::size_t size;
	total += base::readFormHeader(file, type, size);
	std::cout << "Found FORM " << type << ": " << size << " bytes\n";

	_cmpVersion = base::tagToVersion(type);
	if ((_cmpVersion < 1) || (_cmpVersion > 5)) {
		std::cout << "Expected FORM of type 0001, 0002, 0003, 0004, or 0005. Found: " << type << "\n";
		exit(0);
	}

	if (_cmpVersion > 2) {
		total += appr::read(file);
	}

	if (_cmpVersion > 4) {
		// Radar
		total += readRADR(file);
	}

	while (total < cmpSize)
	{
		if (1 == _cmpVersion) {
			total += readPART0001(file);
		}
		else {
			total += readPART(file);
		}
	}

	if (_cmpVersion > 3) {
		// Create test shape...
		// TODO...
	}

	if (cmpSize == total) {
		std::cout << "Finished reading CMPA\n";
	}
	else
	{
		std::cout << "Failed in reading CMPA\n"
			<< "Read " << total << " out of " << cmpSize
			<< "\n";
		exit(0);
	}

	return total;
}

std::size_t cmp::readPART0001(std::istream& file)
{
	std::size_t partSize;
	base::readRecordHeader(file, "PART", partSize);

	std::size_t total = 0;

	std::cout << "********** Not supported yet. Transform matrix is invalid! **********\n";

	part newPart;

	total += base::read(file, newPart.filename);
	newPart.filename = std::string("appearance/") + newPart.filename;
	std::cout << "Part: " << newPart.filename << std::endl;

	vector3 position;
	total += position.read(file);

	vector3 yawPitchRoll; // In degrees
	total += yawPitchRoll.read(file);

	_parts.push_back(newPart);

	if (partSize == total)
	{
		std::cout << "Finished reading PART" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading PART\n"
			<< "Read " << total << " out of " << partSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t cmp::readPART(std::istream& file)
{
	std::size_t partSize;
	base::readRecordHeader(file, "PART", partSize);

	std::size_t total = 0;

	part newPart;

	total += base::read(file, newPart.filename);
	newPart.filename = std::string("appearance/") + newPart.filename;
	std::cout << "Part: " << newPart.filename << std::endl;

	// Read 3x4 transform matrix
	total += newPart.transform.read(file);
	std::cout << "Matrix: \n" << newPart.transform << std::endl;

	_parts.push_back(newPart);

	if (partSize == total)
	{
		std::cout << "Finished reading PART" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading PART\n"
			<< "Read " << total << " out of " << partSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t cmp::readRADR(std::istream& file)
{
	std::size_t radrSize;
	std::size_t total = base::readFormHeader(file, "RADR", radrSize);
	total -= 8;

	std::size_t size;
	total += base::readRecordHeader(file, "INFO", size);

	uint32_t hasRadar;
	total += base::read(file, hasRadar);
	_hasRadar = (hasRadar > 0);
	std::cout << "Has Radar: " << std::boolalpha << _hasRadar << "\n";

	if (_hasRadar) {
		total += _radar.read(file);
	}

	if (radrSize == total) {
		std::cout << "Finished reading RADR\n";
	}
	else {
		std::cout << "Failed in reading RADR\n"
			<< "Read " << total << " out of " << radrSize << "\n";
		exit(0);
	}

	return total;
}

