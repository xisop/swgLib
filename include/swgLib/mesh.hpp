/** -*-c++-*-
 *  \class  mesh
 *  \file   mesh.hpp
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
#include <swgLib/sps.hpp>

#include <swgLib/cshd.hpp>
#include <swgLib/sht.hpp>

#include <fstream>
#include <string>
#include <vector>

#ifndef MESH_HPP
#define MESH_HPP 1

namespace ml
{
	enum maxTextures {
		MAX_TEXTURES = 8
	};

	class mesh : public appr
	{
	public:
		mesh();
		~mesh();

		std::size_t readMESH(std::istream& file, bool skipSIDX=false);

		const sps& getSPS() const;

	protected:
		int8_t _version;

		sps _sps;

	private:
		std::size_t readV2V3(std::istream& file, bool skipSIDX = false);
		std::size_t readV4V5(std::istream& file, bool skipSIDX = false);
	};
}
#endif
