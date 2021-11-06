/** -*-c++-*-
 *  \class  wsMap
 *  \file   wsMap.hpp
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

#include <istream>
#include <vector>

#ifndef WSMAP_HPP
#define WSMAP_HPP 1

namespace ml
{
	class wsMap
	{
	public:
		wsMap();
		~wsMap();

		std::size_t read(std::istream& file);

	protected:
		uint32_t _version;
		float    _mapWidth; // Meters
		float    _chunkWidth; // Meters
		int32_t  _width;
		int32_t  _height;

		std::vector<uint8_t> _waterMap;
		std::vector<uint8_t> _slopeMap;
	private:
	};
}

#endif
