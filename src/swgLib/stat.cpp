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

#include <swgLib/base.hpp>
#include <swgLib/stat.hpp>

#include <iostream>
#include <cstdlib>

ml::stat::stat() :
	shot(),
	_statVersion(0),
	_statBaseObjectFilename("")
{
}

ml::stat::~stat()
{
}

std::size_t ml::stat::readSTAT(std::istream& file)
{
	// Remember start of stream in case type is wrong...
	std::size_t streamStart = file.tellg();

	std::size_t statSize;
	std::size_t total = base::readFormHeader(file, "STAT", statSize);
	statSize += 8;
	std::cout << "Found STAT form\n";

	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);

	if ("DERV" != type) {
		std::cout << "Shared Static Object Template (STAT) file not detected\n";
		if ("0000" == type) {
			std::cout << "File appears to be Space Terrain (STAT) type.\n";
		}
		std::cout << "Resetting file pointer\n";
		file.seekg(streamStart);
		return 0;
	}

	total += readDERV(file, _statBaseObjectFilename);
	std::cout << "STAT Base object filename: " << _statBaseObjectFilename << "\n";

	total += base::readFormHeader(file, type, size);
	_statVersion = base::tagToVersion(type);
	std::cout << "STAT version: " << (int)_statVersion << "\n";

	int32_t numParameters;
	total += readPCNT(file, numParameters);
	std::cout << "Num parameters: " << numParameters << "\n";

	// Doesn't look like STAT supports parameters..
	//for (int32_t i = 0; i < numParameters; ++i)	{}

	total += readSHOT(file);

	if (statSize == total)
	{
		std::cout << "Finished reading STOT\n";
	}
	else
	{
		std::cout << "FAILED in reading STAT\n"
			<< "Read " << total << " out of " << statSize << "\n";
		exit(0);
	}

	return total;
}

void ml::stat::print(std::ostream& os) const
{
}
