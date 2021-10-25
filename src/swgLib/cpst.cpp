/** -*-c++-*-
 *  \class  cpst
 *  \file   cpst.cpp
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

#include <swgLib/cpst.hpp>
#include <swgLib/base.hpp>
#include <swgLib/collisionUtil.hpp>

using namespace ml;

cpst::cpst() :
	exsp() {
}

cpst::~cpst() {
}

std::size_t cpst::read(std::istream& file) {
	std::size_t cpstSize;
	std::size_t total = base::readFormHeader(file, "CPST", cpstSize);
	cpstSize += 8;
	std::cout << "Found FORM CPST: " << cpstSize - 12 << " bytes\n";

	std::size_t size;
	total += base::readFormHeader(file, "0000", size);
	std::cout << "Found FORM 0000: " << size - 4 << " bytes\n";

	// Read all collision shapes and append to list...
	while (total < cpstSize) {
		baseCollisionPtr collisionPtr;
		total += collisionUtil::read(file, collisionPtr);
		if (collisionPtr) {
			_collisionPtrs.push_back(collisionPtr);
		}
	}

	if (cpstSize == total)
	{
		std::cout << "Finished reading CPST\n";
	}
	else
	{
		std::cout << "FAILED in reading CPST\n"
			<< "Read " << total << " out of " << cpstSize << "\n";
		exit(0);
	}

	return total;
}

uint32_t cpst::getNumCollisionShapes() const {
	return (uint32_t)_collisionPtrs.size();
}
const std::vector<baseCollisionPtr>& cpst::getCollisionShapes() const {
	return _collisionPtrs;
}
