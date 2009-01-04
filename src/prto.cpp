/** -*-c++-*-
 *  \class  prto
 *  \file   prto.cpp
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
#include <meshLib/prto.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

prto::prto()
{
}

prto::~prto()
{
}

unsigned int prto::readPRTO( std::istream &file, std::string path )
{
  basePath = path;
  unsigned int total = 0;
  std::string form;
  unsigned int prtoSize;
  std::string type;

  total += readFormHeader( file, form, prtoSize, type );
  prtoSize += 8;
  if( form != "FORM" || type != "PRTO" )
    {
      std::cout << "Expected Form of type PRTO: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found PRTO form"
	    << ": " << prtoSize-12 << " bytes"
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

  total += readDATA( file );
  total += readPRTS( file );
  total += readCELS( file );
  total += readPGRF( file );
  total += readCRC( file );

  if( prtoSize == total )
    {
      std::cout << "Finished reading PRTO" << std::endl;
    }
  else
    {
      std::cout << "FAILED in reading PRTO" << std::endl;
      std::cout << "Read " << total << " out of " << prtoSize
		<< std::endl;
    }
    
  return total;
}

unsigned int prto::readDATA( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int dataSize;
    std::string type;

    total += readRecordHeader( file, type, dataSize );
    dataSize += 8;
    if( type != "DATA" )
    {
        std::cout << "Expected record of type DATA: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type 
	      << ": "
	      << dataSize-8 << " bytes"
	      << std::endl;

    file.read( (char*)&numPortals, sizeof( numPortals ) );
    total += sizeof( numPortals );
    file.read( (char*)&numCells, sizeof( numCells ) );
    total += sizeof( numCells );

    std::cout << "Num portals: " << numPortals << std::endl;
    std::cout << "Num cells: " << numCells << std::endl;

    if( dataSize == total )
    {
	std::cout << "Finished reading DATA" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading DATA" << std::endl;
	std::cout << "Read " << total << " out of " << dataSize
                  << std::endl;
    }

    return total;
}

unsigned int prto::readLGHT( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int lghtSize;
    std::string type;

    total += readRecordHeader( file, type, lghtSize );
    lghtSize += 8;
    if( type != "LGHT" )
    {
        std::cout << "Expected record of type LGHT: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type 
	      << ": "
	      << lghtSize-8 << " bytes"
	      << std::endl;

    unsigned int numLights;
    file.read( (char*)&numLights, sizeof( numLights ) );
    total += sizeof( numLights );
    std::cout << "Num lights: " << numLights << std::endl;


    ml::matrix3 mat;
    ml::vector3 vec;

    unsigned char x;
    float z;
    for( unsigned int i = 0; i < numLights; ++i )
      {
	std::cout << "Light: " << i << std::endl;
	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << (unsigned int)x << std::endl;
	
	//
	file.read( (char*)&z, sizeof( z ) );
	total += sizeof( z );
	std::cout << z << " ";
	
	file.read( (char*)&z, sizeof( z ) );
	total += sizeof( z );
	std::cout << z << " ";
	
	file.read( (char*)&z, sizeof( z ) );
	total += sizeof( z );
	std::cout << z << " ";
	
	file.read( (char*)&z, sizeof( z ) );
	total += sizeof( z );
	std::cout << z << std::endl;

	//
	file.read( (char*)&z, sizeof( z ) );
	total += sizeof( z );
	std::cout << z << " ";
	
	file.read( (char*)&z, sizeof( z ) );
	total += sizeof( z );
	std::cout << z << " ";
	
	file.read( (char*)&z, sizeof( z ) );
	total += sizeof( z );
	std::cout << z << " ";
	
	file.read( (char*)&z, sizeof( z ) );
	total += sizeof( z );
	std::cout << z << std::endl;

	total += base::readMatrixAndPosition( file, mat, vec );
	std::cout << "Matrix: " << std::endl;
	mat.print();

	std::cout << "Position: ";
	vec.print();
	

	file.read( (char*)&z, sizeof( z ) );
	total += sizeof( z );
	std::cout << z << " ";
	
	file.read( (char*)&z, sizeof( z ) );
	total += sizeof( z );
	std::cout << z << " ";
	
	file.read( (char*)&z, sizeof( z ) );
	total += sizeof( z );
	std::cout << z << std::endl;
      }


    if( lghtSize == total )
    {
	std::cout << "Finished reading LGHT" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading LGHT" << std::endl;
	std::cout << "Read " << total << " out of " << lghtSize
                  << std::endl;
    }

    return total;
}

unsigned int prto::readPRTS( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int prtsSize;
    std::string type;

    total += readFormHeader( file, form, prtsSize, type );
    prtsSize += 8;
    if( form != "FORM" || type != "PRTS" )
    {
	std::cout << "Expected Form of type PRTS: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << prtsSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    unsigned int position;
    for( unsigned int i = 0; i < numPortals; ++i )
      {
	// Peek at next record, but keep file at same place.
        position = file.tellg();
        readFormHeader( file, form, size, type );
        file.seekg( position, std::ios_base::beg );

	if( "PRTL" == form )
	  {
	    std::cout << std::endl;
	    std::cout << "PRTL " << i << std::endl;

	    total += readPRTLRecord( file );
	  }
	else if( "IDTL" == type )
	  {
	    std::vector<vector3> vec;
	    std::vector<unsigned int> index;
	    total += readIDTL( file, vec, index );
	  }
      }

    if( prtsSize == total )
    {
	std::cout << "Finished reading PRTS" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading PRTS" << std::endl;
	std::cout << "Read " << total << " out of " << prtsSize
                  << std::endl;
    }

    return total;
}

unsigned int prto::readCELS( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int celsSize;
    std::string type;

    total += readFormHeader( file, form, celsSize, type );
    celsSize += 8;
    if( form != "FORM" || type != "CELS" )
    {
	std::cout << "Expected Form of type CELS: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << celsSize-12 << " bytes"
	      << std::endl;

    for( unsigned int i = 0; i < numCells; ++i )
      {
	std::cout << std::endl;
	std::cout << "CELL " << i << std::endl;
	total += readCELL( file );
      }

    if( celsSize == total )
    {
	std::cout << "Finished reading CELS" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading CELS" << std::endl;
	std::cout << "Read " << total << " out of " << celsSize
                  << std::endl;
    }

    return total;
}

unsigned int prto::readPRTLRecord( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int prtlSize;
    std::string type;

    total += readRecordHeader( file, type, prtlSize );
    prtlSize += 8;
    if( type != "PRTL" )
    {
        std::cout << "Expected record of type PRTL: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type 
	      << ": "
	      << prtlSize-8 << " bytes"
	      << std::endl;

    unsigned int numVerts;
    file.read( (char*)&numVerts, sizeof( numVerts ) );
    total += sizeof( numVerts );
    std::cout << "NumVerts: " << numVerts << std::endl;

    float x, y, z;
    for( unsigned int i = 0; i < numVerts; ++i )
      {
	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	
	file.read( (char*)&y, sizeof( y ) );
	total += sizeof( y );
	
	file.read( (char*)&z, sizeof( z ) );
	total += sizeof( z );
#if 0
	std::cout << "vert: " << x << ", "
		  << y << ", " << z
		  << std::endl;
#endif
      }

    if( prtlSize == total )
    {
	std::cout << "Finished reading PRTL" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading PRTL" << std::endl;
	std::cout << "Read " << total << " out of " << prtlSize
                  << std::endl;
    }

    return total;
}

unsigned int prto::readCELL( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int cellSize;
    std::string type;

    total += readFormHeader( file, form, cellSize, type );
    cellSize += 8;
    if( form != "FORM" || type != "CELL" )
    {
	std::cout << "Expected Form of type CELL: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << cellSize-12 << " bytes"
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

    unsigned int numCellPortals;
    total += readCELLDATA( file, numCellPortals );

    unsigned int position;
    while( total < cellSize )
      {
	// Peek at next record, but keep file at same place.
        position = file.tellg();
        readFormHeader( file, form, size, type );
        file.seekg( position, std::ios_base::beg );

	if( "CMSH" == type )
	  {
	    total += readCMSH( file );
	  }
	else if( "CMPT" == type )
	  {
	    total += readCMPT( file );
	  }
	else if( "NULL" == type )
	  {
	    total += readFormHeader( file, form, size, type );
	  }
	else if( "PRTL" == type )
	  {
	    ml::matrix3 mat;
	    ml::vector3 vec;
	    total += readPRTL( file, mat, vec );
	  }
	else if( "EXBX" == type )
	  {
	    float cx, cy, cz, radius;
	    float x1, y1, z1;
	    float x2, y2, z2;
	    total += readEXBX( file,
			       cx, cy, cz, radius,
			       x1, y1, z1,
			       x2, y2, z2
			       );
	  }
	else if( "LGHT" == form )
	  {
	    total += readLGHT( file );
	  }
	else
	  {
	    std::cout << "Unexpected type: " << type << std::endl;
	    exit( 0 );
	  }
      }
 
    if( cellSize == total )
    {
	std::cout << "Finished reading CELL" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading CELL" << std::endl;
	std::cout << "Read " << total << " out of " << cellSize
                  << std::endl;
    }

    return total;
}

unsigned int prto::readCMPT( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int cmptSize;
    std::string type;

    total += readFormHeader( file, form, cmptSize, type );
    cmptSize += 8;
    if( form != "FORM" || type != "CMPT" )
    {
	std::cout << "Expected Form of type CMPT: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << cmptSize-12 << " bytes"
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

    total += readCPST( file );

    if( cmptSize == total )
    {
	std::cout << "Finished reading CMPT" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading CMPT" << std::endl;
	std::cout << "Read " << total << " out of " << cmptSize
                  << std::endl;
    }

    return total;
}

unsigned int prto::readCPST( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int cpstSize;
    std::string type;

    total += readFormHeader( file, form, cpstSize, type );
    cpstSize += 8;
    if( form != "FORM" || type != "CPST" )
    {
	std::cout << "Expected Form of type CPST: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << cpstSize-12 << " bytes"
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

    while( total < cpstSize )
      {
	// Peek at next record, but keep file at same place.
        long position = file.tellg();
        readFormHeader( file, form, size, type );
        file.seekg( position, std::ios_base::beg );

	if( "CMSH" == type )
	  {
	    total += readCMSH( file );
	  }
	else if( "EXBX" == type )
	  {
	    float cx, cy, cz, radius;
	    float x1, y1, z1;
	    float x2, y2, z2;
	    total += readEXBX( file,
			       cx, cy, cz, radius,
			       x1, y1, z1,
			       x2, y2, z2
			       );
	  }
	else
	  {
	    std::cout << "Unexpected type: " << type << std::endl;
	    exit( 0 );
	  }
      }

    if( cpstSize == total )
    {
	std::cout << "Finished reading CPST" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading CPST" << std::endl;
	std::cout << "Read " << total << " out of " << cpstSize
                  << std::endl;
    }

    return total;
}

unsigned int prto::readCMSH( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int cmshSize;
    std::string type;

    total += readFormHeader( file, form, cmshSize, type );
    cmshSize += 8;
    if( form != "FORM" || type != "CMSH" )
    {
	std::cout << "Expected Form of type CMSH: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << cmshSize-12 << " bytes"
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

    std::vector<vector3> vec;
    std::vector<unsigned int> index;

    total += readIDTL( file, vec, index );
 
    if( cmshSize == total )
    {
	std::cout << "Finished reading CMSH" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading CMSH" << std::endl;
	std::cout << "Read " << total << " out of " << cmshSize
                  << std::endl;
    }

    return total;
}

unsigned int prto::readCELLDATA( std::istream &file,
				 unsigned int &numCellPortals )
{
    unsigned int total = 0;

    std::string form;
    unsigned int dataSize;
    std::string type;

    total += readRecordHeader( file, type, dataSize );
    dataSize += 8;
    if( type != "DATA" )
    {
        std::cout << "Expected record of type DATA: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type 
	      << ": "
	      << dataSize-8 << " bytes"
	      << std::endl;

    ml::cell newCell;

    file.read( (char*)&numCellPortals, sizeof( numCellPortals ) );
    total += sizeof( numCellPortals );
    std::cout << "Num portals in this cell: " << numCellPortals << std::endl;

    unsigned char u1;
    file.read( (char*)&u1, sizeof( u1 ) );
    newCell.setUnknown1( u1 );
    total += sizeof( u1 );
    std::cout << "???: " << (unsigned int)u1 << std::endl;

    char temp[255];
    std::string cellName;
    file.getline( temp, 255, 0 );
    cellName = temp;
    newCell.setName( cellName );
    total += cellName.size() + 1;
    std::cout << "Cell name: " << cellName << std::endl;

    std::string cellModel;
    file.getline( temp, 255, 0 );
    cellModel = temp;
    newCell.setModelFilename( cellModel );
    total += cellModel.size() + 1;
    std::cout << "Cell model: " << cellModel << std::endl;

    unsigned char hasFloor;
    file.read( (char*)&hasFloor, sizeof( hasFloor ) );
    newCell.setHasFloor( 0 < hasFloor );
    total += sizeof( hasFloor );
    std::cout << "Has floor: " << (unsigned int)hasFloor << std::endl;

    if( hasFloor > 0 )
      {
	std::string cellFloor;
	file.getline( temp, 255, 0 );
	cellFloor = temp;
	newCell.setFloorFilename( cellFloor );
	total += cellFloor.size() + 1;
	std::cout << "Cell floor: " << cellFloor << std::endl;
      }
    
    if( dataSize == total )
      {
	cells.push_back( newCell );
	std::cout << "Finished reading DATA" << std::endl;
      }
    else
      {
	std::cout << "FAILED in reading DATA" << std::endl;
	std::cout << "Read " << total << " out of " << dataSize
                  << std::endl;
      }
    
    return total;
}

unsigned int prto::readPRTL( std::istream &file,
			     ml::matrix3 &mat, 
			     ml::vector3 &vec )
{
    unsigned int total = 0;

    std::string form;
    unsigned int prtlSize;
    std::string type;

    total += readFormHeader( file, form, prtlSize, type );
    prtlSize += 8;
    if( form != "FORM" || type != "PRTL" )
    {
	std::cout << "Expected Form of type PRTL: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << prtlSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readRecordHeader( file, type, size );
    if( type != "0004" && type != "0005" )
    {
        std::cout << "Expected record of type 0004: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type
	      << ": " << size << " bytes"
	      << std::endl;

    //float w;
    //unsigned short x;
    unsigned char y;
    unsigned int z;

    file.read( (char *)&y, sizeof( y ) );
    total += sizeof( y );
    std::cout << (unsigned int)y << std::endl;
    
    file.read( (char *)&z, sizeof( z ) );
    total += sizeof( z );
    std::cout << "Portal geometry index?: " << z << std::endl;
    
    file.read( (char *)&y, sizeof( y ) );
    total += sizeof( y );
    std::cout << (unsigned int)y << std::endl;
    
    file.read( (char *)&z, sizeof( z ) );
    total += sizeof( z );
    std::cout << "Adjacent cell: " << z << std::endl;
    
    char temp[255];
    std::string name;
    file.getline( temp, size, 0 );
    name = temp;
    std::cout << "'" << name << "'" << std::endl;
    total += name.size()+1;

    file.read( (char *)&y, sizeof( y ) );
    total += sizeof( y );
    std::cout << (unsigned int)y << std::endl;

    total += base::readMatrixAndPosition( file, mat, vec );
    std::cout << "Matrix: " << std::endl;
    mat.print();
    
    std::cout << "Position: ";
    vec.print();
	
    if( prtlSize == total )
    {
	std::cout << "Finished reading PRTL" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading PRTL" << std::endl;
	std::cout << "Read " << total << " out of " << prtlSize
                  << std::endl;
    }

    return total;
}

unsigned int prto::readPGRF( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int pgrfSize;
    std::string type;

    total += readFormHeader( file, form, pgrfSize, type );
    pgrfSize += 8;
    if( form != "FORM" || type != "PGRF" )
    {
	std::cout << "Expected Form of type PGRF: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << pgrfSize-12 << " bytes"
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

    total += readMETA( file );
    total += readPNOD( file );
    total += readPEDG( file );
    total += readECNT( file );
    total += readESTR( file );

    if( pgrfSize == total )
    {
	std::cout << "Finished reading PGRF" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading PGRF" << std::endl;
	std::cout << "Read " << total << " out of " << pgrfSize
                  << std::endl;
    }

    return total;
}

unsigned int prto::readCRC( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int crcSize;
    std::string type;

    total += readRecordHeader( file, type, crcSize );
    crcSize += 8;
    if( type != "CRC " )
    {
        std::cout << "Expected record of type CRC: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type 
	      << ": "
	      << crcSize-8 << " bytes"
	      << std::endl;

    
    file.read( (char*)&crc, sizeof( crc ) );
    total += sizeof( crc );
    std::cout << "CRC: 0x" << std::hex << crc
	      << std::dec << std::endl;

    if( crcSize == total )
    {
	std::cout << "Finished reading CRC" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading CRC" << std::endl;
	std::cout << "Read " << total << " out of " << crcSize
                  << std::endl;
    }

    return total;
}

unsigned int prto::readMETA( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int metaSize;
    std::string type;

    total += readRecordHeader( file, type, metaSize );
    metaSize += 8;
    if( type != "META" )
    {
        std::cout << "Expected record of type META: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type 
	      << ": "
	      << metaSize-8 << " bytes"
	      << std::endl;

    unsigned int x;
    file.read( (char*)&x, sizeof( x ) );
    total += sizeof( x );
    std::cout << "X: " << x << std::endl;

    if( metaSize == total )
    {
	std::cout << "Finished reading META" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading META" << std::endl;
	std::cout << "Read " << total << " out of " << metaSize
                  << std::endl;
    }

    return total;
}

unsigned int prto::readPNOD( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int pnodSize;
    std::string type;

    total += readRecordHeader( file, type, pnodSize );
    pnodSize += 8;
    if( type != "PNOD" )
    {
        std::cout << "Expected record of type PNOD: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type 
	      << ": "
	      << pnodSize-8 << " bytes"
	      << std::endl;

    unsigned int num;
    file.read( (char*)&num, sizeof( num ) );
    total += sizeof( num );
    std::cout << "Num: " << num << std::endl;

    unsigned int x;
    float y;
    for( unsigned int i = 0; i < num; ++i )
      {
	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << "Portal number: " << x << std::endl;

	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << std::hex << "0x" << x << std::dec << std::endl;

	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << "???: " << x << std::endl;

	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << "???: " << x << std::endl;

	file.read( (char*)&y, sizeof( y ) );
	total += sizeof( y );
	std::cout << y << std::endl;

	file.read( (char*)&y, sizeof( y ) );
	total += sizeof( y );
	std::cout << y << std::endl;

	file.read( (char*)&y, sizeof( y ) );
	total += sizeof( y );
	std::cout << y << std::endl;

	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << std::endl;
      }

    if( pnodSize == total )
    {
	std::cout << "Finished reading PNOD" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading PNOD" << std::endl;
	std::cout << "Read " << total << " out of " << pnodSize
                  << std::endl;
    }

    return total;
}

unsigned int prto::readPEDG( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int pedgSize;
    std::string type;

    total += readRecordHeader( file, type, pedgSize );
    pedgSize += 8;
    if( type != "PEDG" )
    {
        std::cout << "Expected record of type PEDG: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type 
	      << ": "
	      << pedgSize-8 << " bytes"
	      << std::endl;

    unsigned int num;
    file.read( (char*)&num, sizeof( num ) );
    total += sizeof( num );
    std::cout << "Num: " << num << std::endl;

    unsigned int x;
    for( unsigned int i = 0; i < num; ++i )
      {
	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << " ";

	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << " ";

	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << " ";

	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << std::endl;
      }

    if( pedgSize == total )
    {
	std::cout << "Finished reading PEDG" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading PEDG" << std::endl;
	std::cout << "Read " << total << " out of " << pedgSize
                  << std::endl;
    }

    return total;
}

unsigned int prto::readECNT( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int ecntSize;
    std::string type;

    total += readRecordHeader( file, type, ecntSize );
    ecntSize += 8;
    if( type != "ECNT" )
    {
        std::cout << "Expected record of type ECNT: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type 
	      << ": "
	      << ecntSize-8 << " bytes"
	      << std::endl;

    unsigned int num;
    file.read( (char*)&num, sizeof( num ) );
    total += sizeof( num );
    std::cout << "Num: " << num << std::endl;

    unsigned int x;
    for( unsigned int i = 0; i < num; ++i )
      {
	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << "Entry points for cell "
		  << i << ": " << x << std::endl;
      }

    if( ecntSize == total )
    {
	std::cout << "Finished reading ECNT" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading ECNT" << std::endl;
	std::cout << "Read " << total << " out of " << ecntSize
                  << std::endl;
    }

    return total;
}

unsigned int prto::readESTR( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int estrSize;
    std::string type;

    total += readRecordHeader( file, type, estrSize );
    estrSize += 8;
    if( type != "ESTR" )
    {
        std::cout << "Expected record of type ESTR: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found record " << type 
	      << ": "
	      << estrSize-8 << " bytes"
	      << std::endl;

    unsigned int num;
    file.read( (char*)&num, sizeof( num ) );
    total += sizeof( num );
    std::cout << "Num: " << num << std::endl;

    unsigned int x;
    for( unsigned int i = 0; i < num; ++i )
      {
	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << std::endl;
      }

    if( estrSize == total )
    {
	std::cout << "Finished reading ESTR" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading ESTR" << std::endl;
	std::cout << "Read " << total << " out of " << estrSize
                  << std::endl;
    }

    return total;
}



