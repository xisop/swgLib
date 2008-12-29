/** -*-c++-*-
 *  \class  foot
 *  \file   foot.cpp
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
#include <meshLib/foot.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

foot::foot()
{
}

foot::~foot()
{
}

unsigned int foot::readFOOT( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int footSize;
    std::string type;

    total += readFormHeader( file, form, footSize, type );
    footSize += 8;
    if( form != "FORM" || type != "FOOT" )
    {
	std::cout << "Expected Form of type FOOT: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found FOOT form" << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" )
    {
	std::cout << "Expected Form" << std::endl;
	exit( 0 );
    }
    std::cout << "Found form of type: " << type<< std::endl;

    total += readINFO( file );
    total += readPRNT( file );
    
    if( footSize == total )
    {
	std::cout << "Finished reading FOOT" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading FOOT" << std::endl;
	std::cout << "Read " << total << " out of " << footSize
                  << std::endl;
     }

    return total;
}


unsigned int foot::readINFO( std::istream &file  )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int infoSize;
    total += readRecordHeader( file, type, infoSize );
    infoSize += 8;
    if( type != "INFO" )
    {
        std::cout << "Expected record of type INFO: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    unsigned int x;
    file.read( (char *)&numColumns, sizeof( numColumns ) );
    total += sizeof( numColumns );
    std::cout << "Number columns: " << numColumns << std::endl;
    
    file.read( (char *)&numRows, sizeof( numRows ) );
    total += sizeof( numRows );
    std::cout << "Num rows: " << numRows << std::endl;
    
    file.read( (char *)&x, sizeof( x ) );
    total += sizeof( x );
    std::cout << "???: " << x << std::endl;
    
    file.read( (char *)&x, sizeof( x ) );
    total += sizeof( x );
    std::cout << "???: " << x << std::endl;

    file.read( (char *)&blockWidth, sizeof( blockWidth ) );
    total += sizeof( blockWidth );
    std::cout << std::fixed << blockWidth << std::endl;
    
    file.read( (char *)&blockHeight, sizeof( blockHeight ) );
    total += sizeof( blockHeight );
    std::cout << std::fixed << blockHeight << std::endl;

    if( infoSize == total )
    {
        std::cout << "Finished reading INFO" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading INFO" << std::endl;
        std::cout << "Read " << total << " out of " << infoSize
                  << std::endl;
     }

    return total;
}

unsigned int foot::readPRNT( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int prntSize;
    total += readRecordHeader( file, type, prntSize );
    prntSize += 8;
    if( type != "PRNT" )
    {
        std::cout << "Expected record of type PRNT: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;


    char temp[255];
    for( unsigned int i = 0; i < numRows; ++i )
      {
	file.getline( temp, 255, 0 );
	std::string prntString( temp );
	total += prntString.size() + 1;
	std::cout << prntString << std::endl;
      }


    if( prntSize == total )
    {
        std::cout << "Finished reading PRNT" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading PRNT" << std::endl;
        std::cout << "Read " << total << " out of " << prntSize
                  << std::endl;
     }

    return total;
}

void foot::print() const
{
}

