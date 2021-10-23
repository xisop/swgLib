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

std::size_t stot::readSTOT(std::istream& file)
{
	std::size_t stotSize;
	std::size_t total = base::readFormHeader(file, "STOT", stotSize);
	stotSize += 8;
	std::cout << "Found STOT form" << std::endl;

	total += readDERV(file, _stotBaseObjectFilename);
	std::cout << "STOT Base object filename: " << _stotBaseObjectFilename << "\n";

	std::string type;
	std::size_t size;
	total += base::readFormHeader(file, type, size);
	_stotVersion = base::tagToVersion(type);
	std::cout << "STOT version: " << (int)_stotVersion << "\n";

	int32_t numParameters;
	total += readPCNT(file, numParameters);
	for (int32_t i = 0; i < numParameters; ++i)
	{
		total += readSTOTParameter(file);
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
		exit(0);
	}

	return total;
}

void stot::print() const
{
}

#if 1
std::size_t stot::readSTOTParameter(std::istream& file)
{
	std::size_t xxxxSize;
	std::string type;
	std::size_t total = base::readRecordHeader(file, type, xxxxSize);
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
		exit(0);
	}

	return total;
}
#else
std::size_t stot::readSTOTParameter(std::istream& file)
{
	std::size_t xxxxSize;
	std::size_t total = base::readRecordHeader(file, "XXXX", xxxxSize);
	xxxxSize += 8;

	std::string parameter;
	total += base::read(file, parameter);
	//std::cout << "Parameter: " << parameter << std::endl;

	// 0 - None
	// 1 - Single
	// 2 - Weighted list (count(int32), weight(int32), value)
	// 3 - Range [min, max]
	// 4 - Die Roll [numDice(int32), dieSides(int32), base(int32)
	int8_t dataType;
	total += base::read(file, dataType);

	std::cout << "Parameter data type: ";
	switch (dataType) {
	case 1: std::cout << "Single"; break;
	case 2: std::cout << "Weighted list"; break;
	case 3: std::cout << "Range"; break;
	case 4: std::cout << "Die Roll"; break;
	default: std::cout << "None:" << (int)dataType;
	}
	std::cout << " not handled\n";

	const std::size_t valueSize(xxxxSize - total);
	//std::cout << "Value size: " << valueSize << "\n";
	if (parameter == "paletteColorCustomizationVariables")
	{
		if (1 == dataType) {
			//total += base::read(file, _paletteColorCustomizationVariables, valueSize);
			//std::cout << "Palette color customization variables: '" << _paletteColorCustomizationVariables << "'\n";
		}
	}
	else if (parameter == "detailedDescription")
	{
		if (1 == dataType) {
			total += base::read(file, _detailedDescription, valueSize);
			std::cout << "Detailed description: " << _detailedDescription << "\n";
		}
	}
	else if (parameter == "lookAtText")
	{
		if (1 == dataType) {
			total += base::read(file, _lookAtText, valueSize);
			std::cout << "Detailed description: " << _lookAtText << "\n";
		}
	}
	else if (parameter == "snapToTerrain")
	{
		if (1 == dataType) {
			total += base::read(file, _snapToTerrain);
			std::cout << "Snap to terrain: " << std::boolalpha << _snapToTerrain << "\n";
		}
	}
	else if (parameter == "containerType")
	{
		int8_t dataDeltaType;
		total += base::read(file, dataDeltaType);
		if (1 == dataType) {
			total += base::read(file, _containerType);
			std::cout << "Container type: " << _containerType << "\n";
		}
	}
	else if (parameter == "containerVolumeLimit")
	{
		int8_t dataDeltaType;
		total += base::read(file, dataDeltaType);
		if (1 == dataType) {
			total += base::read(file, _containerVolumeLimit);
			std::cout << "Container volume limit: " << _containerVolumeLimit << "\n";
		}
	}
	else if (parameter == "tintPalette")
	{
		if (1 == dataType) {
			total += base::read(file, _tintPalette, valueSize);
			std::cout << "Tint palette: '" << _tintPalette << "'\n";
		}
	}
	else if (parameter == "slotDescriptorFilename")
	{
		if (1 == dataType) {
			total += base::read(file, _slotDescriptorFilename, valueSize);
			std::cout << "Slot descriptor filename: '" << _slotDescriptorFilename << "'\n";
		}
	}
	else if (parameter == "arrangementDescriptorFilename")
	{
		if (1 == dataType) {
			total += base::read(file, _arrangementDescriptorFilename, valueSize);
			std::cout << "Arrangement descriptor filename: '" << _arrangementDescriptorFilename << "'\n";
		}
	}
	else if (parameter == "appearanceFilename")
	{
		if (1 == dataType) {
			total += base::read(file, _appearanceFilename, valueSize);
			std::cout << "Appearance filename: '" << _appearanceFilename << "'\n";
		}
	}
	else if (parameter == "portalLayoutFilename")
	{
		if (1 == dataType) {
			total += base::read(file, _portalLayoutFilename, valueSize);
			std::cout << "Portal layout filename: '" << _portalLayoutFilename << "'\n";
		}
	}
	else if (parameter == "clientDataFile")
	{
		if (1 == dataType) {
			total += base::read(file, _clientDataFile, valueSize);
			std::cout << "Client data file: '" << _clientDataFile << "'\n";
		}
	}
	else if (parameter == "collisionMaterialFlags")
	{
		int8_t dataDeltaType;
		total += base::read(file, dataDeltaType);
		if (1 == dataType) {
			total += base::read(file, _collisionMaterialFlags);
			std::cout << "Collision material flags: 0x" << std::hex << _collisionMaterialFlags << std::dec << "\n";
		}
	}
	else if (parameter == "collisionMaterialPassFlags")
	{
		int8_t dataDeltaType;
		total += base::read(file, dataDeltaType);
		if (1 == dataType) {
			total += base::read(file, _collisionMaterialPassFlags);
			std::cout << "Collision material pass flags: 0x" << std::hex << _collisionMaterialPassFlags << std::dec << "\n";
		}
	}
	else if (parameter == "collisionMaterialBlockFlags")
	{
		int8_t dataDeltaType;
		total += base::read(file, dataDeltaType);
		if (1 == dataType) {
			total += base::read(file, _collisionMaterialBlockFlags);
			std::cout << "Collision material block flags: 0x" << std::hex << _collisionMaterialBlockFlags << std::dec << "\n";
		}
	}
	else if (parameter == "collisionActionFlags")
	{
		int8_t dataDeltaType;
		total += base::read(file, dataDeltaType);
		if (1 == dataType) {
			total += base::read(file, _collisionActionFlags);
			std::cout << "Collision action flags: 0x" << std::hex << _collisionActionFlags << std::dec << "\n";
		}
	}
	else if (parameter == "collisionActionPassFlags")
	{
		int8_t dataDeltaType;
		total += base::read(file, dataDeltaType);
		if (1 == dataType) {
			total += base::read(file, _collisionActionPassFlags);
			std::cout << "Collision action pass flags: 0x" << std::hex << _collisionActionPassFlags << std::dec << "\n";
		}
	}
	else if (parameter == "collisionActionBlockFlags")
	{
		int8_t dataDeltaType;
		total += base::read(file, dataDeltaType);
		if (1 == dataType) {
			total += base::read(file, _collisionActionBlockFlags);
			std::cout << "Collision action block flags: 0x" << std::hex << _collisionActionBlockFlags << std::dec << "\n";
		}
	}
	else if (parameter == "scale")
	{
		int8_t dataDeltaType;
		total += base::read(file, dataDeltaType);
		if (1 == dataType) {
			total += base::read(file, _scale);
			std::cout << "Scale: " << _scale << "\n";
		}
	}
	else if (parameter == "gameObjectType")
	{
		int8_t dataDeltaType;
		total += base::read(file, dataDeltaType);
		if (1 == dataType) {
			total += base::read(file, _gameObjectType);
			std::cout << "Game object type: " << _gameObjectType << "\n";
		}
	}
	else if (parameter == "sendToClient")
	{
		if (1 == dataType) {
			total += base::read(file, _sendToClient);
			std::cout << "Send to client: " << std::boolalpha << _sendToClient << "\n";
		}
	}
	else if (parameter == "scaleThresholdBeforeExtentTest")
	{
		int8_t dataDeltaType;
		total += base::read(file, dataDeltaType);
		if (1 == dataType) {
			total += base::read(file, _scaleThresholdBeforeExtentTest);
			std::cout << "Scale threshold before extent test: " << _scaleThresholdBeforeExtentTest << "\n";
		}
	}
	else if (parameter == "clearFloraRadius")
	{
		int8_t dataDeltaType;
		total += base::read(file, dataDeltaType);
		if (1 == dataType) {
			total += base::read(file, _clearFloraRadius);
			std::cout << "Clear flora radius: " << _clearFloraRadius << "\n";
		}
	}
	else if (parameter == "surfaceType")
	{
		int8_t dataDeltaType;
		total += base::read(file, dataDeltaType);
		if (1 == dataType) {
			total += base::read(file, _surfaceType);
			std::cout << "Surface type: " << _surfaceType << "\n";
		}
	}
	else if (parameter == "noBuildRadius")
	{
		int8_t dataDeltaType;
		total += base::read(file, dataDeltaType);
		if (1 == dataType) {
			total += base::read(file, _noBuildRadius);
			std::cout << "No build radius: " << _noBuildRadius << "\n";
		}
	}
	else if (parameter == "onlyVisibleInTools")
	{
		if (1 == dataType) {
			total += base::read(file, _onlyVisibleInTools);
			std::cout << "Only visible in tools: " << std::boolalpha << _onlyVisibleInTools << "\n";
		}
	}
	else if (parameter == "locationReservationRadius")
	{
		int8_t dataDeltaType;
		total += base::read(file, dataDeltaType);
		if (1 == dataType) {
			total += base::read(file, _locationReservationRadius);
			std::cout << "Location reservation radius: " << _locationReservationRadius << "\n";
		}
	}
	else if (parameter == "forceNoCollision")
	{
		if (1 == dataType) {
			total += base::read(file, _forceNoCollision);
			std::cout << "Force no collision: " << std::boolalpha << _forceNoCollision << "\n";
		}
	}
	else
	{
		std::cout << "Unknown: " << parameter << std::endl;
		exit(0);
	}

	if (xxxxSize != total) {
		std::cout << "FAILED in reading XXXX" << std::endl;
		std::cout << "Read " << total << " out of " << xxxxSize
			<< std::endl;
		exit(0);
	}

	return total;
}

#endif