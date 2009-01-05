/** -*-c++-*-
 *  \class  skmg
 *  \file   skmg.cpp
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
#include <meshLib/skmg.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

skmg::skmg()
{
}

skmg::~skmg()
{
}

unsigned int skmg::readSKMG( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int skmgSize;
    std::string type;

    total += readFormHeader( file, form, skmgSize, type );
    skmgSize += 8;
    if( form != "FORM" || type != "SKMG" )
    {
	std::cout << "Expected Form of type SKMG: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found SKMG form" << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" )
    {
	std::cout << "Expected Form" << std::endl;
	exit( 0 );
    }
    std::cout << "Found form of type: " << type<< std::endl;

    total += readINFO( file );

    total += readSKTM( file );

    unsigned int position;
    while( total < skmgSize )
      {
	// Peek at next record, but keep file at same place.
        position = file.tellg();
        readFormHeader( file, form, size, type );
        file.seekg( position, std::ios_base::beg );

	if( form == "FORM" )
	  {
	    if( type == "PSDT" )
	      {
		total += readPSDT( file );
	      }
	    else if( type == "BLTS" )
	      {
		total += readBLTS( file );
	      }
	    else
	      {
		std::cout << "Unexpected form: " << type << std::endl;
		exit( 0 );
	      }
	  }
	else if( form == "XFNM" )
	  {
	    total += readXFNM( file );
	  }
	else if( form == "POSN" )
	  {
	    total += readPOSN( file );
	  }
	else if( form == "TWHD" )
	  {
	    total += readTWHD( file );
	  }
	else if( form == "TWDT" )
	  {
	    total += readTWDT( file );
	  }
	else if( form == "NORM" )
	  {
	    total += readNORM( file );
	  }
	else if( form == "DOT3" )
	  {
	    total += readDOT3( file );
	  }
	else if( form == "OZN " )
	  {
	    total += readOZN( file );
	  }
	else if( form == "FOZC" )
	  {
	    total += readFOZC( file );
	  }
	else if( form == "OZC " )
	  {
	    total += readOZC( file );
	  }
	else if( form == "ZTO " )
	  {
	    total += readZTO( file );
	  }
	else
	  {
	    std::cout << "Unexpected record: " << form << std::endl;
	    exit( 0 );
	  }
	
      }

    if( skmgSize == total )
    {
	std::cout << "Finished reading SKMG" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading SKMG" << std::endl;
	std::cout << "Read " << total << " out of " << skmgSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readPSDT( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int psdtSize;
    std::string type;

    total += readFormHeader( file, form, psdtSize, type );
    psdtSize += 8;
    if( form != "FORM" || type != "PSDT" )
    {
	std::cout << "Expected Form of type PSDT: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found PSDT form" << std::endl;

    unsigned int size;
    unsigned int position;
    while( total < psdtSize )
      {
	// Peek at next record, but keep file at same place.
        position = file.tellg();
        readFormHeader( file, form, size, type );
        file.seekg( position, std::ios_base::beg );

	if( form == "FORM" )
	  {
	    if( type == "TCSF" )
	      {
		total += readTCSF( file );
	      }
	    else if( type == "PRIM" )
	      {
		total += readPRIM( file );
	      }
	    else
	      {
		std::cout << "Unexpected form: " << type << std::endl;
		exit( 0 );
	      }
	  }
	else if( form == "NAME" )
	  {
	    total += readNAME( file );
	  }
	else if( form == "PIDX" )
	  {
	    total += readPIDX( file );
	  }
	else if( form == "NIDX" )
	  {
	    total += readNIDX( file );
	  }
	else if( form == "TXCI" )
	  {
	    total += readTXCI( file );
	  }
	else if( form == "DOT3" )
	  {
	    total += readDOT3Index( file );
	  }
	else
	  {
	    std::cout << "Unexpected record: " << form << std::endl;
	    exit( 0 );
	  }
	
      }

    if( psdtSize == total )
    {
	std::cout << "Finished reading PSDT" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading PSDT" << std::endl;
	std::cout << "Read " << total << " out of " << psdtSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readBLTS( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int bltsSize;
    std::string type;

    total += readFormHeader( file, form, bltsSize, type );
    bltsSize += 8;
    if( form != "FORM" || type != "BLTS" )
    {
	std::cout << "Expected Form of type BLTS: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found BLTS form" << std::endl;

    std::cout << "Num BLT: " << numBLT << std::endl;
    for( unsigned int i = 0; i < numBLT; ++i )
      {
	total += readBLT( file );
      }

    if( bltsSize == total )
    {
	std::cout << "Finished reading BLTS" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading BLTS" << std::endl;
	std::cout << "Read " << total << " out of " << bltsSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readBLT( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int bltSize;
    std::string type;

    total += readFormHeader( file, form, bltSize, type );
    bltSize += 8;
    if( form != "FORM" || type != "BLT " )
    {
	std::cout << "Expected Form of type BLT: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found BLT form" << std::endl;

    unsigned int numBLTPos;
    unsigned int numBLTNorm;
    total += readBLTINFO( file, numBLTPos, numBLTNorm );

#if 1
    total += readUnknown( file, bltSize-total );
#else
    file.seekg( bltSize-total, std::ios_base::cur );
    total = bltSize;
#endif

#if 0
    total += readPOSN( file,  );
    total += readNORM( file );
    total += readDOT3( file );
#endif
    if( bltSize == total )
    {
	std::cout << "Finished reading BLT" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading BLT" << std::endl;
	std::cout << "Read " << total << " out of " << bltSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readOZN( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int oznSize;
    total += readRecordHeader( file, type, oznSize );
    oznSize += 8;
    if( type != "OZN " )
    {
        std::cout << "Expected record of type OZN: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    char temp[255];
    std::string name;
    while( total < oznSize )
      {
	file.getline( temp, 255, 0 );
	name = temp;
	total += name.size() + 1;
	std::cout << name << std::endl;
      }

    if( oznSize == total )
    {
        std::cout << "Finished reading OZN" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading OZN" << std::endl;
        std::cout << "Read " << total << " out of " << oznSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readFOZC( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int fozcSize;
    total += readRecordHeader( file, type, fozcSize );
    fozcSize += 8;
    if( type != "FOZC" )
    {
        std::cout << "Expected record of type FOZC: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    unsigned short num;
    file.read( (char*)&num, sizeof( num ) );
    total += sizeof( num );
    std::cout << "Num: " << num << std::endl;

    unsigned short x;
    for( unsigned int i = 0; i < num; ++i )
      {
	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << std::endl;
      }

    if( fozcSize == total )
    {
        std::cout << "Finished reading FOZC" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading FOZC" << std::endl;
        std::cout << "Read " << total << " out of " << fozcSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readOZC( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int ozcSize;
    total += readRecordHeader( file, type, ozcSize );
    ozcSize += 8;
    if( type != "OZC " )
    {
        std::cout << "Expected record of type OZC: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    unsigned short x;
    while( total < ozcSize )
      {
	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << " ";

	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << std::endl;
      }

    if( ozcSize == total )
    {
        std::cout << "Finished reading OZC" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading OZC" << std::endl;
        std::cout << "Read " << total << " out of " << ozcSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readZTO( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int ztoSize;
    total += readRecordHeader( file, type, ztoSize );
    ztoSize += 8;
    if( type != "ZTO " )
    {
        std::cout << "Expected record of type ZTO: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    unsigned short x;
    while( total < ztoSize )
      {
	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << std::endl;
      }

    if( ztoSize == total )
    {
        std::cout << "Finished reading ZTO" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading ZTO" << std::endl;
        std::cout << "Read " << total << " out of " << ztoSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readSKTM( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int sktmSize;
    total += readRecordHeader( file, type, sktmSize );
    sktmSize += 8;
    if( type != "SKTM" )
    {
        std::cout << "Expected record of type SKTM: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    char temp[255];
    file.getline( temp, 255, 0 );
    skeletonFilename = temp;
    total += skeletonFilename.size() + 1;
    std::cout << skeletonFilename << std::endl;

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

unsigned int skmg::readTXCI( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int txciSize;
    total += readRecordHeader( file, type, txciSize );
    txciSize += 8;
    if( type != "TXCI" )
    {
        std::cout << "Expected record of type TXCI: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    unsigned int x;

    file.read( (char*)&x, sizeof( x ) );
    total += sizeof( x );
    std::cout << x << std::endl;

    file.read( (char*)&x, sizeof( x ) );
    total += sizeof( x );
    std::cout << x << std::endl;

    if( txciSize == total )
    {
        std::cout << "Finished reading TXCI" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading TXCI" << std::endl;
        std::cout << "Read " << total << " out of " << txciSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readTCSF( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int tcsfSize;
    std::string type;

    total += readFormHeader( file, form, tcsfSize, type );
    tcsfSize += 8;
    if( form != "FORM" || type != "TCSF" )
    {
	std::cout << "Expected Form of type TCSF: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found TCSF form" << std::endl;

    total += readTCSD( file );

    if( tcsfSize == total )
    {
	std::cout << "Finished reading TCSF" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading TCSF" << std::endl;
	std::cout << "Read " << total << " out of " << tcsfSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readPRIM( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int primSize;
    std::string type;

    total += readFormHeader( file, form, primSize, type );
    primSize += 8;
    if( form != "FORM" || type != "PRIM" )
    {
	std::cout << "Expected Form of type PRIM: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found PRIM form" << std::endl;

    total += readPRIMINFO( file );

    unsigned int size;
    unsigned int position;
    while( total < primSize )
      {
	// Peek at next record, but keep file at same place.
        position = file.tellg();
        readFormHeader( file, form, size, type );
        file.seekg( position, std::ios_base::beg );

	if( form == "FORM" )
	  {
	    std::cout << "Unexpected form: " << type << std::endl;
	    exit( 0 );
	  }
	else if( form == "ITL " )
	  {
	    total += readITL( file );
	  }
	else if( form == "OITL" )
	  {
	    total += readOITL( file );
	  }
	else
	  {
	    std::cout << "Unexpected record: " << form << std::endl;
	    exit( 0 );
	  }
	
      }

    if( primSize == total )
    {
	std::cout << "Finished reading PRIM" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading PRIM" << std::endl;
	std::cout << "Read " << total << " out of " << primSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readTCSD( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int tcsdSize;
    total += readRecordHeader( file, type, tcsdSize );
    tcsdSize += 8;
    if( type != "TCSD" )
    {
        std::cout << "Expected record of type TCSD: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    std::cout << "Num TCSD: " << numIndex << std::endl;
    float x;
    for( unsigned int i = 0; i < numIndex; ++i )
      {
	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << ", ";
	
	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << std::endl;
      }

    if( tcsdSize == total )
    {
        std::cout << "Finished reading TCSD" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading TCSD" << std::endl;
        std::cout << "Read " << total << " out of " << tcsdSize
                  << std::endl;
     }

    return total;
}

// Index into Position/Normal/Texture index arrays.
unsigned int skmg::readITL( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int itlSize;
    total += readRecordHeader( file, type, itlSize );
    itlSize += 8;
    if( type != "ITL " )
    {
        std::cout << "Expected record of type ITL: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    unsigned int numITL;
    file.read( (char*)&numITL, sizeof( numITL ) );
    total += sizeof( numITL );
    std::cout << "Num ITL: " << numITL << std::endl;;

    unsigned int x;
    for( unsigned int i = 0; i < numITL; ++i )
      {
	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << ", ";
	
	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << ", ";

	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << std::endl;
      }

    if( itlSize == total )
    {
        std::cout << "Finished reading ITL" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading ITL" << std::endl;
        std::cout << "Read " << total << " out of " << itlSize
                  << std::endl;
     }

    return total;
}

// Index into Position/Normal/Texture index arrays.
unsigned int skmg::readOITL( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int oitlSize;
    total += readRecordHeader( file, type, oitlSize );
    oitlSize += 8;
    if( type != "OITL" )
    {
        std::cout << "Expected record of type OITL: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    unsigned int numOITL;
    file.read( (char*)&numOITL, sizeof( numOITL ) );
    total += sizeof( numOITL );
    std::cout << "Num OITL: " << numOITL << std::endl;;

    unsigned int x;
    unsigned short y;
    for( unsigned int i = 0; i < numOITL; ++i )
      {
	file.read( (char*)&y, sizeof( y ) );
	total += sizeof( y );
	std::cout << y << " ";

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

    if( oitlSize == total )
    {
        std::cout << "Finished reading OITL" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading OITL" << std::endl;
        std::cout << "Read " << total << " out of " << oitlSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readINFO( std::istream &file )
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

    unsigned int u1;
    unsigned int u2;
    unsigned int u3;
    unsigned int u7;
    unsigned int u8;
    unsigned int u10;

    file.read( (char*)&u1, sizeof( u1 ) );
    total += sizeof( u1 );
    std::cout << u1 << std::endl;

    file.read( (char*)&u2, sizeof( u2 ) );
    total += sizeof( u2 );
    std::cout << u2 << std::endl;

    file.read( (char*)&u3, sizeof( u3 ) );
    total += sizeof( u3 );
    std::cout << u3 << std::endl;

    file.read( (char*)&numBones, sizeof( numBones ) );
    total += sizeof( numBones );
    std::cout << numBones << std::endl;

    file.read( (char*)&numPoints, sizeof( numPoints ) );
    total += sizeof( numPoints );
    std::cout << numPoints << std::endl;

    file.read( (char*)&numTwdt, sizeof( numTwdt ) );
    total += sizeof( numTwdt );
    std::cout << numTwdt << std::endl;

    file.read( (char*)&numNorm, sizeof( numNorm ) );
    total += sizeof( numNorm );
    std::cout << numNorm << std::endl;

    file.read( (char*)&u7, sizeof( u7 ) );
    total += sizeof( u7 );
    std::cout << u7 << std::endl;

    file.read( (char*)&numBLT, sizeof( numBLT ) );
    total += sizeof( numBLT );
    std::cout << numBLT << std::endl;

    file.read( (char*)&u8, sizeof( u8 ) );
    total += sizeof( u8 );
    std::cout << u8 << std::endl;

    // Something to do with OITL
    file.read( (char*)&u10, sizeof( u10 ) );
    total += sizeof( u10 );
    std::cout << u10 << std::endl;

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

unsigned int skmg::readPRIMINFO( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int priminfoSize;
    total += readRecordHeader( file, type, priminfoSize );
    priminfoSize += 8;
    if( type != "INFO" )
    {
        std::cout << "Expected record of type PRIMINFO: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    unsigned int u1;
    file.read( (char*)&u1, sizeof( u1 ) );
    total += sizeof( u1 );
    std::cout << u1 << std::endl;

    if( priminfoSize == total )
    {
        std::cout << "Finished reading PRIMINFO" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading PRIMINFO" << std::endl;
        std::cout << "Read " << total << " out of " << priminfoSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readBLTINFO( std::istream &file,
				unsigned int &numBLTPos,
				unsigned int &numBLTNorm
				)
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int bltinfoSize;
    total += readRecordHeader( file, type, bltinfoSize );
    bltinfoSize += 8;
    if( type != "INFO" )
    {
        std::cout << "Expected record of type BLTINFO: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    file.read( (char*)&numBLTPos, sizeof( numBLTPos ) );
    total += sizeof( numBLTPos );
    std::cout << "Num points: " << numBLTPos << std::endl;

    file.read( (char*)&numBLTNorm, sizeof( numBLTNorm ) );
    total += sizeof( numBLTNorm );
    std::cout << "Num normals: " << numBLTNorm << std::endl;

    char temp[255];
    std::string blendName;
    file.getline( temp, 255, 0 );
    blendName = temp;
    total += blendName.size() + 1;
    std::cout << blendName << std::endl;

    if( bltinfoSize == total )
    {
        std::cout << "Finished reading BLTINFO" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading BLTINFO" << std::endl;
        std::cout << "Read " << total << " out of " << bltinfoSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readPOSN( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int posnSize;
    total += readRecordHeader( file, type, posnSize );
    posnSize += 8;
    if( type != "POSN" )
    {
        std::cout << "Expected record of type POSN: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    std::cout << "Num points: " << numPoints << std::endl;
    float tempX, tempY, tempZ;
    for( unsigned int i = 0; i < numPoints; ++i )
      {
	file.read( (char*)&tempX, sizeof( tempX ) );
	total += sizeof( tempX );
	file.read( (char*)&tempY, sizeof( tempY ) );
	total += sizeof( tempY );
	file.read( (char*)&tempZ, sizeof( tempZ ) );
	total += sizeof( tempZ );

	x.push_back( tempX );
	y.push_back( tempY );
	z.push_back( tempZ );
      }

    if( posnSize == total )
    {
        std::cout << "Finished reading POSN" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading POSN" << std::endl;
        std::cout << "Read " << total << " out of " << posnSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readNORM( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int normSize;
    total += readRecordHeader( file, type, normSize );
    normSize += 8;
    if( type != "NORM" )
    {
        std::cout << "Expected record of type NORM: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    std::cout << "Num Norm:  " << numNorm << std::endl;
    float tempX, tempY, tempZ;
    for( unsigned int i = 0; i < numNorm; ++i )
      {
	file.read( (char*)&tempX, sizeof( tempX ) );
	total += sizeof( tempX );
	file.read( (char*)&tempY, sizeof( tempY ) );
	total += sizeof( tempY );
	file.read( (char*)&tempZ, sizeof( tempZ ) );
	total += sizeof( tempZ );
	//std::cout << tempX << ", " << tempY << ", " << tempZ << std::endl;

	nx.push_back( tempX );
	ny.push_back( tempY );
	nz.push_back( tempZ );
      }

    if( normSize == total )
    {
        std::cout << "Finished reading NORM" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading NORM" << std::endl;
        std::cout << "Read " << total << " out of " << normSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readDOT3( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int dot3Size;
    total += readRecordHeader( file, type, dot3Size );
    dot3Size += 8;
    if( type != "DOT3" )
    {
        std::cout << "Expected record of type DOT3: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    
    file.read( (char*)&numDot3, sizeof( numDot3 ) );
    total += sizeof( numDot3 );

    std::cout << "Num DOT3: " << numDot3 << std::endl;
    float tempX;
    for( unsigned int i = 0; i < numDot3; ++i )
      {
	file.read( (char*)&tempX, sizeof( tempX ) );
	total += sizeof( tempX );
	std::cout << tempX << " ";

	file.read( (char*)&tempX, sizeof( tempX ) );
	total += sizeof( tempX );
	std::cout << tempX << " ";

	file.read( (char*)&tempX, sizeof( tempX ) );
	total += sizeof( tempX );
	std::cout << tempX << " ";

	file.read( (char*)&tempX, sizeof( tempX ) );
	total += sizeof( tempX );
	std::cout << tempX << std::endl;;
      }

    if( dot3Size == total )
    {
        std::cout << "Finished reading DOT3" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading DOT3" << std::endl;
        std::cout << "Read " << total << " out of " << dot3Size
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readDOT3Index( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int dot3Size;
    total += readRecordHeader( file, type, dot3Size );
    dot3Size += 8;
    if( type != "DOT3" )
    {
        std::cout << "Expected record of type DOT3: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    unsigned int index;
    while( total < dot3Size )
      {
	file.read( (char*)&index, sizeof( index ) );
	total += sizeof( index );
	std::cout << index << std::endl;
      }

    if( dot3Size == total )
    {
        std::cout << "Finished reading DOT3" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading DOT3" << std::endl;
        std::cout << "Read " << total << " out of " << dot3Size
                  << std::endl;
     }

    return total;
}

// One entry for each point.  Each record is number of bones to which
// the point belongs.
unsigned int skmg::readTWHD( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int twhdSize;
    total += readRecordHeader( file, type, twhdSize );
    twhdSize += 8;
    if( type != "TWHD" )
    {
        std::cout << "Expected record of type TWHD: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    std::cout << "Num points: " << numPoints << std::endl;
    unsigned int x;
    for( unsigned int i = 0; i < numPoints; ++i )
      {
	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << std::endl;
      }

    if( twhdSize == total )
    {
        std::cout << "Finished reading TWHD" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading TWHD" << std::endl;
        std::cout << "Read " << total << " out of " << twhdSize
                  << std::endl;
     }

    return total;
}

// Each entry is a bone and weight factor
unsigned int skmg::readTWDT( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int twdtSize;
    total += readRecordHeader( file, type, twdtSize );
    twdtSize += 8;
    if( type != "TWDT" )
    {
        std::cout << "Expected record of type TWDT: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    std::cout << "Num TWDT:  " << numTwdt << std::endl;
    unsigned int x;
    float y;
    for( unsigned int i = 0; i < numTwdt; ++i )
      {
	file.read( (char*)&x, sizeof( x ) );
	total += sizeof( x );
	std::cout << x << " ";

	file.read( (char*)&y, sizeof( y ) );
	total += sizeof( y );
	std::cout << std::fixed << y << std::endl;
      }

    if( twdtSize == total )
    {
        std::cout << "Finished reading TWDT" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading TWDT" << std::endl;
        std::cout << "Read " << total << " out of " << twdtSize
                  << std::endl;
     }

    return total;
}

// Index into points list
unsigned int skmg::readPIDX( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int pidxSize;
    total += readRecordHeader( file, type, pidxSize );
    pidxSize += 8;
    if( type != "PIDX" )
    {
        std::cout << "Expected record of type PIDX: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    file.read( (char*)&numIndex, sizeof( numIndex ) );
    total += sizeof( numIndex );
    std::cout << "Num index: " << numIndex << std::endl;

    unsigned int index;

    for( unsigned int i = 0; i < numIndex; ++i )
      {
	file.read( (char*)&index, sizeof( index ) );
	total += sizeof( index );
	//std::cout << index << std::endl;
      }

    if( pidxSize == total )
    {
        std::cout << "Finished reading PIDX" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading PIDX" << std::endl;
        std::cout << "Read " << total << " out of " << pidxSize
                  << std::endl;
     }

    return total;
}

// Index into Normal array
unsigned int skmg::readNIDX( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int nidxSize;
    total += readRecordHeader( file, type, nidxSize );
    nidxSize += 8;
    if( type != "NIDX" )
    {
        std::cout << "Expected record of type NIDX: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    std::cout << "Num index: " << numIndex << std::endl;
    unsigned int index;
    for( unsigned int i = 0; i < numIndex; ++i )
      {
	file.read( (char*)&index, sizeof( index ) );
	total += sizeof( index );
	//std::cout << index << std::endl;
      }

    if( nidxSize == total )
    {
        std::cout << "Finished reading NIDX" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading NIDX" << std::endl;
        std::cout << "Read " << total << " out of " << nidxSize
                  << std::endl;
     }

    return total;
}

unsigned int skmg::readNAME( std::istream &file )
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
    file.getline( temp, 255, 0 );
    shaderFilename = temp;
    total += shaderFilename.size() + 1;
    std::cout << shaderFilename << std::endl;

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

unsigned int skmg::readXFNM( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    std::string type;

    unsigned int xfnmSize;
    total += readRecordHeader( file, type, xfnmSize );
    xfnmSize += 8;
    if( type != "XFNM" )
    {
        std::cout << "Expected record of type XFNM: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    char temp[255];
    std::string boneName;
    for( unsigned int i = 0; i < numBones; ++i )
      {
	file.getline( temp, 255, 0 );
	boneName = temp;
	total += boneName.size() + 1;
	std::cout << boneName << std::endl;
      }

    if( xfnmSize == total )
    {
        std::cout << "Finished reading XFNM" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading XFNM" << std::endl;
        std::cout << "Read " << total << " out of " << xfnmSize
                  << std::endl;
     }

    return total;
}

void skmg::print() const
{
}

#if 0
/// Parent bone
unsigned int skmg::readPRNT( std::istream &file, unsigned int num )
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

unsigned int skmg::readRPRE( std::istream &file, unsigned int num )
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

unsigned int skmg::readRPST( std::istream &file, unsigned int num )
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

unsigned int skmg::readBPTR( std::istream &file, unsigned int num )
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

unsigned int skmg::readBPRO( std::istream &file, unsigned int num )
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

unsigned int skmg::readJROR( std::istream &file, unsigned int num )
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

#endif
