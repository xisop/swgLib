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

#include <swgLib/cmsh.hpp>
#include <swgLib/portal.hpp>
#include <swgLib/lght.hpp>
#include <vector>
#include <string>

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
		/*
		void setUnknown1(unsigned char u1);
		unsigned char getUnknown1() const;

		void setName(const std::string& name);
		const std::string& getName() const;

		void setModelFilename(const std::string& filename);
		const std::string& getModelFilename() const;

		void setHasFloor(bool floor);
		bool hasFloor() const;

		void setFloorFilename(const std::string& filename);
		const std::string& getFloorFilename() const;
		*/
	protected:
		uint8_t _version; // 0001, 0002, 0003, 0004, 0005
		int32_t _numPortals;
		bool _canSeeParentCell;
		std::string _name;
		std::string _appearanceName;
		bool _hasFloor;
		std::string _floorName;

		cmsh _collisionMesh;
		std::vector<portal> _portals;

		int32_t _numLights;
		std::vector<lght> _lights;

	private:
	};
}

#endif
