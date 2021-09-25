/** -*-c++-*-
 *  \class  pffp
 *  \file   pffp.hpp
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

#ifndef PFFP_HPP
#define PFFP_HPP 1

#include<swgLib/base.hpp>

#include <istream>

namespace ml
{
	class pffp
	{
	public:
		pffp();
		~pffp();

		std::size_t read(std::istream& file);
	protected:
		uint8_t _version; // 0000 or 0001

		bool    _lighting;
		bool    _lightingSpecularEnable;
		bool    _lightingColorVertex;
		tag     _lightingMaterialTag;
		uint8_t _lightingAmbientColorSource;
		uint8_t _lightingDiffuseColorSource;
		uint8_t _lightingSpecularColorSource;
		uint8_t _lightingEmissiveColorSource;

	private:
	};
}

#endif
