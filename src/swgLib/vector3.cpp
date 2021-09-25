/** -*-c++-*-
 *  \class  vector3
 *  \file   vector3.cpp
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

#include <swgLib/vector3.hpp>
#include <cstring>

using namespace ml;

vector3::vector3()
{
	v[0] = v[1] = v[2] = 0.0;
}

vector3::vector3(const float* V)
{
	memcpy(v, V, sizeof(float) * 3);
}

vector3::vector3(const float& X, const float& Y, const float& Z)
{
	v[0] = X;
	v[1] = Y;
	v[2] = Z;
}

vector3::~vector3() {}

std::size_t vector3::read(std::istream& file) {
	file.read((char*)v, sizeof(float) * 3);
	return(sizeof(float) * 3);
}

std::size_t vector3::write(std::ostream& file) const {
	file.write((char*)v, sizeof(float) * 3);
	return(sizeof(float) * 3);
}

void vector3::get(float& X, float& Y, float& Z) const
{
	X = v[0];
	Y = v[1];
	Z = v[2];
}

float vector3::getX() const
{
	return v[0];
}

float vector3::getY() const
{
	return v[1];
}

float vector3::getZ() const
{
	return v[2];
}

void vector3::set(const float& X, const float& Y, const float& Z)
{
	v[0] = X;
	v[1] = Y;
	v[2] = Z;
}

void vector3::set(const float* V)
{
	memcpy(v, V, sizeof(float) * 3);
}

void vector3::set(const vector3& vec)
{
	memcpy(v, vec.v, sizeof(float) * 3);
}

void vector3::print(std::ostream& os) const
{
	os << std::fixed << v[0] << ", "
		<< v[1] << ", " << v[2];
}

std::ostream& operator<<(std::ostream& os, const ml::vector3& v) {
	v.print(os);
	return os;
}
