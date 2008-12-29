/** -*-c++-*-
 *  \class  sbot
 *  \file   sbot.cpp
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
#include <meshLib/sbot.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

sbot::sbot()
{
}

sbot::~sbot()
{
}

unsigned int sbot::readSBOT( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int sbotSize;
    std::string type;

    total += readFormHeader( file, form, sbotSize, type );
    sbotSize += 8;
    if( form != "FORM" || type != "SBOT" )
    {
	std::cout << "Expected Form of type SBOT: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found SBOT form" << std::endl;

    total += readDERV( file, sbotBaseObjectFilename );

    unsigned int size0001;
    total += readFormHeader( file, form, size0001, type );
    size0001 += 8;
    if( form != "FORM" || type != "0001" )
    {
	std::cout << "Expected Form of type 0001: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found 0001 form" << std::endl;

    total += readPCNT( file, numNodes );
    for( unsigned int i = 0; i < numNodes; ++i )
      {
	total += readSBOTXXXX( file );
      }

    total += readSTOT( file );


    if( sbotSize == total )
    {
	std::cout << "Finished reading SBOT" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading SBOT" << std::endl;
	std::cout << "Read " << total << " out of " << sbotSize
                  << std::endl;
     }

    return total;
}

void sbot::print() const
{
}

unsigned int sbot::readSBOTXXXX( std::istream &file )
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
    std::string property( temp );
    std::cout << "Property: " << property << std::endl;
    total += property.size() + 1;

    unsigned char enabled;

    if( property == "terrainModificationFileName" )
      {
	file.read( (char *)&enabled, 1 ); ++total;
	if( enabled > 0 )
	  {
	    file.getline( temp, 255, 0 );
	    terrainModificationFilename = temp;
	    total += terrainModificationFilename.size() + 1;
	    
	    std::cout << property << ": "
		      << terrainModificationFilename
		      << std::endl;
	  }
      }
    else if( property == "interiorLayoutFileName" )
      {
	file.read( (char *)&enabled, 1 ); ++total;
	if( enabled > 0 )
	  {
	    file.getline( temp, 255, 0 );
	    interiorLayoutFilename = temp;
	    total += interiorLayoutFilename.size() + 1;
	    
	    std::cout << property << ": "
		      << interiorLayoutFilename
		      << std::endl;
	  }
      }
    else
      {
	std::cout << "Unknown: " << property << std::endl;
	exit( 0 );
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
