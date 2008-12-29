/** -*-c++-*-
 *  \class  spam
 *  \file   spam.cpp
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

#include <meshLib/spam.hpp>

#include <iostream>
#include <cstdlib>

#define MAX_SWG_STRING 2048

using namespace ml;

spam::spam()
{
}

spam::~spam()
{
}

unsigned int spam::readSPAM( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int spamSize;
    std::string type;

    total += readFormHeader( file, form, spamSize, type );
    spamSize += 8;
    if( form != "FORM" || type != "SPAM" )
    {
	std::cout << "Expected Form of type SPAM: " << type << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found SPAM form"
	      << ": " << spamSize-12 << " bytes"
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

    total += readAPPE( file );
    total += readCLIE( file );
    total += readPORT( file );
    total += readSOUN( file );

    if( spamSize == total )
    {
#if DEBUG
	sd::cout << "Finished reading SPAM" << std::endl;
#endif
    }
    else
    {
	std::cout << "FAILED in reading SPAM" << std::endl;
	std::cout << "Possibly a corrupt or non-finished file" << std::endl;
	std::cout << "Read " << total << " out of " << spamSize
                  << std::endl;
    }
    
    return total;
}


unsigned int spam::readAPPE( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int formSize;
    std::string type;

    total += readFormHeader( file, form, formSize, type );
    formSize += 8;
    if( form != "FORM" || type != "APPE" )
    {
	std::cout << "Expected form of type APPE: " << type << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found APPE form"
	      << ": " << formSize-8 << " bytes"
	      << std::endl;
#endif
    
    unsigned int size;
    total += readRecordHeader( file, type, size );
    size += 8;
    if( type != "0000" )
    {
	std::cout << "Expected record of type 0000: " << type << std::endl;
	exit( 0 );
    }

    char temp[255];
    while( total < size )
    {
	file.getline( temp, 255, 0 );
	std::string newName( temp );
	total += newName.size() + 1;
	appe.push_back( newName );
    }

    if( formSize == total )
    {
#if DEBUG
	std::cout << "Finished reading APPE" << std::endl;
#endif
    }
    else
    {
	std::cout << "FAILED in reading APPE" << std::endl;
	std::cout << "Read " << total << " out of " << formSize
                  << std::endl;
    }
    
    return total;
}

unsigned int spam::readCLIE( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int formSize;
    std::string type;

    total += readFormHeader( file, form, formSize, type );
    formSize += 8;
    if( form != "FORM" || type != "CLIE" )
    {
	std::cout << "Expected form of type CLIE: " << type << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found CLIE form"
	      << ": " << formSize-8 << " bytes"
	      << std::endl;
#endif
    
    unsigned int size;
    total += readRecordHeader( file, type, size );
    size += 8;
    if( type != "0000" )
    {
	std::cout << "Expected record of type 0000: " << type << std::endl;
	exit( 0 );
    }

    char temp[255];
    while( total < size )
    {
	file.getline( temp, 255, 0 );
	std::string newName( temp );
	total += newName.size() + 1;
	clie.push_back( newName );
    }

    if( formSize == total )
    {
#if DEBUG
	std::cout << "Finished reading CLIE" << std::endl;
#endif
    }
    else
    {
	std::cout << "FAILED in reading CLIE" << std::endl;
	std::cout << "Read " << total << " out of " << formSize
                  << std::endl;
    }
    
    return total;
}

unsigned int spam::readPORT( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int formSize;
    std::string type;

    total += readFormHeader( file, form, formSize, type );
    formSize += 8;
    if( form != "FORM" || type != "PORT" )
    {
	std::cout << "Expected form of type PORT: " << type << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found PORT form"
	      << ": " << formSize-8 << " bytes"
	      << std::endl;
#endif
    
    unsigned int size;
    total += readRecordHeader( file, type, size );
    size += 8;
    if( type != "0000" )
    {
	std::cout << "Expected record of type 0000: " << type << std::endl;
	exit( 0 );
    }

    char temp[255];
    while( total < size )
    {
	file.getline( temp, 255, 0 );
	std::string newName( temp );
	total += newName.size() + 1;
	port.push_back( newName );
    }

    if( formSize == total )
    {
#if DEBUG
	std::cout << "Finished reading PORT" << std::endl;
#endif
    }
    else
    {
	std::cout << "FAILED in reading PORT" << std::endl;
	std::cout << "Read " << total << " out of " << formSize
                  << std::endl;
    }
    
    return total;
}

unsigned int spam::readSOUN( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int formSize;
    std::string type;

    total += readFormHeader( file, form, formSize, type );
    formSize += 8;
    if( form != "FORM" || type != "SOUN" )
    {
	std::cout << "Expected form of type SOUN: " << type << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found SOUN form"
	      << ": " << formSize-8 << " bytes"
	      << std::endl;
#endif
    
    unsigned int size;
    total += readRecordHeader( file, type, size );
    size += 8;
    if( type != "0000" )
    {
	std::cout << "Expected record of type 0000: " << type << std::endl;
	exit( 0 );
    }

    char temp[255];
    while( total < size )
    {
	file.getline( temp, 255, 0 );
	std::string newName( temp );
	total += newName.size() + 1;
	soun.push_back( newName );
    }

    if( formSize == total )
    {
#if DEBUG
	std::cout << "Finished reading SOUN" << std::endl;
#endif
    }
    else
    {
	std::cout << "FAILED in reading SOUN" << std::endl;
	std::cout << "Read " << total << " out of " << formSize
                  << std::endl;
    }
    
    return total;
}

void spam::print() const
{
    std::cout << "Appe" << std::endl;
    for( unsigned int i = 0; i < appe.size(); ++i )
    {
        std::cout << i << ": "
                  << appe[i]
                  << std::endl;
    }
    std::cout << "Clie" << std::endl;
    for( unsigned int i = 0; i < clie.size(); ++i )
    {
        std::cout << i << ": "
                  << clie[i]
                  << std::endl;
    }
    std::cout << "Port" << std::endl;
    for( unsigned int i = 0; i < port.size(); ++i )
    {
        std::cout << i << ": "
                  << port[i]
                  << std::endl;
    }
    std::cout << "Soun" << std::endl;
    for( unsigned int i = 0; i < soun.size(); ++i )
    {
        std::cout << i << ": "
                  << soun[i]
                  << std::endl;
    }
}
