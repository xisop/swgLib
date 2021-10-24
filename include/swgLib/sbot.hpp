/** -*-c++-*-
 *  \class  sbot
 *  \file   sbot.hpp
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
#include <swgLib/stot.hpp>

#include <istream>
#include <ostream>
#include <string>

#ifndef SBOT_HPP
#define SBOT_HPP 1

namespace ml
{
	// Shared Building Object Template (SBOT)
	class sbot : public stot
	{
	public:
		sbot();
		~sbot();

		std::size_t readSBOT(std::istream& file);

		const std::string& getBaseObjectFilename() const;
		const std::string& getTerrainModificationFilename() const;
		const std::string& getInteriorLayoutFilename() const;

		void print(std::ostream& os) const;

	protected:
		std::size_t readSBOTParameter(std::istream& file);

		int8_t _sbotVersion;
		std::string _sbotBaseObjectFilename;
		std::string _terrainModificationFileName;
		std::string _interiorLayoutFileName;
	};
}
#endif
