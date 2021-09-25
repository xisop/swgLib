/** -*-c++-*-
 *  \class  idtl
 *  \file   idtl.cpp
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

#include <swgLib/idtl.hpp>
#include <swgLib/base.hpp>

using namespace ml;

idtl::idtl() {
}

idtl::~idtl() {
}

std::size_t idtl::read(std::istream& file) {
	std::size_t idtlSize;
	std::size_t total = base::readFormHeader(file, "IDTL", idtlSize);
	idtlSize += 8;
	std::cout << "Found IDTL FORM: " << idtlSize - 12 << " bytes\n";

	std::size_t size;
	std::string form, type;
	total += base::readFormHeader(file, "0000", size);
	size += 8;
	std::cout << "Found 0000 FORM: " << size - 12 << " bytes\n";

	total += readVERT(file, _vertex);
	total += readINDX(file, _index);

	if (idtlSize == total)
	{
		std::cout << "Finished reading IDTL" << std::endl;
	}
	else
	{
		std::cout << "Failed in reading IDTL" << std::endl;
		std::cout << "Read " << total << " out of " << idtlSize
			<< std::endl;
	}

	return total;
}

std::size_t idtl::readVERT(std::istream& file, std::vector<vector3>& vec)
{
	std::size_t vertSize;
	std::size_t total = base::readRecordHeader(file, "VERT", vertSize);
	vertSize += 8;
	std::cout << "Found VERT record" << std::endl;

	unsigned int numVerts = (vertSize - 8) / (sizeof(float) * 3);
	std::cout << "Number of vertices: " << numVerts << std::endl;
	vec.resize(numVerts);
	int32_t count = 0;
	for (auto& v : vec) {
		total += v.read(file);

		std::cout << "Vertex: " << v << "\n";

	}

	if (vertSize == total)
	{
		std::cout << "Finished reading VERT" << std::endl;
	}
	else
	{
		std::cout << "Failed in reading VERT" << std::endl;
		std::cout << "Read " << total << " out of " << vertSize
			<< std::endl;
	}

	return total;
}

std::size_t idtl::readINDX(std::istream& file, std::vector<int32_t>& index)
{
	std::size_t indxSize;
	std::size_t total = base::readRecordHeader(file, "INDX", indxSize);
	indxSize += 8;
	std::cout << "Found INDX record" << std::endl;

	int32_t numIndex = (indxSize - 8) / sizeof(int32_t);
	index.resize(numIndex);
	int32_t count = 0;
	for (auto& i : index) {
		total += base::read(file, i);
		if (0 == count) {
			std::cout << "Triangle: ";
		}

		std::cout << i << ", ";

		if (2 == count) {
			std::cout << "\n";
			count = 0;
		}
		else { ++count; }
	}

	if (indxSize == total)
	{
		std::cout << "Finished reading INDX" << std::endl;
	}
	else
	{
		std::cout << "Failed in reading INDX" << std::endl;
		std::cout << "Read " << total << " out of " << indxSize
			<< std::endl;
	}

	return total;
}
