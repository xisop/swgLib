/** -*-c++-*-
 *  \class  appr
 *  \file   appr.hpp
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

#include <swgLib/vector3.hpp>
#include <swgLib/matrix3.hpp>
#include <swgLib/cmsh.hpp>
#include <swgLib/baseCollision.hpp>

#include <istream>
#include <vector>

#ifndef APPR_HPP
#define APPR_HPP 1

namespace ml
{
	class appr
	{
	public:
		appr();
		~appr();

		std::size_t read(std::istream& file);
		std::size_t readHPTS(std::istream& file);

	protected:
		uint8_t _apprVersion;

		std::size_t readFLOR(std::istream& file);

		baseCollisionPtr _boundingPtr;
		baseCollisionPtr _collisionPtr;

		std::vector<matrix3x4> _hardpointTransform;
		std::vector<std::string> _hardpointName;

		bool _hasFloor;
		std::string _floorName;

	private:
	};
}

#endif
