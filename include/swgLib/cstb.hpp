/** -*-c++-*-
 *  \class  cstb
 *  \file   cstb.hpp
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

#include <swgLib/base.hpp>

#include <fstream>
#include <vector>
#include <string>

#ifndef CSTB_HPP
#define CSTB_HPP

namespace ml
{
  class cstb : public base
  {
  public:
    cstb();
    ~cstb();
    bool isRightType( std::istream &file )
    {
      return isOfType( file, "CSTB" );
    }
    unsigned int readCSTB( std::istream &file );
    void print() const;

  protected:
    unsigned int readDATA( std::istream &file );
    unsigned int readCRCT( std::istream &file );
    unsigned int readSTRT( std::istream &file );
    unsigned int readSTNG( std::istream &file );
	
    unsigned int num;
    std::vector<unsigned int> crc;
    std::vector<unsigned int> offset;
    std::vector<std::string> name;

  private:
    
  };
}
#endif
