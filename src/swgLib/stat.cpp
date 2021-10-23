/** -*-c++-*-
 *  \class  stat
 *  \file   stat.cpp
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

/**
   This class still needs work.
 */

#include <swgLib/base.hpp>
#include <swgLib/stat.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

ml::stat::stat()
{
}

ml::stat::~stat()
{
}

std::size_t ml::stat::readSTAT(std::istream& file)
{
	std::size_t statSize;
	std::size_t total = base::readFormHeader(file, "STAT", statSize);
	statSize += 8;
	std::cout << "Found STAT form" << std::endl;

	// Peek at next record, but keep file at same place.
	std::size_t size;
	std::string form, type;
	base::peekHeader(file, form, size, type);

	if ("DERV" == type)
	{
		total += readDERV(file, statBaseObjectFilename);
	}

	std::size_t size0000;
	total += base::readFormHeader(file, form, size0000, type);
	size0000 += 8;
	if (form != "FORM" || type != "0000")
	{
		std::cout << "Expected Form of type 0000: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found 0000 form" << std::endl;

	int32_t numParameters;
	total += readPCNT(file, numParameters);
	for (int32_t i = 0; i < numParameters; ++i)
	{
		total += readSTATParameter(file);
	}

	total += readSHOT(file);

	if (statSize == total)
	{
		std::cout << "Finished reading STAT" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading STAT" << std::endl;
		std::cout << "Read " << total << " out of " << statSize
			<< std::endl;
		exit(0);
	}

	return total;
}

void ml::stat::print() const
{
}

std::size_t ml::stat::readSTATParameter(std::istream& file) {
	std::cout << "readSTATParameter not implemented\n";
	exit(0);
	return 0;
}