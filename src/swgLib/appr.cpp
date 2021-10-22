/** -*-c++-*-
 *  \class  appr
 *  \file   appr.cpp
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

#include <swgLib/appr.hpp>
#include <swgLib/exbx.hpp>
#include <swgLib/base.hpp>

using namespace ml;

appr::appr() {
}

appr::~appr() {
}

std::size_t appr::read(std::istream& file) {
	std::size_t apprSize;
	std::size_t total = base::readFormHeader(file, "APPR", apprSize);
	apprSize += 8;
	std::cout << "Found APPR form: " << apprSize << " bytes\n";

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	_apprVersion = base::tagToVersion(type);
	if ((_apprVersion < 1) || (3 < _apprVersion)) {
		std::cout << "Expected type [0001..0003]: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found form of type: " << type << std::endl;

	// Load extents (box/sphere/mesh)...
	total += readEXBX(file);

	if (3 == _apprVersion) {
		std::string form;
		base::peekHeader(file, form, size, type);
		std::cout << "Peek: " << form << ", " << type << "\n";
		if ("NULL" == type) {
			total += base::readFormHeader(file, "NULL", size);
		}
		else {
			// Load collision extents...
			total += _collisionMesh.read(file);
		}
	}

	// Load hard points
	total += readHPTS(file);

	if (_apprVersion > 1) {
		// Load floors...
		total += readFLOR(file);
	}

	if (apprSize == total)
	{
		std::cout << "Finished reading APPR\n";
	}
	else
	{
		std::cout << "Failed in reading APPR\n";
		std::cout << "Read " << total << " out of " << apprSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t appr::readEXBX(std::istream& file) {
	return exbx::read(file, _exspCenter, _exspRadius, _exbxCorner1, _exbxCorner2);
}

std::size_t appr::readHPTS(std::istream& file) {
	std::size_t hptsSize;
	std::size_t total = base::readFormHeader(file, "HPTS", hptsSize);
	hptsSize += 8;
	std::cout << "Found HPTS form: " << hptsSize << " bytes\n";

	while (total < hptsSize) {
		std::size_t hpntSize;
		total += base::readRecordHeader(file, "HPNT", hpntSize);
		std::cout << "Found HPNT form: " << hpntSize << " bytes\n";
		std::string name;
		matrix3x4 mat;
		total += base::read(file, mat);
		total += base::read(file, name);
		std::cout << "Hardpoint " << _hardpointName.size() << ":\n"
			<< "Name: " << name << "\n"
			<< "Transform: \n" << mat << "\n";
		_hardpointTransform.push_back(mat);
		_hardpointName.push_back(name);
	}

	if (hptsSize == total)
	{
		std::cout << "Finished reading HPTS" << std::endl;
	}
	else
	{
		std::cout << "Failed in reading hpts" << std::endl;
		std::cout << "Read " << total << " out of " << hptsSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t appr::readFLOR(std::istream& file) {
	std::size_t florSize;
	std::size_t total = base::readFormHeader(file, "FLOR", florSize);
	florSize += 8;
	std::cout << "Found FLOR form: " << florSize << " bytes\n";

	std::size_t dataSize;
	total += base::readRecordHeader(file, "DATA", dataSize);

	total += base::read(file, _hasFloor);
	std::cout << "Has floor: " << std::boolalpha << _hasFloor << "\n";

	if (_hasFloor) {
		total += base::read(file, _floorName);
		std::cout << "Floor name: " << _floorName << "\n";
	}

	if (florSize == total)
	{
		std::cout << "Finished reading FLOR\n";
	}
	else
	{
		std::cout << "Failed in reading FLOR" << std::endl;
		std::cout << "Read " << total << " out of " << florSize << "\n";
		exit(0);
	}

	return total;
}

void appr::getBoundingSphere(vector3& center, float& radius) const
{
	center = _exspCenter;
	radius = _exspRadius;
}

void appr::getBoundingBox(vector3& corner1, vector3& corner2)
{
	corner1 = _exbxCorner1;
	corner2 = _exbxCorner2;
}

