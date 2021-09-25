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
		std::size_t readEXBX(std::istream& file);
		std::size_t readHPTS(std::istream& file);

		void getBoundingSphere(vector3& center, float& radius) const;
		void appr::getBoundingBox(vector3& corner1, vector3& corner2);

	protected:
		std::size_t readFLOR(std::istream& file);

		vector3 _exspCenter;
		float   _exspRadius;

		vector3 _exbxCorner1;
		vector3 _exbxCorner2;

		cmsh _collisionMesh;

		std::vector<matrix3x4> _hardpointTransform;
		std::vector<std::string> _hardpointName;

		bool _hasFloor;
		std::string _floorName;

	private:
	};
}

#endif
