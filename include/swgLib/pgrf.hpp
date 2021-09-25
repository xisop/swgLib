/** -*-c++-*-
 *  \class  pgrf
 *  \file   pgrf.hpp
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

#include <swgLib/vector3.hpp>
#include <istream>
#include <vector>

#ifndef PGRF_HPP
#define PGRF_HPP 1

namespace ml
{
	class pgrf
	{
	public:
		pgrf();
		~pgrf();

		std::size_t read(std::istream& file);

	protected:
		uint8_t _version; // 0000, 0001

		// Path Node data...
		std::vector<int32_t> _index;
		std::vector<int32_t> _id;
		std::vector<int32_t> _key;
		std::vector<int32_t> _type;
		std::vector<vector3> _position;
		std::vector<float>   _radius;

		// Path Edge data...
		std::vector<int32_t> _aIndex;
		std::vector<int32_t> _bIndex;
		std::vector<float> _laneWidthRight;
		std::vector<float> _laneWidthLeft;

		// Edge count data...
		std::vector<int32_t> _edgeCount;

		// Edge start data...
		std::vector<int32_t> _edgeStart;

	private:
	};
}

#endif
