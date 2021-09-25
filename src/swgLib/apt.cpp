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

/**
   This class still needs work.
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
	std::size_t total = 0;
	std::string form;
	std::size_t aptSize;
	std::string type;

	total += readFormHeader(file, form, aptSize, type);
	aptSize += 8;
	if (form != "FORM" || type != "APT ")
	{
		std::cout << "Expected Form of type APT: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found APT form" << std::endl;

	std::size_t size;
	total += readFormHeader(file, form, size, type);
	if (form != "FORM")
	{
		std::cout << "Expected Form" << std::endl;
		exit(0);
	}
	std::cout << "Found form of type: " << type << std::endl;

	total += readNAME(file, childFilename);

	if (aptSize == total)
	{
		std::cout << "Finished reading APT" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading APT" << std::endl;
		std::cout << "Read " << total << " out of " << aptSize
			<< std::endl;
	}

	return total;
}

std::size_t apt::readNAME(std::istream& file, std::string& filename)
{
	std::size_t total = 0;
	std::string form;
	std::string type;

	std::size_t nameSize;
	total += readRecordHeader(file, type, nameSize);
	nameSize += 8;
	if (type != "NAME")
	{
		std::cout << "Expected record of type NAME: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found " << type << std::endl;

	char temp[255];
	file.getline(temp, 255, 0);
	filename = temp;
	total += filename.size() + 1;

	if (nameSize == total)
	{
		std::cout << "Finished reading NAME" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading NAME" << std::endl;
		std::cout << "Read " << total << " out of " << nameSize
			<< std::endl;
	}

	return total;
}

void apt::print() const
{
}

