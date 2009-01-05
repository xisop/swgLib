/** -*-c++-*-
 *  \class  cshd
 *  \file   cshd.cpp
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
#include <meshLib/base.hpp>
#include <meshLib/cshd.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

cshd::cshd()
{
}

cshd::~cshd()
{
}

unsigned int cshd::readCSHD( std::istream &file, std::string path )
{
  basePath = path;
  unsigned int total = 0;
  std::string form;
  unsigned int cshdSize;
  std::string type;

  total += readFormHeader( file, form, cshdSize, type );
  cshdSize += 8;
  if( form != "FORM" || type != "CSHD" )
    {
      std::cout << "Expected Form of type CSHD: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found CSHD form" << std::endl;

  unsigned int size;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" )
    {
      std::cout << "Expected FORM: " << form << std::endl;
      exit( 0 );
    }
  std::cout << "Found " << form << " " << type << std::endl;

  unsigned int position;
  while( total < cshdSize )
    {
      // Peek at next record, but keep file at same place.
      position = file.tellg();
      readFormHeader( file, form, size, type );
      file.seekg( position, std::ios_base::beg );

      if( form == "FORM" )
	{
	  if( type == "SSHT" )
	    {
	      total += this->readSHT( file, path );
	    }
	  else if( type == "TFAC" )
	    {
	      total += readTFAC( file );
	    }
	  else if( type == "TXTR" )
	    {
	      total += readTXTR( file );
	    }
	  else
	    {
	      std::cout << "Unexpected form: " << type << std::endl;
	      exit( 0 );
	    }
	}
      else
	{
	  std::cout << "Unexpected record: " << form << std::endl;
	  exit( 0 );
	}
    }

  if( cshdSize == total )
    {
      std::cout << "Finished reading CSHD" << std::endl;
    }
  else
    {
      std::cout << "FAILED in reading CSHD" << std::endl;
      std::cout << "Read " << total << " out of " << cshdSize
		<< std::endl;
    }

  return total;
}

unsigned int cshd::readTFAC( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int tfacSize;
    std::string type;

    total += readFormHeader( file, form, tfacSize, type );
    tfacSize += 8; // Add size of FORM and size fields.
    if( form != "FORM" || type != "TFAC" )
    {
        std::cout << "Expected Form of type TFAC: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << form << " " << type << std::endl;


    while( total < tfacSize )
    {
	total += readPAL( file );
    }

    if( tfacSize == total )
    {
        std::cout << "Finished reading TFAC" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading TFAC" << std::endl;
        std::cout << "Read " << total << " out of " << tfacSize
                  << std::endl;
    }

    return total;
}

unsigned int cshd::readPAL( std::istream &file )
{
    unsigned int total = 0;

    unsigned int palSize;
    std::string type;

    total += readRecordHeader( file, type, palSize );
    if( type != "PAL " )
    {
        std::cout << "Expected record of type PAL: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    total += readUnknown( file, palSize );

    if( palSize == (total-8) )
    {
        std::cout << "Finished reading PAL" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading PAL" << std::endl;
        std::cout << "Read " << total << " out of " << palSize
                  << std::endl;
    }

    return total;
}

unsigned int cshd::readTXTR( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int txtrSize;
    std::string type;

    unsigned int position = file.tellg();

    total += readFormHeader( file, form, txtrSize, type );
    txtrSize += 8;
    if( form != "FORM" || type != "TXTR" )
    {
	std::cout << "No FORM TXTR found." << std::endl;
	file.seekg( position, std::ios_base::beg );
	return 0;
    }
    std::cout << "Found " << form << " " << type
	      << ": " << txtrSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readRecordHeader( file, type, size );
    if( type != "DATA" )
    {
	std::cout << "Expected record of type DATA: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found record " << type 
	      << ": " << size << " bytes" 
	      << std::endl;

    unsigned short numTexNames;
    file.read( (char *)&numTexNames, sizeof( unsigned short ) );
    total += sizeof( unsigned short );
    char name[255];
    for( unsigned int i = 0; i < numTexNames; ++i )
      {
	file.getline( name, 255, 0 );
	std::string texName( name );
	total += texName.size()+1;
	std::cout << "Texture name: " << name << std::endl;
      }

    total += readCUST( file );

    if( txtrSize == total )
    {
	std::cout << "Finished reading TXTR" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading TXTR" << std::endl;
	std::cout << "Read " << total << " out of " << txtrSize
                  << std::endl;
    }

    return total;
}

unsigned int cshd::readCUST( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int custSize;
    std::string type;

    unsigned int position = file.tellg();

    total += readFormHeader( file, form, custSize, type );
    custSize += 8;
    if( form != "FORM" || type != "CUST" )
    {
	std::cout << "No FORM CUST found." << std::endl;
	file.seekg( position, std::ios_base::beg );
	return 0;
    }
    std::cout << "Found " << form << " " << type
	      << ": " << custSize-12 << " bytes"
	      << std::endl;

    while( total < custSize )
      {
	total += readTX1D( file );
      }

    if( custSize == total )
    {
	std::cout << "Finished reading CUST" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading CUST" << std::endl;
	std::cout << "Read " << total << " out of " << custSize
                  << std::endl;
    }

    return total;
}

unsigned int cshd::readTX1D( std::istream &file )
{
    unsigned int total = 0;

    unsigned int tx1dSize;
    std::string type;

    total += readRecordHeader( file, type, tx1dSize );
    if( type != "TX1D" )
    {
        std::cout << "Expected record of type TX1D: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    total += readUnknown( file, tx1dSize );

    if( tx1dSize == (total-8) )
    {
        std::cout << "Finished reading TX1D" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading TX1D" << std::endl;
        std::cout << "Read " << total << " out of " << tx1dSize
                  << std::endl;
    }

    return total;
}
