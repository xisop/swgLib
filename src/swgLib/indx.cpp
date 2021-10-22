/** -*-c++-*-
 *  \class  indx
 *  \file   indx.cpp
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

#include <swgLib/indx.hpp>
#include <swgLib/base.hpp>

using namespace ml;

indx::indx() {
}

indx::~indx() {
}

std::size_t indx::read(std::istream& file, bool index16) {
	std::size_t total = base::read(file, _numIndices);
	std::cout << "Number of indices: " << _numIndices << "\n";

	if (index16) {
		// 16-bits per index...
		const std::size_t indexDataSize(_numIndices * 2);

		// Read data...
		std::vector<uint16_t> tempIndex(_numIndices);
		file.read((char*)(tempIndex.data()), indexDataSize);

		_index.resize(_numIndices);
		for (uint32_t i = 0; i < _numIndices; ++i) {
			_index[i] = tempIndex[i];
		}
		total += indexDataSize;
	}
	else {
		// 32-bits per index...
		const std::size_t indexDataSize(_numIndices * 4);
		// Read data...
		file.read((char*)(_index.data()), indexDataSize);

		total += indexDataSize;
	}

	return total;
}

const uint32_t indx::getNumIndices() const {
	return _numIndices;
}

std::vector<int32_t>& indx::getIndices() {
	return _index;
}

const std::vector<int32_t>& indx::getIndices() const {
	return _index;

}

const int32_t indx::getIndex(const uint32_t& i) const {
	return _index.at(i);
}
