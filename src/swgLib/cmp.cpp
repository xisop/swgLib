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

#include <swgLib/base.hpp>
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

std::size_t cmp::read(std::istream& file)
{
	std::size_t cmpSize;
	std::size_t total = base::readFormHeader(file, "CMPA", cmpSize);
	cmpSize += 8;
	std::cout << "Found form CMPA: " << cmpSize << "\n";

	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);
	_cmpVersion = base::tagToVersion(type);
	if ((_cmpVersion < 1) || (_cmpVersion > 5)) {
		std::cout << "Expected FORM of type 0001, 0002, 0003, 0004, or 0005. Found: " << type << "\n";
		exit(0);
	}
	std::cout << "CMPA version: " << (int)_cmpVersion << "\n";

	switch (_cmpVersion) {
	case 1: total += readCMPv1(file); break;
	case 2: total += readCMPv2(file); break;
	case 3: total += readCMPv3(file); break;
	case 4: total += readCMPv4(file); break;
	case 5: total += readCMPv5(file); break;
	default:
		std::cout << "Unhandled CMPA version: " << (int)_cmpVersion << "\n";
		exit(0);
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

std::size_t cmp::readCMPv1(std::istream& file)
{
	std::size_t size0001;
	std::size_t total = base::readFormHeader(file, "0001", size0001);
	size0001 += 8;

	// Load all parts
	while (total < size0001) {
		std::size_t size;
		total += base::readRecordHeader(file, "PART", size);
		part newPart;
		total += base::read(file, newPart.filename);
		newPart.filename = std::string("appearance/") + newPart.filename;
		std::cout << "Part: " << newPart.filename << std::endl;

		newPart.validTransform = false;
		total += base::read(file, newPart.position);
		total += base::read(file, newPart.yawPitchRoll);
		std::cout << "Position: " << newPart.position << "\n"
			<< "Yaw, Pitch, Roll: " << newPart.yawPitchRoll << "\n";
	}

	if (size0001 != total) {
		std::cout << "Failed in reading 0001\n"
			<< "Read " << total << " out of " << size0001
			<< "\n";
		exit(0);
	}

	return total;
}

std::size_t cmp::readCMPv2(std::istream& file)
{
	std::size_t size0002;
	std::size_t total = base::readFormHeader(file, "0002", size0002);
	size0002 += 8;

	// Load all parts
	while (total < size0002) {
		total += readPART(file);
	}

	if (size0002 != total) {
		std::cout << "Failed in reading 0002\n"
			<< "Read " << total << " out of " << size0002
			<< "\n";
		exit(0);
	}

	return total;
}

std::size_t cmp::readCMPv3(std::istream& file)
{
	std::size_t size0003;
	std::size_t total = base::readFormHeader(file, "0003", size0003);
	size0003 += 8;

	// Load base appearance...
	total += appr::read(file);

	// Load all parts
	while (total < size0003) {
		total += readPART(file);
	}

	if (size0003 != total) {
		std::cout << "Failed in reading 0003\n"
			<< "Read " << total << " out of " << size0003
			<< "\n";
		exit(0);
	}

	return total;
}

std::size_t cmp::readCMPv4(std::istream& file)
{
	std::size_t size0004;
	std::size_t total = base::readFormHeader(file, "0004", size0004);
	size0004 += 8;

	// Load base appearance...
	total += appr::read(file);

	// Load all parts
	while (total < size0004) {
		total += readPART(file);
	}

	// TODO: Create test shape?

	if (size0004 != total) {
		std::cout << "Failed in reading 0004\n"
			<< "Read " << total << " out of " << size0004
			<< "\n";
		exit(0);
	}

	return total;
}

std::size_t cmp::readCMPv5(std::istream& file)
{
	std::size_t size0005;
	std::size_t total = base::readFormHeader(file, "0005", size0005);
	size0005 += 8;

	// Load base appearance...
	total += appr::read(file);

	// Load radar shape...
	total += readRADR(file);

	// Load all parts
	while (total < size0005) {
		total += readPART(file);
	}

	// TODO: Create test shape?

	if (size0005 != total) {
		std::cout << "Failed in reading 0005\n"
			<< "Read " << total << " out of " << size0005
			<< "\n";
		exit(0);
	}

	return total;
}

std::size_t cmp::readPART(std::istream& file)
{
	std::size_t partSize;
	std::size_t total = base::readRecordHeader(file, "PART", partSize);
	partSize += 8;

	part newPart;

	total += base::read(file, newPart.filename);
	newPart.filename = std::string("appearance/") + newPart.filename;
	std::cout << "Part: " << newPart.filename << "\n";

	// Read 3x4 transform matrix
	newPart.validTransform = true;
	total += newPart.transform.read(file);
	std::cout << "Matrix: \n" << newPart.transform << "\n";

	_parts.push_back(newPart);

	if (partSize != total) {
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
	radrSize += 8;

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

