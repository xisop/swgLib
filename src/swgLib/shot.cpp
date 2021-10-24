/** -*-c++-*-
 *  \class  shot
 *  \file   shot.cpp
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
#include <swgLib/shot.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

shot::shot() :
	_baseObjectFilename(""),
	_shotVersion(0),
	_objectName(""),
	_detailedDescription(""),
	_lookAtText(""),
	_snapToTerrain(false),
	_containerType(0),
	_containerVolumeLimit(0),
	_tintPalette(""),
	_slotDescriptorFilename(""),
	_arrangementDescriptorFilename(""),
	_appearanceFilename(""),
	_portalLayoutFilename(""),
	_clientDataFile(""),
	_collisionMaterialFlags(0),
	_collisionMaterialPassFlags(0),
	_collisionMaterialBlockFlags(0),
	_collisionActionFlags(0),
	_collisionActionPassFlags(0),
	_collisionActionBlockFlags(0),
	_scale(1.0f),
	_gameObjectType(0),
	_sendToClient(false),
	_scaleThresholdBeforeExtentTest(1.0f),
	_clearFloraRadius(0.0f),
	_surfaceType(0),
	_noBuildRadius(0.0f),
	_onlyVisibleInTools(false),
	_locationReservationRadius(1.0f),
	_forceNoCollision(false)
{
}

shot::~shot()
{
}

std::size_t shot::readSHOT(std::istream& file)
{
	std::size_t shotSize;
	std::size_t total = base::readFormHeader(file, "SHOT", shotSize);
	shotSize += 8;
	std::cout << "Found SHOT form: " << shotSize << "\n";

	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);

	// Optional
	if ("DERV" == type) {
		total += readDERV(file, _baseObjectFilename);
		std::cout << "SHOT Base object filename: " << _baseObjectFilename << "\n";
	}

	total += base::readFormHeader(file, type, size);
	_shotVersion = base::tagToVersion(type);
	std::cout << "SHOT version: " << (int)_shotVersion << "\n";

	// Read parameter count...
	int32_t numParameters;
	total += readPCNT(file, numParameters);

	for (int32_t i = 0; i < numParameters; ++i)
	{
		total += readSHOTParameter(file);
	}

	if (shotSize == total)
	{
		std::cout << "Finished reading SHOT" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading SHOT" << std::endl;
		std::cout << "Read " << total << " out of " << shotSize
			<< std::endl;
		exit(0);
	}

	return total;
}

std::size_t shot::readDERV(std::istream& file, std::string& filename)
{
	std::size_t dervSize;
	std::size_t total = base::readFormHeader(file, "DERV", dervSize);
	dervSize += 8;

	std::size_t xxxxSize;
	total += base::readRecordHeader(file, "XXXX", xxxxSize);

	total += base::read(file, filename);
	//std::cout << "Filename: " << filename << std::endl;

	if (dervSize != total)
	{
		std::cout << "FAILED in reading DERV" << std::endl;
		std::cout << "Read " << total << " out of " << dervSize
			<< std::endl;
		exit(0);
	}

	return total;
}

void shot::print(std::ostream& os) const
{
}

const std::string& shot::getBaseObjectFilename() const { return _baseObjectFilename; }

const std::string& shot::getObjectName() const { return _objectName; }

const std::string& shot::getDetailedDescription() const { return _detailedDescription; }

const std::string& shot::getLookAtText() const { return _lookAtText; }

bool shot::isSnapToTerrain() const { return _snapToTerrain; }

const int32_t& shot::getContainerType() const { return _containerType; }

const int32_t& shot::getContainerVolumeLimit() const { return _containerVolumeLimit; }

const std::string& shot::getTintPalette() const { return _tintPalette; }

const std::string& shot::getSlotDescriptorFilename() const { return _slotDescriptorFilename; }

const std::string& shot::getArrangementDescriptorFilename() const { return _arrangementDescriptorFilename; }

const std::string& shot::getAppearanceFilename() const { return _appearanceFilename; }

const std::string& shot::getPortalLayoutFilename() const { return _portalLayoutFilename; }

const std::string& shot::getClientDataFile() const { return _clientDataFile; }

const int32_t& shot::getCollisionMaterialFlags() const { return _collisionMaterialFlags; }

const int32_t& shot::getCollisionMaterialPassFlags() const { return _collisionMaterialPassFlags; }

const int32_t& shot::getCollisionMaterialBlockFlags() const { return _collisionMaterialBlockFlags; }

const int32_t& shot::getCollisionActionFlags() const { return _collisionActionFlags; }

const int32_t& shot::getCollisionActionPassFlags() const { return _collisionActionPassFlags; }

const int32_t& shot::getCollisionActionBlockFlags() const { return _collisionActionBlockFlags; }

const float& shot::getScale() const { return _scale; }

const int32_t& shot::getGameObjectType() const { return _gameObjectType; }

bool shot::isSendToClient() const { return _sendToClient; }

const float& shot::getScaleThresholdBeforeExtentTest() const { return _scaleThresholdBeforeExtentTest; }

const float& shot::getClearFloraRadius() const { return _clearFloraRadius; }

const int32_t& shot::getSurfaceType() const { return _surfaceType; }

const float& shot::getNoBuildRadius() const { return _noBuildRadius; }

bool shot::isOnlyVisibleInTools() const { return _onlyVisibleInTools; }

const float& shot::getLocationReservationRadius() const { return _locationReservationRadius; }

bool shot::isForceNoCollision() const { return _forceNoCollision; }

std::size_t shot::readPCNT(std::istream& file, int32_t& numParameters)
{
	std::size_t pcntSize;
	std::size_t total = base::readRecordHeader(file, "PCNT", pcntSize);

	if (4 != pcntSize) {
		std::cout << "Expected size 4: " << pcntSize << std::endl;
		exit(0);
	}
	pcntSize += 8;

	total += base::read(file, numParameters);
	std::cout << "Number of parameters: " << numParameters << std::endl;

	if (pcntSize != total) {
		std::cout << "FAILED in reading PCNT" << std::endl;
		std::cout << "Read " << total << " out of " << pcntSize
			<< std::endl;
		exit(0);
	}

	return total;
}

std::size_t shot::readSHOTParameter(std::istream& file)
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

	if ((0 != dataType) && (1 != dataType)) {
		std::cout << "Parameter data type: ";
		switch (dataType) {
		case 1: std::cout << "Single"; break;
		case 2: std::cout << "Weighted list"; break;
		case 3: std::cout << "Range"; break;
		case 4: std::cout << "Die Roll"; break;
		default: std::cout << "None:" << (int)dataType;
		}
		std::cout << " not handled\n";
		exit(0);
	}

	const std::size_t valueSize(xxxxSize - total);
	//std::cout << "Value size: " << valueSize << "\n";
	if (parameter == "objectName")
	{
		if (1 == dataType) {
			total += base::read(file, _objectName, valueSize);
			std::cout << "Object name: '" << _objectName << "'\n";
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
			std::cout << "Look at text: " << _lookAtText << "\n";
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
