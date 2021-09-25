/** -*-c++-*-
 *  \class  portal
 *  \file   portal.cpp
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
#include <swgLib/portal.hpp>

#include <ios>

using namespace ml;

portal::portal() {
}

portal::~portal() {
}

std::size_t portal::read(std::istream& file) {
	std::size_t prtlSize;
	std::size_t total = base::readFormHeader(file, "PRTL", prtlSize);
	prtlSize += 8;
	std::cout << "Found FORM PRTL: " << prtlSize - 12 << " bytes\n";

	std::size_t size;
	std::string type;
	total += base::readRecordHeader(file, type, size);

	_version = 0;
	if ("0001" == type) {
		_version = 1;
	}
	else if ("0002" == type) {
		_version = 2;
	}
	else if ("0003" == type) {
		_version = 3;
	}
	else if ("0004" == type) {
		_version = 4;
	}
	else if ("0005" == type) {
		_version = 5;
	}
	else {
		std::cout << "Expected record of type 0001, 0002, 0003, 0004, or 0005: " << type << "\n";
		exit(0);
	}
	std::cout << "Found record " << type << ": " << size << " bytes\n";

	std::cout << "Portal version: " << (int)_version << "\n";

	if (5 == _version) {
		total += base::read(file, _disabled);
		std::cout << "Disabled: " << std::boolalpha << _disabled << "\n";
	}

	if (1 < _version) {
		total += base::read(file, _passable);
		std::cout << "Passable: " << std::boolalpha << _passable << "\n";
	}

	total += base::read(file, _geometryIndex);
	std::cout << "Geometry index: " << _geometryIndex << "\n";

	total += base::read(file, _geometryWindClockwise	);
	std::cout << "Geometry wind clockwise: " << std::boolalpha << _geometryWindClockwise << "\n";

	// Portal Geometry...
	//... getPortalGeometry(int portalIndex, int cell, int cellPortalIndex, bool clockwise) 

	total += base::read(file, _targetCellIndex );
	std::cout << "Target cell index: " << _targetCellIndex << "\n";

	if (2 < _version) {
		total += base::read(file, _doorStyle);
		std::cout << "Door style: " << _doorStyle << "\n";
	}

	if( 3 < _version){
		total += base::read(file, _hasDoorHardpoint);
		std::cout << "Has door hardpoint: " << std::boolalpha << _hasDoorHardpoint << "\n";
		total += base::read(file, _doorHardpointTransform);
		std::cout << "Door hardpoint transform:\n" << _doorHardpointTransform << "\n";
	}

	if (prtlSize == total)
	{
		std::cout << "Finished reading PRTL" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading PRTL" << std::endl;
		std::cout << "Read " << total << " out of " << prtlSize << "\n";
	}
	return total;
}

//void setPortalID(const unsigned int& id);
//unsigned int getPortalID() const;

void portal::setDisabled(bool disabled ) { _disabled = disabled; }
bool portal::isDisabled() const { return _disabled; }

void portal::setPassable(bool passable ) { _passable = passable; }
bool portal::isPassable() const { return _passable; }

void portal::setGeometryIndex(const int32_t& geomIndex) { _geometryIndex = geomIndex; }
const int32_t& portal::getGeometryIndex() const { return _geometryIndex; }

void portal::setGeometryClockwise(bool clockwise ) { _geometryWindClockwise = clockwise; }
bool portal::isGeometryClockwise() const { return _geometryWindClockwise; }

void portal::setTargetCellIndex(const int32_t& index) { _targetCellIndex = index; }
const int32_t& portal::getTargetCellIndex() const { return _targetCellIndex; }

void portal::setDoorStyle(const std::string& doorStyle) { _doorStyle = doorStyle; }
const std::string& portal::getDoorStyle() const { return _doorStyle; }

void portal::setHasDoorHardpoint(bool hasDoorHardpoint ) { _hasDoorHardpoint = hasDoorHardpoint; }
bool portal::hasDoorHardpoint() const { return _hasDoorHardpoint; }

void portal::setDoorHardpointTransform(const matrix3x4& matrix) { _doorHardpointTransform = matrix; }
const matrix3x4& portal::getDoorHardpointTransformMatrix() const { return _doorHardpointTransform; }

void portal::print() const
{
	/*
	std::cout << "Portal: " << portalId << std::endl;
	std::cout << "Portal geometry index: " << portalGeometryIndex
		<< std::endl;
	std::cout << "Destination cell: " << destinationCell << std::endl;
	std::cout << "Portal model filename: " << portalModelFilename
		<< std::endl;
	std::cout << "Portal matrix: " << std::endl;
	mat.print();

	std::cout << "Portal position: ";
	pos.print();
	*/
}
