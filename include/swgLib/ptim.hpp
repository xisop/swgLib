/** -*-c++-*-
 *  \class  ptim
 *  \file   ptim.hpp
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

#ifndef PTIM_HPP
#define PTIM_HPP 1

namespace ml
{
	class ptim
	{
	public:
		ptim();
		~ptim();

		std::size_t read(std::istream& file);

		const uint32_t& getVersion() const;
		const float& getStartDelayMin() const;
		const float& getStartDelayMax() const;
		const float& getLoopDelayMin() const;
		const float& getLoopDelayMax() const;
		const int32_t& getLoopCountMin() const;
		const int32_t& getLoopCountMax() const;

	protected:
		uint32_t _version;
		float _startDelayMin;
		float _startDelayMax;
		float _loopDelayMin;
		float _loopDelayMax;
		int32_t _loopCountMin;
		int32_t _loopCountMax;
	};
}
#endif
