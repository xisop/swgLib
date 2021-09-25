/** -*-c++-*-
 *  \class  peft
 *  \file   peft.hpp
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
#include <string>
#include <map>

#ifndef PEFT_HPP
#define PEFT_HPP

namespace ml
{
  class peft : public base
  {
  public:
    peft();
    ~peft();
    bool isRightType( std::istream &file )
    {
      return isOfType( file, "PEFT" );
    }
    unsigned int readPEFT( std::istream &file, std::string path="" );

  protected:
    unsigned int readPTIM( std::istream &file );
    unsigned int read0000( std::istream &file, unsigned int &num );
    unsigned int readEMGP( std::istream &file );
    unsigned int readEMTR( std::istream &file );
    unsigned int readEMTR0000( std::istream &file );
    unsigned int readWVFM( std::istream &file );
    unsigned int readPTQD( std::istream &file );
    unsigned int readPTCL( std::istream &file );
    unsigned int readPTCL0002( std::istream &file );
    unsigned int readCLRR( std::istream &file );
    unsigned int readPTEX( std::istream &file );
				
  private:
    std::string particleShader;
  };
}
#endif
