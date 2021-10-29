/** -*-c++-*-
 *  \class  wvfm
 *  \file   wvfm.hpp
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

#ifndef WVFM_HPP
#define WVFM_HPP 1

namespace ml
{
	class wvfm
	{
	public:
		struct controlPoint {
			float _percent; // Range 0..1
			float _value;
			float _randomMax;
			float _randomMin;
		};

	public:
		wvfm();
		~wvfm();

		std::size_t read(std::istream& file);

		void scale(const float& percent);

	protected:
		uint32_t _version;
		int32_t _interpolationType;
		int32_t _sampleType;
		float _valueMin;
		float _valueMax;
		int32_t _controlPointCount;

		std::vector<controlPoint> _controlPoint;


	private:
	};
}
#endif
