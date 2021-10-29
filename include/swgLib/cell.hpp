/** -*-c++-*-
 *  \class  cell
 *  \file   cell.hpp
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

#include <swgLib/baseCollision.hpp>
#include <swgLib/portal.hpp>
#include <swgLib/lght.hpp>

#include <string>
#include <vector>

#ifndef CELL_HPP
#define CELL_HPP 1

namespace ml
{
	class cell
	{
	public:
		cell();
		~cell();

		std::size_t read(std::istream& file);

		const bool canSeeParentCell() const;
		const std::string& getName() const;
		const std::string& getAppearanceFilename() const;
		const bool hasFloor() const;
		const std::string& getFloorFilename() const;
		const std::vector<portal>& getPortals() const;
		const std::vector<lght>& getLights() const;

	protected:
		uint32_t _version; // 0001, 0002, 0003, 0004, 0005
		int32_t _numPortals;
		bool _canSeeParentCell;
		std::string _name;
		std::string _appearanceName;
		bool _hasFloor;
		std::string _floorName;

		baseCollisionPtr _collisionPtr;
		std::vector<portal> _portals;

		int32_t _numLights;
		std::vector<lght> _lights;

	private:
	};
}

#endif
