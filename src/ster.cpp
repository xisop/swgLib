/** -*-c++-*-
 *  \class  ster
 *  \file   ster.cpp
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

#include <meshLib/base.hpp>
#include <meshLib/ster.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

ster::ster()
{
}

ster::~ster()
{
}

unsigned int ster::readSTER( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int sterSize;
    std::string type;

    total += readFormHeader( file, form, sterSize, type );
    sterSize += 8;
    if( form != "FORM" || type != "STER" )
    {
	std::cout << "Expected Form of type STER: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found STER form" << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" )
    {
	std::cout << "Expected FORM: " << form << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type << std::endl;

    total += readPCNT( file );
    for( unsigned int i = 0; i < numNodes; ++i )
      {
	total += readXXXX( file );
      }


    if( sterSize == total )
    {
	std::cout << "Finished reading STER" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading STER" << std::endl;
	std::cout << "Read " << total << " out of " << sterSize
                  << std::endl;
     }

    return total;
}

void ster::print() const
{
}

unsigned int ster::readPCNT( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int pcntSize;
    std::string type;

    total += readRecordHeader( file, type, pcntSize );
    if( type != "PCNT" )
    {
        std::cout << "Expected record of type PCNT: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    if( 4 != pcntSize )
      {
        std::cout << "Expected size 4: " << pcntSize << std::endl;
        exit( 0 );
      }

    file.read( (char *)&numNodes, sizeof( numNodes ) );
    total += sizeof( numNodes );
    std::cout << "<numNodes>" << numNodes << "</numNodes>" << std::endl;

    if( pcntSize == (total-8) )
    {
        std::cout << "Finished reading PCNT" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading PCNT" << std::endl;
        std::cout << "Read " << total << " out of " << pcntSize
                  << std::endl;
    }

    return total;
}

unsigned int ster::readXXXX( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int xxxxSize;
    std::string type;

    total += readRecordHeader( file, type, xxxxSize );
    if( type != "XXXX" )
    {
        std::cout << "Expected record of type XXXX: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    char temp[255];
    file.getline( temp, 255, 0 );
    std::string name( temp );
    std::cout << "<name>" << name << "</name>" << std::endl;
    total += name.size() + 1;

    unsigned char x;
    file.read( (char *)&x, sizeof( x ) );
    total += sizeof( x );
    std::cout << "<?>" << (unsigned int)x << "</?>" << std::endl;

    file.getline( temp, 255, 0 );
    std::string name2( temp );
    std::cout << "<name>" << name2 << "</name>" << std::endl;
    total += name2.size() + 1;

    if( "cover" == name )
      {
	unsigned short y;
	file.read( (char *)&y, sizeof( y ) );
	total += sizeof( y );
	std::cout << "<?>" << y << "</?>" << std::endl;

	unsigned char z;
	file.read( (char *)&z, sizeof( z ) );
	total += sizeof( z );
	std::cout << "<?>" << (unsigned int)z << "</?>" << std::endl;
      }

    if( xxxxSize == (total-8) )
    {
        std::cout << "Finished reading XXXX" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading XXXX" << std::endl;
        std::cout << "Read " << total << " out of " << xxxxSize
                  << std::endl;
    }

    return total;
}
