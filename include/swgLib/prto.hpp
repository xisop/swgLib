/** -*-c++-*-
 *  \class  prto
 *  \file   prto.hpp
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

#include <swgLib/cell.hpp>
#include <swgLib/portalGeometry.hpp>
#include <swgLib/pgrf.hpp>

#include <istream>

#ifndef PRTO_HPP
#define PRTO_HPP 1

namespace ml
{
	class prto
	{
	public:
		prto();
		~prto();

		std::size_t readPRTO(std::istream& file);

		const int32_t &getNumCells() const;
		const int32_t &getNumPortals() const;

		const std::vector<portalGeometry>& getPortalGeometry() const;
		const std::vector<cell> &getCells() const;

	protected:
		uint8_t _version;
		int32_t _numPortals;
		int32_t _numCells;

		std::vector<portalGeometry> _portalGeometry;
		std::vector<cell> _cells;

		pgrf _pgrf;

		int32_t _crc;

	private:

	};
}
#endif
