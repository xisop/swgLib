/** -*-c++-*-
 *  \class  rgrp
 *  \file   rgrp.cpp
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

#include <swgLib/rgrp.hpp>
#include <swgLib/base.hpp>

using namespace ml;

rgrp::rgrp() {
}

rgrp::~rgrp() {
}

std::size_t rgrp::read(std::istream& file) {
	std::size_t rgrpSize;
	std::size_t total = base::readFormHeader(file, "RGRP", rgrpSize);
	rgrpSize += 8;
	std::cout << "Found RGRP form: " << rgrpSize << " bytes\n";

	std::size_t size;
	std::string form, type;
	base::peekHeader(file, form, size, type);
	_version = base::tagToVersion(type);
	if (_version > 4) {
		std::cout << "Expected type [0000...0004]: " << type << "\n";
		exit(0);
	}
	std::cout << "Radial Group version: " << _version << "\n";

	switch (_version) {
	case 0: total += readV0(file); break;
	case 1: total += readV1(file); break;
	case 2: total += readV2(file); break;
	case 3: total += readV3(file); break;
	case 4: total += readV4(file); break;
	default:
		std::cout << "Unexpected version: " << _version << "\n";
		exit(0);
	}

	if (rgrpSize == total) {
		std::cout << "Finished reading RGRP\n";
	}
	else {
		std::cout << "Failed in reading RGRP\n";
		std::cout << "Read " << total << " out of " << rgrpSize << "\n";
		exit(0);
	}

	return total;
}

const std::vector<rgrp::family>& rgrp::getFamily() const {
	return _family;
}

std::size_t rgrp::readV0(std::istream& file) {
	std::size_t size0000;
	std::size_t total = base::readFormHeader(file, "0000", size0000);
	size0000 += 8;

	std::size_t size;
	while (total < size0000) {
		total += base::readRecordHeader(file, "RFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		total += base::read(file, newFamily.numChildren);

		newFamily.children.resize(newFamily.numChildren);
		for (auto& child : newFamily.children) {
			total += base::read(file, child.shaderName);
			total += base::read(file, child.weight);
			total += base::read(file, child.distance);
			total += base::read(file, child.minWidth); child.maxWidth = child.minWidth;
			total += base::read(file, child.minHeight); child.maxHeight = child.minHeight;
		}
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

std::size_t rgrp::readV1(std::istream& file) {
	std::size_t size0001;
	std::size_t total = base::readFormHeader(file, "0001", size0001);
	size0001 += 8;

	std::size_t size;
	while (total < size0001) {
		total += base::readRecordHeader(file, "RFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		total += base::read(file, newFamily.density);
		total += base::read(file, newFamily.numChildren);

		newFamily.children.resize(newFamily.numChildren);
		for (auto& child : newFamily.children) {
			total += base::read(file, child.shaderName);
			total += base::read(file, child.weight);
			total += base::read(file, child.distance);
			total += base::read(file, child.minWidth); child.maxWidth = child.minWidth;
			total += base::read(file, child.minHeight); child.maxHeight = child.minHeight;
		}
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

std::size_t rgrp::readV2(std::istream& file) {
	std::size_t size0002;
	std::size_t total = base::readFormHeader(file, "0002", size0002);
	size0002 += 8;

	std::size_t size;
	while (total < size0002) {
		total += base::readRecordHeader(file, "RFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		total += base::read(file, newFamily.density);
		total += base::read(file, newFamily.numChildren);

		newFamily.children.resize(newFamily.numChildren);
		for (auto& child : newFamily.children) {
			int32_t temp;
			total += base::read(file, child.shaderName);
			total += base::read(file, child.weight);
			total += base::read(file, child.distance);
			total += base::read(file, child.minWidth); child.maxWidth = child.minWidth;
			total += base::read(file, child.minHeight); child.maxHeight = child.minHeight;
			total += base::read(file, temp); child.shouldSway = (0 != temp);
			total += base::read(file, child.displacement);
			total += base::read(file, child.period);
		}
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

std::size_t rgrp::readV3(std::istream& file) {
	std::size_t size0003;
	std::size_t total = base::readFormHeader(file, "0003", size0003);
	size0003 += 8;

	std::size_t size;
	while (total < size0003) {
		total += base::readRecordHeader(file, "RFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		total += base::read(file, newFamily.density);
		total += base::read(file, newFamily.numChildren);

		newFamily.children.resize(newFamily.numChildren);
		for (auto& child : newFamily.children) {
			int32_t temp;
			total += base::read(file, child.shaderName);
			total += base::read(file, child.weight);
			total += base::read(file, child.distance);
			total += base::read(file, child.minWidth); child.maxWidth = child.minWidth;
			total += base::read(file, child.minHeight); child.maxHeight = child.minHeight;
			total += base::read(file, temp); child.shouldSway = (0 != temp);
			total += base::read(file, child.displacement);
			total += base::read(file, child.period);
			total += base::read(file, temp); child.alignToTerrain = (0 != temp);
			total += base::read(file, temp); child.createPlus = (0 != temp);
		}
	}

	if (size0003 == total) {
		std::cout << "Finished reading 0003\n";
	}
	else {
		std::cout << "Failed in reading 0003\n";
		std::cout << "Read " << total << " out of " << size0003 << "\n";
		exit(0);
	}

	return total;
}

std::size_t rgrp::readV4(std::istream& file) {
	std::size_t size0004;
	std::size_t total = base::readFormHeader(file, "0004", size0004);
	size0004 += 8;

	std::size_t size;
	while (total < size0004) {
		total += base::readRecordHeader(file, "RFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		total += base::read(file, newFamily.density);
		total += base::read(file, newFamily.numChildren);

		newFamily.children.resize(newFamily.numChildren);
		for (auto& child : newFamily.children) {
			int32_t temp;
			total += base::read(file, child.shaderName);
			total += base::read(file, child.weight);
			total += base::read(file, child.distance);
			total += base::read(file, child.minWidth);
			total += base::read(file, child.maxWidth);
			total += base::read(file, child.minHeight);
			total += base::read(file, child.maxHeight);
			total += base::read(file, temp); child.maintainAspectRatio = (0 != temp);
			total += base::read(file, temp); child.shouldSway = (0 != temp);
			total += base::read(file, child.displacement);
			total += base::read(file, child.period);
			total += base::read(file, temp); child.alignToTerrain = (0 != temp);
			total += base::read(file, temp); child.createPlus = (0 != temp);
		}
	}

	if (size0004 == total) {
		std::cout << "Finished reading 0004\n";
	}
	else {
		std::cout << "Failed in reading 0004\n";
		std::cout << "Read " << total << " out of " << size0004 << "\n";
		exit(0);
	}

	return total;
}

