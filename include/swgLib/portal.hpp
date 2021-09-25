/** -*-c++-*-
 *  \class  portal
 *  \file   portal.hpp
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
#include <string>
#include <swgLib/matrix3.hpp>
#include <swgLib/vector3.hpp>

#ifndef PORTAL_HPP
#define PORTAL_HPP 1

namespace ml
{
	class portal
	{
	public:
		portal();
		~portal();

		std::size_t read(std::istream& file);

		//void setPortalID(const unsigned int& id);
		//unsigned int getPortalID() const;

		void setDisabled(bool disabled = true);
		bool isDisabled() const;

		void setPassable(bool passable = true);
		bool isPassable() const;

		void setGeometryIndex(const int32_t& geomIndex);
		const int32_t& getGeometryIndex() const;

		void setGeometryClockwise(bool clockwise = true);
		bool isGeometryClockwise() const;

		void setTargetCellIndex(const int32_t& index);
		const int32_t& getTargetCellIndex() const;

		void setDoorStyle(const std::string& doorStyle);
		const std::string& getDoorStyle() const;

		void setHasDoorHardpoint(bool hasDoorHardpoint = true);
		bool hasDoorHardpoint() const;

		void setDoorHardpointTransform(const matrix3x4& matrix);
		const matrix3x4& getDoorHardpointTransformMatrix() const;

		void print() const;

	protected:
		uint8_t _version;
		bool _disabled;
		bool _passable;
		int32_t _geometryIndex;
		bool _geometryWindClockwise;
		// Portal geometry...
		int32_t _targetCellIndex;
		std::string _doorStyle;
		bool _hasDoorHardpoint;
		matrix3x4 _doorHardpointTransform;

	private:
	};
}

#endif
