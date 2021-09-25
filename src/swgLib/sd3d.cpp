/** -*-c++-*-
 *  \class  sd3d
 *  \file   sd3d.cpp
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
#include <swgLib/sd3d.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

sd3d::sd3d()
{
}

sd3d::~sd3d()
{
}

unsigned int sd3d::readSD3D(std::istream& file)
{
	std::size_t sd3dSize;
	std::size_t total = readFormHeader(file, "SD3D", sd3dSize);
	sd3dSize += 8;
	std::cout << "Found SD3D form" << std::endl;

	total += readSD2D(file);
	total += read0001(file);

	if (sd3dSize == total)
	{
		std::cout << "Finished reading SD3D" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading SD3D" << std::endl;
		std::cout << "Read " << total << " out of " << sd3dSize
			<< std::endl;
	}

	return total;
}

unsigned int sd3d::read0001(std::istream& file)
{
	std::string type;
	std::size_t size;
	std::size_t total = readRecordHeader(file, type, size);
	size += 8;
	if (type != "0001")
	{
		std::cout << "Expected record of type 0001: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found " << type << std::endl;

	if (size == total)
	{
		std::cout << "Finished reading 0001" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading 0001" << std::endl;
		std::cout << "Read " << total << " out of " << size
			<< std::endl;
	}

	return total;
}

void sd3d::print() const
{
}

