/** -*-c++-*-
 *  \class  lght
 *  \file   lght.hpp
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

#include <swgLib/portal.hpp>
#include <swgLib/color4.hpp>
#include <swgLib/matrix3.hpp>

#include <istream>

#ifndef LGHT_HPP
#define LGHT_HPP 1

namespace ml
{
	class lght
	{
	public:
		enum {
			Ambient = 0,
			Parallel = 1,
			Point = 2
		};

	public:
		lght();
		~lght();

		std::size_t read(std::istream& file);

		const int8_t& getType() const;
		const color4& getDiffuseColor() const;
		const color4& getSpecularColor() const;
		const matrix3x4& getTransform() const;
		const float& getConstantAttenuation() const;
		const float& getLinearAttenuation() const;
		const float& getQuadraticAttentuation() const;

	protected:
		int8_t _type;
		color4 _diffuseColor;
		color4 _specularColor;
		matrix3x4 _transform;
		float _constantAttenuation;
		float _linearAttenuation;
		float _quadraticAttenuation;
	};
}

#endif
