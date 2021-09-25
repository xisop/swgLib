/** -*-c++-*-
 *  \class  swts
 *  \file   swts.cpp
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
#include <swgLib/swts.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

swts::swts()
{
}

swts::~swts()
{
}

unsigned int swts::readSWTS(std::istream& file, std::string path)
{
	basePath = path;
	std::size_t swtsSize;
	std::string type;

	std::size_t total = readFormHeader(file, "SWTS", swtsSize);
	swtsSize += 8;
	std::cout << "Found SWTS form" << std::endl;

	std::size_t size;
	std::string form;
	total += readFormHeader(file, form, size, type);
	if (form != "FORM")
	{
		std::cout << "Expected FORM: " << form << std::endl;
		exit(0);
	}
	std::cout << "Found " << form << " " << type << std::endl;

	while (total < swtsSize)
	{
		// Peek at next record, but keep file at same place.
		peekHeader(file, form, size, type);

		if (form == "FORM")
		{
			if (type == "DTST")
			{
				total += readDTST(file);
			}
			else if (type == "DRTS")
			{
				total += readDRTS(file);
			}
			else
			{
				std::cout << "Unexpected form: " << type << std::endl;
				exit(0);
			}
		}
		else if (form == "TEXT")
		{
			total += readTEXT(file);
		}
		else if (form == "NAME")
		{
			total += readNAME(file);
		}
		else
		{
			std::cout << "Unexpected record: " << form << std::endl;
			exit(0);
		}
	}

	if (swtsSize == total)
	{
		std::cout << "Finished reading SWTS" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading SWTS" << std::endl;
		std::cout << "Read " << total << " out of " << swtsSize
			<< std::endl;
	}

	return total;
}

unsigned int swts::readDTST(std::istream& file)
{
	std::size_t dtstSize;
	std::string type;

	std::size_t total = readFormHeader(file, "DTST", dtstSize);
	dtstSize += 8; // Add size of FORM and size fields.
	std::cout << "Found FORM " << type << std::endl;

	std::size_t size;
	total += readRecordHeader(file, type, size);
	if (type != "0000")
	{
		std::cout << "Expected record of type 0000: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found " << type << std::endl;

	if (size != 8)
	{
		std::cout << "Expected size of 8: " << size << std::endl;
	}

	total += base::read(file, numTextures);
	total += base::read(file, deltaTime);

	std::cout << "Num textures: " << numTextures << std::endl;
	std::cout << "Delta time: " << deltaTime << std::endl;

	sequenceTime = deltaTime * numTextures;

	if (dtstSize == total)
	{
		std::cout << "Finished reading DTST" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading DTST" << std::endl;
		std::cout << "Read " << total << " out of " << dtstSize
			<< std::endl;
	}

	return total;
}

unsigned int swts::readDRTS(std::istream& file)
{
	std::size_t dtstSize;
	std::string type;

	std::size_t total = readFormHeader(file, "DRTS", dtstSize);
	dtstSize += 8; // Add size of FORM and size fields.
	std::cout << "Found FORM " << type << std::endl;

	std::size_t size;
	total += readRecordHeader(file, type, size);
	if (type != "0000")
	{
		std::cout << "Expected record of type 0000: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found " << type << std::endl;

	if (size != 12)
	{
		std::cout << "Expected size of 12: " << size << std::endl;
	}

	float u1;

	total += base::read(file, numTextures);
	total += base::read(file, deltaTime);
	total += base::read(file, u1);

	std::cout << "Num textures: " << numTextures << std::endl;
	std::cout << "Delta time: " << deltaTime << std::endl;
	std::cout << "Unknown: " << u1 << std::endl;

	sequenceTime = deltaTime * numTextures;

	if (dtstSize == total)
	{
		std::cout << "Finished reading DRTS" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading DRTS" << std::endl;
		std::cout << "Read " << total << " out of " << dtstSize
			<< std::endl;
	}

	return total;
}

unsigned int swts::readNAME(std::istream& file)
{
	std::size_t nameSize;
	std::string type;

	std::size_t total = readRecordHeader(file, type, nameSize);
	nameSize += 8;
	if (type != "NAME")
	{
		std::cout << "Expected record of type NAME: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found " << type << std::endl;

	total += base::read(file, shaderFilename);

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

unsigned int swts::readTEXT(std::istream& file)
{
	std::size_t textSize;
	std::string type;

	std::size_t total = readRecordHeader(file, type, textSize);
	textSize += 8;
	if (type != "TEXT")
	{
		std::cout << "Expected record of type TEXT: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found " << type << std::endl;

	char temp[255];
	file.read(temp, 4);
	total += 4;
	temp[4] = 0;
	textureTagList.push_back(std::string(temp));

	std::string textName(temp);
	total += base::read(file, textName);
	textureList.push_back(textName);

	if (textSize == total)
	{
		std::cout << "Finished reading TEXT" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading TEXT" << std::endl;
		std::cout << "Read " << total << " out of " << textSize
			<< std::endl;
	}

	return total;
}

void swts::print() const
{
	for (unsigned int i = 0; i < textureList.size(); ++i)
	{
		std::cout << i << ": " << textureTagList[i] << " "
			<< textureList[i] << std::endl;
	}
}

bool swts::getTextureInfo(const unsigned int& index,
	std::string& textureFilename,
	std::string& textureTag) const
{
	if (index >= getNumTextures())
	{
		return false;
	}

	textureFilename = textureList[index];
	textureTag = textureTagList[index];

	return true;
}

