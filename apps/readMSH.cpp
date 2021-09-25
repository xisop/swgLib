/** -*-c++-*-
 *  \file   readMSH.cpp
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

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include <swgLib/mesh.hpp>

int main(int argc, char** argv)
{

	if (2 != argc)
	{
		std::cout << "readMSH <file>" << std::endl;
		return 0;
	}

	std::ifstream meshFile(argv[1], std::ios_base::binary);

	if (!meshFile.is_open())
	{
		std::cout << "Unable to open file: " << argv[1] << std::endl;
		exit(0);
	}

	ml::mesh mesh;
	mesh.readMESH(meshFile);

	meshFile.close();

	return 0;
}
