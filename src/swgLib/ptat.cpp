/** -*-c++-*-
 *  \class  ptat
 *  \file   ptat.cpp
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

#include <swgLib/ptat.hpp>
#include <swgLib/base.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>
#include <memory>

using namespace ml;

// Procedural Terrain Appearance
ptat::ptat()
{
}

ptat::~ptat()
{
}

std::size_t ptat::read(std::istream& file)
{
	// PTAT or MPTA Form ( Level 0 )
	std::string type;
	std::size_t ptatSize;
	std::size_t total = base::readFormHeader(file, type, ptatSize);
	ptatSize += 8;

	if (("MPTA" != type) && ("PTAT" != type)) {
		std::cout << "Found form: " << type << ": expected type MPTA or PTAT\n";
		exit(0);
	}
	std::cout << "Found " << type << " form\n";

	std::size_t size;
	total += base::readFormHeader(file, type, size);
	_ptatVersion = base::tagToVersion(type);
	if ((_ptatVersion < 13) || (15 < _ptatVersion)) {
		std::cout << "Expected type [0013..0015]: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found form of type: " << type << std::endl;
	std::cout << "PTAT version: " << _ptatVersion << "\n";
	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _name);
	total += base::read(file, _mapWidth);
	total += base::read(file, _chunkWidth);
	total += base::read(file, _tilesPerChunk);
	int32_t useGWT_int;	total += base::read(file, useGWT_int);
	_useGlobalWaterTable = (0 != useGWT_int);
	total += base::read(file, _globalWaterTableHeight);
	total += base::read(file, _globalWaterTableShaderSize);
	total += base::read(file, _globalWaterTableShader);
	total += base::read(file, _environmentCycleTime);

	std::cout
		<< "Terrain name: " << _name << "\n"
		<< "Terrain Width: " << _mapWidth << "(m)\n"
		<< "Chunk width: " << _chunkWidth << "(m)\n"
		<< "Tiles per chunk: " << _tilesPerChunk << "\n"
		<< "Use global water table: " << std::boolalpha << _useGlobalWaterTable << "\n"
		<< "Global water table height: " << _globalWaterTableHeight << "\n"
		<< "Global water table shader size: " << _globalWaterTableShaderSize << "\n"
		<< "Global water table shader: " << _globalWaterTableShader << "\n"
		<< "Environment cycle time: " << _environmentCycleTime << "\n";

	if (13 == _ptatVersion) {
		std::string junkStr;
		float junkFloat;
		int32_t junkInt32;
		total += base::read(file, junkStr);
		total += base::read(file, junkStr);
		total += base::read(file, junkFloat);
		total += base::read(file, junkStr);
		total += base::read(file, junkFloat);
		total += base::read(file, junkStr);
		total += base::read(file, junkFloat);
		total += base::read(file, junkStr);
		total += base::read(file, junkFloat);
		total += base::read(file, junkInt32);
		total += base::read(file, junkStr);
	}

	total += base::read(file, _collidableMinDist);
	total += base::read(file, _collidableMaxDist);
	total += base::read(file, _collidableTileSize);
	total += base::read(file, _collidableTileBorder);
	total += base::read(file, _collidableSeed);

	std::cout << "Collidable:\n"
		<< "\t Min distance: " << _collidableMinDist << "\n"
		<< "\t Max distance: " << _collidableMaxDist << "\n"
		<< "\t    Tile size: " << _collidableTileSize << "\n"
		<< "\t  Tile border: " << _collidableTileBorder << "\n"
		<< "\t         Seed: " << _collidableSeed << "\n";

	total += base::read(file, _nonCollidableMinDist);
	total += base::read(file, _nonCollidableMaxDist);
	total += base::read(file, _nonCollidableTileSize);
	total += base::read(file, _nonCollidableTileBorder);
	total += base::read(file, _nonCollidableSeed);

	std::cout << "Non-collidable:\n"
		<< "\t Min distance: " << _nonCollidableMinDist << "\n"
		<< "\t Max distance: " << _nonCollidableMaxDist << "\n"
		<< "\t    Tile size: " << _nonCollidableTileSize << "\n"
		<< "\t  Tile border: " << _nonCollidableTileBorder << "\n"
		<< "\t         Seed: " << _nonCollidableSeed << "\n";

	total += base::read(file, _radialMinDist);
	total += base::read(file, _radialMaxDist);
	total += base::read(file, _radialTileSize);
	total += base::read(file, _radialTileBorder);
	total += base::read(file, _radialSeed);

	std::cout << "Radial:\n"
		<< "\t Min distance: " << _radialMinDist << "\n"
		<< "\t Max distance: " << _radialMaxDist << "\n"
		<< "\t    Tile size: " << _radialTileSize << "\n"
		<< "\t  Tile border: " << _radialTileBorder << "\n"
		<< "\t         Seed: " << _radialSeed << "\n";

	total += base::read(file, _farRadialMinDist);
	total += base::read(file, _farRadialMaxDist);
	total += base::read(file, _farRadialTileSize);
	total += base::read(file, _farRadialTileBorder);
	total += base::read(file, _farRadialSeed);

	std::cout << "Far Radial:\n"
		<< "\t Min distance: " << _farRadialMinDist << "\n"
		<< "\t Max distance: " << _farRadialMaxDist << "\n"
		<< "\t    Tile size: " << _farRadialTileSize << "\n"
		<< "\t  Tile border: " << _farRadialTileBorder << "\n"
		<< "\t         Seed: " << _farRadialSeed << "\n";

	if (_ptatVersion >= 15) {
		total += base::read(file, _legacyMap);
		std::cout << "Legacy map: " << std::boolalpha << _legacyMap << "\n";
	}

	// Read terrain generation parameters...
	total += _tgen.read(file);

	// Read water and slope maps...
	total += _wsMap.read(file);

	if (_ptatVersion >= 15) {
		std::string form;
		base::peekHeader(file, form, size, type);
		std::cout << "Peek: " << form << ":" << type << "\n";
		//	total += base::skip(file, size + 8);
				//total += base::read static collidable flora
		//total += base::read static collidable flora height
	}

	if (ptatSize == total)
	{
		std::cout << "Finished reading PTAT\n";
	}
	else
	{
		std::cout << "Failed in reading PTAT\n"
			<< "Read " << total << " out of " << ptatSize << "\n";
		exit(0);
	}

	return total;
}

