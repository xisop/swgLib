/** -*-c++-*-
 *  \class  stot
 *  \file   stot.cpp
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
#include <swgLib/stot.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

stot::stot()
{
}

stot::~stot()
{
}

unsigned int stot::readSTOT(std::istream& file)
{
	std::size_t stotSize;
	std::size_t total = readFormHeader(file, "STOT", stotSize);
	stotSize += 8;
	std::cout << "Found STOT form" << std::endl;

	total += readDERV(file, stotBaseObjectFilename);

	std::size_t size;
	std::string form, type;
	total += readFormHeader(file, form, size, type);
	size += 8;
	if (form != "FORM")
	{
		std::cout << "Expected FORM: " << form << std::endl;
		exit(0);
	}
	std::cout << "Found FORM:" << type << std::endl;

	total += readPCNT(file, numNodes);
	for (unsigned int i = 0; i < numNodes; ++i)
	{
		total += readSTOTXXXX(file);
	}

	total += readSHOT(file);

	if (stotSize == total)
	{
		std::cout << "Finished reading STOT" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading STOT" << std::endl;
		std::cout << "Read " << total << " out of " << stotSize
			<< std::endl;
	}

	return total;
}

void stot::print() const
{
}

unsigned int stot::readSTOTXXXX(std::istream& file)
{
	std::size_t xxxxSize;
	std::string type;
	std::size_t total = readRecordHeader(file, type, xxxxSize);
	if (type != "XXXX")
	{
		std::cout << "Expected record of type XXXX: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found " << type << std::endl;

	std::string property;
	total += base::read(file, property);
	std::cout << "Property: " << property << std::endl;

	unsigned char enabled;
	if (property == "paletteColorCustomizationVariables")
	{
		total += base::read(file, enabled);
		if (enabled > 0) {
			unsigned char junk1, junk2, junk3, junk4;
			total += base::read(file, junk1);
			total += base::read(file, junk2);
			total += base::read(file, junk3);
			total += base::read(file, junk4);
		}
	}
	else if (property == "rangedIntCustomizationVariables")
	{
		total += base::read(file, enabled);
		if (enabled > 0) {
			unsigned char junk1, junk2, junk3, junk4;
			total += base::read(file, junk1);
			total += base::read(file, junk2);
			total += base::read(file, junk3);
			total += base::read(file, junk4);
		}
	}
	else if (property == "constStringCustomizationVariables")
	{
		total += base::read(file, enabled);
		if (enabled > 0) {
			unsigned char junk1, junk2, junk3, junk4;
			total += base::read(file, junk1);
			total += base::read(file, junk2);
			total += base::read(file, junk3);
			total += base::read(file, junk4);
		}
	}
	else if (property == "socketDestinations")
	{
		total += base::read(file, enabled);
		if (enabled > 0) {
			unsigned char junk1, junk2, junk3, junk4;
			total += base::read(file, junk1);
			total += base::read(file, junk2);
			total += base::read(file, junk3);
			total += base::read(file, junk4);
		}
	}
	else if (property == "structureFootprintFileName")
	{
		total += base::read(file, enabled);
		if (enabled > 0) {
			unsigned char junk1, junk2, junk3, junk4;
			total += base::read(file, junk1);
			total += base::read(file, junk2);
			total += base::read(file, junk3);
			total += base::read(file, junk4);
		}
	}
	else if (property == "useStructureFootprintOutline")
	{
		total += base::read(file, enabled);
		if (enabled > 0) {
			unsigned char junk1, junk2, junk3, junk4;
			total += base::read(file, junk1);
			total += base::read(file, junk2);
			total += base::read(file, junk3);
			total += base::read(file, junk4);
		}
	}
	else if (property == "targetable")
	{
		total += base::read(file, enabled);
		if (enabled > 0) {
			unsigned char junk1, junk2, junk3, junk4;
			total += base::read(file, junk1);
			total += base::read(file, junk2);
			total += base::read(file, junk3);
			total += base::read(file, junk4);
		}
	}
	else if (property == "certificationsRequired")
	{
		total += base::read(file, enabled);
		if (enabled > 0) {
			unsigned char junk1, junk2, junk3, junk4;
			total += base::read(file, junk1);
			total += base::read(file, junk2);
			total += base::read(file, junk3);
			total += base::read(file, junk4);
		}
	}
	else if (property == "customizationVariableMapping")
	{
		total += base::read(file, enabled);
		if (enabled > 0) {
			unsigned char junk1, junk2, junk3, junk4;
			total += base::read(file, junk1);
			total += base::read(file, junk2);
			total += base::read(file, junk3);
			total += base::read(file, junk4);
		}
	}
	else
	{
		std::cout << "Unknown: " << property << std::endl;
		exit(0);
	}

	if (xxxxSize == (total - 8))
	{
		std::cout << "Finished reading XXXX" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading XXXX" << std::endl;
		std::cout << "Read " << total << " out of " << xxxxSize
			<< std::endl;
	}

	return total;
}
