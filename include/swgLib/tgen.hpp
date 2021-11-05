/** -*-c++-*-
 *  \class  tgen
 *  \file   tgen.hpp
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

#include <swgLib/sgrp.hpp>
#include <swgLib/fgrp.hpp>
#include <swgLib/rgrp.hpp>
#include <swgLib/egrp.hpp>
#include <swgLib/mgrp.hpp>

#include <swgLib/tgenLayer.hpp>

#ifndef TGEN_HPP
#define TGEN_HPP 1

namespace ml
{
	class tgen
	{
	public:
		tgen();
		~tgen();

		std::size_t read(std::istream& file);

	protected:
		uint32_t _tgenVersion;

		sgrp _shaderGroup;
		fgrp _floraGroup;
		rgrp _radialGroup;
		egrp _environmentGroup;
		mgrp _fractalGroup;
		//bgrp _bitmapGroup;

		std::vector<tgenLayer> _layers;
	private:
	};
}

#endif
