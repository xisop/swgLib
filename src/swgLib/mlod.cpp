/** -*-c++-*-
 *  \class  mlod
 *  \file   mlod.cpp
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

#include <swgLib/mlod.hpp>
#include <swgLib/mesh.hpp>
#include <swgLib/cshd.hpp>
#include <swgLib/sht.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

mlod::mlod()
{
}

mlod::~mlod()
{
}

std::size_t mlod::readMLOD(std::istream& file, const std::string& path)
{
	basePath = path;
	std::size_t mlodSize;
	std::size_t total = readFormHeader(file, "MLOD", mlodSize);
	mlodSize += 8;
	std::cout << "Found MLOD form" << std::endl;

	std::size_t size;
	total += readFormHeader(file, "0000", size);

	std::cout << "Found FORM 0000" << std::endl;

	unsigned short numNames;
	total += readINFO(file, numNames);

	std::cout << "Num names: " << numNames << std::endl;

	for (unsigned int i = 0; i < numNames; ++i)
	{
		std::string name;
		total += readNAME(file, name);
		meshFilename.push_back(name);
		std::cout << "Name: " << name << std::endl;
	}

	if (mlodSize == total)
	{
		std::cout << "Finished reading MLOD" << std::endl;
	}
	else
	{
		std::cout << "Failed in reading MLOD" << std::endl;
		std::cout << "Read " << total << " out of " << mlodSize
			<< std::endl;
	}

	return total;
}

std::size_t mlod::readINFO(std::istream& file, unsigned short& num)
{
	std::size_t size;
	std::string type;

	std::size_t total = readRecordHeader(file, type, size);
	if (type != "INFO")
	{
		std::cout << "Expected record of type INFO: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found INFO record" << std::endl;

	total += base::read(file, num);

	return total;
}

std::size_t mlod::readNAME(std::istream& file, std::string& name)
{
	std::size_t size;
	std::string type;
	std::size_t total = readRecordHeader(file, type, size);
	if (type != "NAME")
	{
		std::cout << "Expected record of type NAME: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found NAME record" << std::endl;

	total += base::read(file, name);

	return total;
}
