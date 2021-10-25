/** -*-c++-*-
 *  \class  xcyl
 *  \file   xcyl.cpp
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

#include <swgLib/xcyl.hpp>
#include <swgLib/base.hpp>
#include <swgLib/collisionUtil.hpp>

using namespace ml;

xcyl::xcyl() :
	exsp(),
	_base(0.0f, 0.0f, 0.0f),
	_radius(1.0f),
	_height(1.0f) {
}

xcyl::~xcyl() {
}

std::size_t xcyl::read(std::istream& file) {
	std::size_t xcylSize;
	std::size_t total = base::readFormHeader(file, "XCYL", xcylSize);
	xcylSize += 8;
	std::cout << "Found FORM XCYL: " << xcylSize - 12 << " bytes\n";

	std::size_t size;
	total += base::readFormHeader(file, "0000", size);
	std::cout << "Found FORM 0000: " << size - 4 << " bytes\n";

	total += base::readRecordHeader(file, "CYLN", size);

	total += base::read(file, _base);
	total += base::read(file, _radius);
	total += base::read(file, _height);
	std::cout
		<< "Base: " << _base << "\n"
		<< "Radius: " << _radius << "\n"
		<< "Height: " << _height << "\n";

	if (xcylSize == total)
	{
		std::cout << "Finished reading XCYL\n";
	}
	else
	{
		std::cout << "FAILED in reading XCYL\n"
			<< "Read " << total << " out of " << xcylSize << "\n";
		exit(0);
	}

	return total;
}
