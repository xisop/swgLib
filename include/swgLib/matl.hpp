/** -*-c++-*-
 *  \class  matl
 *  \file   matl.hpp
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

#ifndef MATL_HPP
#define MATL_HPP 1

#include <istream>
#include <string>

#include <swgLib/base.hpp>
#include <swgLib/color4.hpp>

namespace ml
{
	class matl
	{
	public:
		matl();
		~matl();

		std::size_t read(std::istream& file);

		void setNameTag(const tag& nameTag);
		const tag& getNameTag() const;

		const color4& getAmbient() const;
		const color4& getDiffuse() const;
		const color4& getEmissive() const;
		const color4& getSpecular() const;
		const float& getPower() const;

	protected:
		tag _nameTag;

		color4 _ambient;
		color4 _diffuse;
		color4 _emissive;
		color4 _specular;
		float  _power;

	private:
	};
}

#endif
