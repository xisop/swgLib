/** -*-c++-*-
 *  \class  clrr
 *  \file   clrr.cpp
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
#include <swgLib/clrr.hpp>
#include <swgLib/base.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

clrr::clrr()
{
}

clrr::~clrr()
{
}

std::size_t clrr::read(std::istream& file)
{
	std::size_t clrrSize;
	std::size_t total = base::readFormHeader(file, "CLRR", clrrSize);
	clrrSize += 8;
	std::cout << "Found FORM CLRR: " << clrrSize - 12 << " bytes\n";

	std::string type;
	std::size_t size;
	total += base::readRecordHeader(file, type, size);
	_version = base::tagToVersion(type);
	if (_version > 1) {
		std::cout << "Expected type [0000..0001]: " << type << "\n";
		exit(0);
	}
	std::cout << "Color Ramp Version: " << _version << "\n";

	total += base::read(file, _interpolationType);
	if (_version > 0) {
		total += base::read(file, _sampleType);
	}
	total += base::read(file, _numPoints);

	_points.resize(_numPoints);
	for (auto& p : _points) {
		total += base::read(file, p._percent);
		total += base::read(file, p._red);
		total += base::read(file, p._green);
		total += base::read(file, p._blue);
	}

	if (clrrSize == total) {
		std::cout << "Finished reading CLRR\n";
	}
	else
	{
		std::cout << "FAILED in reading CLRR\n"
			<< "Read " << total << " out of " << clrrSize << "\n";
		exit(0);
	}

	return total;
}

const uint32_t& clrr::getInterpolationType() const {
	return _interpolationType;
}

const uint32_t& clrr::getSampleType() const {
	return _sampleType;
}

const uint32_t& clrr::getNumPoints() const {
	return _numPoints;
}

const std::vector<point>& clrr::getPoints() const {
	return _points;
}

