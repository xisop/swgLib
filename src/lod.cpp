/** -*-c++-*-
 *  \class  lod
 *  \file   lod.cpp
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

#include <meshLib/lod.hpp>
#include <meshLib/msh.hpp>
#include <meshLib/cshd.hpp>
#include <meshLib/sht.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

lod::lod()
  : x1( -1.0 ),
    y1( -1.0 ),
    z1( -1.0 ),
    x2(  1.0 ),
    y2(  1.0 ),
    z2(  1.0 )
{
}

lod::~lod()
{
}

bool lod::getChild( unsigned int index, 
		    std::string &filename,
		    float &Near, float &Far ) const
{
  if( index > getNumLODs()-1 )
    {
      return false;
    }

  Near = near[index];
  Far = far[index];
  filename = childFilename[index];

  return true;
}

unsigned int lod::readLOD( std::istream &file, std::string path )
{
  basePath = path;
  std::string form;
  unsigned int dtlaSize;
  std::string type;

  unsigned int total = readFormHeader( file, form, dtlaSize, type );
  dtlaSize += 8;
  if( form != "FORM" || type != "DTLA" )
    {
      std::cout << "Expected Form of type DTLA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found DTLA form" << std::endl;

  unsigned int size;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" )
    {
      std::cout << "Expected Form " << std::endl;
      exit( 0 );
    }
  std::cout << "Found form of type: " << type << std::endl;

  while( total < dtlaSize )
    {
      // Peek at next record, but keep file at same place.
      base::peekHeader( file, form, size, type );
	
      if( form == "FORM" )
	{
	  if( type == "APPR" )
	    {
	      total += base::readAPPR( file, cx, cy, cz, radius,
				       x1, y1, z1, x2, y2, z2 );
	    }
	  else if( type == "DATA" )
	    {
	      total += readChildren( file );
	    }
	  else if( type == "RADR" )
	    {
	      total += readRADR( file );
	    }
	  else if( type == "TEST" )
	    {
	      total += readTEST( file );
	    }
	  else if( type == "WRIT" )
	    {
	      total += readWRIT( file );
	    }
	  else
	    {
	      std::cout << "Unexpected FORM: " << type << std::endl;
	      exit( 0 );
	    }
	}
      else if( form == "PIVT" )
	{
	  total += readPIVT( file );
	}
      else if( form == "INFO" )
	{
	  total += readINFO( file );
	}
      else
	{
	  std::cout << "Unexpected record: " << form << std::endl;
	  exit( 0 );
	}
    }

  if( dtlaSize == total )
    {
      std::cout << "Finished reading DTLA" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading DTLA" << std::endl;
      std::cout << "Read " << total << " out of " << dtlaSize
		<< std::endl;
    }

  return total;
}

unsigned int lod::readPIVT( std::istream &file )
{
    unsigned int size;
    std::string type;

    unsigned int total = readRecordHeader( file, type, size );
    if( type != "PIVT" )
    {
	std::cout << "Expected record of type PIVT: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found PIVT record" << std::endl;

    total += readUnknown( file, size );
    
    return total;
}

unsigned int lod::readINFO( std::istream &file )
{
    unsigned int size;
    std::string type;

    unsigned int total = readRecordHeader( file, type, size );
    if( type != "INFO" )
    {
	std::cout << "Expected record of type INFO: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found INFO record" << std::endl;

    unsigned int num = size/12;

    for( unsigned int i=0; i < num; ++i )
    {
	unsigned int childNo;
	file.read( (char*)&childNo, sizeof( childNo ) );
	total += sizeof( childNo );

	float n;
	file.read( (char*)&n, sizeof( n ) );
	total += sizeof( n );
	near.push_back( n );

	float f;
	file.read( (char*)&f, sizeof( f ) );
	total += sizeof( f );
	far.push_back( f );

	std::cout << childNo << ": "
		  << near[far.size()-1] << "..."
		  << far[far.size()-1]
		  << std::endl;
    }
    
    return total;
}

unsigned int lod::readCHLD( std::istream &file )
{
    unsigned int size;
    std::string type;

    unsigned int total = readRecordHeader( file, type, size );
    size += 8;
    if( type != "CHLD" )
    {
	std::cout << "Expected record of type CHLD: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found CHLD record" << std::endl;

    unsigned int childNumber;
    file.read( (char *)&childNumber, sizeof( childNumber ) );
    total += sizeof( childNumber );

    char temp[255];
    file.getline( temp, 255, 0 );
    std::string tempFilename( temp );
    total += tempFilename.size() + 1;

    // Some files already have appearance/ in them
    if( tempFilename.substr( 0, 11 ) != "appearance/" )
      {
	std::string filename = "appearance/";
	filename += tempFilename;
	std::cout << childNumber << ": " << filename << std::endl;
	childFilename.push_back( filename );
      }
    else
      {
	childFilename.push_back( tempFilename );
      }

    if( size == total )
    {
        std::cout << "Finished reading CHLD" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading CHLD" << std::endl;
        std::cout << "Read " << total << " out of " << size
                  << std::endl;
     }

    return total;
}

unsigned int lod::readChildren( std::istream &file )
{
    std::string form;
    unsigned int size;
    std::string type;

    unsigned int total = readFormHeader( file, form, size, type );
    if( form != "FORM" || type != "DATA" )
    {
	std::cout << "Expected Form of type DATA: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found DATA form" << std::endl;

    while( total < size-4 )
    {
	total += readCHLD( file );
    }

    return total;
}

unsigned int lod::readRADR( std::istream &file )
{
    std::string form;
    unsigned int radrSize;
    std::string type;

    unsigned int total = readFormHeader( file, form, radrSize, type );
    radrSize += 8;
    if( form != "FORM" || type != "RADR" )
    {
	std::cout << "Expected Form of type RADR: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found RADR form" << std::endl;

    unsigned int size;
    total += readRecordHeader( file, type, size );
    if( type != "INFO" )
    {
	std::cout << "Expected record of type INFO: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found INFO record" << std::endl;

    unsigned int numNodes;
    file.read( (char *)&numNodes, sizeof( numNodes ) );
    total += sizeof( numNodes );

    std::cout << "Num nodes: " << numNodes << std::endl;

    // Need to loop here and read IDTL (and others?)
    while( total < radrSize )
      {
	peekHeader( file, form, size, type );
	if( "FORM" == form )
	  {
	    total += readUnknown( file, size );
	  }
	else
	  {
	    total += readUnknown( file, size );
	  }
      }

    if( radrSize == total )
    {
	std::cout << "Finished reading RADR" << std::endl;
    }
    else
    {
	std::cout << "Failed in reading RADR" << std::endl;
        std::cout << "Read " << total << " out of " << radrSize
                  << std::endl;
    }

    return total;
}

unsigned int lod::readTEST( std::istream &file )
{
    std::string form;
    unsigned int testSize;
    std::string type;

    unsigned int total = readFormHeader( file, form, testSize, type );
    testSize += 8;
    if( form != "FORM" || type != "TEST" )
    {
	std::cout << "Expected Form of type TEST: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found TEST form" << std::endl;

    unsigned int size;
    total += readRecordHeader( file, type, size );
    if( type != "INFO" )
    {
	std::cout << "Expected record of type INFO: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found INFO record" << std::endl;
    unsigned int numNodes;
    file.read( (char *)&numNodes, sizeof( numNodes ) );
    total += sizeof( numNodes );

    std::cout << "Num nodes: " << numNodes << std::endl;

    // Need to loop here and read IDTL (and others?)
    while( total < testSize )
      {
	peekHeader( file, form, size, type );
	if( "FORM" == form )
	  {
	    if( "IDTL" == type )
	      {
		std::vector<vector3> vec;
		std::vector<unsigned int> index;

		total += base::readIDTL( file, vec, index );
	      }
	    else
	      {
		total += readUnknown( file, size );
	      }
	  }
	else
	  {
	    total += readUnknown( file, size );
	  }
      }

    if( testSize == total )
    {
	std::cout << "Finished reading TEST" << std::endl;
    }
    else
    {
	std::cout << "Failed in reading TEST" << std::endl;
        std::cout << "Read " << total << " out of " << testSize
                  << std::endl;
    }

    return total;
}

unsigned int lod::readWRIT( std::istream &file )
{
    std::string form;
    unsigned int writSize;
    std::string type;

    unsigned int total = readFormHeader( file, form, writSize, type );
    writSize += 8;
    if( form != "FORM" || type != "WRIT" )
    {
	std::cout << "Expected Form of type WRIT: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found WRIT form" << std::endl;

    unsigned int size;
    total += readRecordHeader( file, type, size );
    if( type != "INFO" )
    {
	std::cout << "Expected record of type INFO: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found INFO record" << std::endl;

    unsigned int numNodes;
    file.read( (char *)&numNodes, sizeof( numNodes ) );
    total += sizeof( numNodes );

    std::cout << "Num nodes: " << numNodes << std::endl;

    // Need to loop here and read IDTL (and others?)
    while( total < writSize )
      {
	peekHeader( file, form, size, type );
	if( "FORM" == form )
	  {
	    total += readUnknown( file, size );
	  }
	else
	  {
	    total += readUnknown( file, size );
	  }
      }

    if( writSize == total )
    {
	std::cout << "Finished reading WRIT" << std::endl;
    }
    else
    {
	std::cout << "Failed in reading WRIT" << std::endl;
        std::cout << "Read " << total << " out of " << writSize
                  << std::endl;
    }

    return total;
}

