/** -*-c++-*-
 *  \class  dtal
 *  \file   dtal.cpp
 *  \author Ken Sewell

 swgLib is used for the parsing and exporting SWG models.
 Copyright (C) 2009-2021 Ken Sewell

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

#include <swgLib/dtal.hpp>
#include <swgLib/base.hpp>

using namespace ml;

dtal::dtal() :
	cpst() {
}

dtal::~dtal() {}

std::size_t dtal::read(std::istream& file) {
	std::size_t dtalSize;
	std::size_t total = base::readFormHeader(file, "DTAL", dtalSize);
	dtalSize += 8;
	std::cout << "Found DTAL form: " << dtalSize - 12 << " bytes\n";

	std::size_t size;
	total += base::readFormHeader(file, "0000", size);

	total += cpst::read(file);

	if (total == dtalSize)
	{
		std::cout << "Finished reading DTAL.\n";
	}
	else
	{
		std::cout << "Error reading DTAL!\n"
			<< "Read " << total << " out of " << dtalSize << "\n";
		exit(0);
	}

	return total;
}
