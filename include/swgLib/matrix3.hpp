/** -*-c++-*-
 *  \class  matrix3
 *  \file   matrix3.hpp
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

#ifndef MATRIX3_HPP
#define MATRIX3_HPP 1

#include <iostream>

namespace ml
{
	class matrix3x3
	{
	public:
		matrix3x3();
		matrix3x3(const matrix3x3& m);
		matrix3x3(const float* V);
		~matrix3x3();

		std::size_t read(std::istream& file);
		std::size_t write(std::ostream& file) const;

		float get(int index) const;

		void getRow(const uint32_t &row, float &v1, float &v2, float &v3) const;

		void get(float* V) const;

		void getTransposed4x4(float* dest) const;

		void set(const float* V);

		void set(const matrix3x3& m);

		void print(std::ostream& os) const;

	private:
		float v[9];
	};

	class matrix3x4
	{
	public:
		matrix3x4();
		matrix3x4(const matrix3x4& m);
		matrix3x4(const float* V);
		~matrix3x4();

		std::size_t read(std::istream& file);
		std::size_t write(std::ostream& file) const;

		float get(int index) const;

		void getRow(const uint32_t& row, float& v1, float& v2, float& v3, float& v4) const;

		std::size_t get(float* V) const;

		void getTransposed4x4(float* dest) const;

		std::size_t  set(const float* V);

		void set(const matrix3x4& m);

		void print(std::ostream& os) const;

	private:
		float v[12];
	};
}

std::ostream& operator<<(std::ostream& os, const ml::matrix3x3& m);
std::ostream& operator<<(std::ostream& os, const ml::matrix3x4& m);

#endif
