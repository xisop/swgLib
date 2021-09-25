/** -*-c++-*-
 *  \class  mshVertexData
 *  \file   mshVertexData.hpp
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

/*
#ifndef MSHVERTEXDATA_HPP
#define MSHVERTEXDATA_HPP 1

#include <swgLib/vertex.hpp>

#include <istream>
#include <ostream>
#include <vector>

namespace ml
{
	class mshVertexData
	{
	public:
		mshVertexData();
		virtual ~mshVertexData();

		bool read(std::istream& file, unsigned int numVerts);

		void clear();

		unsigned char getBytesPerVertex() const;
		void setBytesPerVertex(const unsigned char bpv);

		static bool isSupportedSize(const unsigned int bpv);

		uint32_t getNumVertices() const;

		const vertex* getVertex(unsigned int vertexNum) const;

		void print(std::ostream &os) const;

	protected:
		unsigned int bytesPerVertex;

		std::vector<vertex*> v;

	private:

	};
}
#endif
*/
