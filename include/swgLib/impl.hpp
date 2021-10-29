/** -*-c++-*-
 *  \class  impl
 *  \file   impl.hpp
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

#ifndef IMPL_HPP
#define IMPL_HPP 1

#include <swgLib/base.hpp>
#include <swgLib/pass.hpp>

#include <istream>
#include <string>
#include <vector>

namespace ml
{
	class impl
	{
	public:
		impl();
		~impl();

		std::size_t read(std::istream& file);

	protected:
		uint32_t _version; // 0000-0009
		std::vector<int32_t> _shaderCapability;
		std::vector<tag> _optionTag;

		int8_t  _numPasses;
		bool    _unknown;
		tag     _phase;
		bool _castsShadow;
		bool _isCollidable;

		std::vector<pass> _pass;


	private:
	};
}

#endif
