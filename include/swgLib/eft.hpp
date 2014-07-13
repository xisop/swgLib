/** -*-c++-*-
 *  \class  eft
 *  \file   eft.hpp
 *  \author Kenneth R. Sewell III

 swgLib is used for the parsing and exporting .msh models.
 Copyright (C) 2006-2009 Kenneth R. Sewell III

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
#include <swgLib/base.hpp>

#include <fstream>
#include <string>
#include <map>

#ifndef EFT_HPP
#define EFT_HPP

namespace ml
{
  class eft : public base
  {
  public:
    eft();
    ~eft();
    bool isRightType( std::istream &file )
    {
      return isOfType( file, "EFCT" );
    }
    unsigned int readEFT( std::istream &file, std::string path="",
			  const unsigned short &depth=0 );

  protected:
    unsigned int readIMPL( std::istream &file, const unsigned short &depth=0 );
    unsigned int readPASS( std::istream &file, const unsigned short &depth=0 );
    unsigned int readPVSH( std::istream &file, const unsigned short &depth=0 );
    unsigned int readPPSH( std::istream &file, const unsigned short &depth=0 );
    unsigned int readPFFP( std::istream &file, const unsigned short &depth=0 );
    unsigned int readSTAG( std::istream &file, const unsigned short &depth=0 );
    unsigned int readPTXM( std::istream &file, const unsigned short &depth=0 );
    unsigned int readSCAP( std::istream &file, const unsigned short &depth=0 );
    unsigned int readOPTN( std::istream &file, const unsigned short &depth=0 );
    unsigned int readIMPLDATA( std::istream &file, const unsigned short &depth=0 );
				
  private:
    unsigned int numTextures;
    bool normalMap;
    unsigned int normalMapUnit;
    std::map<std::string, unsigned char> coordMapping;
    std::string effectName;
  };
}
#endif
