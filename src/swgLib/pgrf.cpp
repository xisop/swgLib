/** -*-c++-*-
 *  \class  pgrf
 *  \file   pgrf.cpp
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

#include <swgLib/pgrf.hpp>
#include <swgLib/base.hpp>

using namespace ml;

pgrf::pgrf() {
}

pgrf::~pgrf() {
}

std::size_t pgrf::read(std::istream& file) {
	std::size_t pgrfSize;
	std::size_t total = base::readFormHeader(file, "PGRF", pgrfSize);
	pgrfSize += 8;
	std::cout << "Found FORM PGRF: " << ": " << pgrfSize - 12 << " bytes\n";

	std::size_t size;
	std::string form, type;
	total += base::readFormHeader(file, form, size, type);
	if (form != "FORM")
	{
		std::cout << "Expected FORM not: " << form << std::endl;
		exit(0);
	}

	_version = 0;
	if ("0000" == type) {
		_version = 1;
	}
	else if ("0001" == type) {
		_version = 2;
	}
	else {
		std::cout << "Expected FORM of type 0000 or 0001: " << type << std::endl;
		exit(0);
	}

	std::cout << "Found FORM " << type
		<< ": " << size - 4 << " bytes\n";

	// Only in versions 1+
	if (0 < _version) {
		// Read META...
		total += base::readRecordHeader(file, "META", size);
		std::cout << "Found record META: " << size << " bytes\n";
		int32_t x;
		total += base::read(file, x);
	}

	// Read Path Node (PNOD)...
	total += base::readRecordHeader(file, "PNOD", size);
	std::cout << "Found record PNOD: " << size << " bytes\n";

	int32_t numNodes = 0;
	total += base::read(file, numNodes);
	std::cout << "Number of path nodes: " << numNodes << "\n";

	_index.resize(numNodes);
	_id.resize(numNodes);
	_key.resize(numNodes);
	_type.resize(numNodes);
	_position.resize(numNodes);
	_radius.resize(numNodes);
	for (auto i = 0; i < numNodes; ++i) {
		total += base::read(file, _index[i]);
		total += base::read(file, _id[i]);
		total += base::read(file, _key[i]);
		total += base::read(file, _type[i]);
		total += base::read(file, _position[i]);
		total += base::read(file, _radius[i]);
		std::cout
			<< "Node " << i << ":\n"
			<< "   Index: " << _index[i] << "\n"
			<< "      ID: " << _id[i] << "\n"
			<< "     Key: " << _key[i] << "\n"
			<< "    Type: " << _type[i] << "\n"
			<< "Position: " << _position[i] << "\n"
			<< "  Radius: " << _radius[i] << "\n\n";
	}

	// Read Path Edge (PEDG)...
	total += base::readRecordHeader(file, "PEDG", size);
	std::cout << "Found record PEDG: " << size << " bytes\n";

	int32_t numEdges = 0;
	total += base::read(file, numEdges);
	std::cout << "Number of path edges: " << numEdges << "\n";
	_aIndex.resize(numEdges);
	_bIndex.resize(numEdges);
	_laneWidthRight.resize(numEdges);
	_laneWidthLeft.resize(numEdges);
	for (auto i = 0; i < numEdges; ++i) {
		total += base::read(file, _aIndex[i]);
		total += base::read(file, _bIndex[i]);
		total += base::read(file, _laneWidthRight[i]);
		total += base::read(file, _laneWidthLeft[i]);
		std::cout
			<< "Edge " << i << ":\n"
			<< "Index A: " << _aIndex[i] << "\n"
			<< "Index B: " << _bIndex[i] << "\n"
			<< "Right lane width: " << _laneWidthRight[i] << "\n"
			<< " Left lane width: " << _laneWidthLeft[i] << "\n\n";
	}

	// Read Edge Count (ECNT)...
	total += base::readRecordHeader(file, "ECNT", size);
	std::cout << "Found record ECNT: " << size << " bytes\n";

	int32_t numEdgeCount = 0;
	total += base::read(file, numEdgeCount);
	std::cout << "Edge count: " << numEdgeCount << "\n";
	
	_edgeCount.resize(numEdgeCount);
	for (auto i = 0; i < numEdgeCount; ++i) {
		total += base::read(file, _edgeCount[i]);
		std::cout << "Edge count " << i << ": " << _edgeCount[i] << "\n";
	}

	// Read Edge Starts (ESTR)...
	total += base::readRecordHeader(file, "ESTR", size);
	std::cout << "Found record ESTR: " << size << " bytes\n";

	int32_t numEdgeStarts = 0;
	total += base::read(file, numEdgeStarts);
	std::cout << "Number of edge starts: " << numEdgeStarts << "\n";

	_edgeStart.resize(numEdgeStarts);
	for (auto i = 0; i < numEdgeStarts; ++i) {
		total += base::read(file, _edgeStart[i]);
		std::cout << "Edge start " << i << ": " << _edgeStart[i] << "\n";
	}

	return total;
}
