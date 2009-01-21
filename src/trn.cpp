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

unsigned int trn::readTRN( std::istream &file, const std::string &debugString )
{
  unsigned int ptatSize;
  // PTAT Form ( Level 0 )
  unsigned int total = readFormHeader( file, "PTAT", ptatSize );
  ptatSize += 8;
  std::cout << "Found PTAT form" << std::endl;

  // Child form of PTAT ( Level 1 )
  std::string form, type;
  unsigned int size;
  total += readFormHeader( file, form, size, type );
  std::cout << "Found form: " << form << std::endl;

  total += readTRNDATA( file, debugString );

  // TGEN Form ( Level 2 )
  total += readTGEN( file, debugString );

  // DATA before WMAP and SMAP
  total += readMapDATA( file, debugString );

  if( ptatSize == total )
    {
      std::cout << "Finished reading PTAT" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading PTAT" << std::endl;
      std::cout << "Read " << total << " out of " << ptatSize
		<< std::endl;
    }

  return total;
}

unsigned int trn::readTRNDATA( std::istream &file,
			       const std::string &debugString )
{
  std::string dbgStr = debugString + "DATA: ";

  unsigned int size;
  std::string type;

  // DATA record ( Level 2 )
  unsigned int total = readRecordHeader( file, type, size );
  size += 8;
  if( type != "DATA" )
    {
      std::cout << "Expected DATA record: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found DATA record" << std::endl;

  std::string name;
  total += base::read( file, name );
  std::cout << dbgStr << name << std::endl;

  unsigned int x4;
  float x5, x6;
    
  std::cout.flags ( std::ios_base::showpoint );
  total += base::read( file, terrainSize );
  std::cout << dbgStr << "Terrain size: " << terrainSize << std::endl;
  
  total += base::read( file, blockSize );
  std::cout << dbgStr << "Block size: " << blockSize << std::endl;

  total += base::read( file, tilesPerBlock );
  std::cout << dbgStr << "Tiles per block: " << tilesPerBlock << std::endl;
    
  total += base::read( file, x4 );
  std::cout << dbgStr << x4 << std::endl;

  total += base::read( file, x5 );
  std::cout << dbgStr << x5 << std::endl;
    
  total += base::read( file, x6 );
  std::cout << dbgStr << x6 << std::endl;

  std::string waterShader;
  total += base::read( file, waterShader );
  std::cout << dbgStr << waterShader << std::endl;

  float x;
  for( unsigned int i = 0; i < 21; ++i )
    {
      total += base::read( file, x );
      std::cout << dbgStr << x << std::endl;
    }

  if( size == total )
    {
      std::cout << debugString << "Finished reading DATA" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading DATA" << std::endl;
      std::cout << "Read " << total << " out of " << size
		<< std::endl;
    }

  return total;
}

unsigned int trn::readTGEN( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "TGEN: ";
  std::string form;
  unsigned int tgenSize;
  std::string type;

  unsigned int total = readFormHeader( file, form, tgenSize, type );
  tgenSize += 8;
  if( form != "FORM" || type != "TGEN" )
    {
      std::cout << "Expected Form of type TGEN: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found TGEN form" << std::endl;

  unsigned int size;
  total += readFormHeader( file, form, size, type );
  size += 8;
  if( form != "FORM" )
    {
      std::cout << "Expected Form: " << form << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found FORM: " << type << std::endl;

  total += readSGRP( file, dbgStr );
  total += readFGRP( file, dbgStr );
  total += readRGRP( file, dbgStr );
  total += readEGRP( file, dbgStr );
  total += readMGRP( file, dbgStr );
  total += readLYRS( file, dbgStr );

  if( tgenSize == total )
    {
      std::cout << debugString << "Finished reading TGEN" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading TGEN" << std::endl;
      std::cout << "Read " << total << " out of " << tgenSize
		<< std::endl;
    }
  return total;
}

// Water or weight maps?
unsigned int trn::readWMAP( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "WMAP: ";
  std::string form;
  unsigned int wmapSize;
  std::string type;

  unsigned int total = readRecordHeader( file, type, wmapSize );
  if( type != "WMAP" )
    {
      std::cout << "Expected record of type WMAP: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found WMAP record of size: "
	    << wmapSize << std::endl;

  if( wmapSize != ( height * width ) )
    {
      std::cout << dbgStr << "WMAP size: " << wmapSize
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
      std::cout << "Finished reading WMAP" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading WMAP" << std::endl;
      std::cout << "Read " << total << " out of " << wmapSize
		<< std::endl;
    }
  return total;
}

// Seed maps?
unsigned int trn::readSMAP( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "SMAP: ";
  std::string form;
  unsigned int smapSize;
  std::string type;

  unsigned int total = readRecordHeader( file, type, smapSize );
  if( type != "SMAP" )
    {
      std::cout << "Expected record of type SMAP: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found SMAP record of size: " << smapSize
	    << std::endl;

  if( smapSize != ( height * width ) )
    {
      std::cout << "SMAP size: " << smapSize
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
      std::cout << debugString << "Finished reading SMAP" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading SMAP" << std::endl;
      std::cout << "Read " << total << " out of " << smapSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readSFAM( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "SFAM: ";
  std::string form;
  unsigned int sfamSize;
  std::string type;

  unsigned int total = readRecordHeader( file, type, sfamSize );
  sfamSize += 8;
  if( type != "SFAM" )
    {
      std::cout << "Expected record of type SFAM: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found SFAM record of size: "
	    << sfamSize << std::endl;

  unsigned int x;
  file.read( (char*)&x, sizeof( x ) );
  total += sizeof( x );
  std::cout << dbgStr << "Shader family number: " << x << std::endl;;

  std::string name;
  total += base::read( file, name );
  std::cout << dbgStr << "'" << name << "'" << std::endl;

  std::string name2;
  total += base::read( file, name2 );
  std::cout << dbgStr << "'" << name2 << "'" << std::endl;

  // Color color...
  unsigned char rgb[3];
  file.read( (char*)rgb, sizeof( unsigned char ) * 3 );
  total += sizeof( unsigned char ) * 3;
  std::cout << dbgStr << "rgb: " << (unsigned int)rgb[0] << ", "
	    << (unsigned int)rgb[1] << ", "
	    << (unsigned int)rgb[2] << std::endl;

  for( unsigned int i = 0; i < 2; ++i )
    {
      float u1;
      file.read( (char*)&u1, sizeof( u1 ) );
      total += sizeof( u1 );
      std::cout << dbgStr << u1 << std::endl;
    }

  unsigned int numLayers;
  file.read( (char*)&numLayers, sizeof( numLayers ) );
  total += sizeof( numLayers );
  std::cout << dbgStr << "numLayers: " << numLayers << std::endl;

  for( unsigned int layer = 0; layer < numLayers; ++layer )
    {
      std::string name3;
      total += base::read( file, name3 );
      std::cout << dbgStr << "'" << name3 << "'" << std::endl;

      float u1;
      file.read( (char*)&u1, sizeof( u1 ) );
      total += sizeof( u1 );
      std::cout << dbgStr << u1 << std::endl;
    }

  if( sfamSize == total )
    {
      std::cout << debugString << "Finished reading SFAM" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading SFAM" << std::endl;
      std::cout << "Read " << total << " out of " << sfamSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readMapDATA( std::istream &file,
			       const std::string &debugString )
{
  std::string dbgStr = debugString + "DATA: ";
  std::string form;
  unsigned int formSize;
  std::string type;

  // Parent Form of DATA,WMAP and SMAP ( Level 2 )
  unsigned int total = readFormHeader( file, form, formSize, type );
  formSize += 8;
  std::cout << debugString << "Found form: " << type << std::endl;

  // DATA
  unsigned int size;
  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected record of type DATA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record of size: " << size << std::endl;

  float x1, x2;
  file.read( (char*)&x1, sizeof( x1 ) );
  file.read( (char*)&x2, sizeof( x2 ) );
  file.read( (char*)&height, sizeof( height ) );
  file.read( (char*)&width, sizeof( width ) );
  total += 16;

  std::cout << dbgStr << " Terrain size(m): " << x1 << std::endl;
  std::cout << dbgStr << "   Block size(m): " << x2 << std::endl;
  std::cout << dbgStr << "      Map height: " << height << std::endl;
  std::cout << dbgStr << "       Map width: " << width << std::endl;

  // WMAP
  total += readWMAP( file, dbgStr );

  // SMAP
  total += readSMAP( file, dbgStr );

  if( formSize == total )
    {
      std::cout << "Finished reading Map Data" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading Map Data" << std::endl;
      std::cout << "Read " << total << " out of " << size
		<< std::endl;
    }
  return total;
}

unsigned int trn::readSGRP( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "SGRP: ";
  std::string form;
  unsigned int sgrpSize;
  std::string type;

  unsigned int total = readFormHeader( file, form, sgrpSize, type );
  sgrpSize += 8;
  if( form != "FORM" || type != "SGRP" )
    {
      std::cout << "Expected Form of type SGRP: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found SGRP form" << std::endl;

  unsigned int size;
  total += readFormHeader( file, form, size, type );
  size += 8;
  if( form != "FORM" )
    {
      std::cout << "Expected Form: " << form << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found FORM: " << type << std::endl;

  while( total < size-12 )
    {
      total += readSFAM( file, dbgStr );
    }

  if( sgrpSize == total )
    {
      std::cout << debugString << "Finished reading SGRP" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading SGRP" << std::endl;
      std::cout << "Read " << total << " out of " << sgrpSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readFFAM( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "FFAM: ";
  unsigned int size;
  std::string type;
  unsigned int total = readRecordHeader( file, type, size );
  size += 8;
  if( type != "FFAM" )
    {
      std::cout << "Expected record of type FFAM: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found FFAM record" << std::endl;
  
  unsigned int u1;
  file.read( (char *)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << dbgStr << u1 << std::endl;

  std::string name;
  total += base::read( file, name );
  std::cout << dbgStr << "'" << name << "'" << std::endl;

  unsigned short u2;
  file.read( (char *)&u2, sizeof( u2 ) );
  total += sizeof( u2 );
  std::cout << dbgStr << u2 << std::endl;

  file.read( (char *)&u2, sizeof( u2 ) );
  total += sizeof( u2 );
  std::cout << dbgStr << u2 << std::endl;

  file.read( (char *)&u2, sizeof( u2 ) );
  total += sizeof( u2 );
  std::cout << dbgStr << u2 << std::endl;

  unsigned char u3;
  file.read( (char *)&u3, sizeof( u3 ) );
  total += sizeof( u3 );
  std::cout << dbgStr << (unsigned int)u3 << std::endl;

  file.read( (char *)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << dbgStr << u1 << std::endl;

  unsigned int numApt;
  file.read( (char *)&numApt, sizeof( numApt ) );
  total += sizeof( numApt );
  std::cout << dbgStr << "numApt: " << numApt << std::endl;

  for( unsigned int i = 0; i < numApt; ++i )
    {
      std::string aptName;
      total += base::read( file, aptName );
      std::cout << dbgStr << "'" << aptName << "'" << std::endl;
	  
      float u4;
      file.read( (char *)&u4, sizeof( u4 ) );
      total += sizeof( u4 );
      std::cout << dbgStr << u4 << std::endl;
	  
      file.read( (char *)&u1, sizeof( u1 ) );
      total += sizeof( u1 );
      std::cout << dbgStr << u1 << std::endl;

      file.read( (char *)&u4, sizeof( u4 ) );
      total += sizeof( u4 );
      std::cout << dbgStr << u4 << std::endl;

      file.read( (char *)&u4, sizeof( u4 ) );
      total += sizeof( u4 );
      std::cout << dbgStr << u4 << std::endl;

      file.read( (char *)&u1, sizeof( u1 ) );
      total += sizeof( u1 );
      std::cout << dbgStr << u1 << std::endl;

      file.read( (char *)&u1, sizeof( u1 ) );
      total += sizeof( u1 );
      std::cout << dbgStr << u1 << std::endl;

      file.read( (char *)&u4, sizeof( u4 ) );
      total += sizeof( u4 );
      std::cout << dbgStr << u4 << std::endl;

      file.read( (char *)&u4, sizeof( u4 ) );
      total += sizeof( u4 );
      std::cout << dbgStr << u4 << std::endl;
    }
  
  if( size == total )
    {
      std::cout << debugString << "Finished reading FFAM" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading FFAM" << std::endl;
      std::cout << "Read " << total << " out of " << size
		<< std::endl;
    }
  return total;
}

unsigned int trn::readFGRP( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "FGRP: ";
  std::string form;
  unsigned int fgrpSize;
  std::string type;

  unsigned int total = readFormHeader( file, form, fgrpSize, type );
  fgrpSize += 8;
  if( form != "FORM" || type != "FGRP" )
    {
      std::cout << "Expected Form of type FGRP: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found FGRP form" << std::endl;

  unsigned int size;
  total += readFormHeader( file, form, size, type );
  size += 8;
  if( form != "FORM" )
    {
      std::cout << "Expected Form: " << form << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found FORM: " << type << std::endl;

  while( total < fgrpSize )
    {
      total += readFFAM( file, dbgStr );
    }

  if( fgrpSize == total )
    {
      std::cout << debugString << "Finished reading FGRP" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading FGRP" << std::endl;
      std::cout << "Read " << total << " out of " << fgrpSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readRFAM( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "RFAM: ";
  unsigned int size;
  std::string type;
  unsigned int total = readRecordHeader( file, type, size );
  size += 8;
  if( type != "RFAM" )
    {
      std::cout << "Expected record of type RFAM: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found RFAM record" << std::endl;
  
  unsigned int u1;
  file.read( (char *)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << dbgStr << u1 << std::endl;

  std::string name;
  total += base::read( file, name );
  std::cout << dbgStr << "'" << name << "'" << std::endl;

  unsigned short u2;
  file.read( (char *)&u2, sizeof( u2 ) );
  total += sizeof( u2 );
  std::cout << dbgStr << u2 << std::endl;

  file.read( (char *)&u2, sizeof( u2 ) );
  total += sizeof( u2 );
  std::cout << dbgStr << u2 << std::endl;

  file.read( (char *)&u2, sizeof( u2 ) );
  total += sizeof( u2 );
  std::cout << dbgStr << u2 << std::endl;

  unsigned char u3;
  file.read( (char *)&u3, sizeof( u3 ) );
  total += sizeof( u3 );
  std::cout << dbgStr << (unsigned int)u3 << std::endl;

  unsigned int numApt;
  file.read( (char *)&numApt, sizeof( numApt ) );
  total += sizeof( numApt );
  std::cout << dbgStr << "numApt: " << numApt << std::endl;

  for( unsigned int i = 0; i < numApt; ++i )
    {
      std::string aptName;
      total += base::read( file, aptName );
      std::cout << dbgStr << "'" << aptName << "'" << std::endl;

      float u4;
      file.read( (char *)&u4, sizeof( u4 ) );
      total += sizeof( u4 );
      std::cout << dbgStr << u4 << std::endl;
	  
      file.read( (char *)&u4, sizeof( u4 ) );
      total += sizeof( u4 );
      std::cout << dbgStr << u4 << std::endl;

      file.read( (char *)&u4, sizeof( u4 ) );
      total += sizeof( u4 );
      std::cout << dbgStr << u4 << std::endl;

      file.read( (char *)&u4, sizeof( u4 ) );
      total += sizeof( u4 );
      std::cout << dbgStr << u4 << std::endl;

      file.read( (char *)&u1, sizeof( u1 ) );
      total += sizeof( u1 );
      std::cout << dbgStr << u1 << std::endl;

      file.read( (char *)&u4, sizeof( u4 ) );
      total += sizeof( u4 );
      std::cout << dbgStr << u4 << std::endl;

      file.read( (char *)&u4, sizeof( u4 ) );
      total += sizeof( u4 );
      std::cout << dbgStr << u4 << std::endl;

      file.read( (char *)&u1, sizeof( u1 ) );
      total += sizeof( u1 );
      std::cout << dbgStr << u1 << std::endl;

      file.read( (char *)&u4, sizeof( u4 ) );
      total += sizeof( u4 );
      std::cout << dbgStr << u4 << std::endl;
    }
  
  if( size == total )
    {
      std::cout << debugString << "Finished reading RFAM" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading RFAM" << std::endl;
      std::cout << "Read " << total << " out of " << size
		<< std::endl;
    }
  return total;
}

unsigned int trn::readRGRP( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "RGRP: ";
  std::string form;
  unsigned int rgrpSize;
  std::string type;

  unsigned int total = readFormHeader( file, form, rgrpSize, type );
  rgrpSize += 8;
  if( form != "FORM" || type != "RGRP" )
    {
      std::cout << "Expected Form of type RGRP: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found RGRP form" << std::endl;

  unsigned int size;
  total += readFormHeader( file, form, size, type );
  size += 8;
  if( form != "FORM" )
    {
      std::cout << "Expected Form: " << form << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found FORM: " << type << std::endl;

  while( total < rgrpSize )
    {
      total += readRFAM( file, dbgStr );
    }
    
  if( rgrpSize == total )
    {
      std::cout << debugString << "Finished reading RGRP" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading RGRP" << std::endl;
      std::cout << "Read " << total << " out of " << rgrpSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readEFAM( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "EFAM: ";
  unsigned int efamSize;
  unsigned int total = readFormHeader( file, "EFAM", efamSize );
  efamSize += 8;
  std::cout << debugString << "Found EFAM form" << std::endl;

  unsigned int size;
  std::string type;
  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected record of type DATA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;

  unsigned int u1;
  total += base::read( file, u1 );
  std::cout << dbgStr << u1 << std::endl;

  char temp[255];
  std::string name;
  file.read( temp, size - 12 );
  temp[size-12] = 0;
  name = temp;
  std::cout << dbgStr << "'" << name << "'" << std::endl;
  total += name.size();

  unsigned short u3;
  total += base::read( file, u3 );
  std::cout << dbgStr << u3 << std::endl;

  total += base::read( file, u3 );
  std::cout << dbgStr << u3 << std::endl;

  float u2;
  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;
  
  if( efamSize == total )
    {
      std::cout << debugString << "Finished reading EFAM" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading EFAM" << std::endl;
      std::cout << "Read " << total << " out of " << efamSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readEGRP( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "EGRP: ";
  std::string form;
  unsigned int egrpSize;
  std::string type;

  unsigned int total = readFormHeader( file, form, egrpSize, type );
  egrpSize += 8;
  if( form != "FORM" || type != "EGRP" )
    {
      std::cout << "Expected Form of type EGRP: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found EGRP form" << std::endl;

  unsigned int size;
  total += readFormHeader( file, form, size, type );
  size += 8;
  if( form != "FORM" )
    {
      std::cout << "Expected Form: " << form << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found FORM: " << type << std::endl;

  while( total < egrpSize )
    {
      total += readEFAM( file, dbgStr );
    }
    
  if( egrpSize == total )
    {
      std::cout << debugString << "Finished reading EGRP" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading EGRP" << std::endl;
      std::cout << "Read " << total << " out of " << egrpSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readMFRC( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "MFRC: ";
  unsigned int mfrcSize;
  std::string form;
  std::string type;
  unsigned int total = readFormHeader( file, form, mfrcSize, type );
  mfrcSize += 8;
  if( form != "FORM" || type != "MFRC" )
    {
      std::cout << "Expected Form of type MFRC: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found MFRC form" << std::endl;

  unsigned int size;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0001" )
    {
      std::cout << "Expected Form of type 0001: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0001 form" << std::endl;

  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected record of type DATA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;

  unsigned int u1;
  total += base::read( file, u1 );
  std::cout << dbgStr << u1 << std::endl;

  float u2;
  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;

  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;

  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;

  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;

  total += base::read( file, u1 );
  std::cout << dbgStr << u1 << std::endl;

  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;

  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;

  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;

  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;

  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;

  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;

  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;

  if( mfrcSize == total )
    {
      std::cout << debugString << "Finished reading MFRC" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading MFRC" << std::endl;
      std::cout << "Read " << total << " out of " << mfrcSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readMFAM( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "MFAM: ";
  unsigned int mfamSize;
  std::string form;
  std::string type;
  unsigned int total = readFormHeader( file, form, mfamSize, type );
  mfamSize += 8;
  if( form != "FORM" || type != "MFAM" )
    {
      std::cout << "Expected Form of type MFAM: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found MFAM form" << std::endl;

  unsigned int size;
  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected record of type DATA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;

  unsigned int u1;
  total += base::read( file, u1 );
  std::cout << dbgStr << u1 << std::endl;

  char temp[255];
  std::string name;
  file.read( temp, size - 4 );
  name = temp;
  std::cout << dbgStr << name << std::endl;
  total += name.size()+1;

  total += readMFRC( file, dbgStr );
  
  if( mfamSize == total )
    {
      std::cout << debugString << "Finished reading MFAM" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading MFAM" << std::endl;
      std::cout << "Read " << total << " out of " << mfamSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readMGRP( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "MGRP: ";
  unsigned int total = 0;

  std::string form;
  unsigned int mgrpSize;
  std::string type;

  total += readFormHeader( file, form, mgrpSize, type );
  mgrpSize += 8;
  if( form != "FORM" || type != "MGRP" )
    {
      std::cout << "Expected Form of type MGRP: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found MGRP form" << std::endl;


  unsigned int size;
  total += readFormHeader( file, form, size, type );
  size += 8;
  if( form != "FORM" )
    {
      std::cout << "Expected Form: " << form << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found FORM: " << type << std::endl;

#if 1
  while( total < mgrpSize )
    {
      total += readMFAM( file, dbgStr );
    }
#else
  total += readUnknown( file, mgrpSize-total );
#endif

   
  if( mgrpSize == total )
    {
      std::cout << debugString << "Finished reading MGRP" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading MGRP" << std::endl;
      std::cout << "Read " << total << " out of " << mgrpSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readLYRS( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "LYRS: ";
  std::string form;
  unsigned int lyrsSize;
  std::string type;

  unsigned int total = readFormHeader( file, form, lyrsSize, type );
  lyrsSize += 8;
  if( form != "FORM" || type != "LYRS" )
    {
      std::cout << "Expected Form of type LYRS: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found LYRS form" << std::endl;

  while( total < lyrsSize )
    {
      total += readLAYR( file, dbgStr );
    }

  if( lyrsSize == total )
    {
      std::cout << debugString << "Finished reading LYRS" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading LYRS" << std::endl;
      std::cout << "Read " << total << " out of " << lyrsSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readLAYR( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "LAYR: ";
  std::string form;
  unsigned int layrSize;
  std::string type;

  unsigned int total = readFormHeader( file, form, layrSize, type );
  layrSize += 8;
  if( form != "FORM" || type != "LAYR" )
    {
      std::cout << "Expected Form of type LAYR: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found LAYR form" << std::endl;


  unsigned int size;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0003" )
    {
      std::cout << "Expected Form of type 0003: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0003 form" << std::endl;

  unsigned int u1;
  std::string name;
  total += readIHDR( file, dbgStr, u1, name );
  total += readADTA( file, dbgStr );

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
	      total += readACCN( file, dbgStr );
	    }
	  else if( "ACRF" == type  )
	    {
	      total += readACRF( file, dbgStr );
	    }
	  else if( "ACRH" == type  )
	    {
	      total += readACRH( file, dbgStr );
	    }
	  else if( "AENV" == type  )
	    {
	      total += readAENV( file, dbgStr );
	    }
	  else if( "AEXC" == type  )
	    {
	      total += readAEXC( file, dbgStr );
	    }
	  else if( "AFDF" == type  )
	    {
	      total += readAFDF( file, dbgStr );
	    }
	  else if( "AFDN" == type  )
	    {
	      total += readAFDN( file, dbgStr );
	    }
	  else if( "AFSC" == type  )
	    {
	      total += readAFSC( file, dbgStr );
	    }
	  else if( "AFSN" == type  )
	    {
	      total += readAFSN( file, dbgStr );
	    }
	  else if( "AHCN" == type  )
	    {
	      total += readAHCN( file, dbgStr );
	    }
	  else if( "AHFR" == type  )
	    {
	      total += readAHFR( file, dbgStr );
	    }
	  else if( "AHTR" == type  )
	    {
	      total += readAHTR( file, dbgStr );
	    }
	  else if( "AROA" == type  )
	    {
	      total += readAROA( file, dbgStr );
	    }
	  else if( "ASCN" == type  )
	    {
	      total += readASCN( file, dbgStr );
	    }
	  else if( "ASRP" == type  )
	    {
	      total += readASRP( file, dbgStr );
	    }
	  else if( "BCIR" == type  )
	    {
	      bcir newBCIR;
	      bcirList.push_back( newBCIR );
	      total += readBCIR( file, dbgStr, bcirList.back() );
	    }
	  else if( "BPLN" == type  )
	    {
	      bpln newBPLN;
	      bplnList.push_back( newBPLN );
	      total += readBPLN( file, dbgStr, bplnList.back() );
	    }
	  else if( "BPOL" == type  )
	    {
	      bpol newBPOL;
	      bpolList.push_back( newBPOL );
	      total += readBPOL( file, dbgStr, bpolList.back() );
	    }
	  else if( "BREC" == type  )
	    {
	      brec newBREC;
	      brecList.push_back( newBREC );
	      total += readBREC( file, dbgStr, brecList.back() );
	    }
	  else if( "FDIR" == type  )
	    {
	      total += readFDIR( file, dbgStr );
	    }
	  else if( "FFRA" == type  )
	    {
	      total += readFFRA( file, dbgStr );
	    }
	  else if( "FHGT" == type  )
	    {
	      total += readFHGT( file, dbgStr );
	    }
	  else if( "FSHD" == type  )
	    {
	      total += readFSHD( file, dbgStr );
	    }
	  else if( "FSLP" == type  )
	    {
	      total += readFSLP( file, dbgStr );
	    }
	  else if( "LAYR" == type  )
	    {
	      total += readLAYR( file, dbgStr );
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

  if( layrSize == total )
    {
      std::cout << debugString << "Finished reading LAYR" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading LAYR" << std::endl;
      std::cout << "Read " << total << " out of " << layrSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readIHDR( std::istream &file,
			    const std::string &debugString,
			    unsigned int &u1,
			    std::string &name )
{
  std::string dbgStr = debugString + "IHDR: ";
  unsigned int ihdrSize;

  unsigned int total = readFormHeader( file, "IHDR", ihdrSize );
  ihdrSize += 8;
  std::cout << debugString << "Found IHDR form" << std::endl;

  unsigned int size;
  total += readFormHeader( file, "0001", size );
  std::cout << dbgStr << "Found 0001 form" << std::endl;

  std::string type;
  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected record of type DATA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;

  file.read( (char *)&u1, sizeof( u1 ) );
  total += sizeof( u1 );

  total += base::read( file, name );
  std::cout << dbgStr << u1 << " " << name << std::endl;

  if( ihdrSize == total )
    {
      std::cout << debugString << "Finished reading IHDR" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading IHDR" << std::endl;
      std::cout << "Read " << total << " out of " << ihdrSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readADTA( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "ADTA: ";
  std::string form;
  unsigned int adtaSize;
  std::string type;

  unsigned int total = readRecordHeader( file, type, adtaSize );
  if( type != "ADTA" )
    {
      std::cout << "Expected record of type ADTA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found ADTA record" << std::endl;

  unsigned int u1;
  file.read( (char *)&u1, sizeof( u1 ) );
  total += sizeof( u1 );

  unsigned int u2;
  file.read( (char *)&u2, sizeof( u2 ) );
  total += sizeof( u2 );

  unsigned int u3;
  file.read( (char *)&u3, sizeof( u3 ) );
  total += sizeof( u3 );

  std::string name;
  total += base::read( file, name );

  std::cout << dbgStr << u1 << " "
	    << u2 << " "
	    << u3 << " '"
	    << name << "'" << std::endl;

  adtaSize += 8;
  if( adtaSize == total )
    {
      std::cout << debugString << "Finished reading ADTA" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading ADTA" << std::endl;
      std::cout << "Read " << total << " out of " << adtaSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readASCN( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "ASCN: ";
  std::string form;
  unsigned int ascnSize;
  std::string type;

  // FORM ASCN
  unsigned int total = readFormHeader( file, form, ascnSize, type );
  ascnSize += 8;
  if( form != "FORM" || type != "ASCN" )
    {
      std::cout << "Expected Form of type ASCN: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found ASCN form" << std::endl;

  // FORM 0001
  unsigned int size;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0001" )
    {
      std::cout << "Expected Form of type 0001: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0001 form" << std::endl;

  // IHDR
  {
    unsigned int u1;
    std::string name;
    total += readIHDR( file, dbgStr, u1, name );
  }

  // DATA
  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected record of type DATA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;

  unsigned int u1;
  file.read( (char *)&u1, sizeof( u1 ) );
  total += sizeof( u1 );

  unsigned int u2;
  file.read( (char *)&u2, sizeof( u2 ) );
  total += sizeof( u2 );

  float u3;
  file.read( (char *)&u3, sizeof( u3 ) );
  total += sizeof( u3 );

  std::cout << dbgStr << u1 << " " << u2 << " " << u3 << std::endl;

  if( ascnSize == total )
    {
      std::cout << debugString << "Finished reading ASCN" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading ASCN" << std::endl;
      std::cout << "Read " << total << " out of " << ascnSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readAENV( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "AENV: ";
  std::string form;
  unsigned int aenvSize;
  std::string type;

  // FORM AENV
  unsigned int total = readFormHeader( file, form, aenvSize, type );
  aenvSize += 8;
  if( form != "FORM" || type != "AENV" )
    {
      std::cout << "Expected Form of type AENV: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found AENV form" << std::endl;

  // FORM 0001
  unsigned int size;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0000" )
    {
      std::cout << "Expected Form of type 0000: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0000 form" << std::endl;

  // IHDR
  {
    unsigned int u1;
    std::string name;
    total += readIHDR( file, dbgStr, u1, name );
  }

  // DATA
  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected record of type DATA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;

  unsigned int u1;
  total += base::read( file, u1 );
  std::cout << dbgStr << u1 << " ";

  total += base::read( file, u1 );
  std::cout << u1 << " ";

  float u2;
  total += base::read( file, u2 );
  std::cout << u2 << std::endl;
    
  if( aenvSize == total )
    {
      std::cout << debugString << "Finished reading AENV" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading AENV" << std::endl;
      std::cout << "Read " << total << " out of " << aenvSize
		<< std::endl;
    }
  return total;
}


// Boundary Rectangle.
unsigned int trn::readBREC( std::istream &file,
			    const std::string &debugString,
			    brec &newBREC )
{
  std::string dbgStr = debugString + "BREC: ";
  unsigned int brecSize;
  unsigned int total = readFormHeader( file, "BREC", brecSize );
  brecSize += 8;
  std::cout << debugString << "Found BREC form" << std::endl;

  unsigned int size;
  std::string form, type;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0003" )
    {
      std::cout << "Expected Form of type 0003: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0003 form" << std::endl;

  {
    unsigned int u1;
    std::string name;
    total += readIHDR( file, dbgStr, u1, name );
  }

  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected record of type DATA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;

  total += base::read( file, newBREC.x1 );
  total += base::read( file, newBREC.y1 );
  total += base::read( file, newBREC.x2 );
  total += base::read( file, newBREC.y2 );

  std::cout << dbgStr << newBREC.x1 << ", "
	    << newBREC.y1 << "..."
	    << newBREC.x2 << ", "
	    << newBREC.y2 << std::endl;

  total += base::read( file, newBREC.u1 );
  std::cout << dbgStr << newBREC.u1 << std::endl;

  total += base::read( file, newBREC.u2 );
  std::cout << dbgStr << newBREC.u2 << std::endl;

  total += base::read( file, newBREC.u3 );
  std::cout << dbgStr << newBREC.u3 << std::endl;

  total += base::read( file, newBREC.u4 );
  std::cout << dbgStr << newBREC.u4 << std::endl;

  total += base::read( file, newBREC.u5 );
  std::cout << dbgStr << newBREC.u5 << std::endl;

  total += base::read( file, newBREC.u6 );
  std::cout << dbgStr << newBREC.u6 << std::endl;

  total += base::read( file, newBREC.name );
  std::cout << dbgStr << "'" << newBREC.name << "'" << std::endl;

  if( brecSize == total )
    {
      std::cout << debugString << "Finished reading BREC" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading BREC" << std::endl;
      std::cout << "Read " << total << " out of " << brecSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readAHFR( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "AHFR: ";
  unsigned int ahfrSize;
  unsigned int total = readFormHeader( file, "AHFR", ahfrSize );
  ahfrSize += 8;
  std::cout << debugString << "Found AHFR form" << std::endl;

  std::string form, type;
  unsigned int size;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0003" )
    {
      std::cout << "Expected Form of type 0003: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0003 form" << std::endl;

  {
    unsigned int u1;
    std::string name;
    total += readIHDR( file, dbgStr, u1, name );
  }

  total += readFormHeader( file, "DATA", size );
  std::cout << dbgStr << "Found DATA form" << std::endl;

  // PARM record
  total += readRecordHeader( file, type, size );
  if( type != "PARM" )
    {
      std::cout << "Expected PARM record: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found PARM record" << std::endl;

  unsigned int u1;
  total += base::read( file, u1 );
  std::cout << dbgStr << u1 << " ";

  total += base::read( file, u1 );
  std::cout << u1 << " ";

  float u2;
  total += base::read( file, u2 );
  std::cout << u2 << std::endl;

  if( ahfrSize == total )
    {
      std::cout << debugString << "Finished reading AHFR" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading AHFR" << std::endl;
      std::cout << "Read " << total << " out of " << ahfrSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readFFRA( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "FFRA: ";
  unsigned int ffraSize;
  unsigned int total = readFormHeader( file, "FFRA", ffraSize );
  ffraSize += 8;
  std::cout << debugString << "Found FFRA form" << std::endl;

  unsigned int size;
  std::string form, type;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0005" )
    {
      std::cout << "Expected Form of type 0005: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0005 form" << std::endl;

  {
    unsigned int u1;
    std::string name;
    total += readIHDR( file, dbgStr, u1, name );
  }

  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "DATA" )
    {
      std::cout << "Expected Form of type DATA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA form" << std::endl;

  // PARM record
  total += readRecordHeader( file, type, size );
  if( type != "PARM" )
    {
      std::cout << "Expected PARM record: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found PARM record" << std::endl;

  unsigned int u1;
  total += base::read( file, u1 );
  std::cout << dbgStr << u1 << std::endl;

  total += base::read( file, u1 );
  std::cout << dbgStr << u1 << std::endl;;

  float u2;
  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;;

  total += base::read( file, u1 );
  std::cout << dbgStr << u2 << std::endl;;

  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;;

  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;;

  if( ffraSize == total )
    {
      std::cout << debugString << "Finished reading FFRA" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading FFRA" << std::endl;
      std::cout << "Read " << total << " out of " << ffraSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readACCN( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "ACCN: ";
  unsigned int accnSize;
  unsigned int total = readFormHeader( file, "ACCN", accnSize );
  accnSize += 8;
  std::cout << debugString << "Found ACCN form" << std::endl;

  unsigned int size;
  std::string form, type;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0000" )
    {
      std::cout << "Expected Form of type 0000: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0000 form" << std::endl;

  {
    unsigned int u1;
    std::string name;
    total += readIHDR( file, dbgStr, u1, name );
  }

  // DATA record
  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected DATA record: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;

  unsigned int u1;
  total += base::read( file, u1 );
  std::cout << dbgStr << u1 << std::endl;;

  char u2;
  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;;

  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;;

  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;;

  if( accnSize == total )
    {
      std::cout << debugString << "Finished reading ACCN" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading ACCN" << std::endl;
      std::cout << "Read " << total << " out of " << accnSize
		<< std::endl;
    }
  return total;
}

// Boundary Polyline.
unsigned int trn::readBPLN( std::istream &file,
			    const std::string &debugString,
			    bpln &newBPLN )
{
  std::string dbgStr = debugString + "BPLN: ";
  unsigned int bplnSize;
  unsigned int total = readFormHeader( file, "BPLN", bplnSize );
  bplnSize += 8;
  std::cout << debugString << "Found BPLN form" << std::endl;
  
  unsigned int size;
  std::string form, type;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0001" )
    {
      std::cout << "Expected Form of type 0001: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0001 form" << std::endl;
  
  {
    unsigned int u1;
    std::string name;
    total += readIHDR( file, dbgStr, u1, name );
  }
  
  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected record of type DATA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;
  
  unsigned int num;
  total += base::read( file, num );
  std::cout << dbgStr << num << std::endl;

  float x, y;
  for( unsigned int i = 0; i < num; ++i )
    {
      total += base::read( file, x );
      total += base::read( file, y );
      newBPLN.x.push_back( x );
      newBPLN.y.push_back( y );
      
      std::cout << dbgStr << x << ", " << y << std::endl;
    }

  total += base::read( file, newBPLN.u1 );
  std::cout << dbgStr << newBPLN.u1 << std::endl;

  total += base::read( file, newBPLN.u2 );
  std::cout << dbgStr << newBPLN.u2 << std::endl;

  total += base::read( file, newBPLN.u3 );
  std::cout << dbgStr << newBPLN.u3 << std::endl;

  if( bplnSize == total )
    {
      std::cout << debugString << "Finished reading BPLN" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading BPLN" << std::endl;
      std::cout << "Read " << total << " out of " << bplnSize
		<< std::endl;
    }
  return total;
}

// Boundary Polygon.
unsigned int trn::readBPOL( std::istream &file,
			    const std::string &debugString,
			    bpol &newBPOL )
{
  std::string dbgStr = debugString + "BPOL: ";
  unsigned int bpolSize;
  unsigned int total = readFormHeader( file, "BPOL", bpolSize );
  bpolSize += 8;
  std::cout << debugString << "Found BPOL form" << std::endl;

  unsigned int size;
  std::string form, type;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0005" )
    {
      std::cout << "Expected Form of type 0005: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0005 form" << std::endl;

  {
    unsigned int u1;
    std::string name;
    total += readIHDR( file, dbgStr, u1, name );
  }

  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected record of type DATA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;

  unsigned int num;
  total += base::read( file, num );
  std::cout << dbgStr << num << std::endl;

  float x, y;
  for( unsigned int i = 0; i < num; ++i )
    {
      total += base::read( file, x );
      total += base::read( file, y );
      newBPOL.x.push_back( x );
      newBPOL.y.push_back( y );

      std::cout << dbgStr << x << ", " << y << std::endl;
    }

  total += base::read( file, newBPOL.u1 );
  std::cout << dbgStr << newBPOL.u1 << std::endl;

  total += base::read( file, newBPOL.altitude );
  std::cout << dbgStr << "Altitude: " << newBPOL.altitude << std::endl;

  total += base::read( file, newBPOL.u3 );
  std::cout << dbgStr << newBPOL.u3 << std::endl;

  total += base::read( file, newBPOL.u4 );
  std::cout << dbgStr << newBPOL.u4 << std::endl;

  total += base::read( file, newBPOL.u5 );
  std::cout << dbgStr << newBPOL.u5 << std::endl;

  total += base::read( file, newBPOL.name );
  std::cout << dbgStr << "'" << newBPOL.name << "'" << std::endl;

  if( bpolSize == total )
    {
      std::cout << debugString << "Finished reading BPOL" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading BPOL" << std::endl;
      std::cout << "Read " << total << " out of " << bpolSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readASRP( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "ASRP: ";
  unsigned int asrpSize;
  unsigned int total = readFormHeader( file, "ASRP", asrpSize );
  asrpSize += 8;
  std::cout << debugString << "Found ASRP form" << std::endl;

  total += readUnknown( file, asrpSize-total );

  if( asrpSize == total )
    {
      std::cout << debugString << "Finished reading ASRP" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading ASRP" << std::endl;
      std::cout << "Read " << total << " out of " << asrpSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readAEXC( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "AEXC: ";
  unsigned int aexcSize;
  unsigned int total = readFormHeader( file, "AEXC", aexcSize );
  aexcSize += 8;
  std::cout << debugString << "Found AEXC form" << std::endl;

  total += readUnknown( file, aexcSize-total );

  if( aexcSize == total )
    {
      std::cout << debugString << "Finished reading AEXC" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading AEXC" << std::endl;
      std::cout << "Read " << total << " out of " << aexcSize
		<< std::endl;
    }
  return total;
}

// Boundary Circle.
unsigned int trn::readBCIR( std::istream &file,
			    const std::string &debugString,
			    bcir &newBCIR )
{
  std::string dbgStr = debugString + "BCIR: ";
  unsigned int bcirSize;
  unsigned int total = readFormHeader( file, "BCIR", bcirSize );
  bcirSize += 8;
  std::cout << debugString << "Found BCIR form" << std::endl;

  unsigned int size;
  std::string form, type;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0002" )
    {
      std::cout << "Expected Form of type 0002: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0002 form" << std::endl;

  {
    unsigned int u1;
    std::string name;
    total += readIHDR( file, dbgStr, u1, name );
  }

  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected record of type DATA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;

  total += base::read( file, newBCIR.x );
  total += base::read( file, newBCIR.y );
  std::cout << dbgStr << newBCIR.x << ", " << newBCIR.y << std::endl;

  total += base::read( file, newBCIR.radius );
  std::cout << dbgStr << newBCIR.radius << std::endl;

  total += base::read( file, newBCIR.u1 );
  std::cout << dbgStr << newBCIR.u1 << std::endl;

  total += base::read( file, newBCIR.u2 );
  std::cout << dbgStr << newBCIR.u2 << std::endl;

  if( bcirSize == total )
    {
      std::cout << debugString << "Finished reading BCIR" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading BCIR" << std::endl;
      std::cout << "Read " << total << " out of " << bcirSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readAHCN( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "AHCN: ";
  unsigned int ahcnSize;
  unsigned int total = readFormHeader( file, "AHCN", ahcnSize );
  ahcnSize += 8;
  std::cout << debugString << "Found AHCN form" << std::endl;

  unsigned int size;
  std::string form, type;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0000" )
    {
      std::cout << "Expected Form of type 0000: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0000 form" << std::endl;

  {
    unsigned int u1;
    std::string name;
    total += readIHDR( file, dbgStr, u1, name );
  }

  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected record of type DATA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;

  unsigned int u1;
  total += base::read( file, u1 );
  std::cout << dbgStr << u1 << std::endl;

  float u2;
  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;

  if( ahcnSize == total )
    {
      std::cout << debugString << "Finished reading AHCN" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading AHCN" << std::endl;
      std::cout << "Read " << total << " out of " << ahcnSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readFHGT( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "FHGT: ";
  std::string form;
  unsigned int fhgtSize;
  std::string type;

  unsigned int total = readFormHeader( file, form, fhgtSize, type );
  fhgtSize += 8;
  if( form != "FORM" || type != "FHGT" )
    {
      std::cout << "Expected Form of type FHGT: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found FHGT form" << std::endl;

  unsigned int size;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0002" )
    {
      std::cout << "Expected Form of type 0002: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0002 form" << std::endl;

  {
    unsigned int u1;
    std::string name;
    total += readIHDR( file, dbgStr, u1, name );
  }

  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected record of type DATA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;

  float u1;
  file.read( (char *)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << dbgStr << u1 << std::endl;

  file.read( (char *)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << dbgStr << u1 << std::endl;

  unsigned int u2;
  file.read( (char *)&u2, sizeof( u2 ) );
  total += sizeof( u2 );
  std::cout << dbgStr << u2 << std::endl;

  file.read( (char *)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << dbgStr << u1 << std::endl;

  if( fhgtSize == total )
    {
      std::cout << debugString << "Finished reading FHGT" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading FHGT" << std::endl;
      std::cout << "Read " << total << " out of " << fhgtSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readAHTR( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "AHTR: ";
  unsigned int ahtrSize;
  unsigned int total = readFormHeader( file, "AHTR", ahtrSize );
  ahtrSize += 8;
  std::cout << debugString << "Found AHTR form" << std::endl;

  unsigned int size;
  std::string form, type;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0004" )
    {
      std::cout << "Expected Form of type 0004: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0004 form" << std::endl;

  {
    unsigned int u1;
    std::string name;
    total += readIHDR( file, dbgStr, u1, name );
  }

  // DATA record
  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected DATA record: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;

  float u1;
  file.read( (char*)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << dbgStr << u1 << std::endl;;

  file.read( (char*)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << dbgStr << u1 << std::endl;;

  if( ahtrSize == total )
    {
      std::cout << debugString << "Finished reading AHTR" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading AHTR" << std::endl;
      std::cout << "Read " << total << " out of " << ahtrSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readACRF( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "ACRF: ";
  unsigned int acrfSize;
  unsigned int total = readFormHeader( file, "ACRF", acrfSize );
  acrfSize += 8;
  std::cout << debugString << "Found ACRF form" << std::endl;

  unsigned int size;
  total += readFormHeader( file, "0001", size );
  std::cout << dbgStr << "Found 0001 form" << std::endl;

  {
    unsigned int u1;
    std::string name;
    total += readIHDR( file, dbgStr, u1, name );
  }

  total += readFormHeader( file, "DATA", size );
  std::cout << dbgStr << "Found DATA form" << std::endl;

  // PARM record
  std::string type;
  total += readRecordHeader( file, type, size );
  if( type != "PARM" )
    {
      std::cout << "Expected PARM record: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found PARM record" << std::endl;

  unsigned int u1;
  total += base::read( file, u1 );
  std::cout << dbgStr << u1 << " ";

  total += base::read( file, u1 );
  std::cout << u1 << " ";

  std::string name;
  total += base::read( file, name );
  base::fixSlash( name );
  std::cout << "'" << name << "'" << std::endl;

  if( acrfSize == total )
    {
      std::cout << debugString << "Finished reading ACRF" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading ACRF" << std::endl;
      std::cout << "Read " << total << " out of " << acrfSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readFSLP( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "FSLP: ";
  unsigned int fslpSize;
  unsigned int total = readFormHeader( file, "FSLP", fslpSize );
  fslpSize += 8;
  std::cout << dbgStr << "Found FSLP form" << std::endl;

  unsigned int size;
  std::string form, type;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0002" )
    {
      std::cout << "Expected Form of type 0002: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0002 form" << std::endl;

  {
    unsigned int u1;
    std::string name;
    total += readIHDR( file, dbgStr, u1, name );
  }

  // DATA record
  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected DATA record: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;

  float u1;
  total += base::read( file, u1 );
  std::cout << dbgStr << u1 << std::endl;;

  total += base::read( file, u1 );
  std::cout << dbgStr << u1 << std::endl;;

  unsigned int u2;
  total += base::read( file, u2 );
  std::cout << dbgStr << u2 << std::endl;;

  total += base::read( file, u1 );
  std::cout << dbgStr << u1 << std::endl;;

  if( fslpSize == total )
    {
      std::cout << debugString << "Finished reading FSLP" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading FSLP" << std::endl;
      std::cout << "Read " << total << " out of " << fslpSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readAFSC( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "AFSC: ";
  unsigned int afscSize;
  unsigned int total = readFormHeader( file, "AFSC", afscSize );
  afscSize += 8;
  std::cout << debugString << "Found AFSC form" << std::endl;

  unsigned int size;
  std::string form, type;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0004" )
    {
      std::cout << "Expected Form of type 0004: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0004 form" << std::endl;

  {
    unsigned int u1;
    std::string name;
    total += readIHDR( file, dbgStr, u1, name );
  }

  // DATA record
  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected DATA record: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;

  unsigned int u1;
  file.read( (char*)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << dbgStr << u1 << std::endl;;

  file.read( (char*)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << dbgStr << u1 << std::endl;;

  file.read( (char*)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << dbgStr << u1 << std::endl;;

  file.read( (char*)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << dbgStr << u1 << std::endl;;

  float u2;
  file.read( (char*)&u2, sizeof( u2 ) );
  total += sizeof( u2 );
  std::cout << dbgStr << u2 << std::endl;;

  if( afscSize == total )
    {
      std::cout << debugString << "Finished reading AFSC" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading AFSC" << std::endl;
      std::cout << "Read " << total << " out of " << afscSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readFSHD( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "FSHD: ";
  unsigned int total = 0;

  std::string form;
  unsigned int fshdSize;
  std::string type;

  total += readFormHeader( file, form, fshdSize, type );
  fshdSize += 8;
  if( form != "FORM" || type != "FSHD" )
    {
      std::cout << "Expected Form of type FSHD: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found FSHD form" << std::endl;

  total += readUnknown( file, fshdSize-total );

  if( fshdSize == total )
    {
      std::cout << debugString << "Finished reading FSHD" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading FSHD" << std::endl;
      std::cout << "Read " << total << " out of " << fshdSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readAFDN( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "AFDN: ";
  std::string form;
  unsigned int afdnSize;
  std::string type;

  unsigned int total = readFormHeader( file, form, afdnSize, type );
  afdnSize += 8;
  if( form != "FORM" || type != "AFDN" )
    {
      std::cout << "Expected Form of type AFDN: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found AFDN form" << std::endl;

  unsigned int size;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0002" )
    {
      std::cout << "Expected Form of type 0002: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found 0002 form" << std::endl;

  {
    unsigned int u1;
    std::string name;
    total += readIHDR( file, dbgStr, u1, name );
  }

  // DATA record
  total += readRecordHeader( file, type, size );
  if( type != "DATA" )
    {
      std::cout << "Expected DATA record: " << type << std::endl;
      exit( 0 );
    }
  std::cout << dbgStr << "Found DATA record" << std::endl;

  unsigned int u1;
  file.read( (char*)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << dbgStr << u1 << " ";

  file.read( (char*)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << u1 << " ";

  file.read( (char*)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << u1 << " ";

  file.read( (char*)&u1, sizeof( u1 ) );
  total += sizeof( u1 );
  std::cout << u1 << " ";

  float u2;
  file.read( (char*)&u2, sizeof( u2 ) );
  total += sizeof( u2 );
  std::cout << u2 << std::endl;;

  if( afdnSize == total )
    {
      std::cout << debugString << "Finished reading AFDN" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading AFDN" << std::endl;
      std::cout << "Read " << total << " out of " << afdnSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readAFSN( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "AFSN: ";
  std::string form;
  unsigned int afsnSize;
  std::string type;

  unsigned int total = readFormHeader( file, form, afsnSize, type );
  afsnSize += 8;
  if( form != "FORM" || type != "AFSN" )
    {
      std::cout << "Expected Form of type AFSN: " << type << std::endl;
      exit( 0 );
    }
  std::cout << debugString << "Found AFSN form" << std::endl;

  total += readUnknown( file, afsnSize-total );

  if( afsnSize == total )
    {
      std::cout << debugString << "Finished reading AFSN" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading AFSN" << std::endl;
      std::cout << "Read " << total << " out of " << afsnSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readAFDF( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "AFDF: ";
  unsigned int total = 0;

  std::string form;
  unsigned int afdfSize;
  std::string type;

  total += readFormHeader( file, form, afdfSize, type );
  afdfSize += 8;
  if( form != "FORM" || type != "AFDF" )
    {
      std::cout << "Expected Form of type AFDF: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found AFDF form" << std::endl;

  total += readUnknown( file, afdfSize-total );

  if( afdfSize == total )
    {
      std::cout << debugString << "Finished reading AFDF" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading AFDF" << std::endl;
      std::cout << "Read " << total << " out of " << afdfSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readACRH( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "ACRH: ";
  unsigned int total = 0;

  std::string form;
  unsigned int acrhSize;
  std::string type;

  total += readFormHeader( file, form, acrhSize, type );
  acrhSize += 8;
  if( form != "FORM" || type != "ACRH" )
    {
      std::cout << "Expected Form of type ACRH: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found ACRH form" << std::endl;

  total += readUnknown( file, acrhSize-total );

  if( acrhSize == total )
    {
      std::cout << debugString << "Finished reading ACRH" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading ACRH" << std::endl;
      std::cout << "Read " << total << " out of " << acrhSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readAROA( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "AROA: ";
  unsigned int total = 0;

  std::string form;
  unsigned int aroaSize;
  std::string type;

  total += readFormHeader( file, form, aroaSize, type );
  aroaSize += 8;
  if( form != "FORM" || type != "AROA" )
    {
      std::cout << "Expected Form of type AROA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found AROA form" << std::endl;

  total += readUnknown( file, aroaSize-total );

  if( aroaSize == total )
    {
      std::cout << debugString << "Finished reading AROA" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading AROA" << std::endl;
      std::cout << "Read " << total << " out of " << aroaSize
		<< std::endl;
    }
  return total;
}

unsigned int trn::readFDIR( std::istream &file, const std::string &debugString )
{
  std::string dbgStr = debugString + "FDIR: ";
  unsigned int total = 0;

  std::string form;
  unsigned int fdirSize;
  std::string type;

  total += readFormHeader( file, form, fdirSize, type );
  fdirSize += 8;
  if( form != "FORM" || type != "FDIR" )
    {
      std::cout << "Expected Form of type FDIR: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found FDIR form" << std::endl;

  total += readUnknown( file, fdirSize-total );

  if( fdirSize == total )
    {
      std::cout << debugString << "Finished reading FDIR" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading FDIR" << std::endl;
      std::cout << "Read " << total << " out of " << fdirSize
		<< std::endl;
    }
  return total;
}

