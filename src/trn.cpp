/** -*-c++-*-
 *  \class  trn
 *  \file   trn.cpp
 *  \author Kenneth R. Sewell III

 meshLib is used for the parsing and exporting .trn models.
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

#include <meshLib/trn.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

trn::trn()
{
}

trn::~trn()
{
}

unsigned int trn::readTRN( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int ptatSize;
    std::string type;

    // PTAT Form ( Level 0 )
    total += readFormHeader( file, form, ptatSize, type );
    ptatSize += 8;
    if( form != "FORM" || type != "PTAT" )
    {
	std::cerr << "Expected Form of type PTAT: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found PTAT form" << std::endl;

    // Child form of PTAT ( Level 1 )
    unsigned int size;
    total += readFormHeader( file, form, size, type );
    std::cerr << "Found form: " << form << std::endl;

    total += readTRNDATA( file );

    // TGEN Form ( Level 2 )
    total += readTGEN( file );

    // DATA before WMAP and SMAP
    total += readMapDATA( file );

    if( ptatSize == total )
    {
	std::cerr << "Finished reading PTAT" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading PTAT" << std::endl;
	std::cerr << "Read " << total << " out of " << ptatSize
		  << std::endl;
    }

    return total;
}

unsigned int trn::readTRNDATA( std::istream &file )
{
    unsigned int total = 0;

    unsigned int size;
    std::string type;

    // DATA record ( Level 2 )
    total += readRecordHeader( file, type, size );
    if( type != "DATA" )
    {
	std::cerr << "Expected DATA record: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found DATA record" << std::endl;

    char temp[255];
    file.getline( temp, 255, 0 );
    std::string name( temp );
    std::cout << name << std::endl;
    total += name.size() + 1;

    float x1, x2;
    unsigned int x3, x4;
    float x5, x6;
    
    std::cout.flags ( std::ios_base::showpoint );
    file.read( (char *)&x1, sizeof( x1 ) );
    total += sizeof( x1 );
    std::cout << x1 << std::endl;
    
    file.read( (char *)&x2, sizeof( x2 ) );
    total += sizeof( x2 );
    std::cout << x2 << std::endl;

    file.read( (char *)&x3, sizeof( x3 ) );
    total += sizeof( x3 );
    std::cout << x3 << std::endl;
    
    file.read( (char *)&x4, sizeof( x4 ) );
    total += sizeof( x4 );
    std::cout << x4 << std::endl;

    file.read( (char *)&x5, sizeof( x5 ) );
    total += sizeof( x5 );
    std::cout << x5 << std::endl;
    
    file.read( (char *)&x6, sizeof( x6 ) );
    total += sizeof( x6 );
    std::cout << x6 << std::endl;

    file.getline( temp, 255, 0 );
    std::string waterShader( temp );
    std::cout << waterShader << std::endl;
    total += waterShader.size() + 1;

    float x;
    for( unsigned int i = 0; i < 21; ++i )
    {
	file.read( (char *)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << std::endl;
    }

    size += 8;
    if( size == total )
    {
	std::cerr << "Finished reading DATA" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading DATA" << std::endl;
	std::cerr << "Read " << total << " out of " << size
		  << std::endl;
    }

    return total;
}

unsigned int trn::readTGEN( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int tgenSize;
    std::string type;

    total += readFormHeader( file, form, tgenSize, type );
    tgenSize += 8;
    if( form != "FORM" || type != "TGEN" )
    {
	std::cerr << "Expected Form of type TGEN: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found TGEN form" << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    size += 8;
    if( form != "FORM" )
    {
	std::cerr << "Expected Form: " << form << std::endl;
	exit( 0 );
    }
    std::cerr << "Found FORM: " << type << std::endl;

    total += readSGRP( file );
    total += readFGRP( file );
    total += readRGRP( file );
    total += readEGRP( file );
    total += readMGRP( file );
    total += readLYRS( file );

    if( tgenSize == total )
    {
	std::cerr << "Finished reading TGEN" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading TGEN" << std::endl;
	std::cerr << "Read " << total << " out of " << tgenSize
		  << std::endl;
    }
    return total;
}

// Water or weight maps?
unsigned int trn::readWMAP( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int wmapSize;
    std::string type;

    total += readRecordHeader( file, type, wmapSize );
    if( type != "WMAP" )
    {
	std::cerr << "Expected record of type WMAP: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found WMAP record of size: " << wmapSize << std::endl;

    if( wmapSize != ( height * width ) )
      {
	std::cerr << "WMAP size: " << wmapSize
		  << " does not match expected size: "
		  << ( height * width ) << std::endl;
	exit( 0 );
      }

    unsigned char x;
    for( unsigned int i = 0; i < height; ++i )
    {
      for( unsigned int j = 0; j < width; ++j )
	{
	  file.read( (char*)&x, sizeof( x ) );
#if 0
	  std::cout  << " 0 0 " << (unsigned int)x;
#else
	  std::cout  << (unsigned int)x << " ";
#endif
	}
      std::cout << std::endl;
    }
    std::cout << std::endl;

    total += wmapSize;
    
    wmapSize += 8;
    if( wmapSize == total )
    {
	std::cerr << "Finished reading WMAP" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading WMAP" << std::endl;
	std::cerr << "Read " << total << " out of " << wmapSize
		  << std::endl;
    }
    return total;
}

// Seed maps?
unsigned int trn::readSMAP( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int smapSize;
    std::string type;

    total += readRecordHeader( file, type, smapSize );
    if( type != "SMAP" )
    {
	std::cerr << "Expected record of type SMAP: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found SMAP record of size: " << smapSize << std::endl;

    if( smapSize != ( height * width ) )
      {
	std::cerr << "SMAP size: " << smapSize
		  << " does not match expected size: "
		  << ( height * width ) << std::endl;
	exit( 0 );
      }

    unsigned char x;
    for( unsigned int i = 0; i < height; ++i )
    {
      for( unsigned int j = 0; j < width; ++j )
	{
	  file.read( (char*)&x, sizeof( x ) );
#if 0
	  std::cout << (unsigned int)x << " 0 0 ";
#else
	  std::cout << (unsigned int)x << " ";
#endif
	}
      std::cout << std::endl;
    }
    std::cout << std::endl;

    total += smapSize;

    smapSize += 8;
    if( smapSize == total )
    {
	std::cerr << "Finished reading SMAP" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading SMAP" << std::endl;
	std::cerr << "Read " << total << " out of " << smapSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readSFAM( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int sfamSize;
    std::string type;

    total += readRecordHeader( file, type, sfamSize );
    if( type != "SFAM" )
    {
	std::cerr << "Expected record of type SFAM: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found SFAM record of size: " << sfamSize << std::endl;

    unsigned int x;
    file.read( (char*)&x, sizeof( x ) );
    total += sizeof( x );
    std::cout << "Shader family number: " << x << std::endl;;

    char temp[255];
    file.getline( temp, 255, 0 );
    std::string name( temp );
    std::cout << name << std::endl;
    total += name.size() + 1;

    file.getline( temp, 255, 0 );
    std::string name2( temp );
    std::cout << name2 << std::endl;
    total += name2.size() + 1;

    // Color color...
    unsigned char rgb[3];
    file.read( (char*)rgb, sizeof( unsigned char ) * 3 );
    total += sizeof( unsigned char ) * 3;
    std::cout << "rgb: " << (unsigned int)rgb[0] << ", "
	      << (unsigned int)rgb[1] << ", "
	      << (unsigned int)rgb[2] << std::endl;

    for( unsigned int i = 0; i < 2; ++i )
      {
	float u1;
	file.read( (char*)&u1, sizeof( u1 ) );
	total += sizeof( u1 );
	std::cout << u1 << std::endl;
      }

    unsigned int numLayers;
    file.read( (char*)&numLayers, sizeof( numLayers ) );
    total += sizeof( numLayers );
    std::cout << "numLayers: " << numLayers << std::endl;


    for( unsigned int layer = 0; layer < numLayers; ++layer )
      {
	file.getline( temp, 255, 0 );
	std::string name3( temp );
	std::cout << name3 << std::endl;
	total += name3.size() + 1;
	
	float u1;
	file.read( (char*)&u1, sizeof( u1 ) );
	total += sizeof( u1 );
	std::cout << u1 << std::endl;
      }

    sfamSize += 8;
    if( sfamSize == total )
    {
	std::cerr << "Finished reading SFAM" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading SFAM" << std::endl;
	std::cerr << "Read " << total << " out of " << sfamSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readMapDATA( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int formSize;
    std::string type;

    // Parent Form of DATA,WMAP and SMAP ( Level 2 )
    total += readFormHeader( file, form, formSize, type );
    formSize += 8;
    std::cerr << "Found form: " << type << std::endl;

    // DATA
    unsigned int size;
    total += readRecordHeader( file, type, size );
    if( type != "DATA" )
    {
	std::cerr << "Expected record of type DATA: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found DATA record of size: " << size << std::endl;

    float x1, x2;
    file.read( (char*)&x1, sizeof( x1 ) );
    file.read( (char*)&x2, sizeof( x2 ) );
    file.read( (char*)&height, sizeof( height ) );
    file.read( (char*)&width, sizeof( width ) );
    total += 16;

    std::cout << "Highest point(?): " << x1 << std::endl;
    std::cout << " Lowest point(?): " << x2 << std::endl;
    std::cout << "      Map height: " << height << std::endl;
    std::cout << "       Map width: " << width << std::endl;

    // WMAP
    total += readWMAP( file );

    // SMAP
    total += readSMAP( file );

    if( formSize == total )
    {
	std::cerr << "Finished reading Map Data" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading Map Data" << std::endl;
	std::cerr << "Read " << total << " out of " << size
		  << std::endl;
    }
    return total;
}

unsigned int trn::readSGRP( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int sgrpSize;
    std::string type;

    total += readFormHeader( file, form, sgrpSize, type );
    sgrpSize += 8;
    if( form != "FORM" || type != "SGRP" )
    {
	std::cerr << "Expected Form of type SGRP: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found SGRP form" << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    size += 8;
    if( form != "FORM" )
    {
	std::cerr << "Expected Form: " << form << std::endl;
	exit( 0 );
    }
    std::cerr << "Found FORM: " << type << std::endl;

    while( total < size-12 )
      {
	total += readSFAM( file );
      }

    if( sgrpSize == total )
    {
	std::cerr << "Finished reading SGRP" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading SGRP" << std::endl;
	std::cerr << "Read " << total << " out of " << sgrpSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readFFAM( std::istream &file, const std::string &debugString )
{
  unsigned int size;
  std::string type;
  unsigned int total = readRecordHeader( file, type, size );
  size += 8;
  if( type != "FFAM" )
    {
      std::cerr << "Expected record of type FFAM: " << type << std::endl;
      exit( 0 );
    }
  std::cerr << "Found FFAM record" << std::endl;
  
  unsigned int u1;
  file.read( (char *)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << u1 << std::endl;

#if 1
  char temp[255];
  std::string name;
  file.getline( temp, 255, 0 );
  name = temp;
  std::cout << name << std::endl;
  total += name.size() + 1;
#endif
  total += readUnknown( file, size-total );
  
  if( size == total )
    {
      std::cerr << "Finished reading FFAM" << std::endl;
    }
  else
    {
      std::cerr << "Failed in reading FFAM" << std::endl;
      std::cerr << "Read " << total << " out of " << size
		<< std::endl;
    }
  return total;
}

unsigned int trn::readFGRP( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int fgrpSize;
    std::string type;

    total += readFormHeader( file, form, fgrpSize, type );
    fgrpSize += 8;
    if( form != "FORM" || type != "FGRP" )
    {
	std::cerr << "Expected Form of type FGRP: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found FGRP form" << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    size += 8;
    if( form != "FORM" )
    {
	std::cerr << "Expected Form: " << form << std::endl;
	exit( 0 );
    }
    std::cerr << "Found FORM: " << type << std::endl;

    while( total < fgrpSize )
      {
	total += readFFAM( file, "FGRP" );
      }

    if( fgrpSize == total )
    {
	std::cerr << "Finished reading FGRP" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading FGRP" << std::endl;
	std::cerr << "Read " << total << " out of " << fgrpSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readRFAM( std::istream &file, const std::string &debugString )
{
  unsigned int size;
  std::string type;
  unsigned int total = readRecordHeader( file, type, size );
  size += 8;
  if( type != "RFAM" )
    {
      std::cerr << "Expected record of type RFAM: " << type << std::endl;
      exit( 0 );
    }
  std::cerr << "Found RFAM record" << std::endl;
  
  total += readUnknown( file, size-8 );
  
  if( size == total )
    {
      std::cerr << "Finished reading RFAM" << std::endl;
    }
  else
    {
      std::cerr << "Failed in reading RFAM" << std::endl;
      std::cerr << "Read " << total << " out of " << size
		<< std::endl;
    }
  return total;
}


unsigned int trn::readRGRP( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int rgrpSize;
    std::string type;

    total += readFormHeader( file, form, rgrpSize, type );
    rgrpSize += 8;
    if( form != "FORM" || type != "RGRP" )
    {
	std::cerr << "Expected Form of type RGRP: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found RGRP form" << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    size += 8;
    if( form != "FORM" )
    {
	std::cerr << "Expected Form: " << form << std::endl;
	exit( 0 );
    }
    std::cerr << "Found FORM: " << type << std::endl;

    while( total < rgrpSize )
      {
	total += readRFAM( file, "RGRP" );
      }
    
    if( rgrpSize == total )
    {
	std::cerr << "Finished reading RGRP" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading RGRP" << std::endl;
	std::cerr << "Read " << total << " out of " << rgrpSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readEGRP( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int egrpSize;
    std::string type;

    total += readFormHeader( file, form, egrpSize, type );
    egrpSize += 8;
    if( form != "FORM" || type != "EGRP" )
    {
	std::cerr << "Expected Form of type EGRP: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found EGRP form" << std::endl;


    unsigned int size;
    total += readFormHeader( file, form, size, type );
    size += 8;
    if( form != "FORM" )
    {
	std::cerr << "Expected Form: " << form << std::endl;
	exit( 0 );
    }
    std::cerr << "Found FORM: " << type << std::endl;


    total += readUnknown( file, size-total );
    total += readUnknown( file, egrpSize-total );



    
    if( egrpSize == total )
    {
	std::cerr << "Finished reading EGRP" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading EGRP" << std::endl;
	std::cerr << "Read " << total << " out of " << egrpSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readMGRP( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int mgrpSize;
    std::string type;

    total += readFormHeader( file, form, mgrpSize, type );
    mgrpSize += 8;
    if( form != "FORM" || type != "MGRP" )
    {
	std::cerr << "Expected Form of type MGRP: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found MGRP form" << std::endl;


    unsigned int size;
    total += readFormHeader( file, form, size, type );
    size += 8;
    if( form != "FORM" )
    {
	std::cerr << "Expected Form: " << form << std::endl;
	exit( 0 );
    }
    std::cerr << "Found FORM: " << type << std::endl;


    total += readUnknown( file, size-total );
    total += readUnknown( file, mgrpSize-total );



    
    if( mgrpSize == total )
    {
	std::cerr << "Finished reading MGRP" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading MGRP" << std::endl;
	std::cerr << "Read " << total << " out of " << mgrpSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readLYRS( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int lyrsSize;
    std::string type;

    total += readFormHeader( file, form, lyrsSize, type );
    lyrsSize += 8;
    if( form != "FORM" || type != "LYRS" )
    {
	std::cerr << "Expected Form of type LYRS: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found LYRS form" << std::endl;


    while( total < lyrsSize )
    {
	total += readLAYR( file );
    }
    


    if( lyrsSize == total )
    {
	std::cerr << "Finished reading LYRS" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading LYRS" << std::endl;
	std::cerr << "Read " << total << " out of " << lyrsSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readLAYR( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int layrSize;
    std::string type;

    total += readFormHeader( file, form, layrSize, type );
    layrSize += 8;
    if( form != "FORM" || type != "LAYR" )
    {
	std::cerr << "Expected Form of type LAYR: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found LAYR form" << std::endl;


    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" || type != "0003" )
    {
	std::cerr << "Expected Form of type 0003: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found 0003 form" << std::endl;


    total += readIHDR( file );
    total += readADTA( file );

    unsigned int position;
    while( total < layrSize )
    {
        // Peek at next record, but keep file at same place.
        position = file.tellg();
        readFormHeader( file, form, size, type );
        file.seekg( position, std::ios_base::beg );
	
        if( form == "FORM" )
        {
	    if( "ACCN" == type  )
	    {
		total += readACCN( file );
	    }
	    else if( "ACRF" == type  )
	    {
		total += readACRF( file );
	    }
	    else if( "ACRH" == type  )
	    {
		total += readACRH( file );
	    }
	    else if( "AENV" == type  )
	    {
		total += readAENV( file );
	    }
	    else if( "AEXC" == type  )
	    {
		total += readAEXC( file );
	    }
	    else if( "AFDF" == type  )
	    {
		total += readAFDF( file );
	    }
	    else if( "AFDN" == type  )
	    {
		total += readAFDN( file );
	    }
	    else if( "AFSC" == type  )
	    {
		total += readAFSC( file );
	    }
	    else if( "AFSN" == type  )
	    {
		total += readAFSN( file );
	    }
	    else if( "AHCN" == type  )
	    {
		total += readAHCN( file );
	    }
	    else if( "AHFR" == type  )
	    {
		total += readAHFR( file );
	    }
	    else if( "AHTR" == type  )
	    {
		total += readAHTR( file );
	    }
	    else if( "AROA" == type  )
	    {
		total += readAROA( file );
	    }
	    else if( "ASCN" == type  )
	    {
		total += readASCN( file );
	    }
	    else if( "ASRP" == type  )
	    {
		total += readASRP( file );
	    }
	    else if( "BCIR" == type  )
	    {
		total += readBCIR( file );
	    }
	    else if( "BPLN" == type  )
	    {
		total += readBPLN( file );
	    }
	    else if( "BPOL" == type  )
	    {
		total += readBPOL( file );
	    }
	    else if( "BREC" == type  )
	    {
		total += readBREC( file );
	    }
	    else if( "FDIR" == type  )
	    {
		total += readFDIR( file );
	    }
	    else if( "FFRA" == type  )
	    {
		total += readFFRA( file );
	    }
	    else if( "FHGT" == type  )
	    {
		total += readFHGT( file );
	    }
	    else if( "FSHD" == type  )
	    {
		total += readFSHD( file );
	    }
	    else if( "FSLP" == type  )
	    {
		total += readFSLP( file );
	    }
	    else if( "LAYR" == type  )
	    {
		total += readLAYR( file );
	    }
            else
            {
                std::cerr << "Unexpected form: " << type << std::endl;
                exit( 0 );
            }
 	}
	else
        {
            std::cerr << "Unexpected record: " << form << std::endl;
            exit( 0 );
        }
    }

    if( layrSize == total )
    {
	std::cerr << "Finished reading LAYR" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading LAYR" << std::endl;
	std::cerr << "Read " << total << " out of " << layrSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readIHDR( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int ihdrSize;
    std::string type;

    total += readFormHeader( file, form, ihdrSize, type );
    ihdrSize += 8;
    if( form != "FORM" || type != "IHDR" )
    {
	std::cerr << "Expected Form of type IHDR: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found IHDR form" << std::endl;


    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" || type != "0001" )
    {
	std::cerr << "Expected Form of type 0001: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found 0001 form" << std::endl;

    total += readRecordHeader( file, type, size );
    if( type != "DATA" )
    {
	std::cerr << "Expected record of type DATA: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found DATA record" << std::endl;

    unsigned int u1;
    file.read( (char *)&u1, sizeof( u1 ) );
    total += sizeof( u1 );

    char temp[255];
    file.read( temp, size-sizeof(u1) );
    std::string dataString( temp );
    total += dataString.size() + 1;

    std::cout << u1 << " " << dataString << std::endl;

    std::cout << "</record>" << std::endl;



    if( ihdrSize == total )
    {
	std::cerr << "Finished reading IHDR" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading IHDR" << std::endl;
	std::cerr << "Read " << total << " out of " << ihdrSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readADTA( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int adtaSize;
    std::string type;

    total += readRecordHeader( file, type, adtaSize );
    if( type != "ADTA" )
    {
	std::cerr << "Expected record of type ADTA: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found ADTA record" << std::endl;




#if 1
    unsigned int u1;
    file.read( (char *)&u1, sizeof( u1 ) );
    total += sizeof( u1 );

    unsigned int u2;
    file.read( (char *)&u2, sizeof( u2 ) );
    total += sizeof( u2 );

    unsigned int u3;
    file.read( (char *)&u3, sizeof( u3 ) );
    total += sizeof( u3 );

    char temp[255];
    file.read( temp, adtaSize-(sizeof(u1)+sizeof(u2)+sizeof(u3) ) );
    std::string dataString( temp );
    total += dataString.size() + 1;

    std::cout << u1 << " "
	      << u2 << " "
	      << u3 << " '"
	      << dataString << "'" << std::endl;
#else
    total += readUnknown( file, adtaSize );
#endif
    
    std::cout << "</record>" << std::endl;

    adtaSize += 8;
    if( adtaSize == total )
    {
	std::cerr << "Finished reading ADTA" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading ADTA" << std::endl;
	std::cerr << "Read " << total << " out of " << adtaSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readASCN( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int ascnSize;
    std::string type;

    // FORM ASCN
    total += readFormHeader( file, form, ascnSize, type );
    ascnSize += 8;
    if( form != "FORM" || type != "ASCN" )
    {
	std::cerr << "Expected Form of type ASCN: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found ASCN form" << std::endl;


    // FORM 0001
    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" || type != "0001" )
    {
	std::cerr << "Expected Form of type 0001: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found 0001 form" << std::endl;


    // IHDR
    total += readIHDR( file );

    // DATA
    total += readRecordHeader( file, type, size );
    if( type != "DATA" )
    {
	std::cerr << "Expected record of type DATA: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found ASCNDATA record" << std::endl;



#if 1
    unsigned int u1;
    file.read( (char *)&u1, sizeof( u1 ) );
    total += sizeof( u1 );

    unsigned int u2;
    file.read( (char *)&u2, sizeof( u2 ) );
    total += sizeof( u2 );

    float u3;
    file.read( (char *)&u3, sizeof( u3 ) );
    total += sizeof( u3 );

    std::cout << u1 << " " << u2 << " " << u3 << std::endl;
#else
    total += readUnknown( file, size );
#endif

    std::cout << "</record>" << std::endl;


    
    if( ascnSize == total )
    {
	std::cerr << "Finished reading ASCN" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading ASCN" << std::endl;
	std::cerr << "Read " << total << " out of " << ascnSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readAENV( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int aenvSize;
    std::string type;

    // FORM AENV
    total += readFormHeader( file, form, aenvSize, type );
    aenvSize += 8;
    if( form != "FORM" || type != "AENV" )
    {
	std::cerr << "Expected Form of type AENV: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found AENV form" << std::endl;


    // FORM 0001
    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" || type != "0000" )
    {
	std::cerr << "Expected Form of type 0000: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found 0000 form" << std::endl;


    // IHDR
    total += readIHDR( file );

    // DATA
    total += readRecordHeader( file, type, size );
    if( type != "DATA" )
    {
	std::cerr << "Expected record of type DATA: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found DATA record" << std::endl;

    total += readUnknown( file, size );

    std::cout << "</record>" << std::endl;


    
    if( aenvSize == total )
    {
	std::cerr << "Finished reading AENV" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading AENV" << std::endl;
	std::cerr << "Read " << total << " out of " << aenvSize
		  << std::endl;
    }
    return total;
}


unsigned int trn::readBREC( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int brecSize;
    std::string type;

    total += readFormHeader( file, form, brecSize, type );
    brecSize += 8;
    if( form != "FORM" || type != "BREC" )
    {
	std::cerr << "Expected Form of type BREC: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found BREC form" << std::endl;


    total += readUnknown( file, brecSize-total );
    


    if( brecSize == total )
    {
	std::cerr << "Finished reading BREC" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading BREC" << std::endl;
	std::cerr << "Read " << total << " out of " << brecSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readAHFR( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int ahfrSize;
    std::string type;

    total += readFormHeader( file, form, ahfrSize, type );
    ahfrSize += 8;
    if( form != "FORM" || type != "AHFR" )
    {
	std::cerr << "Expected Form of type AHFR: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found AHFR form" << std::endl;


    total += readUnknown( file, ahfrSize-total );



    if( ahfrSize == total )
    {
	std::cerr << "Finished reading AHFR" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading AHFR" << std::endl;
	std::cerr << "Read " << total << " out of " << ahfrSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readFFRA( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int ffraSize;
    std::string type;

    total += readFormHeader( file, form, ffraSize, type );
    ffraSize += 8;
    if( form != "FORM" || type != "FFRA" )
    {
	std::cerr << "Expected Form of type FFRA: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found FFRA form" << std::endl;


    total += readUnknown( file, ffraSize-total );
    


    if( ffraSize == total )
    {
	std::cerr << "Finished reading FFRA" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading FFRA" << std::endl;
	std::cerr << "Read " << total << " out of " << ffraSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readACCN( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int accnSize;
    std::string type;

    total += readFormHeader( file, form, accnSize, type );
    accnSize += 8;
    if( form != "FORM" || type != "ACCN" )
    {
	std::cerr << "Expected Form of type ACCN: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found ACCN form" << std::endl;


    total += readUnknown( file, accnSize-total );
    


    if( accnSize == total )
    {
	std::cerr << "Finished reading ACCN" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading ACCN" << std::endl;
	std::cerr << "Read " << total << " out of " << accnSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readBPLN( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int bplnSize;
    std::string type;

    total += readFormHeader( file, form, bplnSize, type );
    bplnSize += 8;
    if( form != "FORM" || type != "BPLN" )
    {
	std::cerr << "Expected Form of type BPLN: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found BPLN form" << std::endl;


    total += readUnknown( file, bplnSize-total );
    


    if( bplnSize == total )
    {
	std::cerr << "Finished reading BPLN" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading BPLN" << std::endl;
	std::cerr << "Read " << total << " out of " << bplnSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readBPOL( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int bpolSize;
    std::string type;

    total += readFormHeader( file, form, bpolSize, type );
    bpolSize += 8;
    if( form != "FORM" || type != "BPOL" )
    {
	std::cerr << "Expected Form of type BPOL: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found BPOL form" << std::endl;


    total += readUnknown( file, bpolSize-total );
    


    if( bpolSize == total )
    {
	std::cerr << "Finished reading BPOL" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading BPOL" << std::endl;
	std::cerr << "Read " << total << " out of " << bpolSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readASRP( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int asrpSize;
    std::string type;

    total += readFormHeader( file, form, asrpSize, type );
    asrpSize += 8;
    if( form != "FORM" || type != "ASRP" )
    {
	std::cerr << "Expected Form of type ASRP: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found ASRP form" << std::endl;


    total += readUnknown( file, asrpSize-total );
    


    if( asrpSize == total )
    {
	std::cerr << "Finished reading ASRP" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading ASRP" << std::endl;
	std::cerr << "Read " << total << " out of " << asrpSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readAEXC( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int aexcSize;
    std::string type;

    total += readFormHeader( file, form, aexcSize, type );
    aexcSize += 8;
    if( form != "FORM" || type != "AEXC" )
    {
	std::cerr << "Expected Form of type AEXC: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found AEXC form" << std::endl;


    total += readUnknown( file, aexcSize-total );
    


    if( aexcSize == total )
    {
	std::cerr << "Finished reading AEXC" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading AEXC" << std::endl;
	std::cerr << "Read " << total << " out of " << aexcSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readBCIR( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int bcirSize;
    std::string type;

    total += readFormHeader( file, form, bcirSize, type );
    bcirSize += 8;
    if( form != "FORM" || type != "BCIR" )
    {
	std::cerr << "Expected Form of type BCIR: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found BCIR form" << std::endl;


    total += readUnknown( file, bcirSize-total );
    


    if( bcirSize == total )
    {
	std::cerr << "Finished reading BCIR" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading BCIR" << std::endl;
	std::cerr << "Read " << total << " out of " << bcirSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readAHCN( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int ahcnSize;
    std::string type;

    total += readFormHeader( file, form, ahcnSize, type );
    ahcnSize += 8;
    if( form != "FORM" || type != "AHCN" )
    {
	std::cerr << "Expected Form of type AHCN: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found AHCN form" << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" || type != "0000" )
    {
	std::cerr << "Expected Form of type 0000: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found 0000 form" << std::endl;

    total += readIHDR( file );

    total += readRecordHeader( file, type, size );
    if( type != "DATA" )
    {
	std::cerr << "Expected record of type DATA: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found DATA record" << std::endl;

    unsigned int u1;
    file.read( (char *)&u1, sizeof( u1 ) );
    total += sizeof( u1 );
    std::cout << u1 << std::endl;

    float u2;
    file.read( (char *)&u2, sizeof( u2 ) );
    total += sizeof( u2 );
    std::cout << u2 << std::endl;

    if( ahcnSize == total )
    {
	std::cerr << "Finished reading AHCN" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading AHCN" << std::endl;
	std::cerr << "Read " << total << " out of " << ahcnSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readFHGT( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int fhgtSize;
    std::string type;

    total += readFormHeader( file, form, fhgtSize, type );
    fhgtSize += 8;
    if( form != "FORM" || type != "FHGT" )
    {
	std::cerr << "Expected Form of type FHGT: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found FHGT form" << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" || type != "0002" )
    {
	std::cerr << "Expected Form of type 0002: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found 0002 form" << std::endl;

    total += readIHDR( file );

    total += readRecordHeader( file, type, size );
    if( type != "DATA" )
    {
	std::cerr << "Expected record of type DATA: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found DATA record" << std::endl;

    float u1;
    file.read( (char *)&u1, sizeof( u1 ) );
    total += sizeof( u1 );
    std::cout << u1 << std::endl;

    file.read( (char *)&u1, sizeof( u1 ) );
    total += sizeof( u1 );
    std::cout << u1 << std::endl;

    unsigned int u2;
    file.read( (char *)&u2, sizeof( u2 ) );
    total += sizeof( u2 );
    std::cout << u2 << std::endl;

    file.read( (char *)&u1, sizeof( u1 ) );
    total += sizeof( u1 );
    std::cout << u1 << std::endl;

    if( fhgtSize == total )
    {
	std::cerr << "Finished reading FHGT" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading FHGT" << std::endl;
	std::cerr << "Read " << total << " out of " << fhgtSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readAHTR( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int ahtrSize;
    std::string type;

    total += readFormHeader( file, form, ahtrSize, type );
    ahtrSize += 8;
    if( form != "FORM" || type != "AHTR" )
    {
	std::cerr << "Expected Form of type AHTR: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found AHTR form" << std::endl;


    total += readUnknown( file, ahtrSize-total );
    


    if( ahtrSize == total )
    {
	std::cerr << "Finished reading AHTR" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading AHTR" << std::endl;
	std::cerr << "Read " << total << " out of " << ahtrSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readACRF( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int acrfSize;
    std::string type;

    total += readFormHeader( file, form, acrfSize, type );
    acrfSize += 8;
    if( form != "FORM" || type != "ACRF" )
    {
	std::cerr << "Expected Form of type ACRF: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found ACRF form" << std::endl;


    total += readUnknown( file, acrfSize-total );
    


    if( acrfSize == total )
    {
	std::cerr << "Finished reading ACRF" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading ACRF" << std::endl;
	std::cerr << "Read " << total << " out of " << acrfSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readFSLP( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int fslpSize;
    std::string type;

    total += readFormHeader( file, form, fslpSize, type );
    fslpSize += 8;
    if( form != "FORM" || type != "FSLP" )
    {
	std::cerr << "Expected Form of type FSLP: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found FSLP form" << std::endl;


    total += readUnknown( file, fslpSize-total );
    


    if( fslpSize == total )
    {
	std::cerr << "Finished reading FSLP" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading FSLP" << std::endl;
	std::cerr << "Read " << total << " out of " << fslpSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readAFSC( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int afscSize;
    std::string type;

    total += readFormHeader( file, form, afscSize, type );
    afscSize += 8;
    if( form != "FORM" || type != "AFSC" )
    {
	std::cerr << "Expected Form of type AFSC: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found AFSC form" << std::endl;


    total += readUnknown( file, afscSize-total );
    


    if( afscSize == total )
    {
	std::cerr << "Finished reading AFSC" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading AFSC" << std::endl;
	std::cerr << "Read " << total << " out of " << afscSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readFSHD( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int fshdSize;
    std::string type;

    total += readFormHeader( file, form, fshdSize, type );
    fshdSize += 8;
    if( form != "FORM" || type != "FSHD" )
    {
	std::cerr << "Expected Form of type FSHD: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found FSHD form" << std::endl;


    total += readUnknown( file, fshdSize-total );
    


    if( fshdSize == total )
    {
	std::cerr << "Finished reading FSHD" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading FSHD" << std::endl;
	std::cerr << "Read " << total << " out of " << fshdSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readAFDN( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int afdnSize;
    std::string type;

    total += readFormHeader( file, form, afdnSize, type );
    afdnSize += 8;
    if( form != "FORM" || type != "AFDN" )
    {
	std::cerr << "Expected Form of type AFDN: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found AFDN form" << std::endl;


    total += readUnknown( file, afdnSize-total );
    


    if( afdnSize == total )
    {
	std::cerr << "Finished reading AFDN" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading AFDN" << std::endl;
	std::cerr << "Read " << total << " out of " << afdnSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readAFSN( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int afsnSize;
    std::string type;

    total += readFormHeader( file, form, afsnSize, type );
    afsnSize += 8;
    if( form != "FORM" || type != "AFSN" )
    {
	std::cerr << "Expected Form of type AFSN: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found AFSN form" << std::endl;


    total += readUnknown( file, afsnSize-total );
    


   if( afsnSize == total )
    {
	std::cerr << "Finished reading AFSN" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading AFSN" << std::endl;
	std::cerr << "Read " << total << " out of " << afsnSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readAFDF( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int afdfSize;
    std::string type;

    total += readFormHeader( file, form, afdfSize, type );
    afdfSize += 8;
    if( form != "FORM" || type != "AFDF" )
    {
	std::cerr << "Expected Form of type AFDF: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found AFDF form" << std::endl;


    total += readUnknown( file, afdfSize-total );
    


    if( afdfSize == total )
    {
	std::cerr << "Finished reading AFDF" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading AFDF" << std::endl;
	std::cerr << "Read " << total << " out of " << afdfSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readACRH( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int acrhSize;
    std::string type;

    total += readFormHeader( file, form, acrhSize, type );
    acrhSize += 8;
    if( form != "FORM" || type != "ACRH" )
    {
	std::cerr << "Expected Form of type ACRH: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found ACRH form" << std::endl;


    total += readUnknown( file, acrhSize-total );
    


    if( acrhSize == total )
    {
	std::cerr << "Finished reading ACRH" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading ACRH" << std::endl;
	std::cerr << "Read " << total << " out of " << acrhSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readAROA( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int aroaSize;
    std::string type;

    total += readFormHeader( file, form, aroaSize, type );
    aroaSize += 8;
    if( form != "FORM" || type != "AROA" )
    {
	std::cerr << "Expected Form of type AROA: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found AROA form" << std::endl;


    total += readUnknown( file, aroaSize-total );
    


    if( aroaSize == total )
    {
	std::cerr << "Finished reading AROA" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading AROA" << std::endl;
	std::cerr << "Read " << total << " out of " << aroaSize
		  << std::endl;
    }
    return total;
}

unsigned int trn::readFDIR( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int fdirSize;
    std::string type;

    total += readFormHeader( file, form, fdirSize, type );
    fdirSize += 8;
    if( form != "FORM" || type != "FDIR" )
    {
	std::cerr << "Expected Form of type FDIR: " << type << std::endl;
	exit( 0 );
    }
    std::cerr << "Found FDIR form" << std::endl;


    total += readUnknown( file, fdirSize-total );
    


    if( fdirSize == total )
    {
	std::cerr << "Finished reading FDIR" << std::endl;
    }
    else
    {
	std::cerr << "Failed in reading FDIR" << std::endl;
	std::cerr << "Read " << total << " out of " << fdirSize
		  << std::endl;
    }
    return total;
}

