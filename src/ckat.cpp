/** -*-c++-*-
 *  \class  ckat
 *  \file   ckat.cpp
 *  \author Kenneth R. Sewell III

 meshLib is used for the parsing and exporting .msh models.
 Copyright (C) 2009 Kenneth R. Sewell III

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
#include <meshLib/ckat.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

ckat::ckat()
{
}

ckat::~ckat()
{
}

unsigned int ckat::readCKAT( std::istream &file, std::string path )
{
  basePath = path;
  unsigned int ckatSize;
  unsigned int total = readFormHeader( file, "CKAT", ckatSize );
  ckatSize += 8;
  std::cout << "Found CKAT form"
	    << ": " << ckatSize-12 << " bytes"
	    << std::endl;

  unsigned int size;
  std::string form, type;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" )
    {
      std::cout << "Expected FORM: " << form << std::endl;
      exit( 0 );
    }
  std::cout << "Found " << form << " " << type
	    << ": " << size-4 << " bytes"
	    << std::endl;

  total += readUnknown( file, ckatSize-total );

  if( ckatSize == total )
    {
      std::cout << "Finished reading CKAT" << std::endl;
    }
  else
    {
      std::cout << "FAILED in reading CKAT" << std::endl;
      std::cout << "Read " << total << " out of " << ckatSize
		<< std::endl;
    }
    
  return total;
}
