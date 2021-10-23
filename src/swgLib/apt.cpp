/** -*-c++-*-
 *  \class  apt
 *  \file   apt.cpp
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
#include <swgLib/apt.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

apt::apt()
{
}

apt::~apt()
{
}

std::size_t apt::readAPT(std::istream& file)
{
	std::size_t aptSize;
	std::size_t total = base::readFormHeader(file, "APT ", aptSize);
	aptSize += 8;
	std::cout << "Found form APT: " << aptSize << "\n";

	total += base::readFormHeader(file, "0000", aptSize);

	std::size_t nameSize;
	total += base::readRecordHeader(file, "NAME", nameSize);
	nameSize += 8;

	total += base::read(file, _filename);
	std::cout << "Name: " << _filename << "\n";

	if (aptSize == total) {
		std::cout << "Finished reading APT\n";
	}
	else {
		std::cout << "FAILED in reading APT\n"
			<< "Read " << total << " out of " << aptSize << "\n";
		exit(0);
	}

	return total;
}

void apt::print() const
{
}

std::string apt::getFilename() const {
	return _filename;
}
