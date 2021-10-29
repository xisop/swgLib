/** -*-c++-*-
 *  \class  patt
 *  \file   patt.hpp
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

#ifndef PATT_HPP
#define PATT_HPP 1

namespace ml
{
	class patt
	{
	public:
		patt();
		~patt();

		std::size_t read(std::istream& file);

		const std::string getAttachmentPath() const;
		const float& getStartPercentMin() const;
		const float& getStartPercentMax() const;
		bool willKillAttachmentOnParticleDeath() const;
		const uint32_t getSpawn() const;

	protected:
		uint32_t _version;
		std::string _attachmentPath;
		float _startPercentMin;
		float _startPercentMax;
		bool _killAttachmentOnParticleDeath;
		uint32_t _spawn;
	};
}
#endif
