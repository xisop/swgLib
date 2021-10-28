/** -*-c++-*-
 *  \class  cell
 *  \file   cell.cpp
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

#include <swgLib/cell.hpp>
#include <swgLib/base.hpp>
#include <swgLib/collisionUtil.hpp>

using namespace ml;

cell::cell() {
}

cell::~cell() {
}

std::size_t cell::read(std::istream& file) {
	std::size_t cellSize;
	std::size_t total = base::readFormHeader(file, "CELL", cellSize);
	cellSize += 8;
	std::cout << "Found FORM CELL: "
		<< cellSize - 12 << " bytes\n";

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);
	_version = base::tagToVersion(type);
	std::cout << "CELL version: " << (int)_version << "\n";

	if ((_version < 1) || (_version > 5))
	{
		std::cout << "Expected FORM of type 0001, 0002, 0003, 0004, or 0005. Found: " << type << "\n";
		exit(0);
	}

	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _numPortals);
	std::cout << "Num portals: " << _numPortals << "\n";

	total += base::read(file, _canSeeParentCell);
	std::cout << "Can see parent cell: " << std::boolalpha << _canSeeParentCell << "\n";

	if (3 < _version) {
		total += base::read(file, _name);
		std::cout << "Name: " << _name << "\n";
	}

	total += base::read(file, _appearanceName);
	std::cout << "Appearance name: " << _appearanceName << "\n";

	// Only present in versions 2+
	if (1 < _version) {
		total += base::read(file, _hasFloor);
		std::cout << "Has floor: " << std::boolalpha << _hasFloor << "\n";

		total += base::read(file, _floorName);
		std::cout << "Floor name: " << _floorName << "\n";
	}

	// Only present in versions 5+
	if (4 < _version) {
		// Read collision primitive...
		total += collisionUtil::read(file, _collisionPtr);
	}

	// Read portals...
	_portals.resize(_numPortals);
	for (auto& p : _portals) {
		total += p.read(file);
	}

	// Only present in versions 3+
	if (2 < _version) {
		total += base::readRecordHeader(file, "LGHT", size);
		std::cout << "Found Record LGHT: " << size << " bytes\n";

		total += base::read(file, _numLights);
		std::cout << "Num lights: " << _numLights << "\n";
		_lights.resize(_numLights);

		// Read lights...
		for (auto& light : _lights) {
			total += light.read(file);
		}
	}

	if (cellSize == total)
	{
		std::cout << "Finished reading CELL\n";
	}
	else
	{
		std::cout << "FAILED in reading CELL\n";
		std::cout << "Read " << total << " out of " << cellSize << "\n";
		exit(0);
	}

	return total;
}

const bool cell::canSeeParentCell() const {
	return _canSeeParentCell;
}

const std::string& cell::getName() const {
	return _name;
}

const std::string& cell::getAppearanceFilename() const {
	return _appearanceName;
}

const bool cell::hasFloor() const {
	return _hasFloor;
}

const std::string& cell::getFloorFilename() const {
	return _floorName;
}

const std::vector<portal>& cell::getPortals() const {
	return _portals;
}

const std::vector<lght>& cell::getLights() const {
	return _lights;
}
