/** -*-c++-*-
 *  \class  cshd
 *  \file   cshd.hpp
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
#include <swgLib/sht.hpp>

#include <fstream>
#include <string>

#ifndef CSHD_HPP
#define CSHD_HPP 1

namespace ml
{

	class cshd : public sht
	{
	public:
		cshd();
		~cshd();

		unsigned int readCSHD(std::istream& file, std::string path = "");

	protected:
		unsigned int readTFAC(std::istream& file);
		unsigned int readPAL(std::istream& file);
		unsigned int readTXTR(std::istream& file);
		unsigned int readCUST(std::istream& file);
		unsigned int readTX1D(std::istream& file);

	private:

	};
}
#endif
