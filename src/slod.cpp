/** -*-c++-*-
 *  \class  slod
 *  \file   slod.cpp
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
#include <meshLib/slod.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

slod::slod()
{
}

slod::~slod()
{
}

unsigned int slod::readSLOD( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int slodSize;
    std::string type;

    total += readFormHeader( file, form, slodSize, type );
    slodSize += 8;
    if( form != "FORM" || type != "SLOD" )
    {
	std::cout << "Expected Form of type SLOD: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found SLOD form" << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" )
    {
	std::cout << "Expected Form" << std::endl;
	exit( 0 );
    }
    std::cout << "Found form of type: " << type<< std::endl;

    unsigned short numSktm;
    total += readINFO( file, numSktm );

    for( unsigned int i = 0; i < numSktm; ++i )
      {
	total += readSKTM( file );
      }
    
    if( slodSize == total )
    {
	std::cout << "Finished reading SLOD" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading SLOD" << std::endl;
	std::cout << "Read " << total << " out of " << slodSize
                  << std::endl;
     }

    return total;
}

unsigned int slod::readSKTM( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int sktmSize;
    std::string type;

    total += readFormHeader( file, form, sktmSize, type );
    sktmSize += 8;
    if( form != "FORM" || type != "SKTM" )
    {
	std::cout << "Expected Form of type SKTM: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found SKTM form" << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" )
    {
	std::cout << "Expected Form" << std::endl;
	exit( 0 );
    }
    std::cout << "Found form of type: " << type<< std::endl;

    unsigned int num;
    total += readINFO( file, num );

    total += readNAME( file, num );

    total += readPRNT( file, num );
    total += readRPRE( file, num );
    total += readRPST( file, num );
    total += readBPTR( file, num );
    total += readBPRO( file, num );
    total += readJROR( file, num );

    
    if( sktmSize == total )
    {
	std::cout << "Finished reading SKTM" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading SKTM" << std::endl;
	std::cout << "Read " << total << " out of " << sktmSize
                  << std::endl;
     }

    return total;
}

unsigned int slod::readINFO( std::istream &file, unsigned short &value )
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

    
    file.read( (char*)&value, sizeof( value ) );
    total += sizeof( value );

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

unsigned int slod::readINFO( std::istream &file, unsigned int &value )
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

    
    file.read( (char*)&value, sizeof( value ) );
    total += sizeof( value );

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

unsigned int slod::readNAME( std::istream &file, unsigned int num )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int nameSize;
    total += readRecordHeader( file, type, nameSize );
    nameSize += 8;
    if( type != "NAME" )
    {
        std::cout << "Expected record of type NAME: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    char temp[255];
    std::string filename;
    for( unsigned int i = 0; i < num; ++i )
      {
	file.getline( temp, 255, 0 );
	filename = temp;
	total += filename.size() + 1;
	std::cout << i << ": " << filename << std::endl;
	groupNames.push_back( filename );
      }

    if( nameSize == total )
    {
        std::cout << "Finished reading NAME" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading NAME" << std::endl;
        std::cout << "Read " << total << " out of " << nameSize
                  << std::endl;
     }

    return total;
}

void slod::print() const
{
}

/// Parent bone
unsigned int slod::readPRNT( std::istream &file, unsigned int num )
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

    if( (prntSize-8)/num != 4 )
      {
	std::cout << "Expected 4 byte values" << std::endl;
	exit( 0 );
      }

    unsigned int value;
    for( unsigned int i = 0; i < num; ++i )
      {
	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";
	std::cout << std::endl;

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

unsigned int slod::readRPRE( std::istream &file, unsigned int num )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int rpreSize;
    total += readRecordHeader( file, type, rpreSize );
    rpreSize += 8;
    if( type != "RPRE" )
    {
        std::cout << "Expected record of type RPRE: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    if( (rpreSize-8)/num != 16 )
      {
	std::cout << "Expected 16 byte values" << std::endl;
	exit( 0 );
      }

    float value;
    for( unsigned int i = 0; i < num; ++i )
      {
	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";

	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";

	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";

	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";
	std::cout << std::endl;
      }

    if( rpreSize == total )
    {
        std::cout << "Finished reading RPRE" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading RPRE" << std::endl;
        std::cout << "Read " << total << " out of " << rpreSize
                  << std::endl;
     }

    return total;
}

unsigned int slod::readRPST( std::istream &file, unsigned int num )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int rpstSize;
    total += readRecordHeader( file, type, rpstSize );
    rpstSize += 8;
    if( type != "RPST" )
    {
        std::cout << "Expected record of type RPST: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    if( (rpstSize-8)/num != 16 )
      {
	std::cout << "Expected 16 byte values" << std::endl;
	exit( 0 );
      }

    float value;
    for( unsigned int i = 0; i < num; ++i )
      {
	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";

	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";

	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";

	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";
	std::cout << std::endl;
      }

    if( rpstSize == total )
    {
        std::cout << "Finished reading RPST" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading RPST" << std::endl;
        std::cout << "Read " << total << " out of " << rpstSize
                  << std::endl;
     }

    return total;
}

unsigned int slod::readBPTR( std::istream &file, unsigned int num )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int bptrSize;
    total += readRecordHeader( file, type, bptrSize );
    bptrSize += 8;
    if( type != "BPTR" )
    {
        std::cout << "Expected record of type BPTR: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    if( (bptrSize-8)/num != 12 )
      {
	std::cout << "Expected 12 byte values" << std::endl;
	exit( 0 );
      }

    float value;
    for( unsigned int i = 0; i < num; ++i )
      {
	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";

	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";

	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";
	std::cout << std::endl;
      }

    if( bptrSize == total )
    {
        std::cout << "Finished reading BPTR" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading BPTR" << std::endl;
        std::cout << "Read " << total << " out of " << bptrSize
                  << std::endl;
     }

    return total;
}

unsigned int slod::readBPRO( std::istream &file, unsigned int num )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int bproSize;
    total += readRecordHeader( file, type, bproSize );
    bproSize += 8;
    if( type != "BPRO" )
    {
        std::cout << "Expected record of type BPRO: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    if( (bproSize-8)/num != 16 )
      {
	std::cout << "Expected 16 byte values" << std::endl;
	exit( 0 );
      }

    float value;
    for( unsigned int i = 0; i < num; ++i )
      {
	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";

	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";

	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";

	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";
	std::cout << std::endl;
      }

    if( bproSize == total )
    {
        std::cout << "Finished reading BPRO" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading BPRO" << std::endl;
        std::cout << "Read " << total << " out of " << bproSize
                  << std::endl;
     }

    return total;
}

unsigned int slod::readJROR( std::istream &file, unsigned int num )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int jrorSize;
    total += readRecordHeader( file, type, jrorSize );
    jrorSize += 8;
    if( type != "JROR" )
    {
        std::cout << "Expected record of type JROR: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    if( (jrorSize-8)/num != 4 )
      {
	std::cout << "Expected 4 byte values" << std::endl;
	exit( 0 );
      }

    unsigned int value;
    for( unsigned int i = 0; i < num; ++i )
      {
	file.read( (char*)&value, sizeof( value ) );
	total += sizeof( value );
	std::cout << value << " ";
      }
    std::cout << std::endl;

    if( jrorSize == total )
    {
        std::cout << "Finished reading JROR" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading JROR" << std::endl;
        std::cout << "Read " << total << " out of " << jrorSize
                  << std::endl;
     }

    return total;
}

