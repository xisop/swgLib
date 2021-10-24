/** -*-c++-*-
 *  \class  ilf
 *  \file   ilf.hpp
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

#include <swgLib/matrix3.hpp>
#include <swgLib/vector3.hpp>

#include <istream>
#include <ostream>
#include <fstream>
#include <vector>
#include <string>

#ifndef ILF_HPP
#define ILF_HPP 1

namespace ml
{
	// Interior Layout File
	class ilf
	{
	public:
		class node {
		public:
			node();
			~node();

			std::size_t read(std::istream &file);
			std::size_t write(std::ostream &file) const;
			void print(std::ostream& os) const;

		protected:
			std::string _objectFilename;
			std::string _cellName;
			matrix3x4   _transform;
		};

	public:
		ilf();
		~ilf();

		std::size_t createILF(std::istream& infile, std::ofstream& outfile);
		std::size_t readILF(std::istream& file);
		bool canWrite() const { return true; }

		uint32_t getNumNodes() const
		{
			return uint32_t(_nodes.size());
		}

		const node& getNode(const uint32_t& index) const;

	protected:
		std::size_t readNODE(std::istream& file);

		std::vector<node> _nodes;

	private:

	};
}
#endif
