/** -*-c++-*-
 *  \class  egrp
 *  \file   egrp.cpp
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

#include <swgLib/egrp.hpp>
#include <swgLib/base.hpp>

using namespace ml;

egrp::egrp() {
}

egrp::~egrp() {
}

std::size_t egrp::read(std::istream& file) {
	std::size_t egrpSize;
	std::size_t total = base::readFormHeader(file, "EGRP", egrpSize);
	egrpSize += 8;
	std::cout << "Found EGRP form: " << egrpSize << " bytes\n";

	std::size_t size;
	std::string form, type;
	base::peekHeader(file, form, size, type);
	_version = base::tagToVersion(type);
	if (2 < _version) {
		std::cout << "Expected type [0000...0002]: " << type << "\n";
		exit(0);
	}
	std::cout << "Environment Group version: " << _version << "\n";

	switch (_version) {
	case 0: total += readV0(file); break;
	case 1: total += readV1(file); break;
	case 2: total += readV2(file); break;
	default:
		std::cout << "Unexpected version: " << _version << "\n";
		exit(0);
	}

	if (egrpSize == total) {
		std::cout << "Finished reading EGRP\n";
	}
	else {
		std::cout << "Failed in reading egrp\n";
		std::cout << "Read " << total << " out of " << egrpSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t egrp::readV0(std::istream& file) {
	std::size_t size0000;
	std::size_t total = base::readFormHeader(file, "0000", size0000);
	size0000 += 8;

	std::size_t size;
	while (total < size0000) {
		total += base::readFormHeader(file, "EFAM", size);
		total += base::readRecordHeader(file, "DATA", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		total += base::read(file, newFamily.featherClamp);
	}

	if (size0000 == total) {
		std::cout << "Finished reading 0000\n";
	}
	else {
		std::cout << "Failed in reading 0000\n";
		std::cout << "Read " << total << " out of " << size0000 << "\n";
		exit(0);
	}

	return total;
}

std::size_t egrp::readV1(std::istream& file) {
	std::size_t size0001;
	std::size_t total = base::readFormHeader(file, "0001", size0001);
	size0001 += 8;

	std::size_t size;
	while (total < size0001) {
		total += base::readFormHeader(file, "EFAM", size);
		total += base::readRecordHeader(file, "DATA", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		total += base::read(file, newFamily.featherClamp);
	}

	if (size0001 == total) {
		std::cout << "Finished reading 0001\n";
	}
	else {
		std::cout << "Failed in reading 0001\n";
		std::cout << "Read " << total << " out of " << size0001 << "\n";
		exit(0);
	}

	return total;
}

std::size_t egrp::readV2(std::istream& file) {
	std::size_t size0002;
	std::size_t total = base::readFormHeader(file, "0002", size0002);
	size0002 += 8;

	std::size_t size;
	while (total < size0002) {
		total += base::readFormHeader(file, "EFAM", size);
		total += base::readRecordHeader(file, "DATA", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		total += base::read(file, newFamily.featherClamp);

		std::cout << "Family name: " << newFamily.name << "\n";
	}

	if (size0002 == total) {
		std::cout << "Finished reading 0002\n";
	}
	else {
		std::cout << "Failed in reading 0002\n";
		std::cout << "Read " << total << " out of " << size0002 << "\n";
		exit(0);
	}

	return total;
}
