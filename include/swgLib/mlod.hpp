/** -*-c++-*-
 *  \class  mlod
 *  \file   mlod.hpp
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
#include <swgLib/model.hpp>
#include <fstream>
#include <vector>

#ifndef MLOD_HPP
#define MLOD_HPP 1

namespace ml
{
	class mlod : public model
	{
	public:
		mlod();
		~mlod();
		bool isRightType(std::istream& file)
		{
			return isOfType(file, "MLOD");
		}

		std::size_t readMLOD(std::istream& file, const std::string& path = "");

		uint32_t getNumMesh() const
		{
			return uint32_t(meshFilename.size());
		}

		const std::string& getMeshFilename(unsigned int index) const
		{
			return meshFilename[index];
		}

	protected:
		std::size_t readINFO(std::istream& file, unsigned short& num);
		std::size_t readNAME(std::istream& file, std::string& name);

		std::vector< std::string > meshFilename;

	private:
	};
}
#endif
