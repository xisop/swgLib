/** -*-c++-*-
 *  \class  cmp
 *  \file   cmp.hpp
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
#include <swgLib/appr.hpp>
#include <swgLib/idtl.hpp>
#include <swgLib/vector3.hpp>
#include <swgLib/matrix3.hpp>

#include <fstream>
#include <string>
#include <vector>

#ifndef CMP_HPP
#define CMP_HPP 1

namespace ml
{
	// Component Appearance
	class cmp : public appr
	{
	public:
		struct part {
			std::string filename;
			bool validTransform;
			matrix3x4 transform;
			vector3 position;
			vector3 yawPitchRoll;
		};

		cmp();
		~cmp();

		std::size_t read(std::istream& file);

		uint32_t getNumParts() const;

		const part &getPart(const uint32_t &index) const;
		const std::vector<part>& getParts() const;

	protected:
		std::size_t readCMPv1(std::istream& file);
		std::size_t readCMPv2(std::istream& file);
		std::size_t readCMPv3(std::istream& file);
		std::size_t readCMPv4(std::istream& file);
		std::size_t readCMPv5(std::istream& file);
		std::size_t readPART(std::istream& file);
		std::size_t readRADR(std::istream& file);

	private:
		uint32_t _cmpVersion; // Valid values: [0001-0005]

		std::vector<part> _parts;

		bool _hasRadar;
		idtl _radar;
	};
}
#endif
