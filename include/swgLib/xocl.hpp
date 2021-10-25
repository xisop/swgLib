/** -*-c++-*-
 *  \class  xocl
 *  \file   xocl.hpp
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

#include <swgLib/exsp.hpp>
#include <swgLib/vector3.hpp>

#include <istream>
#include <memory>
#include <vector>

#ifndef XOCL_HPP
#define XOCL_HPP 1

namespace ml
{
	class xocl : public exsp
	{
	public:
		xocl();
		~xocl();

		std::size_t read(std::istream& file) override;

	protected:
		vector3 _base;
		vector3 _axis;
		float   _radius;
		float   _height;

	private:
	};
}

typedef std::shared_ptr<ml::xocl> xoclPtr;

#endif
