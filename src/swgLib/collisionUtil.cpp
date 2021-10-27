/** -*-c++-*-
 *  \class  collisionUtil
 *  \file   collisionUtil.cpp
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

#include <swgLib/base.hpp>
#include <swgLib/collisionUtil.hpp>
#include <swgLib/xsmp.hpp>
#include <swgLib/exsp.hpp>
#include <swgLib/cpst.hpp>
#include <swgLib/cmpt.hpp>
#include <swgLib/dtal.hpp>
#include <swgLib/exbx.hpp>
#include <swgLib/cmsh.hpp>
#include <swgLib/xcyl.hpp>
#include <swgLib/xocl.hpp>

using namespace ml;

collisionUtil::collisionUtil() {
}

collisionUtil::~collisionUtil() {
}

std::size_t collisionUtil::read(std::istream& file, baseCollisionPtr& collisionPtr) {
	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);

	if ("FORM" == form) {
		if ("XSMP" == type) {
			collisionPtr = xsmpPtr(new ml::xsmp);
		}
		else if ("EXSP" == type) {
			collisionPtr = exspPtr(new ml::exsp);
		}
		else if ("CPST" == type) {
			collisionPtr = cpstPtr(new ml::cpst);
		}
		else if ("CMPT" == type) {
			collisionPtr = cmptPtr(new ml::cmpt);
		}
		else if ("DTAL" == type) {
			collisionPtr = dtalPtr(new ml::dtal);
		}
		else if ("EXBX" == type) {
			collisionPtr = exbxPtr(new ml::exbx);
		}
		else if ("CMSH" == type) {
			collisionPtr = cmshPtr(new ml::cmsh);
		}
		else if ("XCYL" == type) {
			collisionPtr = xcylPtr(new ml::xcyl);
		}
		else if ("XOCL" == type) {
			collisionPtr = xoclPtr(new ml::xocl);
		}
		else if ("NULL" == type) {
			// Consume NULL record...
			return base::readFormHeader(file, form, size, type);
		}
		else {
			std::cout << "Unknown collision primitive: " << type << "\n";
			return 0;
		}

		if (collisionPtr) {
			return collisionPtr->read(file);
		}
	}
	else {
		std::cout << "Expected FORM instead of record: " << type << "\n";
		return 0;
	}

	return 0;
}

