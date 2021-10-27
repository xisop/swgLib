/** -*-c++-*-
 *  \class  mesh
 *  \file   mesh.cpp
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

#include <swgLib/mesh.hpp>
#include <swgLib/cshd.hpp>
#include <swgLib/sht.hpp>
#include <swgLib/collisionUtil.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

mesh::mesh() :
	appr(),
	_version(0),
	_sps()
{
}

mesh::~mesh() {
}

std::size_t mesh::readMESH(std::istream& file, bool skipSIDX)
{
	std::size_t meshSize;
	std::size_t total = base::readFormHeader(file, "MESH", meshSize);
	meshSize += 8;
	std::cout << "Found form MESH: " << meshSize << "\n";

	std::string type;
	std::size_t size;
	total += base::readFormHeader(file, type, size);
	std::cout << "Found FORM " << type << ": " << size << " bytes\n";

	_version = base::tagToVersion(type);
	if ((_version < 2) || (_version > 5)) {
		std::cout << "Expected FORM of type 0002, 0003, 0004, or 0005. Found: " << type << "\n";
		exit(0);
	}

	if (_version < 4) {
		// Load shader primitive set...
		total += _sps.read(file, skipSIDX);

		// Load bounding sphere...
		total += _boundSphere.readOld(file);

		// Load extents (box/sphere/mesh)...
		total += collisionUtil::read(file, _boundingPtr);

		if (total < meshSize) {
			// Load hardpoints...
			total += appr::readHPTS(file);
		}
	}
	else {
		total += appr::read(file);
		// TODO: Copy bounding sphere data from APPR to _boundSphere
		total += _sps.read(file, skipSIDX);
	}

	if (total == meshSize)
	{
		std::cout << "Finished reading MESH.\n";
	}
	else
	{
		std::cout << "Error reading MESH!\n"
			<< "Read " << total << " out of " << meshSize << "\n";
		exit(0);
	}

	return total;
}

const sps& mesh::getSPS() const { return _sps; }
