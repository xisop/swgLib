/** -*-c++-*-
 *  \class  stat
 *  \file   stat.cpp
 *  \author Kenneth R. Sewell III

 meshLib is used for the parsing and exporting .msh models.
 Copyright (C) 2006,2007 Kenneth R. Sewell III

 This file is part of meshLib.

 meshLib is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 meshLib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with meshLib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/**
   This class still needs work.
 */

#include <meshLib/base.hpp>
#include <meshLib/stat.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

stat::stat()
{
}

stat::~stat()
{
}

unsigned int stat::readSTAT( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int statSize;
    std::string type;

    total += readFormHeader( file, form, statSize, type );
    statSize += 8;
    if( form != "FORM" || type != "STAT" )
    {
	std::cout << "Expected Form of type STAT: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found STAT form" << std::endl;

    // Peek at next record, but keep file at same place.
    unsigned int position = file.tellg();
    unsigned int size;
    readFormHeader( file, form, size, type );
    file.seekg( position, std::ios_base::beg );
    
    if( "DERV" == form )
      {
	total += readDERV( file, statBaseObjectFilename );
      }

    unsigned int size0000;
    total += readFormHeader( file, form, size0000, type );
    size0000 += 8;
    if( form != "FORM" || type != "0000" )
    {
	std::cout << "Expected Form of type 0000: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found 0000 form" << std::endl;

    total += readPCNT( file, numNodes );
    for( unsigned int i = 0; i < numNodes; ++i )
      {
	total += readXXXX( file );
      }

    total += readSHOT( file );


    if( statSize == total )
    {
	std::cout << "Finished reading STAT" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading STAT" << std::endl;
	std::cout << "Read " << total << " out of " << statSize
                  << std::endl;
     }

    return total;
}

void stat::print() const
{
}

