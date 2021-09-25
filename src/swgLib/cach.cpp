/** -*-c++-*-
 *  \class  cach
 *  \file   cach.cpp
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

#include <swgLib/cach.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

cach::cach()
{
}

cach::~cach()
{
}

std::size_t cach::readCACH(std::istream& file, std::string path)
{
	basePath = path;
	std::size_t total = 0;
	std::string form;
	std::size_t cachSize;
	std::string type;

	total += readFormHeader(file, "CACH", cachSize);
	cachSize += 8;
	std::cout << "Found CACH form" << std::endl;

	std::size_t size;
	total += readRecordHeader(file, type, size);
	if (type != "0000")
	{
		std::cout << "Expected type " << std::endl;
		exit(0);
	}
	std::cout << "Found type: " << type << std::endl;

	std::string name;
	while (total < cachSize)
	{
		total += base::read(file, name);
		cacheList.push_back(name);
		std::cout << name << std::endl;
	}

	if (cachSize == total)
	{
		std::cout << "Finished reading CACH" << std::endl;
	}
	else
	{
		std::cout << "Failed in reading CACH" << std::endl;
		std::cout << "Read " << total << " out of " << cachSize
			<< std::endl;
	}

	return total;
}

