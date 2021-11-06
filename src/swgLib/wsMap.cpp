/** -*-c++-*-
 *  \class  wsMap
 *  \file   wsMap.cpp
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

#include <swgLib/wsMap.hpp>
#include <swgLib/base.hpp>

using namespace ml;

wsMap::wsMap() :
	_version(0) {
}

wsMap::~wsMap() {
}

std::size_t wsMap::read(std::istream& file) {
	uint32_t type;
	std::size_t wsMapSize;
	std::size_t total = base::readFormHeader(file, type, wsMapSize);
	wsMapSize += 8;

	_version = base::tagToVersion(type);
	if (_version > 1) {
		std::cout << "Unexpected version: " << _version << "\n";
	}
	std::cout << "W/S Map version: " << _version << "\n";

	if (1 == _version) {
		total += base::readRecordHeader(file, tag::TAG_DATA);
		total += base::read(file, _mapWidth);
		total += base::read(file, _chunkWidth);
		total += base::read(file, _width);
		total += base::read(file, _height);

		{
			std::size_t wmapSize;
			std::size_t wmapRead = base::readRecordHeader(file, tag::TAG_WMAP, wmapSize);
			_waterMap.resize(wmapSize);
			file.read((char*)_waterMap.data(), (wmapSize));
			wmapRead += wmapSize;
			total += wmapRead;
		}

		{
			std::size_t smapSize;
			std::size_t smapRead = base::readRecordHeader(file, tag::TAG_SMAP, smapSize);
			_slopeMap.resize(smapSize);
			file.read((char*)_slopeMap.data(), (smapSize));
			smapRead += smapSize;
			total += smapRead;
		}
 	}

	if (wsMapSize == total) {
		std::cout << "Finished reading wsMap\n";
	}
	else {
		std::cout << "Failed in reading wsMap\n";
		std::cout << "Read " << total << " out of " << wsMapSize << "\n";
		exit(0);
	}

	return total;
}
