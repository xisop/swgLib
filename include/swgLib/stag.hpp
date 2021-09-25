/** -*-c++-*-
 *  \class  stag
 *  \file   stag.hpp
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

#ifndef STAG_HPP
#define STAG_HPP 1

#include<swgLib/base.hpp>

#include <istream>

namespace ml
{
	class stag
	{
	public:
		stag();
		~stag();

		std::size_t read(std::istream& file);
	protected:
		uint8_t _version; // 0000 or 0001

		uint8_t _colorOperation;

		uint8_t _colorArgument0;
		bool    _colorArgument0Complement;
		bool    _colorArgument0AlphaReplicate;

		uint8_t _colorArgument1;
		bool    _colorArgument1Complement;
		bool    _colorArgument1AlphaReplicate;

		uint8_t _colorArgument2;
		bool    _colorArgument2Complement;
		bool    _colorArgument2AlphaReplicate;

		uint8_t _alphaOperation;

		uint8_t _alphaArgument0;
		bool    _alphaArgument0Complement;

		uint8_t _alphaArgument1;
		bool    _alphaArgument1Complement;

		uint8_t _alphaArgument2;
		bool    _alphaArgument2Complement;

		uint8_t _resultArgument;

		tag     _textureTag;
		tag     _textureCoordinateSetTag;
		uint8_t _textureAddressU;
		uint8_t _textureAddressV;
		uint8_t _textureAddressW;
		uint8_t _textureMipFilter;
		uint8_t _textureMinificationFilter;
		uint8_t _textureMagnificationFilter;
		uint8_t _textureCoordinateGeneration;

	private:
	};
}

#endif
