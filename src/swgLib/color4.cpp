/** -*-c++-*-
 *  \class  color4
 *  \file   color4.cpp
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

#include <swgLib/color4.hpp>
#include <swgLib/base.hpp>
#include <cstring>

using namespace ml;

color4::color4(const float& a, const float& r, const float& g, const float& b)
{
	v[0] = a;
	v[1] = r;
	v[2] = g;
	v[3] = b;
}

color4::color4(const uint8_t& a, const uint8_t& r, const uint8_t& g, const uint8_t& b)
{
	v[0] = a / 255.0f;
	v[1] = r / 255.0f;
	v[2] = g / 255.0f;
	v[3] = b / 255.0f;
}

color4::color4(const color4& color)
{
	memcpy(v, color.v, sizeof(float) * 4);
}

color4::color4(const float* V)
{
	memcpy(v, V, sizeof(float) * 4);
}

color4::~color4() {}

std::size_t color4::read32(std::istream& file) {
	std::size_t total = base::read(file, v[0]); // Alpha
	total += base::read(file, v[1]); // Red
	total += base::read(file, v[2]); // Green
	total += base::read(file, v[3]); // Blue

	return total;
}

std::size_t color4::read8(std::istream& file) {
	uint8_t a, r, g, b;
	std::size_t total = base::read(file, a); // Alpha
	total += base::read(file, r); // Red
	total += base::read(file, g); // Green
	total += base::read(file, b); // Blue

	v[0] = (a / 255.0f);
	v[1] = (r / 255.0f);
	v[2] = (g / 255.0f);
	v[3] = (b / 255.0f);

	return total;
}

void color4::get(float& A, float& R, float& G, float& B) const
{
	A = v[0];
	R = v[1];
	G = v[2];
	B = v[3];
}

float color4::getA() const { return v[0]; }
float color4::getR() const { return v[1]; }
float color4::getG() const { return v[2]; }
float color4::getB() const { return v[3]; }

void color4::set(const float& A, const float& R, const float& G, const float& B)
{
	v[0] = A;
	v[1] = R;
	v[2] = G;
	v[3] = B;
}

void color4::set(const float* V)
{
	memcpy(v, V, sizeof(float) * 4);
}

void color4::set(const color4& vec)
{
	memcpy(v, vec.v, sizeof(float) * 4);
}

void color4::print(std::ostream& os) const
{
	os << std::fixed << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3];
}

std::ostream& operator<<(std::ostream& os, const ml::color4& c) {
	c.print(os);
	return os;
}
