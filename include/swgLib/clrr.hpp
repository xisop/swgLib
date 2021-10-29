/** -*-c++-*-
 *  \class  clrr
 *  \file   clrr.hpp
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

#include <istream>
#include <string>
#include <vector>

#ifndef CLRR_HPP
#define CLRR_HPP 1

namespace ml
{
	struct point {
		float _percent;
		float _red;
		float _green;
		float _blue;
	};

	class clrr
	{
	public:
		clrr();
		~clrr();

		std::size_t read(std::istream& file);

		const uint32_t& getInterpolationType() const;
		const uint32_t& getSampleType() const;
		const uint32_t& getNumPoints() const;
		const std::vector<point>& getPoints() const;

	protected:
		uint32_t _version;
		uint32_t _interpolationType;
		uint32_t _sampleType;
		uint32_t _numPoints;
		std::vector<point> _points;

	private:
	};
}
#endif
