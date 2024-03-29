/** -*-c++-*-
 *  \class  sidx
 *  \file   sidx.hpp
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

#ifndef SIDX_HPP
#define SIDX_HPP 1

#include <swgLib/vector3.hpp>
#include <swgLib/indx.hpp>

#include <istream>
#include <vector>

namespace ml
{
	class sidx
	{
	public:
		sidx();
		~sidx();

		std::size_t read(std::istream& file, bool sidx16=false);

		const int32_t& getNumArrays() const;
		const vector3& getVector(const std::size_t &v) const;
		const indx& getArray(const std::size_t& a) const;

	protected:
		int32_t _numArrays;

		std::vector<vector3> _first;
		std::vector<indx> _second;

	private:
	};
}

#endif
