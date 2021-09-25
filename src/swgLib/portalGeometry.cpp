/** -*-c++-*-
 *  \class  portalGeometry
 *  \file   portalGeometry.cpp
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
#include <swgLib/portalGeometry.hpp>

#include <ios>

using namespace ml;

portalGeometry::portalGeometry() {
}

portalGeometry::~portalGeometry() {
}

std::size_t portalGeometry::read(std::istream& file) {

	std::size_t prtlSize;
	std::size_t total = base::readRecordHeader(file, "PRTL", prtlSize);
	prtlSize += 8;
	std::cout << "Found Record PRTL: " << prtlSize - 12 << " bytes\n";

	total += base::read(file, _numVertex);
	std::cout << "Num vertices: " << _numVertex << "\n";
	_vertex.resize(_numVertex);

	for (auto &v : _vertex) {
		total += base::read(file, v);
		std::cout << "Vertex: " << v << "\n";
	}

	return total;
}
