/** -*-c++-*-
 *  \class  cstb
 *  \file   cstb.cpp
 *  \author Kenneth R. Sewell III

 meshLib is used for the parsing and exporting .msh models.
 Copyright (C) 2006-2009 Kenneth R. Sewell III

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

#include <meshLib/cstb.hpp>
#include <iostream>
#include <cstdlib>

#define MAX_SWG_STRING 2048

using namespace ml;

cstb::cstb()
{
}

cstb::~cstb()
{
}

unsigned int cstb::readCSTB( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int cstbSize;
    std::string type;

    total += readFormHeader( file, form, cstbSize, type );
    cstbSize += 8;
    if( form != "FORM" || type != "CSTB" )
    {
	std::cout << "Expected Form of type CSTB: " << type << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found CSTB form"
	      << ": " << cstbSize-12 << " bytes"
	      << std::endl;
#endif

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" )
    {
	std::cout << "Expected FORM: " << form << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found " << form << " " << type
	      << ": " << size-4 << " bytes"
	      << std::endl;
#endif

    total += readDATA( file );
    total += readCRCT( file );
    total += readSTRT( file );
    total += readSTNG( file );

    if( cstbSize == total )
    {
#if DEBUG
	sd::cout << "Finished reading CSTB" << std::endl;
#endif
    }
    else
    {
	std::cout << "FAILED in reading CSTB" << std::endl;
	std::cout << "Possibly a corrupt or non-finished file" << std::endl;
	std::cout << "Read " << total << " out of " << cstbSize
                  << std::endl;
    }
    
    return total;
}


unsigned int cstb::readDATA( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int size;
    std::string type;

    total += readRecordHeader( file, type, size );
    size += 8;
    if( type != "DATA" )
    {
	std::cout << "Expected record of type DATA: " << type << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found DATA form"
	      << ": " << size-8 << " bytes"
	      << std::endl;
#endif

    file.read( (char*)&num, sizeof( num ) );
    total += sizeof( num );

    if( size == total )
    {
#if DEBUG
	std::cout << "Finished reading DATA" << std::endl;
#endif
    }
    else
    {
	std::cout << "FAILED in reading DATA" << std::endl;
	std::cout << "Read " << total << " out of " << size
                  << std::endl;
    }
    
    return total;
}

unsigned int cstb::readCRCT( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int size;
    std::string type;

    total += readRecordHeader( file, type, size );
    size += 8;
    if( type != "CRCT" )
    {
	std::cout << "Expected record of type CRCT: " << type << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found CRCT form"
	      << ": " << size-8 << " bytes"
	      << std::endl;
#endif

    unsigned int temp;
    for( unsigned int i = 0; i < num; ++i )
    { 
	file.read( (char*)&temp, sizeof( temp ) );
	total += sizeof( temp );
	crc.push_back( temp );
    }

    if( size == total )
    {
#if DEBUG
	std::cout << "Finished reading CRCT" << std::endl;
#endif
    }
    else
    {
	std::cout << "FAILED in reading CRCT" << std::endl;
	std::cout << "Read " << total << " out of " << size
                  << std::endl;
    }
    
    return total;
}

unsigned int cstb::readSTRT( std::istream &file )
{
   unsigned int total = 0;
   std::string form;
   unsigned int size;
   std::string type;

   total += readRecordHeader( file, type, size );
    size += 8;
    if( type != "STRT" )
    {
	std::cout << "Expected record of type STRT: " << type << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found STRT form"
	      << ": " << size-8 << " bytes"
	      << std::endl;
#endif

    unsigned int temp;
    for( unsigned int i = 0; i < num; ++i )
    { 
	file.read( (char*)&temp, sizeof( temp ) );
	total += sizeof( temp );
	offset.push_back( temp );
    }

    if( size == total )
    {
#if DEBUG
	std::cout << "Finished reading STRT" << std::endl;
#endif
    }
    else
    {
	std::cout << "FAILED in reading STRT" << std::endl;
	std::cout << "Read " << total << " out of " << size
                  << std::endl;
    }

    return total;
}

unsigned int cstb::readSTNG( std::istream &file )
{
   unsigned int total = 0;
   std::string form;
   unsigned int size;
   std::string type;
   
   total += readRecordHeader( file, type, size );
   size += 8;
   if( type != "STNG" )
   {
       std::cout << "Expected record of type STNG: " << type << std::endl;
       exit( 0 );
   }
#if DEBUG
   std::cout << "Found STNG form"
	     << ": " << size-8 << " bytes"
	     << std::endl;
#endif

   int pos = file.tellg();
   
   char temp[255];
   for( unsigned int i = 0; i < num; ++i )
   { 
       file.seekg( pos+offset[i], std::ios_base::beg );
       file.getline( temp, 255, 0 );
       std::string newName( temp );
       total += newName.size() + 1;
       name.push_back( newName );
   }
   
    if( size == total )
    {
#if DEBUG
	std::cout << "Finished reading STNG" << std::endl;
#endif
    }
    else
    {
	std::cout << "FAILED in reading STNG" << std::endl;
	std::cout << "Read " << total << " out of " << size
                  << std::endl;
    }

    return total;
}

void cstb::print() const
{
    std::cout << "Index\tCRC\t\tName" << std::endl;
    std::cout << "-----\t---\t\t----" << std::endl;
    for( unsigned int i = 0; i < num; ++i )
    {
	std::cout << i << "\t"
		  << crc[i] << "\t"
		  << name[i]
		  << std::endl;
    }
}
