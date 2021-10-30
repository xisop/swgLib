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
#include <iomanip> // setprecision
#include <cmath> // atan2

using namespace ml;

matrix3x3::matrix3x3()
{
	for (unsigned int i = 0; i < 9; ++i) {
		v[i] = 0.0f;
	}
}

matrix3x3::matrix3x3(const matrix3x3& m)
{
	for (unsigned int i = 0; i < 9; ++i) {
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

float matrix3x3::get(int index) const {
	if (index < 9) { return v[index]; }
	else { return 0.0f; }
}

void matrix3x3::getRow(const uint32_t& row, float& v1, float& v2, float& v3) const {
	if (row > 2) { return; }
	const uint32_t offset = (row * 3);
	v1 = v[offset];
	v2 = v[offset + 1];
	v3 = v[offset + 2];
}

void matrix3x3::get(float* V) const { memcpy(V, v, sizeof(float) * 9); }

void matrix3x3::set(const float* V) { memcpy(v, V, sizeof(float) * 9); }

void matrix3x3::set(const matrix3x3& m) { memcpy(v, m.v, sizeof(float) * 9); }

void matrix3x3::print(std::ostream& os) const {
	os << std::fixed << std::setprecision(4)
		<< std::setw(9) << v[0] << ", "
		<< std::setw(9) << v[1] << ", "
		<< std::setw(9) << v[2] << "\n"
		<< std::setw(9) << v[3] << ", "
		<< std::setw(9) << v[4] << ", "
		<< std::setw(9) << v[5] << "\n"
		<< std::setw(9) << v[6] << ", "
		<< std::setw(9) << v[7] << ", "
		<< std::setw(9) << v[8];
}

matrix3x4::matrix3x4() {
	for (unsigned int i = 0; i < 12; ++i)
	{
		v[i] = 0.0f;
	}
}

matrix3x4::matrix3x4(const matrix3x4& m) {
	for (unsigned int i = 0; i < 12; ++i)
	{
		v[i] = m.v[i];
	}
}

matrix3x4::matrix3x4(const float* V) {
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

float matrix3x4::get(int index) const {
	if (index < 12)
	{
		return v[index];
	}
	else
	{
		return 0.0;
	}
}

std::size_t matrix3x4::get(float* V) const {
	memcpy(V, v, sizeof(float) * 12);
	return std::size_t(12);
}

void matrix3x4::getTranslation(float& tx, float& ty, float& tz) const {
	tx = v[3];
	ty = v[7];
	tz = v[11];
}

void matrix3x4::getRotation(float& rx, float& ry, float& rz) const {
	rx = std::atan2(-v[6], v[10]);
	const float cosYangle = sqrt((v[0] * v[0]) + (v[1] * v[1]));
	ry = std::atan2(v[2], cosYangle);
	const float sinXangle = std::sin(rx);
	const float cosXangle = std::cos(rx);
	rz = std::atan2((cosXangle * v[4]) + (sinXangle * v[8]), (cosXangle * v[5]) + (sinXangle * v[9]));
}

void matrix3x4::getRow(const uint32_t& row, float& v1, float& v2, float& v3, float& v4) const {
	if (row > 2) { return; }
	const uint32_t offset = (row * 4);
	v1 = v[offset];
	v2 = v[offset + 1];
	v3 = v[offset + 2];
	v4 = v[offset + 3];
}

std::size_t matrix3x4::set(const float* V) {
	memcpy(v, V, sizeof(float) * 12);
	return std::size_t(12);
}

void matrix3x4::set(const matrix3x4& m) {
	memcpy(v, m.v, sizeof(float) * 12);
}

void matrix3x4::print(std::ostream& os) const {
	os << std::fixed << std::setprecision(4)
		<< std::setw(9) << v[0] << ", "
		<< std::setw(9) << v[1] << ", "
		<< std::setw(9) << v[2] << ", "
		<< std::setw(9) << v[3] << "\n"
		<< std::setw(9) << v[4] << ", "
		<< std::setw(9) << v[5] << ", "
		<< std::setw(9) << v[6] << ", "
		<< std::setw(9) << v[7] << "\n"
		<< std::setw(9) << v[8] << ", "
		<< std::setw(9) << v[9] << ", "
		<< std::setw(9) << v[10] << ", "
		<< std::setw(9) << v[11];
}

std::ostream& operator<<(std::ostream& os, const ml::matrix3x3& m) {
	m.print(os);
	return os;
}

std::ostream& operator<<(std::ostream& os, const ml::matrix3x4& m) {
	m.print(os);
	return os;
}
