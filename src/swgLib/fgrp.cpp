/** -*-c++-*-
 *  \class  fgrp
 *  \file   fgrp.cpp
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

#include <swgLib/fgrp.hpp>
#include <swgLib/base.hpp>

using namespace ml;

fgrp::fgrp() {
}

fgrp::~fgrp() {
}

std::size_t fgrp::read(std::istream& file) {
	std::size_t fgrpSize;
	std::size_t total = base::readFormHeader(file, "FGRP", fgrpSize);
	fgrpSize += 8;
	std::cout << "Found FGRP form: " << fgrpSize << " bytes\n";

	std::size_t size;
	std::string form, type;
	base::peekHeader(file, form, size, type);
	_version = base::tagToVersion(type);
	if ((1 > _version) || (_version > 8)) {
		std::cout << "Expected type [0001...0008]: " << type << "\n";
		exit(0);
	}
	std::cout << "Flora Group version: " << _version << "\n";

	switch (_version) {
	case 1: total += readV1(file); break;
	case 2: total += readV2(file); break;
	case 3: total += readV3(file); break;
	case 4: total += readV4(file); break;
	case 5: total += readV5(file); break;
	case 6: total += readV6(file); break;
	case 7: total += readV7(file); break;
	case 8: total += readV8(file); break;
	default:
		std::cout << "Unexpected version: " << _version << "\n";
		exit(0);
	}

	if (fgrpSize == total) {
		std::cout << "Finished reading FGRP\n";
	}
	else {
		std::cout << "Failed in reading FGRP\n";
		std::cout << "Read " << total << " out of " << fgrpSize << "\n";
		exit(0);
	}

	return total;
}

const std::vector<fgrp::family>& fgrp::getFamily() const {
	return _family;
}

std::size_t fgrp::readV1(std::istream& file) {
	std::size_t size0001;
	std::size_t total = base::readFormHeader(file, "0001", size0001);
	size0001 += 8;

	std::size_t size;
	while (total < size0001) {
		total += base::readRecordHeader(file, "FFAM", size);

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
			total += base::read(file, child.appearanceName);
			child.appearanceName += ".msh";
			total += base::read(file, child.weight);
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

std::size_t fgrp::readV2(std::istream& file) {
	std::size_t size0002;
	std::size_t total = base::readFormHeader(file, "0002", size0002);
	size0002 += 8;

	std::size_t size;
	while (total < size0002) {
		total += base::readRecordHeader(file, "FFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		float unusedF; total += base::read(file, unusedF);
		float unusedI32; total += base::read(file, unusedI32);
		total += base::read(file, unusedF);
		total += base::read(file, newFamily.numChildren);

		newFamily.children.resize(newFamily.numChildren);
		for (auto& child : newFamily.children) {
			total += base::read(file, child.appearanceName);
			child.appearanceName += ".msh";
			total += base::read(file, child.weight);
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

std::size_t fgrp::readV3(std::istream& file) {
	std::size_t size0003;
	std::size_t total = base::readFormHeader(file, "0003", size0003);
	size0003 += 8;

	std::size_t size;
	while (total < size0003) {
		total += base::readRecordHeader(file, "FFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		float unusedF; total += base::read(file, unusedF);
		float unusedI32; total += base::read(file, unusedI32);
		total += base::read(file, unusedF);
		total += base::read(file, newFamily.numChildren);

		newFamily.children.resize(newFamily.numChildren);
		for (auto& child : newFamily.children) {
			total += base::read(file, child.appearanceName);
			total += base::read(file, child.weight);
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

std::size_t fgrp::readV4(std::istream& file) {
	std::size_t size0004;
	std::size_t total = base::readFormHeader(file, "0004", size0004);
	size0004 += 8;

	std::size_t size;
	while (total < size0004) {
		total += base::readRecordHeader(file, "FFAM", size);

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
			total += base::read(file, child.appearanceName);
			total += base::read(file, child.weight);
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

std::size_t fgrp::readV5(std::istream& file) {
	std::size_t size0005;
	std::size_t total = base::readFormHeader(file, "0005", size0005);
	size0005 += 8;

	std::size_t size;
	while (total < size0005) {
		total += base::readRecordHeader(file, "FFAM", size);

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
			total += base::read(file, child.appearanceName);
			total += base::read(file, child.weight);
			int32_t shouldSway; total += base::read(file, shouldSway);
			child.shouldSway = (0 != shouldSway);
			total += base::read(file, child.displacement);
			total += base::read(file, child.period);
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

std::size_t fgrp::readV6(std::istream& file) {
	std::size_t size0006;
	std::size_t total = base::readFormHeader(file, "0006", size0006);
	size0006 += 8;

	std::size_t size;
	while (total < size0006) {
		total += base::readRecordHeader(file, "FFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		total += base::read(file, newFamily.density);
		int32_t floatsInt; total += base::read(file, floatsInt);
		newFamily.floats = (0 != floatsInt);
		total += base::read(file, newFamily.numChildren);

		newFamily.children.resize(newFamily.numChildren);
		for (auto& child : newFamily.children) {
			total += base::read(file, child.appearanceName);
			total += base::read(file, child.weight);
			int32_t shouldSway; total += base::read(file, shouldSway);
			child.shouldSway = (0 != shouldSway);
			total += base::read(file, child.displacement);
			total += base::read(file, child.period);
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

std::size_t fgrp::readV7(std::istream& file) {
	std::size_t size0007;
	std::size_t total = base::readFormHeader(file, "0007", size0007);
	size0007 += 8;

	std::size_t size;
	while (total < size0007) {
		total += base::readRecordHeader(file, "FFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		total += base::read(file, newFamily.density);
		int32_t floatsInt; total += base::read(file, floatsInt);
		newFamily.floats = (0 != floatsInt);
		total += base::read(file, newFamily.numChildren);

		newFamily.children.resize(newFamily.numChildren);
		for (auto& child : newFamily.children) {
			int32_t temp;
			total += base::read(file, child.appearanceName);
			total += base::read(file, child.weight);
			total += base::read(file, temp); child.shouldSway = (0 != temp);
			total += base::read(file, child.displacement);
			total += base::read(file, child.period);
			total += base::read(file, temp); child.alignToTerrain = (0 != temp);
		}
	}

	if (size0007 == total) {
		std::cout << "Finished reading 0007\n";
	}
	else {
		std::cout << "Failed in reading 0007\n";
		std::cout << "Read " << total << " out of " << size0007 << "\n";
		exit(0);
	}

	return total;
}

std::size_t fgrp::readV8(std::istream& file) {
	std::size_t size0008;
	std::size_t total = base::readFormHeader(file, "0008", size0008);
	size0008 += 8;

	std::size_t size;
	while (total < size0008) {
		total += base::readRecordHeader(file, "FFAM", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.red);
		total += base::read(file, newFamily.green);
		total += base::read(file, newFamily.blue);
		total += base::read(file, newFamily.density);
		int32_t floatsInt; total += base::read(file, floatsInt);
		newFamily.floats = (0 != floatsInt);
		total += base::read(file, newFamily.numChildren);

		std::cout
			<< "Family name: " << newFamily.name << "\n"
			<< "Density: " << newFamily.density << "\n"
			<< "Floats: " << std::boolalpha << newFamily.floats << "\n"
			<< "Num children: " << newFamily.numChildren << "\n";

		newFamily.children.resize(newFamily.numChildren);
		for (auto& child : newFamily.children) {
			int32_t temp;
			total += base::read(file, child.appearanceName);
			total += base::read(file, child.weight);
			total += base::read(file, temp); child.shouldSway = (0 != temp);
			total += base::read(file, child.displacement);
			total += base::read(file, child.period);
			total += base::read(file, temp); child.alignToTerrain = (0 != temp);
			total += base::read(file, temp); child.shouldScale = (0 != temp);
			total += base::read(file, child.minimumScale);
			total += base::read(file, child.maximumScale);
			std::cout << "Child appearance: " << child.appearanceName << "\n"
				<< "Child weight: " << child.weight << "\n"
				<< "Child should sway: " << std::boolalpha << child.shouldSway << "\n"
				<< "Child displacement: " << child.displacement << "\n"
				<< "Child period: " << child.period << "\n"
				<< "Child align to terrain: " << std::boolalpha << child.alignToTerrain << "\n"
				<< "Child should scale: " << std::boolalpha << child.shouldScale << "\n"
				<< "Child min scale: " << child.minimumScale << "\n"
				<< "Child max scale: " << child.maximumScale << "\n";


		}
	}

	if (size0008 == total) {
		std::cout << "Finished reading 0008\n";
	}
	else {
		std::cout << "Failed in reading 0008\n";
		std::cout << "Read " << total << " out of " << size0008 << "\n";
		exit(0);
	}

	return total;
}

