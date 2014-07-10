/** -*-c++-*-
 *  \class  ckat
 *  \file   ckat.hpp
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
#include <meshLib/base.hpp>
#include <meshLib/quat.hpp>

#include <fstream>
#include <string>
#include <map>

#ifndef CKAT_HPP
#define CKAT_HPP

namespace ml
{
  class ckat : public base
  {
  public:

    class keyframe
    {
    public:
      std::map< unsigned int, quat > quatMap;
    };

    ckat();
    ~ckat();
    bool isRightType( std::istream &file )
    {
      return isOfType( file, "CKAT" );
    }
    unsigned int readCKAT( std::istream &file,
			   std::string path="",
			   const unsigned short &depth=0 );

  protected:
    unsigned int readINFO( std::istream &file, const unsigned short &depth=0 );
    unsigned int readXFRM( std::istream &file, const unsigned short &depth=0 );
    unsigned int readAROT( std::istream &file, const unsigned short &depth=0 );
    unsigned int readSROT( std::istream &file, const unsigned short &depth=0 );
    unsigned int readATRN( std::istream &file, const unsigned short &depth=0 );
    unsigned int readSTRN( std::istream &file, const unsigned short &depth=0 );
    unsigned int readXFIN( std::istream &file, const unsigned short &depth=0 );
    unsigned int readQCHN( std::istream &file, const unsigned short &depth=0 );
    unsigned int readCHNL( std::istream &file, const unsigned short &depth=0 );
    unsigned int readMSGS( std::istream &file, const unsigned short &depth=0 );
    unsigned int readMESG( std::istream &file, const unsigned short &depth=0 );
    unsigned int readLOCT( std::istream &file, const unsigned short &depth=0 );
	
    unsigned short numXFIN;
    unsigned short numQCHN;
    unsigned short numSTRN;
    unsigned short numSROT;
    unsigned short numCHNL;

    std::vector< keyframe > keyframeList;

  private:

  };
}
#endif
