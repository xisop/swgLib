/** -*-c++-*-
 *  \class  peft
 *  \file   peft.cpp
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
#include <meshLib/peft.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

peft::peft()
{
}

peft::~peft()
{
}

unsigned int peft::readPEFT( std::istream &file, std::string path )
{
  basePath = path;
  unsigned int total = 0;
  std::string form;
  unsigned int peftSize;
  std::string type;

  total += readFormHeader( file, form, peftSize, type );
  peftSize += 8;
  if( form != "FORM" || type != "PEFT" )
    {
      std::cout << "Expected Form of type PEFT: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found PEFT form"
	    << ": " << peftSize-12 << " bytes"
	    << std::endl;

  unsigned int size;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" )
    {
      std::cout << "Expected FORM: " << form << std::endl;
      exit( 0 );
    }
  std::cout << "Found " << form << " " << type
	    << ": " << size-4 << " bytes"
	    << std::endl;

  total += readPTIM( file );
  unsigned int numEmitterGroups;
  total += read0000( file, numEmitterGroups );
  for( unsigned int i = 0; i < numEmitterGroups; ++i )
    {
      total += readEMGP( file );
    }

  if( peftSize == total )
    {
      std::cout << "Finished reading EFCT" << std::endl;
    }
  else
    {
      std::cout << "FAILED in reading EFCT" << std::endl;
      std::cout << "Read " << total << " out of " << peftSize
		<< std::endl;
    }
    
  return total;
}


unsigned int peft::readEMGP( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int emgpSize;
    std::string type;

    total += readFormHeader( file, form, emgpSize, type );
    emgpSize += 8;
    if( form != "FORM" || type != "EMGP" )
    {
	std::cout << "Expected Form of type EMGP: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << emgpSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" )
      {
	std::cout << "Expected FORM: " << form << std::endl;
	exit( 0 );
      }
    std::cout << "Found " << form << " " << type
	      << ": " << size-4 << " bytes"
	      << std::endl;

    total += readPTIM( file );
    unsigned int numEmitters;
    total += read0000( file, numEmitters );
    for( unsigned int i = 0; i < numEmitters; ++i )
      {
	total += readEMTR( file );
      }

    if( emgpSize == total )
    {
	std::cout << "Finished reading EMGP" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading EMGP" << std::endl;
	std::cout << "Read " << total << " out of " << emgpSize
                  << std::endl;
    }

    return total;
}

unsigned int peft::readEMTR( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int emtrSize;
    std::string type;

    total += readFormHeader( file, form, emtrSize, type );
    emtrSize += 8;
    if( form != "FORM" || type != "EMTR" )
    {
	std::cout << "Expected Form of type EMTR: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << emtrSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" )
      {
	std::cout << "Expected FORM: " << form << std::endl;
	exit( 0 );
      }
    std::cout << "Found " << form << " " << type
	      << ": " << size-4 << " bytes"
	      << std::endl;

    total += readPTIM( file );

    // Skip for now...Fix later
    file.seekg( emtrSize - total, std::ios_base::cur );
    total = emtrSize;

    if( emtrSize == total )
    {
	std::cout << "Finished reading EMTR" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading EMTR" << std::endl;
	std::cout << "Read " << total << " out of " << emtrSize
                  << std::endl;
    }

    return total;
}

unsigned int peft::readPTIM( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int ptimSize;
    std::string type;

    total += readFormHeader( file, form, ptimSize, type );
    ptimSize += 8;
    if( form != "FORM" || type != "PTIM" )
    {
	std::cout << "Expected Form of type PTIM: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << ptimSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readRecordHeader( file, type, size );
    if( type != "0001" )
    {
        std::cout << "Expected record of type 0001: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type
	      << ": " << size << " bytes"
	      << std::endl;

    unsigned int x;
    for( unsigned int i = 0; i < 6; ++i )
      {
	file.read( (char *)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << std::endl;
      }

    if( ptimSize == total )
    {
	std::cout << "Finished reading PTIM" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading PTIM" << std::endl;
	std::cout << "Read " << total << " out of " << ptimSize
                  << std::endl;
    }

    return total;
}

unsigned int peft::read0000( std::istream &file, unsigned int &num )
{
    unsigned int total = 0;

    std::string form;
    unsigned int size;
    std::string type;

    total += readRecordHeader( file, type, size );
    size += 8;
    if( type != "0000" )
    {
        std::cout << "Expected record of type 0000: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type
	      << ": " << size << " bytes" 
	      << std::endl;

    file.read((char *)&num, sizeof( num ) );
    total += sizeof( num );

    if( size == total )
    {
	std::cout << "Finished reading 0000" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading 0000" << std::endl;
	std::cout << "Read " << total << " out of " << size
                  << std::endl;
    }

    return total;
}


#if 0
unsigned int peft::readIMPL( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int implSize;
    std::string type;

    total += readFormHeader( file, form, implSize, type );
    implSize += 8;
    if( form != "FORM" || type != "IMPL" )
    {
	std::cout << "Expected Form of type IMPL: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << implSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM")
    {
	std::cout << "Expected FORM not: " << form << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << size-4 << " bytes"
	      << std::endl;

    unsigned int position;
    while( total < implSize )
    {
	// Peek at next record, but keep file at same place.
        position = file.tellg();
        readFormHeader( file, form, size, type );
        file.seekg( position, std::ios_base::beg );

	if( form == "SCAP" )
	{
	    total += readSCAP( file );
	}
	else if( form == "OPTN" )
	{
	    total += readOPTN( file );
	}
	else if( form == "DATA" )
	{
	    total += readIMPLDATA( file );
	}
	else if( form == "FORM" && type == "PASS" )
	{
	    total += readPASS( file );
	}
	else
	{
	    std::cout << "Unexpected record: " << form << std::endl;
	    exit( 0 );
	}

    }
    if( implSize == total )
    {
	std::cout << "Finished reading IMPL" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading IMPL" << std::endl;
	std::cout << "Read " << total << " out of " << implSize
                  << std::endl;
    }

    return total;
}


unsigned int peft::readSCAP( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int scapSize;
    std::string type;

    total += readRecordHeader( file, type, scapSize );
    if( type != "SCAP" )
    {
        std::cout << "Expected record of type SCAP: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type 
	      << " (Set Capabilities?) "
	      << scapSize << " bytes"
	      << std::endl;

#if 1
    unsigned int numScap = scapSize/4;

    unsigned int data;
    for( unsigned int i = 0; i < numScap; ++i )
    {
        file.read( (char*)&data, sizeof( data ) );
	std::cout << std::bitset<32>( data ) << " " << std::endl;
    }
    total += scapSize;
#else
    total += readUnknown( file, scapSize );
#endif
    if( scapSize == (total-8) )
    {
	std::cout << "Finished reading SCAP" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading SCAP" << std::endl;
	std::cout << "Read " << total << " out of " << scapSize
                  << std::endl;
    }

    return total;
}

unsigned int peft::readOPTN( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int optnSize;
    std::string type;

    total += readRecordHeader( file, type, optnSize );
    if( type != "OPTN" )
    {
        std::cout << "Expected record of type OPTN: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type
	      << ": " << optnSize << " bytes"
	      << std::endl;

    // Skip data for now
    total += readUnknown( file, optnSize );

    if( optnSize == (total-8) )
    {
	std::cout << "Finished reading OPTN" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading OPTN" << std::endl;
	std::cout << "Read " << total << " out of " << optnSize
                  << std::endl;
    }

    return total;
}

unsigned int peft::readIMPLDATA( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int impldataSize;
    std::string type;

    total += readRecordHeader( file, type, impldataSize );
    if( type != "DATA" )
    {
        std::cout << "Expected record of type DATA: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type
	      << ": " << impldataSize << " bytes" 
	      << std::endl;

    // Skip data for now
    total += readUnknown( file, impldataSize );

    if( impldataSize == (total-8) )
    {
	std::cout << "Finished reading DATA" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading DATA" << std::endl;
	std::cout << "Read " << total << " out of " << impldataSize
                  << std::endl;
    }

    return total;
}

unsigned int peft::readPASS( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int passSize;
    std::string type;

    total += readFormHeader( file, form, passSize, type );
    passSize += 8;
    if( form != "FORM" || type != "PASS" )
    {
        std::cout << "Expected FORM of type PASS: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << passSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" )
    {
        std::cout << "Expected FORM not: " << form << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << size-4 << " bytes"
	      << std::endl;

    total += readRecordHeader( file, type, size );
    if( type != "DATA" )
    {
        std::cout << "Expected record of type DATA: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type
	      << ": " << size << " bytes" 
	      << std::endl;

    total += readUnknown( file, size );

    // Peek at next record, but keep file at same place.
    unsigned int position = file.tellg();
    readFormHeader( file, form, size, type );
    file.seekg( position, std::ios_base::beg );

    if( form == "FORM" && type == "PVSH" )
    {
	total += readPVSH( file );
	total += readPPSH( file );
    }
    else if( form == "FORM" && type == "PFFP" )
    {
	total += readPFFP( file );
	total += readSTAG( file );
	if( total < passSize )
	{
	    total += readSTAG( file );
	}
    }
    else
    {
	std::cout << "Unexpected FORM: " << type << std::endl;
	exit( 0 );
    }

    if( passSize == total )
    {
	std::cout << "Finished reading PASS" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading PASS" << std::endl;
	std::cout << "Read " << total << " out of " << passSize
                  << std::endl;
    }

    return total;
}

unsigned int peft::readPVSH( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int pvshSize;
    std::string type;

    total += readFormHeader( file, form, pvshSize, type );
    pvshSize += 8;
    if( form != "FORM" || type != "PVSH" )
    {
	std::cout << "Expected Form of type PVSH: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << " (Vertex shader)"
	      << ": " << pvshSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readRecordHeader( file, type, size );
    if( type != "0000" )
    {
        std::cout << "Expected record of type 0000: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type
	      << ": " << size << " bytes"
	      << std::endl;

    char temp[255];
    std::string vertexProgramName;
    file.getline( temp, size, 0 );
    total += size;
    vertexProgramName = temp;

    std::cout << "Vertex program: " << vertexProgramName << std::endl;


    if( pvshSize == total )
    {
	std::cout << "Finished reading PVSH" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading PVSH" << std::endl;
	std::cout << "Read " << total << " out of " << pvshSize
                  << std::endl;
    }

    return total;
}

unsigned int peft::readPFFP( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int pffpSize;
    std::string type;

    total += readFormHeader( file, form, pffpSize, type );
    pffpSize += 8;
    if( form != "FORM" || type != "PFFP" )
    {
	std::cout << "Expected Form of type PFFP: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type 
	      << " (Fixed Function Pipeline)"
	      << ": " << pffpSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readRecordHeader( file, type, size );
    if( type != "0001" )
    {
        std::cout << "Expected record of type 0001: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type
	      << ": " << size << " bytes"
	      << std::endl;

    total += readUnknown( file, size );

    if( pffpSize == total )
    {
	std::cout << "Finished reading PFFP" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading PFFP" << std::endl;
	std::cout << "Read " << total << " out of " << pffpSize
                  << std::endl;
    }

    return total;
}

unsigned int peft::readSTAG( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int stagSize;
    std::string type;

    total += readFormHeader( file, form, stagSize, type );
    stagSize += 8;
    if( form != "FORM" || type != "STAG" )
    {
	std::cout << "Expected Form of type STAG: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << stagSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readRecordHeader( file, type, size );
    if( type != "0000" )
    {
        std::cout << "Expected record of type 0000: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type
	      << ": " << size << " bytes"
	      << std::endl;

    // Skip data for now
    total += readUnknown( file, size );

    if( stagSize == total )
    {
	std::cout << "Finished reading STAG" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading STAG" << std::endl;
	std::cout << "Read " << total << " out of " << stagSize
                  << std::endl;
    }

    return total;
}

unsigned int peft::readPPSH( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int ppshSize;
    std::string type;

    total += readFormHeader( file, form, ppshSize, type );
    ppshSize += 8;
    if( form != "FORM" || type != "PPSH" )
    {
	std::cout << "Expected Form of type PPSH: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type 
	      << " (Pixel shader)"
	      << ": " << ppshSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" )
    {
        std::cout << "Expected FORM not: " << form << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << size-4 << " bytes"
	      << std::endl;

    total += readRecordHeader( file, type, size );
    if( type != "DATA" )
    {
        std::cout << "Expected record of type DATA: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type
	      << ": " << size << " bytes" 
	      << std::endl;

    std::string pixelShaderName;
    unsigned char num;
    file.read( (char *)&num, sizeof( unsigned char ) );
    char temp[255];
    file.getline( temp, size - sizeof( unsigned char ), 0 );
    pixelShaderName = temp;
    std::cout << "Number of texture arguments to pixel shader: "
	      << (unsigned int)num << std::endl;
    std::cout << "Pixel shader: " << pixelShaderName << std::endl;;
    total += size;

    while( total < ppshSize )
    {
	total += readPTXM( file );
    }

    if( ppshSize == total )
    {
	std::cout << "Finished reading PPSH" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading PPSH" << std::endl;
	std::cout << "Read " << total << " out of " << ppshSize
                  << std::endl;
    }

    return total;
}

unsigned int peft::readPTXM( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int ptxmSize;
    std::string type;

    total += readFormHeader( file, form, ptxmSize, type );
    ptxmSize += 8;
    if( form != "FORM" || type != "PTXM" )
    {
	std::cout << "Expected Form of type PTXM: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << ptxmSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readRecordHeader( file, type, size );
#if 0
    if( type != "0002" )
    {
        std::cout << "Expected record of type 0002: " << type << std::endl;
        exit( 0 );
    }
#endif
    std::cout << "Found record " << type
	      << ": " << size << " bytes"
	      << std::endl;

    std::string name;
    unsigned char num;
    file.read( (char *)&num, sizeof( unsigned char ) );
    file.width( size - sizeof( unsigned char ) );
    file >> name;
    total += size;
    std::cout << "Pixel shader texture " << (unsigned int)num
	      << ": " << name << std::endl;

    if( ptxmSize == total )
    {
	std::cout << "Finished reading PTXM" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading PTXM" << std::endl;
	std::cout << "Read " << total << " out of " << ptxmSize
                  << std::endl;
    }

    return total;
}

#endif
