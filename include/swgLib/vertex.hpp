/** -*-c++-*-
 *  \class  vertex
 *  \file   vertex.hpp
 *  \author Ken Sewell

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

#ifndef VERTEX_HPP
#define VERTEX_HPP 1

#include <istream>
#include <ostream>
#include <string>

namespace ml
{
	class vertex
	{
	public:
		vertex(uint32_t bpv, const char* dataPtr);
		~vertex();

		uint32_t getBytesPerVertex() const;

		static bool isSupportedSize(const unsigned int bpv)
		{
			switch (bpv)
			{
			case 32:
			case 36:
			case 40:
			case 44:
			case 48:
			case 52:
			case 56:
			case 60:
			case 64:
			case 68:
			case 72:
				return true;
				break;
			default: return false;
			};
		}

		void print(std::ostream &os) const;

		void getPosition(float& x, float& y, float& z) const;
		void getNormal(float& nx, float& ny, float& nz) const;
		void getTexCoords(uint32_t& numPairs, float* coords) const;

		void getColor(unsigned char* argb) const;

	protected:
		uint32_t _bytesPerVertex;
		const char* _dataPtr;
	};
}
#endif
