/** -*-c++-*-
 *  \class  idtl
 *  \file   idtl.hpp
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

#include <vector>
#include <swgLib/vector3.hpp>

#ifndef IDTL_HPP
#define IDTL_HPP 1

namespace ml
{
	class idtl
	{
	public:
		idtl();
		~idtl();

		std::size_t read(std::istream& file);
		static std::size_t readVERT(std::istream& file, std::vector<vector3>& vec);
		static std::size_t readINDX(std::istream& file, std::vector<int32_t>& index);

	protected:

	private:
		std::vector<vector3> _vertex;
		std::vector<int32_t> _index;
	};
}

#endif
