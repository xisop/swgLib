/** -*-c++-*-
 *  \class  mshVertexData
 *  \file   mshVertexData.cpp
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
#include <swgLib/mshVertexData.hpp>
#include <iostream>

using namespace ml;


mshVertexData::mshVertexData()
  :bytesPerVertex( 0 )
{
}

mshVertexData::~mshVertexData()
{
  this->clear();
}

bool mshVertexData::read(std::istream& file, unsigned int numVerts)
{
	if (0 == bytesPerVertex)
	{
		std::cerr << "Bytes per vertex needs set first." << std::endl;
		return false;
	}

	for (unsigned int i = 0; i < numVerts; ++i)
	{
		vertex* newVertex = new vertex(bytesPerVertex);
		if (!newVertex->read(file))
		{
			delete newVertex;
			return false;
		}
		v.push_back(newVertex);
	}

	//this->print();

	return true;
}

void mshVertexData::clear()
{
  while( !v.empty() )
	{
	  delete v.back();
	  v.pop_back();
	}
}

unsigned char mshVertexData::getBytesPerVertex() const
{
	return bytesPerVertex;
}

void mshVertexData::setBytesPerVertex(const unsigned char bpv)
{
	bytesPerVertex = bpv;
}

bool mshVertexData::isSupportedSize(const unsigned int bpv)
{
	return vertex::isSupportedSize(bpv);
}

uint32_t mshVertexData::getNumVertices() const
{
	return uint32_t(v.size());
}

const vertex* mshVertexData::getVertex(unsigned int vertexNum) const
{
	return v[vertexNum];
}

void mshVertexData::print(std::ostream& os) const
{
	for (auto i : v) {
		i->print(os);
		os << "\n";
	}
  }
  */