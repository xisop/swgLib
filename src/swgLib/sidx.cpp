/** -*-c++-*-
 *  \class  sidx
 *  \file   sidx.cpp
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

#include <swgLib/sidx.hpp>
#include <swgLib/base.hpp>

using namespace ml;

sidx::sidx() {
}

sidx::~sidx() {
}

std::size_t sidx::read(std::istream& file, bool sidx16) {
	std::size_t total = base::read(file, _numArrays);
	std::cout << "Directional Sorted Index buffers: " << _numArrays << "\n";

	_first.resize(_numArrays);
	_second.resize(_numArrays);

	for (auto i = 0; i < _numArrays; ++i) {
		std::cout << "Sorted index " << i << ":\n";
		total += base::read(file, _first[i]);
		std::cout << "first: " << _first[i] << "\n";
		total += _second[i].read(file, sidx16);
	}

	return total;
}

const int32_t& sidx::getNumArrays() const { return _numArrays; }
const vector3& sidx::getVector(const std::size_t& v) const { return _first.at(v); }
const indx& sidx::getArray(const std::size_t& a) const { return _second.at(a); }
