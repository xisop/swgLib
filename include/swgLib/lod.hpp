/** -*-c++-*-
 *  \class  lod
 *  \file   lod.hpp
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
#include <swgLib/base.hpp>
#include <swgLib/appr.hpp>

#include <istream>
#include <string>
#include <vector>

#ifndef LOD_HPP
#define LOD_HPP 1

namespace ml
{
	class lod : public base
	{
	public:
		struct lodChild {
			int id;
			float near;
			float far;
			std::string name;
		};

		lod();
		~lod();

		bool isRightType(std::istream& file)
		{
			return isOfType(file, "DTLA");
		}

		std::size_t readLOD(std::istream& file, std::string path = "");

		uint32_t getNumLODs() const
		{
			return uint32_t(_child.size());
		}

		// Can be .msh or .cmp
		bool getChild(int32_t id, struct lodChild& requestedChild) const;

		void getBoundingSphere( vector3& center, float& radius ) const
		{
			_appearance.getBoundingSphere(center, radius);
		}

		void getBoundingBox( vector3& corner1, vector3& corner2 )
		{
			_appearance.getBoundingBox(corner1, corner2);
		}

	protected:

	private:
		uint8_t _version;

		appr _appearance;

		// DTLA Version 6+
		uint8_t _lodFlags;
		bool _usePivotPoint;

		bool _disableLODCrossFade;

		std::vector<lodChild> _child;

		// Radar shape...
		bool _hasRadar;
		idtl _radarShape;

		// Test shape...
		bool _hasTest;
		idtl _testShape;

		// Write shape...
		bool _hasWrite;
		idtl _writeShape;
	};
}
#endif
