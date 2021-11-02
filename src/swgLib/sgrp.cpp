/** -*-c++-*-
 *  \class  sgrp
 *  \file   sgrp.cpp
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

#include <swgLib/sgrp.hpp>
#include <swgLib/base.hpp>

using namespace ml;

sgrp::sgrp() {
}

sgrp::~sgrp() {
}

std::size_t sgrp::read(std::istream& file) {
	std::size_t sgrpSize;
	std::size_t total = base::readFormHeader(file, "SGRP", sgrpSize);
	sgrpSize += 8;
	std::cout << "Found SGRP form: " << sgrpSize << " bytes\n";

	std::size_t size;
	std::string form, type;
	base::peekHeader(file, form, size, type);
	_version = base::tagToVersion(type);
	if (_version > 6) {
		std::cout << "Expected type [0000..0006]: " << type << "\n";
		exit(0);
	}
	std::cout << "Shader Group version: " << _version << "\n";

	switch (_version) {
	case 0: total += readV0(file); break;
	case 1: total += readV1(file); break;
	case 2: total += readV2(file); break;
	case 3: total += readV3(file); break;
	case 4: total += readV4(file); break;
	case 5: total += readV5(file); break;
	case 6: total += readV6(file); break;

	default:
		std::cout << "Unexpected shader group version: " << _version << "\n";
		exit(0);
	}

	if (sgrpSize == total) {
		std::cout << "Finished reading SGRP\n";
	}
	else {
		std::cout << "Failed in reading SGRP\n";
		std::cout << "Read " << total << " out of " << sgrpSize << "\n";
		exit(0);
	}

	return total;
}

const std::vector<sgrp::family>& sgrp::getFamily() const {
	return _family;
}

std::size_t sgrp::readV0(std::istream& file) {
	std::size_t size0000;
	std::size_t total = base::readFormHeader(file, "0000", size0000);
	size0000 += 8;

	std::size_t size;
	while (total < size0000) {
		total += base::readRecordHeader(file, "SFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		newFamily.name = "null";
		newFamily.red = 255;
		newFamily.green = 255;
		newFamily.blue = 255;
		newFamily.shaderSize = 2.0f;
		total += base::read(file, newFamily.numChildren);

		newFamily.children.resize(newFamily.numChildren);
		std::cout << "Num children: " << newFamily.numChildren << "\n";
		for (auto& child : newFamily.children) {
			child.familyId = newFamily.familyId;
			total += base::read(file, child.shaderName);
			std::cout << "Child name: " << child.shaderName << "\n";
			child.weight = 1.0f / float(newFamily.numChildren);
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

std::size_t sgrp::readV1(std::istream& file) {
	std::size_t size0001;
	std::size_t total = base::readFormHeader(file, "0001", size0001);
	size0001 += 8;

	std::size_t size;
	while (total < size0001) {
		total += base::readRecordHeader(file, "SFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		newFamily.shaderSize = 2.0f;
		total += base::read(file, newFamily.numChildren);
		
		newFamily.children.resize(newFamily.numChildren);
		std::cout << "Num children: " << newFamily.numChildren << "\n";
		for (auto& child : newFamily.children) {
			child.familyId = newFamily.familyId;
			total += base::read(file, child.shaderName);
			total += base::read(file, child.weight);
			std::cout << "Child name: " << child.shaderName << "\n";
			std::cout << "Child weight: " << child.weight << "\n";

			child.weight = 1.0f / float(newFamily.numChildren);
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

std::size_t sgrp::readV2(std::istream& file) {
	std::size_t size0002;
	std::size_t total = base::readFormHeader(file, "0002", size0002);
	size0002 += 8;

	std::size_t size;
	while (total < size0002) {
		total += base::readRecordHeader(file, "SFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		total += base::read(file, newFamily.shaderSize);
		total += base::read(file, newFamily.numChildren);

		newFamily.children.resize(newFamily.numChildren);
		std::cout << "Num children: " << newFamily.numChildren << "\n";
		for (auto& child : newFamily.children) {
			child.familyId = newFamily.familyId;
			total += base::read(file, child.shaderName);
			total += base::read(file, child.weight);
			std::cout << "Child name: " << child.shaderName << "\n";
			std::cout << "Child weight: " << child.weight << "\n";

			child.weight = 1.0f / float(newFamily.numChildren);
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

std::size_t sgrp::readV3(std::istream& file) {
	std::size_t size0003;
	std::size_t total = base::readFormHeader(file, "0003", size0003);
	size0003 += 8;

	std::size_t size;
	while (total < size0003) {
		total += base::readRecordHeader(file, "SFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		total += base::read(file, newFamily.shaderSize);
		float unused; total += base::read(file, unused);
		total += base::read(file, newFamily.numChildren);

		newFamily.children.resize(newFamily.numChildren);
		std::cout << "Num children: " << newFamily.numChildren << "\n";
		for (auto& child : newFamily.children) {
			child.familyId = newFamily.familyId;
			total += base::read(file, child.shaderName);
			total += base::read(file, child.weight);
			std::cout << "Child name: " << child.shaderName << "\n";
			std::cout << "Child weight: " << child.weight << "\n";

			child.weight = 1.0f / float(newFamily.numChildren);
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

std::size_t sgrp::readV4(std::istream& file) {
	std::size_t size0004;
	std::size_t total = base::readFormHeader(file, "0004", size0004);
	size0004 += 8;

	std::size_t size;
	while (total < size0004) {
		total += base::readRecordHeader(file, "SFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		total += base::read(file, newFamily.shaderSize);
		total += base::read(file, newFamily.featherClamp);
		total += base::read(file, newFamily.numChildren);

		newFamily.children.resize(newFamily.numChildren);
		std::cout << "Num children: " << newFamily.numChildren << "\n";
		for (auto& child : newFamily.children) {
			child.familyId = newFamily.familyId;
			total += base::read(file, child.shaderName);
			total += base::read(file, child.weight);
			std::cout << "Child name: " << child.shaderName << "\n";
			std::cout << "Child weight: " << child.weight << "\n";

			child.weight = 1.0f / float(newFamily.numChildren);
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

std::size_t sgrp::readV5(std::istream& file) {
	std::size_t size0005;
	std::size_t total = base::readFormHeader(file, "0005", size0005);
	size0005 += 8;

	std::size_t size;
	while (total < size0005) {
		total += base::readRecordHeader(file, "SFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		total += base::read(file, newFamily.shaderSize);
		total += base::read(file, newFamily.featherClamp);
		int32_t unused; total += base::read(file, unused);
		total += base::read(file, newFamily.numChildren);

		newFamily.children.resize(newFamily.numChildren);
		std::cout << "Num children: " << newFamily.numChildren << "\n";
		for (auto& child : newFamily.children) {
			child.familyId = newFamily.familyId;
			total += base::read(file, child.shaderName);
			total += base::read(file, child.weight);
			std::cout << "Child name: " << child.shaderName << "\n";
			std::cout << "Child weight: " << child.weight << "\n";

			child.weight = 1.0f / float(newFamily.numChildren);
		}
	}

	if (size0005 == total) {
		std::cout << "Finished reading 0005\n";
	}
	else {
		std::cout << "Failed in reading 0005\n";
		std::cout << "Read " << total << " out of " << size0005 << "\n";
		exit(0);
	}

	return total;
}

std::size_t sgrp::readV6(std::istream& file) {
	std::size_t size0006;
	std::size_t total = base::readFormHeader(file, "0006", size0006);
	size0006 += 8;

	std::size_t size;
	while (total < size0006) {
		total += base::readRecordHeader(file, "SFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.surfacePropertyName);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		total += base::read(file, newFamily.shaderSize);
		total += base::read(file, newFamily.featherClamp);
		total += base::read(file, newFamily.numChildren);

		newFamily.children.resize(newFamily.numChildren);
		std::cout << "Num children: " << newFamily.numChildren << "\n";
		for (auto& child : newFamily.children) {
			child.familyId = newFamily.familyId;
			total += base::read(file, child.shaderName);
			total += base::read(file, child.weight);
			std::cout << "Child name: " << child.shaderName << "\n";
			std::cout << "Child weight: " << child.weight << "\n";

			child.weight = 1.0f / float(newFamily.numChildren);
		}
	}

	if (size0006 == total) {
		std::cout << "Finished reading 0006\n";
	}
	else {
		std::cout << "Failed in reading 0006\n";
		std::cout << "Read " << total << " out of " << size0006 << "\n";
		exit(0);
	}

	return total;
}

