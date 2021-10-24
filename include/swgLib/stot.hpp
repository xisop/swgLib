/** -*-c++-*-
 *  \class  stot
 *  \file   stot.hpp
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
#include <swgLib/shot.hpp>

#include <istream>
#include <ostream>
#include <vector>
#include <string>

#ifndef STOT_HPP
#define STOT_HPP 1

namespace ml
{
	// Shared Tangible Object Template
	class stot : public shot
	{
	public:
		stot();
		~stot();

		std::size_t readSTOT(std::istream& file);
		void print(std::ostream& os) const;

	public:
		// Ranged Int Customization Variable (RICV)
		class ricv {
		public:
			ricv( const std::string &variableName="",
				const int32_t &minValueInclusive=0,
				const int32_t &defaultValue=0,
				const int32_t &maxValueExclusive=0);
			~ricv();

			size_t read(std::istream& file);
			void print(std::ostream& os) const;

		protected:
			std::string _variableName;
			int32_t     _minValueInclusive;
			int32_t     _defaultValue;
			int32_t     _maxValueExclusive;
		};

		// Palette Color Customization Variable (PCCV)
		class pccv {
		public:
			pccv( const std::string &variableName="",
				const std::string &palettePathName="",
				const int32_t &defaultPaletteIndex=0);
			~pccv();

			size_t read(std::istream& file);
			void print(std::ostream& os) const;

		protected:
			std::string _variableName;
			std::string _palettePathName;
			int32_t     _defaultPaletteIndex;
		};

		// Const String Customization Variable (CSCV)
		class cscv {
		public:
			cscv( const std::string &variableName="",
				const std::string &constValue="");
			~cscv();

			size_t read(std::istream& file);
			void print(std::ostream& os) const;

		protected:
			std::string _variableName;
			std::string _constValue;
		};

		// Customization Variable Mapping (CVM)
		class cvm {
		public:
			cvm( const std::string &sourceVariable="",
				const std::string &dependentVariable="");
			~cvm();

			size_t read(std::istream& file);
			void print(std::ostream& os) const;

		protected:
			std::string _sourceVariable;
			std::string _dependentVariable;
		};

	protected:
		std::size_t readSTOTParameter(std::istream& file);

		std::string _stotBaseObjectFilename;
		int8_t _stotVersion;

		std::vector<pccv>        _paletteColorCustomizationVariables;
		std::vector<ricv>        _rangedIntCustomizationVariables;
		std::vector<cscv>        _constStringCustomizationVariables;
		std::vector<int32_t>     _socketDestinations;
		std::string              _structureFootprintFileName;
		bool                     _useStructureFootprintOutline;
		bool                     _targetable;
		std::vector<std::string> _certificationsRequired;
		std::vector<cvm>         _customizationVariableMapping;
		int32_t                  _clientVisabilityFlag;
	};
}
#endif
