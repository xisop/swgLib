/** -*-c++-*-
 *  \class  ptat
 *  \file   ptat.hpp
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
#include <swgLib/appr.hpp>
#include <swgLib/tgen.hpp>
#include <swgLib/wsMap.hpp>

#include <istream>
#include <string>

#ifndef PTAT_HPP
#define PTAT_HPP 1

namespace ml
{
	// Procedural Terrain Appearance
	class ptat : public appr
	{
	public:
		ptat();
		~ptat();

		std::size_t read(std::istream& file);

	protected:
		uint32_t _ptatVersion;

		std::string _name;
		float _mapWidth; // Meters
		float _chunkWidth; // Meters
		int32_t _tilesPerChunk;
		bool _useGlobalWaterTable;
		float _globalWaterTableHeight;
		float _globalWaterTableShaderSize;
		std::string _globalWaterTableShader;
		float _environmentCycleTime;

		float    _collidableMinDist;
		float    _collidableMaxDist;
		float    _collidableTileSize;
		float    _collidableTileBorder;
		uint32_t _collidableSeed;

		float    _nonCollidableMinDist;
		float    _nonCollidableMaxDist;
		float    _nonCollidableTileSize;
		float    _nonCollidableTileBorder;
		uint32_t _nonCollidableSeed;

		float    _radialMinDist;
		float    _radialMaxDist;
		float    _radialTileSize;
		float    _radialTileBorder;
		uint32_t _radialSeed;

		float    _farRadialMinDist;
		float    _farRadialMaxDist;
		float    _farRadialTileSize;
		float    _farRadialTileBorder;
		uint32_t _farRadialSeed;

		bool _legacyMap;

		tgen _tgen;

		wsMap _wsMap;

#if 0
		std::size_t readTGEN(std::istream& file);
		std::size_t readptatDATA(std::istream& file);
		std::size_t readMapDATA(std::istream& file);
		std::size_t readWMAP(std::istream& file);
		std::size_t readSMAP(std::istream& file);
		std::size_t readSGRP(std::istream& file);
		std::size_t readFGRP(std::istream& file);
		std::size_t readRGRP(std::istream& file);
		std::size_t readEGRP(std::istream& file);
		std::size_t readMGRP(std::istream& file);
		std::size_t readLYRS(std::istream& file);
		std::size_t readFFAM(std::istream& file);
		std::size_t readRFAM(std::istream& file);
		std::size_t readEFAM(std::istream& file);
		std::size_t readMFAM(std::istream& file);
		std::size_t readMFRC(std::istream& file);
		std::size_t readADTA(std::istream& file);
		std::size_t readSFAM(std::istream& file,
			sfam& newSFAM);
#endif

	private:
#if 0
		float terrainSize;
		float chunkWidth;
		unsigned int tilesPerChunk;

		float globalWaterTableHeight;
		float globalWaterTableShaderSize;
		std::string globalWaterTableShader;

		float tileSize;

		float collidableFloraMinDistance;
		float collidableFloraMaxDistance;
		float collidableFloraTileSize;
		float collidableFloraTileBorder;
		float collidableFloraSeed;

		float nonCollidableFloraMinDistance;
		float nonCollidableFloraMaxDistance;
		float nonCollidableFloraTileSize;
		float nonCollidableFloraTileBorder;
		float nonCollidableFloraSeed;

		float nearRadialFloraMinDistance;
		float nearRadialFloraMaxDistance;
		float nearRadialFloraTileSize;
		float nearRadialFloraTileBorder;
		float nearRadialFloraSeed;

		float farRadialFloraMinDistance;
		float farRadialFloraMaxDistance;
		float farRadialFloraTileSize;
		float farRadialFloraTileBorder;
		float farRadialFloraSeed;


		unsigned int mapHeight;
		unsigned int mapWidth;

		std::map<unsigned int, sfam> sfamMap;
		std::list<std::shared_ptr<ptatLayer> > layerList;
#endif
	};
}
#endif
