/** -*-c++-*-
 *  \class  matrix3
 *  \file   matrix3.cpp
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

#include <swgLib/matrix3.hpp>
#include <cstring> // memcpy

using namespace ml;

matrix3x3::matrix3x3()
{
	for (unsigned int i = 0; i < 9; ++i)
	{
		v[i] = 0.0f;
	}
}

matrix3x3::matrix3x3(const matrix3x3& m)
{
	for (unsigned int i = 0; i < 9; ++i)
	{
		v[i] = m.v[i];
	}
}

matrix3x3::matrix3x3(const float* V)
{
	memcpy(v, V, sizeof(float) * 9);
}

matrix3x3::~matrix3x3() {}

std::size_t matrix3x3::read(std::istream& file) {
	file.read((char*)v, sizeof(float) * 9);
	return(sizeof(float) * 9);
}

std::size_t matrix3x3::write(std::ostream& file) const {
	file.write((char*)v, sizeof(float) * 9);
	return(sizeof(float) * 9);
}

float matrix3x3::get(int index) const
{
	if (index < 9) { return v[index]; }
	else { return 0.0f; }
}

void matrix3x3::get(float* V) const { memcpy(V, v, sizeof(float) * 9); }

void matrix3x3::set(const float* V) { memcpy(v, V, sizeof(float) * 9); }

void matrix3x3::set(const matrix3x3& m) { memcpy(v, m.v, sizeof(float) * 9); }

void matrix3x3::print(std::ostream& os) const {
	os << v[0] << ", " << v[1] << ", " << v[2] << "\n"
		<< v[3] << ", " << v[4] << ", " << v[5] << "\n"
		<< v[6] << ", " << v[7] << ", " << v[8];
}


matrix3x4::matrix3x4()
{
	for (unsigned int i = 0; i < 12; ++i)
	{
		v[i] = 0.0f;
	}
}

matrix3x4::matrix3x4(const matrix3x4& m)
{
	for (unsigned int i = 0; i < 12; ++i)
	{
		v[i] = m.v[i];
	}
}

matrix3x4::matrix3x4(const float* V)
{
	memcpy(v, V, sizeof(float) * 12);
}

matrix3x4::~matrix3x4() {}

std::size_t matrix3x4::read(std::istream& file) {
	file.read((char*)v, sizeof(float) * 12);
	return(sizeof(float) * 12);
}

std::size_t matrix3x4::write(std::ostream& file) const {
	file.write((char*)v, sizeof(float) * 12);
	return(sizeof(float) * 12);
}

float matrix3x4::get(int index) const
{
	if (index < 12)
	{
		return v[index];
	}
	else
	{
		return 0.0;
	}
}

std::size_t matrix3x4::get(float* V) const
{
	memcpy(V, v, sizeof(float) * 12);
	return std::size_t(12);
}

std::size_t matrix3x4::set(const float* V)
{
	memcpy(v, V, sizeof(float) * 12);
	return std::size_t(12);
}

void matrix3x4::set(const matrix3x4& m)
{
	memcpy(v, m.v, sizeof(float) * 12);
}

void matrix3x4::print(std::ostream& os) const
{
	os << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << "\n"
		<< v[4] << ", " << v[5] << ", " << v[6] << ", " << v[7] << "\n"
		<< v[8] << ", " << v[9] << ", " << v[10] << ", " << v[11];
}

std::ostream& operator<<(std::ostream& os, const ml::matrix3x3& m) {
	m.print(os);
	return os;
}

std::ostream& operator<<(std::ostream& os, const ml::matrix3x4& m) {
	m.print(os);
	return os;
}

