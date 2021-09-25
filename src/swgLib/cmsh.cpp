/** -*-c++-*-
 *  \class  cmsh
 *  \file   cmsh.cpp
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

#include <swgLib/cmsh.hpp>
#include <swgLib/base.hpp>

using namespace ml;

cmsh::cmsh() {
}

cmsh::~cmsh() {
}

std::size_t cmsh::read(std::istream& file) {
	std::size_t cmshSize;
	std::size_t total = base::readFormHeader(file, "CMSH", cmshSize);
	cmshSize += 8;
	std::cout << "Found FORM CMSH: "
		<< cmshSize - 12 << " bytes\n";

	std::size_t size;
	std::string form, type;
	total += base::readFormHeader(file, "0000", size );

	std::cout << "Found FORM 0000: " << size - 4 << " bytes\n";

	total += _idtl.read(file);

	if (cmshSize == total)
	{
		std::cout << "Finished reading CMSH" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading CMSH" << std::endl;
		std::cout << "Read " << total << " out of " << cmshSize
			<< std::endl;
	}

	return total;
}
