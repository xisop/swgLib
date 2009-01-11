/** -*-c++-*-
 *  \class  box
 *  \file   box.cpp
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

#include <meshLib/box.hpp>
#include <meshLib/base.hpp>
#include <cstdlib>

using namespace ml;

unsigned int box::readBOX( std::istream &file )
{
  unsigned int size;
  std::string type;

  unsigned int total = base::readRecordHeader( file, type, size );
  size += 8; // Size of header
  if( type != "BOX " )
    {
      std::cout << "Expected record of type BOX: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found BOX record" << std::endl;

  // Read center and radius of bounding sphere
  file.read( (char*)&x1, sizeof( float ) );
  file.read( (char*)&y1, sizeof( float ) );
  file.read( (char*)&z1, sizeof( float ) );
  file.read( (char*)&x2, sizeof( float ) );
  file.read( (char*)&y2, sizeof( float ) );
  file.read( (char*)&z2, sizeof( float ) );
  total += sizeof( float ) * 6;

  std::cout << "Bounding box corners: " << std::endl;
  std::cout << "   " << x1 << ", " << y1 << ", " << z1 << std::endl;
  std::cout << "   " << x2 << ", " << y2 << ", " << z2 << std::endl;

  if( total == size )
    {
      std::cout << "Finished reading BOX." << std::endl;
    }
  else
    {
      std::cout << "Error reading BOX!" << std::endl;
      std::cout << "Read " << total << " out of " << size
		<< std::endl;
    }

  return total;
}
