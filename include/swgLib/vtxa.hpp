/** -*-c++-*-
 *  \class  vtxa
 *  \file   vtxa.hpp
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

#ifndef VTXA_HPP
#define VTXA_HPP 1

#include <swgLib/color4.hpp>
#include <swgLib/vector3.hpp>
#include <swgLib/vertex.hpp>

#include <istream>
#include <vector>

namespace ml
{
	class vtxa
	{
	public:
		vtxa();
		~vtxa();

		std::size_t read(std::istream& file);

		static bool hasPosition(uint32_t flags);
		static bool isTransformed(uint32_t flags);
		static bool hasNormal(uint32_t flags);
		static bool hasColor0(uint32_t flags);
		static bool hasColor1(uint32_t flags);
		static bool hasPointSize(uint32_t flags);
		void processD3DFVF(uint32_t& codes, uint32_t& numTex);

		bool getPosition(const uint32_t& vertexNumber, float& x, float& y, float& z) const;
		bool getNormal(const uint32_t& vertexNumber, float& nx, float& ny, float& nz) const;
		bool getColor0(const uint32_t& vertexNumber, color4& color) const;
		bool getColor1(const uint32_t& vertexNumber, color4& color) const;

	protected:
		int32_t _numVertices;
		int32_t _numUVs;
		uint32_t _flags;

		// Bytes per vertex
		uint32_t _bytesPerVertex;
		bool _hasPosition;
		bool _hasTransformed;
		bool _hasNormal;
		bool _hasColor0;
		bool _hasColor1;
		bool _hasTexCoord[8];

		uint8_t _texCoordDim[8];

		std::vector<char> _vertexData;

	private:
	};
}

#endif
