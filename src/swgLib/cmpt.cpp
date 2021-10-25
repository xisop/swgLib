/** -*-c++-*-
 *  \class  cmpt
 *  \file   cmpt.cpp
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

#include <swgLib/cmpt.hpp>
#include <swgLib/base.hpp>

using namespace ml;

cmpt::cmpt() {
}

cmpt::~cmpt() {
}

std::size_t cmpt::read(std::istream& file) {
	std::size_t cmptSize;
	std::size_t total = base::readFormHeader(file, "CMPT", cmptSize);
	cmptSize += 8;
	std::cout << "Found FORM CMPT: " << cmptSize - 12 << " bytes\n";

	std::size_t size;
	total += base::readFormHeader(file, "0000", size);
	std::cout << "Found FORM 0000: " << size - 4 << " bytes\n";

	total += cpst::read(file);

	if (cmptSize == total)
	{
		std::cout << "Finished reading CMPT\n";
	}
	else
	{
		std::cout << "FAILED in reading CMPT\n"
			<< "Read " << total << " out of " << cmptSize << "\n";
		exit(0);
	}

	return total;
}
