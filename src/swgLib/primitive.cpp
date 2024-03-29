/** -*-c++-*-
 *  \class  primitive
 *  \file   primitive.cpp
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

#include <swgLib/primitive.hpp>
#include <swgLib/base.hpp>

using namespace ml;

primitive::primitive() :
	_primitiveType(0),
	_hasIndices(false),
	_hasSortedIndices(false) {
}

primitive::~primitive() {
}

std::size_t primitive::readOld(std::istream& file, bool skipSIDX) {
	std::string form, type;
	std::size_t primitiveSize;
	std::size_t total = base::readFormHeader(file, form, primitiveSize, type);
	primitiveSize += 8;
	std::cout << form << ":" << type << "\n";

	std::size_t size;
	total += base::readRecordHeader(file, "INFO", size);

	total += base::read(file, _primitiveType);
	std::cout << "    Primitive type: " << shaderPrimitiveTypeToString(_primitiveType) << "\n";

	// Load vertices
	total += _vertex.read(file);

	// These primitive types need index array...
	if ((_primitiveType >= IndexedPointList) && (_primitiveType <= IndexedTriangleFan)) {
		std::cout << "       Has indices: true\n";
		_hasIndices = true;

		std::size_t indxSize, indxRead;
		base::readRecordHeader(file, "INDX", indxSize);
		std::cout << "Found record INDX: " << indxSize << "\n";

		// Indices are 32-bit
		indxRead = _index.readRaw(file, uint32_t(indxSize/4), false);

		if (indxSize != indxRead) {
			std::cout << "INDX data mismatch. Expected: " << indxSize << ", found: " << indxRead << "\n";
			exit(0);
		}

		total += indxRead + 8;

		if (IndexedTriangleList == _primitiveType) {
			//Reverse winding order of vertices
			_index.reverseTriangleList();
		}
	}
	else {
		std::cout << "       Has indices: false\n";
		_hasIndices = false;
	}
	_hasSortedIndices = false;

	if (primitiveSize == total)
	{
		std::cout << "Finished reading primitive\n";
	}
	else
	{
		std::cout << "FAILED in reading primitive\n";
		std::cout << "Read " << total << " out of " << primitiveSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t primitive::read(std::istream& file, bool skipSIDX) {
	std::string type;
	std::size_t primitiveSize;
	std::size_t total = base::readFormHeader(file, type, primitiveSize);
	primitiveSize += 8;
	uint8_t version = base::tagToVersion(type);
	if (0 == version) {
		std::cout << "Primitive version: 0 (Indices are 32-bit)\n";
	}
	else if (1 == version) {
		std::cout << "Primitive version: 1 (Indices are 16-bit)\n";
	}

	std::size_t size;
	total += base::readRecordHeader(file, "INFO", size);

	total += base::read(file, _primitiveType);
	total += base::read(file, _hasIndices);
	total += base::read(file, _hasSortedIndices);

	std::cout
		<< "    Primitive type: " << shaderPrimitiveTypeToString(_primitiveType) << "\n"
		<< "       Has indices: " << std::boolalpha << _hasIndices << "\n"
		<< "Has sorted indices: " << std::boolalpha << _hasSortedIndices << "\n";

	// Load vertices
	total += _vertex.read(file);

	if (_hasIndices) {
		std::size_t indxSize, indxRead;
		base::readRecordHeader(file, "INDX", indxSize);
		std::cout << "Found record INDX: " << indxSize << "\n";

		// Load indices
		if (0 == version) {
			// Indices are 32-bit
			indxRead = _index.read(file, false);
		}
		else {
			// Indices are 16-bit.
			indxRead = _index.read(file, true);
		}

		if (indxSize != indxRead) {
			std::cout << "INDX data mismatch. Expected: " << indxSize << ", found: " << indxRead << "\n";
			exit(0);
		}

		total += indxRead + 8;

		if (IndexedTriangleList == _primitiveType) {
			//Reverse winding order of vertices
			_index.reverseTriangleList();
		}
	}

	if (_hasSortedIndices) {
		std::size_t sidxSize;
		base::readRecordHeader(file, "SIDX", sidxSize);
		if (skipSIDX) {
			_hasSortedIndices = false;
			file.seekg(sidxSize, std::ios_base::cur);
		}
		else {
			std::cout << "Found record SIDX: " << size << "\n";

			std::size_t sidxRead = 0;
			// Load sorted indices
			if (0 == version) {
				// Indices are 32-bit
				sidxRead = _sortedIndex.read(file, false);
			}
			else {
				// Indices are 16-bit.
				sidxRead = _sortedIndex.read(file, true);
			}

			if (sidxSize != sidxRead) {
				std::cout << "SIDX data mismatch. Expected: " << sidxSize << ", found: " << sidxRead << "\n";
				exit(0);
			}

			total += sidxRead + 8;
		}
	}

	if (primitiveSize == total)
	{
		std::cout << "Finished reading primitive\n";
	}
	else
	{
		std::cout << "FAILED in reading primitive\n";
		std::cout << "Read " << total << " out of " << primitiveSize << "\n";
		exit(0);
	}

	return total;
}

const int32_t& primitive::getPrimitiveType() const { return _primitiveType; }
bool primitive::hasIndices() const { return _hasIndices; }
bool primitive::hasSortedIndices() const { return _hasSortedIndices; }
const vtxa& primitive::getVTXA() const { return _vertex; }
const indx& primitive::getINDX() const { return _index; }
const sidx& primitive::getSIDX() const { return _sortedIndex; }

std::string primitive::shaderPrimitiveTypeToString(const int32_t& type) {
	switch (type) {
	case PointList:            return std::string("Point list"); break;
	case LineList:             return std::string("Line list"); break;
	case LineStrip:            return std::string("Line Strip"); break;
	case TriangleList:         return std::string("Triangle List"); break;
	case TriangleStrip:        return std::string("Triangle Strip"); break;
	case TriangleFan:          return std::string("Triangle Fan"); break;
	case IndexedPointList:     return std::string("Indexed Point List"); break;
	case IndexedLineList:      return std::string("Indexed Line List"); break;
	case IndexedLineStrip:     return std::string("Indexed Line Strip"); break;
	case IndexedTriangleList:  return std::string("Indexed Triangle List"); break;
	case IndexedTriangleStrip: return std::string("Indexed Triangle Strip"); break;
	case IndexedTriangleFan:   return std::string("Indexed Triangle Fan"); break;
	}
	return std::string("Unknown primitive type");
}
