/** -*-c++-*-
 *  \class  color4
 *  \file   color4.hpp
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

#include <iostream>
#include <swgLib/base.hpp>

#ifndef COLOR4_HPP
#define COLOR4_HPP 1

namespace ml
{
	class color4
	{
	public:
		color4(const float& a = 0.0f, const float& r = 0.0f, const float& g = 0.0f, const float& b = 0.0f);
		color4(const uint8_t& a, const uint8_t& r, const uint8_t& g, const uint8_t& b);
		color4(const color4& color);
		color4(const float* V);
		~color4();

		std::size_t read32(std::istream& file);
		std::size_t read8(std::istream& file);

		void get(float& A, float& R, float& G, float& B) const;

		float getA() const;
		float getR() const;
		float getG() const;
		float getB() const;

		void set(const float& A, const float& R, const float& G, const float& B);
		void set(const float* V);
		void set(const color4& vec);

		void print(std::ostream& os) const;

	protected:

	private:
		float v[4];
	};
}

std::ostream& operator<<(std::ostream& os, const ml::color4& c);

#endif
