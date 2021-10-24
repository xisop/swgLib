/** -*-c++-*-
 *  \class  spaceTerrain
 *  \file   spaceTerrain.cpp
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
#include <swgLib/spaceTerrain.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

ml::spaceTerrain::spaceTerrain() :
	_mapWidthInMeters(0.0f),
	_clearColorRed(0.0f),
	_clearColorGreen(0.0f),
	_clearColorBlue(0.0f),
	_ambientAlpha(0.0f),
	_ambientRed(0.0f),
	_ambientGreen(0.0f),
	_ambientBlue(0.0f),
	_light(),
	_environmentTextureName(""),
	_fogEnabled(false),
	_fogAlpha(0.0f),
	_fogRed(0.0f),
	_fogGreen(0.0f),
	_fogBlue(0.0f),
	_fogDensity(0.0f),
	_starColorRampName(""),
	_numberOfStars(0),
	_numberOfDust(0),
	_dustRadius(0.0f),
	_skyboxCubeMap(false),
	_skyboxTextureName(""),
	_distance(),
	_celestial()
{
}

ml::spaceTerrain::~spaceTerrain()
{
}

std::size_t ml::spaceTerrain::read(std::istream& file)
{
  std::size_t streamStart = file.tellg();
  
	std::size_t spaceTerrainSize;
	std::size_t total = base::readFormHeader(file, "STAT", spaceTerrainSize);
	spaceTerrainSize += 8;
	std::cout << "Found STAT form" << std::endl;

	std::string form, type;
	std::size_t size0000;
	total += base::readFormHeader(file, form, size0000, type);
	size0000 += 8;

	if( "0000" != type ) {
	  std::cout << "Space Terrain (STAT) file not detected\n";
	  if("DERV" == type) {
	  std::cout << "File appears to be Shared Static Object Template (STAT) type.\n";
	  }
	  std::cout << "Resetting file pointer\n";
	  file.seekg(streamStart);
	}
	
	std::size_t size;
	while (total < spaceTerrainSize) {
		base::peekHeader(file, form, size, type);

		if ("INFO" == type) {
			total += base::readFormHeader(file, "INFO", size);
			total += base::read(file, _mapWidthInMeters);
			std::cout << "Map width (m): " << _mapWidthInMeters << "\n\n";
		}
		else if ("CLEA" == type) {
			total += base::readFormHeader(file, "CLEA", size);
			total += base::readRecordHeader(file, "0000", size);
			total += base::read(file, _clearColorRed);
			total += base::read(file, _clearColorGreen);
			total += base::read(file, _clearColorBlue);
			std::cout << "Clear color (rgb): "
				<< _clearColorRed << ", "
				<< _clearColorGreen << ", "
				<< _clearColorBlue << "\n\n";
		}
		else if ("AMBI" == type) {
			total += base::readFormHeader(file, "AMBI", size);
			total += base::readRecordHeader(file, "0000", size);
			total += base::read(file, _ambientAlpha);
			total += base::read(file, _ambientRed);
			total += base::read(file, _ambientGreen);
			total += base::read(file, _ambientBlue);
			std::cout << "Ambient color (rgba): "
				<< _ambientRed << ", "
				<< _ambientGreen << ", "
				<< _ambientBlue << ", "
				<< _ambientAlpha << "\n\n";
		}
		else if ("PARA" == type) {
			total += base::readFormHeader(file, "PARA", size);
			total += base::readRecordHeader(file, "0000", size);
			light newLight;
			total += newLight.read(file);
			std::cout << "Light " << _light.size() << ": \n";
			newLight.print(std::cout);
			_light.push_back(newLight);
			std::cout << "\n";
		}
		else if ("ENVI" == type) {
			total += base::readFormHeader(file, "ENVI", size);
			total += base::readRecordHeader(file, "0000", size);
			total += base::read(file, _environmentTextureName);
			std::cout << "Environmental texture: " << _environmentTextureName << "\n\n";
		}
		else if ("FOG " == type) {
			total += base::readFormHeader(file, "FOG ", size);
			total += base::readRecordHeader(file, "0000", size);
			_fogEnabled = true;
			total += base::read(file, _fogAlpha);
			total += base::read(file, _fogRed);
			total += base::read(file, _fogGreen);
			total += base::read(file, _fogBlue);
			_fogDensity = _fogAlpha;
			std::cout << "Fog enabled: true\n"
				<< "Fog color (rgba): "
				<< _fogRed << ", "
				<< _fogGreen << ", "
				<< _fogBlue << ", "
				<< _fogAlpha << "\n"
				<< "Fog density: " << _fogDensity << "\n\n";
		}
		else if ("STAR" == type) {
			total += base::readFormHeader(file, "STAR", size);
			total += base::readRecordHeader(file, "0000", size);
			total += base::read(file, _starColorRampName);
			total += base::read(file, _numberOfStars);
			std::cout << "Star color ramp: " << _starColorRampName << "\n"
				<< "Number of stars: " << _numberOfStars << "\n\n";
		}
		else if ("DUST" == type) {
			total += base::readFormHeader(file, "DUST", size);
			total += base::readRecordHeader(file, "0000", size);
			total += base::read(file, _numberOfDust);
			total += base::read(file, _dustRadius);
			std::cout << "Number of dust: " << _numberOfDust << "\n"
				<< "Dust radius: " << _dustRadius << "\n\n";
		}
		else if ("SKYB" == type) {
			total += base::readFormHeader(file, "SKYB", size);
			total += base::readRecordHeader(file, "0000", size);
			total += base::read(file, _skyboxCubeMap);
			total += base::read(file, _skyboxTextureName);
			// Add full texture path to name
			_skyboxTextureName = std::string("texture/") + _skyboxTextureName;
			std::cout << "Skybox cube map enabled: " << std::boolalpha << _skyboxCubeMap << "\n"
				<< "Skybox texture: " << _skyboxTextureName << "\n\n";
		}
		else if ("DIST" == type) {
			total += base::readFormHeader(file, "DIST", size);
			total += base::readRecordHeader(file, "0000", size);
			distance newDist;
			total += newDist.readDIST(file);
			_distance.push_back(newDist);
			std::cout << "Distant object:\n";
			newDist.print(std::cout);
			std::cout << "\n";
		}
		else if ("PLAN" == type) {
			total += base::readFormHeader(file, "PLAN", size);
			total += base::readRecordHeader(file, "0000", size);
			distance newPlan;
			total += newPlan.readPLAN(file);
			_distance.push_back(newPlan);
			std::cout << "Distant planet:\n";
			newPlan.print(std::cout);
			std::cout << "\n";
		}
		else if ("CELE" == type) {
			total += base::readFormHeader(file, "CELE", size);
			total += base::readRecordHeader(file, "0000", size);
			celestial newCele;
			total += newCele.read(file);
			std::cout << "Celestial " << _celestial.size() << ":\n";
			newCele.print(std::cout);
			_celestial.push_back(newCele);
			std::cout << "\n";
		}
		else {
			std::cout << "Unhandled type: " << type << "\n";
			exit(0);
		}
	}

	if (spaceTerrainSize == total)
	{
		std::cout << "Finished reading STAT\n";
	}
	else
	{
		std::cout << "FAILED in reading STAT\n"
			<< "Read " << total << " out of " << spaceTerrainSize
			<< "\n";
		exit(0);
	}

	return total;
}

void ml::spaceTerrain::print() const
{
}

//*****************************************************************************

ml::spaceTerrain::light::light() :
	_lightDOT3(false),
	_lightDiffuseAlpha(0.0f),
	_lightDiffuseRed(0.0f),
	_lightDiffuseGreen(0.0f),
	_lightDiffuseBlue(0.0f),
	_lightSpecularAlpha(0.0f),
	_lightSpecularRed(0.0f),
	_lightSpecularGreen(0.0f),
	_lightSpecularBlue(0.0f),
	_lightDirX(0.0f),
	_lightDirY(0.0f),
	_lightDirZ(0.0f)
{
}

ml::spaceTerrain::light::~light() {
}

std::size_t ml::spaceTerrain::light::read(std::istream& file) {
	std::size_t total = base::read(file, _lightDOT3);
	total += base::read(file, _lightDiffuseAlpha);
	total += base::read(file, _lightDiffuseRed);
	total += base::read(file, _lightDiffuseGreen);
	total += base::read(file, _lightDiffuseBlue);
	total += base::read(file, _lightSpecularAlpha);
	total += base::read(file, _lightSpecularRed);
	total += base::read(file, _lightSpecularGreen);
	total += base::read(file, _lightSpecularBlue);
	total += base::read(file, _lightDirX);
	total += base::read(file, _lightDirY);
	total += base::read(file, _lightDirZ);

	return total;
}

void ml::spaceTerrain::light::print(std::ostream& os) const {
	os << "Normal map: " << std::boolalpha << _lightDOT3 << "\n"
		<< "Diffuse color (rgba): "
		<< _lightDiffuseRed << ", "
		<< _lightDiffuseGreen << ", "
		<< _lightDiffuseBlue << ", "
		<< _lightDiffuseAlpha << "\n"
		<< "Specular color (rgba): "
		<< _lightSpecularRed << ", "
		<< _lightSpecularGreen << ", "
		<< _lightSpecularBlue << ", "
		<< _lightSpecularAlpha << "\n"
		<< "Light direction vector: "
		<< _lightDirX << ", "
		<< _lightDirY << ", "
		<< _lightDirZ << "\n";
}

//*****************************************************************************

ml::spaceTerrain::celestial::celestial() :
	_backShaderTemplateName(""),
	_backSize(0.0f),
	_frontShaderTemplateName(""),
	_frontSize(0.0f),
	_directionX(0.0f),
	_directionY(0.0f),
	_directionZ(0.0f)
{
}

ml::spaceTerrain::celestial::~celestial() {

}

std::size_t ml::spaceTerrain::celestial::read(std::istream& file) {
	std::size_t total = base::read(file, _backShaderTemplateName);
	total += base::read(file, _backSize);
	total += base::read(file, _frontShaderTemplateName);
	total += base::read(file, _frontSize);
	total += base::read(file, _directionX);
	total += base::read(file, _directionY);
	total += base::read(file, _directionZ);
	return total;
}

void ml::spaceTerrain::celestial::print(std::ostream& os) const {
	os << "Back shader: " << _backShaderTemplateName << "\n"
		<< "Back size: " << _backSize << "\n"
		<< "Front shader: " << _frontShaderTemplateName << "\n"
		<< "Front size: " << _frontSize << "\n"
		<< "Direction vector: "
		<< _directionX << ", "
		<< _directionY << ", "
		<< _directionZ << "\n";
}

//*****************************************************************************

ml::spaceTerrain::distance::distance() :
	_templateName(""),
	_directionX(0.0f),
	_directionY(0.0f),
	_directionZ(0.0f),
	_orienationYaw(0.0f),
	_orienationPitch(0.0f),
	_orienationRoll(0.0f),
	_haloRoll(0.0f),
	_haloScale(0.0f),
	_infiniteDistance(false)
{
}

ml::spaceTerrain::distance::~distance() {
}

std::size_t ml::spaceTerrain::distance::readDIST(std::istream& file) {
	std::size_t total = base::read(file, _templateName);
	total += base::read(file, _directionX);
	total += base::read(file, _directionY);
	total += base::read(file, _directionZ);
	total += base::read(file, _orienationYaw);
	total += base::read(file, _orienationPitch);
	total += base::read(file, _orienationRoll);
	_haloRoll = 0.0f;
	_haloScale = 1.0f;
	total += base::read(file, _infiniteDistance);
	return total;
}

std::size_t ml::spaceTerrain::distance::readPLAN(std::istream& file) {
	std::size_t total = base::read(file, _templateName);
	total += base::read(file, _directionX);
	total += base::read(file, _directionY);
	total += base::read(file, _directionZ);
	total += base::read(file, _orienationYaw);
	total += base::read(file, _orienationPitch);
	total += base::read(file, _orienationRoll);
	total += base::read(file, _haloRoll);
	total += base::read(file, _haloScale);
	//total += base::read(file, _infiniteDistance); // Doesn't seem to be present in data files.
	return total;
}

void ml::spaceTerrain::distance::print(std::ostream& os) const {
	os << "Template name: " << _templateName << "\n"
		<< "Direction vector: "
		<< _directionX << ", "
		<< _directionY << ", "
		<< _directionZ << "\n"
		<< "Orientation: "
		<< _orienationYaw << ", "
		<< _orienationPitch << ", "
		<< _orienationRoll << "\n"
		<< "Halo roll: " << _haloRoll << "\n"
		<< "Halo scale: " << _haloScale << "\n"
		<< "Infinite distance: " << std::boolalpha << _infiniteDistance << "\n";
}
