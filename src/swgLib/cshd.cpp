/** -*-c++-*-
 *  \class  cshd
 *  \file   cshd.cpp
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
#include <swgLib/cshd.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

cshd::cshd()
{
}

cshd::~cshd()
{
}

unsigned int cshd::readCSHD(std::istream& file, std::string path)
{
	std::string basePath = path;
	std::size_t cshdSize;
	std::size_t total = base::readFormHeader(file, "CSHD", cshdSize);
	cshdSize += 8;
	std::cout << "Found CSHD form" << std::endl;

	std::size_t size;
	std::string form, type;
	total += base::readFormHeader(file, form, size, type);
	if (form != "FORM")
	{
		std::cout << "Expected FORM: " << form << std::endl;
		exit(0);
	}
	std::cout << "Found " << form << " " << type << std::endl;

	while (total < cshdSize)
	{
		// Peek at next record, but keep file at same place.
		base::peekHeader(file, form, size, type);

		if (form == "FORM")
		{
			if (type == "SSHT")
			{
				total += this->readSHT(file, path);
			}
			else if (type == "TFAC")
			{
				total += readTFAC(file);
			}
			else if (type == "TXTR")
			{
				total += readTXTR(file);
			}
			else
			{
				std::cout << "Unexpected form: " << type << std::endl;
				exit(0);
			}
		}
		else
		{
			std::cout << "Unexpected record: " << form << std::endl;
			exit(0);
		}
	}

	if (cshdSize == total)
	{
		std::cout << "Finished reading CSHD" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading CSHD" << std::endl;
		std::cout << "Read " << total << " out of " << cshdSize
			<< std::endl;
	}

	return total;
}

unsigned int cshd::readTFAC(std::istream& file)
{
	std::size_t tfacSize;
	std::size_t total = base::readFormHeader(file, "TFAC", tfacSize);
	tfacSize += 8; // Add size of FORM and size fields.
	std::cout << "Found FORM TFAC" << std::endl;

	while (total < tfacSize)
	{
		total += readPAL(file);
	}

	if (tfacSize == total)
	{
		std::cout << "Finished reading TFAC" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading TFAC" << std::endl;
		std::cout << "Read " << total << " out of " << tfacSize
			<< std::endl;
	}

	return total;
}

unsigned int cshd::readPAL(std::istream& file)
{
	std::size_t palSize;
	std::string type;
	std::size_t total = base::readRecordHeader(file, type, palSize);
	if (type != "PAL ")
	{
		std::cout << "Expected record of type PAL: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found " << type << std::endl;

	total += base::readUnknown(file, palSize);

	if (palSize == (total - 8))
	{
		std::cout << "Finished reading PAL" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading PAL" << std::endl;
		std::cout << "Read " << total << " out of " << palSize
			<< std::endl;
	}

	return total;
}

unsigned int cshd::readTXTR(std::istream& file)
{
	std::size_t txtrSize;
	std::size_t total = base::readFormHeader(file, "TXTR", txtrSize);
	txtrSize += 8;
	std::cout << "Found FORM TXTR: " << txtrSize - 12 << " bytes"
		<< std::endl;

	std::size_t size;
	std::string type;
	total += base::readRecordHeader(file, type, size);
	if (type != "DATA")
	{
		std::cout << "Expected record of type DATA: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found record " << type
		<< ": " << size << " bytes"
		<< std::endl;

	unsigned short numTexNames;
	total += base::read(file, numTexNames);

	for (unsigned int i = 0; i < numTexNames; ++i)
	{
		std::string texName;
		total += base::read(file, texName);
		std::cout << "Texture name: " << texName << std::endl;
	}

	total += readCUST(file);

	if (txtrSize == total)
	{
		std::cout << "Finished reading TXTR" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading TXTR" << std::endl;
		std::cout << "Read " << total << " out of " << txtrSize
			<< std::endl;
	}

	return total;
}

unsigned int cshd::readCUST(std::istream& file)
{
	std::size_t custSize;
	std::size_t total = base::readFormHeader(file, "CUST", custSize);
	custSize += 8;
	std::cout << "Found FORM CUST: " << custSize - 12 << " bytes"
		<< std::endl;

	while (total < custSize)
	{
		total += readTX1D(file);
	}

	if (custSize == total)
	{
		std::cout << "Finished reading CUST" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading CUST" << std::endl;
		std::cout << "Read " << total << " out of " << custSize
			<< std::endl;
	}

	return total;
}

unsigned int cshd::readTX1D(std::istream& file)
{
	std::size_t tx1dSize;
	std::string type;

	std::size_t total = base::readRecordHeader(file, type, tx1dSize);
	if (type != "TX1D")
	{
		std::cout << "Expected record of type TX1D: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found " << type << std::endl;

	total += base::readUnknown(file, tx1dSize);

	if (tx1dSize == (total - 8))
	{
		std::cout << "Finished reading TX1D" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading TX1D" << std::endl;
		std::cout << "Read " << total << " out of " << tx1dSize
			<< std::endl;
	}

	return total;
}
