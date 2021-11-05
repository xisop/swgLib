/** -*-c++-*-
 *  \class  tgenBaseLayer
 *  \file   tgenBaseLayer.cpp
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

#include <swgLib/tgenBaseLayer.hpp>
#include <swgLib/base.hpp>

using namespace ml;

tgenBaseLayer::tgenBaseLayer() :
	_tag(""),
	_active(false),
	_pruned(false),
	_name("")
{
}

tgenBaseLayer::~tgenBaseLayer() {
}

std::size_t tgenBaseLayer::read(std::istream& file) {
	std::size_t ihdrSize;
	std::size_t total = base::readFormHeader(file, "IHDR", ihdrSize);
	ihdrSize += 8;

	std::string type;
	std::size_t size;
	total += base::readFormHeader(file, type, size);
	uint32_t version = base::tagToVersion(type);
	if (version > 1) {
		std::cout << "Expected type [0000...0001]: " << type << "\n";
		exit(0);
	}
	std::cout << "Base Layer version: " << version << "\n";

	total += base::readRecordHeader(file, "DATA", size);

	int32_t temp;
	total += base::read(file, temp); _active = (0 != temp);
	total += base::read(file, _name);

	if (0 == version) {
		uint8_t unusedR, unusedG, unusedB;
		total += base::read(file, unusedR);
		total += base::read(file, unusedG);
		total += base::read(file, unusedB);
	}

	if (ihdrSize != total) {
		std::cout << "Failed in reading Base Layer\n";
		std::cout << "Read " << total << " out of " << ihdrSize << "\n";
		exit(0);
	}

	return total;
}

const std::string& tgenBaseLayer::getTag() const { return _tag; }

bool tgenBaseLayer::isActive() const { return _active; }

bool tgenBaseLayer::isPruned() const { return _pruned; }

const std::string& tgenBaseLayer::getName() const { return _name; }
