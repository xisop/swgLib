/** -*-c++-*-
 *  \class  cclt
 *  \file   cclt.cpp
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
#include <swgLib/cclt.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

cclt::cclt()
{
}

cclt::~cclt()
{
}

std::size_t cclt::readCCLT(std::istream& file)
{
	std::size_t total = 0;
	std::string form;
	std::size_t ccltSize;
	std::string type;

	total += base::readFormHeader(file, form, ccltSize, type);
	ccltSize += 8;
	if (form != "FORM" || type != "CCLT")
	{
		std::cout << "Expected Form of type CCLT: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found CCLT form" << std::endl;

	total += readDERV(file, ccltBaseObjectFilename);

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
		total += readCCLTParameter(file);
	}

	total += readSHOT(file);


	if (ccltSize == total)
	{
		std::cout << "Finished reading CCLT" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading CCLT" << std::endl;
		std::cout << "Read " << total << " out of " << ccltSize
			<< std::endl;
		exit(0);
	}

	return total;
}

void cclt::print() const
{
}

std::size_t cclt::readCCLTParameter(std::istream& file) {
	std::cout << "readCCLTParameter not implemented\n";
	exit(0);
	return 0;
}