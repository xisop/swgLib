/** -*-c++-*-
 *  \class  egrp
 *  \file   egrp.hpp
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
#include <vector>

#ifndef EGRP_HPP
#define EGRP_HPP 1

namespace ml
{
	// Environment Group
	class egrp
	{
	public:
		struct family {
			int32_t     familyId;
			std::string name;
			std::string surfacePropertyName;

			uint8_t red;
			uint8_t green;
			uint8_t blue;

			float density;
			bool  floats;
			float shaderSize;
			float featherClamp;

			int32_t numChildren;
		};

	public:
		egrp();
		~egrp();

		std::size_t read(std::istream& file);

		const std::vector<family>& getFamily() const;

	protected:
		std::size_t readV0(std::istream& file);
		std::size_t readV1(std::istream& file);
		std::size_t readV2(std::istream& file);

	protected:
		uint32_t _version;
		std::vector<family> _family;

	private:
	};
}

#endif
