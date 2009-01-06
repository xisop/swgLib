/** -*-c++-*-
 *  \class  trn
 *  \file   trn.hpp
 *  \author Kenneth R. Sewell III

 meshLib is used for the parsing and exporting .trn models.
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
    unsigned int readTRN( std::istream &file,
			  const std::string &debugString = "" );

  protected:
    unsigned int readTGEN( std::istream &file, const std::string & );
    unsigned int readTRNDATA( std::istream &file, const std::string & );
    unsigned int readMapDATA( std::istream &file, const std::string & );
    unsigned int readWMAP( std::istream &file, const std::string & );
    unsigned int readSMAP( std::istream &file, const std::string & );
    unsigned int readSGRP( std::istream &file, const std::string & );
    unsigned int readFGRP( std::istream &file, const std::string & );
    unsigned int readRGRP( std::istream &file, const std::string & );
    unsigned int readEGRP( std::istream &file, const std::string & );
    unsigned int readMGRP( std::istream &file, const std::string & );
    unsigned int readLYRS( std::istream &file, const std::string & );
    unsigned int readLAYR( std::istream &file, const std::string & );

    unsigned int readFFAM( std::istream &file, const std::string & );
    unsigned int readRFAM( std::istream &file, const std::string & );
    unsigned int readEFAM( std::istream &file, const std::string & );
    unsigned int readMFAM( std::istream &file, const std::string & );
    unsigned int readMFRC( std::istream &file, const std::string & );

    unsigned int readACCN( std::istream &file, const std::string & );
    unsigned int readACRF( std::istream &file, const std::string & );
    unsigned int readACRH( std::istream &file, const std::string & );
    unsigned int readADTA( std::istream &file, const std::string & );
    unsigned int readAENV( std::istream &file, const std::string & );
    unsigned int readAEXC( std::istream &file, const std::string & );
    unsigned int readAFDF( std::istream &file, const std::string & );
    unsigned int readAFDN( std::istream &file, const std::string & );
    unsigned int readAFSC( std::istream &file, const std::string & );
    unsigned int readAFSN( std::istream &file, const std::string & );
    unsigned int readAHCN( std::istream &file, const std::string & );
    unsigned int readAHFR( std::istream &file, const std::string & );
    unsigned int readAHTR( std::istream &file, const std::string & );
    unsigned int readAROA( std::istream &file, const std::string & );
    unsigned int readASCN( std::istream &file, const std::string & );
    unsigned int readASRP( std::istream &file, const std::string & );
    unsigned int readBCIR( std::istream &file, const std::string & );
    unsigned int readBPLN( std::istream &file, const std::string & );
    unsigned int readBPOL( std::istream &file, const std::string & );
    unsigned int readBREC( std::istream &file, const std::string & );
    unsigned int readFDIR( std::istream &file, const std::string & );
    unsigned int readFFRA( std::istream &file, const std::string & );
    unsigned int readFHGT( std::istream &file, const std::string & );
    unsigned int readFSHD( std::istream &file, const std::string & );
    unsigned int readFSLP( std::istream &file, const std::string & );
    unsigned int readIHDR( std::istream &file, const std::string & );
    unsigned int readSFAM( std::istream &file, const std::string & );
					  
  private:
    unsigned int height;
    unsigned int width;
  };
}
#endif
