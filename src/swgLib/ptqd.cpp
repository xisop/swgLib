/** -*-c++-*-
 *  \class  ptqd
 *  \file   ptqd.cpp
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
#include <swgLib/ptqd.hpp>
#include <swgLib/base.hpp>

#include <iostream>

using namespace ml;

ptqd::ptqd()
{
}

ptqd::~ptqd()
{
}

std::size_t ptqd::read(std::istream& file)
{
	std::size_t ptqdSize;
	std::size_t total = base::readFormHeader(file, "PTQD", ptqdSize);
	ptqdSize += 8;
	std::cout << "Found PTQD: " << ptqdSize - 12 << " bytes\n";

	total += ptcl::read(file);

	std::string type;
	std::size_t size;
	total += base::readFormHeader(file, type, size);
	_version = base::tagToVersion(type);
	if (_version > 3) {
		std::cout << "Expected type [0000..0003]: " << type << "\n";
		exit(0);
	}
	std::cout << "Particle Description Quad Version: " << _version << "\n";

	total += _rotation.read(file);
	total += _length.read(file);
	total += _width.read(file);
	total += _particleTexture.read(file);

	if (_version > 0) {
		total += base::readRecordHeader(file, "0000", size);
		total += base::read(file, _aspectLocked);
	}

	if (ptqdSize == total) {
		std::cout << "Finished reading PTQD\n";
	}
	else {
		std::cout << "FAILED in reading PTQD\n"
			<< "Read " << total << " out of " << ptqdSize << "\n";
		exit(0);
	}

	return total;
}
