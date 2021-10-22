/** -*-c++-*-
 *  \class  sps
 *  \file   sps.hpp
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

#include <swgLib/shaderPrimitive.hpp>

#include <istream>
#include <string>
#include <vector>

#ifndef SPS_HPP
#define SPS_HPP 1

namespace ml
{
	class sps
	{
	public:
		sps();
		~sps();

		std::size_t read(std::istream& file, bool skipSIDX=false);
		const std::vector<std::string>& getShaderFiles() const;
		const std::vector<shaderPrimitive>& getShaderPrimitives() const;
		const shaderPrimitive& getShaderPrimitive(const std::size_t& sp) const;

	protected:
		uint8_t _version; // 0000 or 0001

		int32_t _numberOfShaders;

		std::vector<std::string> _shaderFiles;
		std::vector<shaderPrimitive> _shaderPrimitives;

	private:
	};
}

#endif
