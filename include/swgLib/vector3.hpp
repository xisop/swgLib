/** -*-c++-*-
 *  \class  vector3
 *  \file   vector3.hpp
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

#ifndef VECTOR3_HPP
#define VECTOR3_HPP 1

namespace ml
{
	class vector3
	{
	public:
		vector3();
		vector3(const float* V);
		vector3(const float& X, const float& Y, const float& Z);
		~vector3();

		std::size_t read(std::istream& file);
		std::size_t write(std::ostream& file) const;

		void get(float& X, float& Y, float& Z) const;
		float getX() const;
		float getY() const;
		float getZ() const;

		void set(const float& X, const float& Y, const float& Z);
		void set(const float* V);
		void set(const vector3& vec);

		void print(std::ostream& os) const;

	protected:

	private:
		float v[3];
	};
}

std::ostream& operator<<(std::ostream& os, const ml::vector3& v);

#endif
