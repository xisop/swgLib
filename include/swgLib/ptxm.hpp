/** -*-c++-*-
 *  \class  ptxm
 *  \file   ptxm.hpp
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

#ifndef PTXM_HPP
#define PTXM_HPP 1

#include<swgLib/base.hpp>

#include <istream>

namespace ml
{
	class ptxm
	{
	public:
		ptxm();
		~ptxm();

		std::size_t read(std::istream& file);
	protected:
		uint32_t _version; // 0000, 0001, or 0002
		int8_t _textureIndex;
		tag    _textureTag;

		uint8_t _textureAddressU;
		uint8_t _textureAddressV;
		uint8_t _textureAddressW;
		uint8_t _textureMipFilter;
		uint8_t _textureMinFilter;
		uint8_t _textureMagFilter;

	private:
	};
}

#endif
