/** -*-c++-*-
 *  \class  cmp
 *  \file   cmp.cpp
 *  \author Ken Sewell

 swgLib is used for the parsing and exporting SWG models.
 Copyright (C) 2006-2021 Ken Sewell

 This file is part of swgLib.

 swgLib is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 swgLib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with swgLib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <swgLib/cmp.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

cmp::cmp()
  : x1( -1.0 ),
    y1( -1.0 ),
    z1( -1.0 ),
    x2(  1.0 ),
    y2(  1.0 ),
    z2(  1.0 )
{
}

cmp::~cmp()
{
}

bool cmp::getPart( unsigned int index,
		   std::string &filename,
		   vector3 &partPosition,
		   matrix3x4 &partScaleRotate
		   ) const
{
  if( index >= getNumParts() )
    {
      return false;
    }

  filename = partFilenames[index];

  //partPosition = position[index];

  partScaleRotate = scaleRotate[index];

  return true;
}

std::size_t cmp::readCMP( std::istream &file, std::string path )
{
  basePath = path;
  std::size_t total = 0;
  std::string form;
  std::size_t cmpaSize;
  std::string type;

  total += readFormHeader( file, form, cmpaSize, type );
  cmpaSize += 8;
  if( form != "FORM" || type != "CMPA" )
    {
      std::cout << "Expected Form of type CMPA: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found CMPA form" << std::endl;

  std::size_t size;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" )
    {
      std::cout << "Expected Form " << std::endl;
      exit( 0 );
    }
  std::cout << "Found form of type: " << type << std::endl;

  while( total < cmpaSize )
    {
      // Peek at next record, but keep file at same place.
      base::peekHeader( file, form, size, type );
	
      if( form == "FORM" )
	{
	  if( type == "APPR" )
	    {
	      total += model::readAPPR( file );
	    }
	  else if( type == "RADR" )
	    {
	      total += readRADR( file );
	    }
	  else
	    {
	      std::cout << "Unexpected FORM: " << type << std::endl;
	      exit( 0 );
	    }
	}
      else if( form == "PART" )
	{
	  total += readPART( file );
	}
      else
	{
	  std::cout << "Unexpected record: " << form << std::endl;
	  exit( 0 );
	}
    }

  if( cmpaSize == total )
    {
      std::cout << "Finished reading CMPA" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading CMPA" << std::endl;
      std::cout << "Read " << total << " out of " << cmpaSize
		<< std::endl;
    }

  return total;
}

std::size_t cmp::readPART( std::istream &file )
{
    std::size_t total = 0;

    std::size_t size;
    std::string type;
    total += readRecordHeader( file, type, size );
    size += 8;
    if( type != "PART" )
    {
	std::cout << "Expected record of type PART: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found PART record" << std::endl;

    std::string partFilename;
    total += base::read( file, partFilename );

    std::string filename("appearance/");
    filename += partFilename;
    std::cout << "Part: " << filename << std::endl;
    
    partFilenames.push_back( filename );

    // Read scale/rotate matrix and position vector
    //vector3 vec;
    matrix3x4 matrix;
    total += model::readTransformMatrix( file, matrix );

    //position.push_back( vec );
    scaleRotate.push_back( matrix );

    //std::cout << "Position: ";
    //vec.print();

    std::cout << "Matrix: \n" << matrix << std::endl;

    if( size == total )
    {
        std::cout << "Finished reading PART" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading PART" << std::endl;
        std::cout << "Read " << total << " out of " << size
                  << std::endl;
     }

    return total;
}

std::size_t cmp::readRADR( std::istream &file )
{
    std::size_t total = 0;

    std::string form;
    std::size_t radrSize;
    std::string type;

    total += readFormHeader( file, form, radrSize, type );
    radrSize += 8;
    if( form != "FORM" || type != "RADR" )
    {
	std::cout << "Expected Form of type RADR: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found RADR form" << std::endl;

    std::size_t size;
    total += readRecordHeader( file, type, size );
    if( type != "INFO" )
    {
	std::cout << "Expected record of type INFO: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found INFO record" << std::endl;

    unsigned int numNodes;
    total += base::read( file, numNodes );
    std::cout << "Num nodes: " << numNodes << std::endl;

    if( numNodes > 0 )
      {
	total += model::readIDTL( file, radrVert, radrIndex );
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

