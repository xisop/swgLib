/** -*-c++-*-
 *  \class  trn
 *  \file   trn.hpp
 *  \author Kenneth R. Sewell III

 meshLib is used for the parsing and exporting .trn models.
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
#include <meshLib/base.hpp>

#include <fstream>
#include <string>

#ifndef TRN_HPP
#define TRN_HPP

namespace ml
{
  class trn : public base
  {
  public:
    trn();
    ~trn();
    bool isRightType( std::istream &file )
    {
      return isOfType( file, "PTAT" );
    }
    unsigned int readTRN( std::istream &file );

  protected:
    unsigned int readTGEN( std::istream &file );
    unsigned int readTRNDATA( std::istream &file );
    unsigned int readMapDATA( std::istream &file );
    unsigned int readWMAP( std::istream &file );
    unsigned int readSMAP( std::istream &file );
    unsigned int readSGRP( std::istream &file );
    unsigned int readFGRP( std::istream &file );
    unsigned int readRGRP( std::istream &file );
    unsigned int readEGRP( std::istream &file );
    unsigned int readMGRP( std::istream &file );
    unsigned int readLYRS( std::istream &file );
    unsigned int readLAYR( std::istream &file );

    unsigned int readACCN( std::istream &file );
    unsigned int readACRF( std::istream &file );
    unsigned int readACRH( std::istream &file );
    unsigned int readADTA( std::istream &file );
    unsigned int readAENV( std::istream &file );
    unsigned int readAEXC( std::istream &file );
    unsigned int readAFDF( std::istream &file );
    unsigned int readAFDN( std::istream &file );
    unsigned int readAFSC( std::istream &file );
    unsigned int readAFSN( std::istream &file );
    unsigned int readAHCN( std::istream &file );
    unsigned int readAHFR( std::istream &file );
    unsigned int readAHTR( std::istream &file );
    unsigned int readAROA( std::istream &file );
    unsigned int readASCN( std::istream &file );
    unsigned int readASRP( std::istream &file );
    unsigned int readBCIR( std::istream &file );
    unsigned int readBPLN( std::istream &file );
    unsigned int readBPOL( std::istream &file );
    unsigned int readBREC( std::istream &file );
    unsigned int readFDIR( std::istream &file );
    unsigned int readFFRA( std::istream &file );
    unsigned int readFHGT( std::istream &file );
    unsigned int readFSHD( std::istream &file );
    unsigned int readFSLP( std::istream &file );
    unsigned int readIHDR( std::istream &file );
    unsigned int readSFAM( std::istream &file );
					  
  private:
    unsigned int height;
    unsigned int width;
  };
}
#endif
