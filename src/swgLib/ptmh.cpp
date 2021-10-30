/** -*-c++-*-
 *  \class  ptmh
 *  \file   ptmh.cpp
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
#include <swgLib/ptmh.hpp>
#include <swgLib/base.hpp>

#include <iostream>

using namespace ml;

ptmh::ptmh()
{
}

ptmh::~ptmh()
{
}

std::size_t ptmh::read(std::istream& file)
{
	std::size_t ptmhSize;
	std::size_t total = base::readFormHeader(file, "PTMH", ptmhSize);
	ptmhSize += 8;
	std::cout << "Found PTMH: " << ptmhSize - 12 << " bytes\n";

	total += ptcl::read(file);

	std::string type;
	std::size_t size;
	total += base::readFormHeader(file, type, size);
	_version = base::tagToVersion(type);
	if (_version > 0) {
		std::cout << "Expected type [0000]: " << type << "\n";
		exit(0);
	}
	std::cout << "Particle Description Mesh Version: " << _version << "\n";

	if (_version > 0) {
		total += base::readRecordHeader(file, "0000", size);
		total += base::read(file, _meshPath);
	}
	
	total += _scale.read(file);
	total += _rotationX.read(file);
	total += _rotationY.read(file);
	total += _rotationZ.read(file);

	if (ptmhSize == total) {
		std::cout << "Finished reading PTMH\n";
	}
	else {
		std::cout << "FAILED in reading PTMH\n"
			<< "Read " << total << " out of " << ptmhSize << "\n";
		exit(0);
	}

	return total;
}
