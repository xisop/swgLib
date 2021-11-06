/** -*-c++-*-
 *  \class  base
 *  \file   base.hpp
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

#include <fstream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

#include <swgLib/vector3.hpp>
#include <swgLib/matrix3.hpp>

#ifndef BASE_HPP
#define BASE_HPP 1

namespace ml
{
	class tag {
	public:
		enum {
			TAG_0000 = 0x30303030, // '0000'
			TAG_0001 = 0x30303031, // '0001'
			TAG_0002 = 0x30303032, // '0002'
			TAG_0003 = 0x30303033, // '0003'
			TAG_0004 = 0x30303034, // '0004'
			TAG_0005 = 0x30303035, // '0005'
			TAG_0006 = 0x30303036, // '0006'
			TAG_0007 = 0x30303037, // '0007'
			TAG_0008 = 0x30303038, // '0008'
			TAG_0009 = 0x30303039, // '0009'
			TAG_DATA = 0x44415441, // 'DATA'
			TAG_FORM = 0x464f524d, // 'FORM'
			TAG_SMAP = 0x534d4150, // 'SMAP'
			TAG_WMAP = 0x574d4150, // 'WMAP'
		};
	public:
		tag();
		~tag();

		bool operator<(const tag& t) const;
		bool operator==(const tag& t) const;

		std::size_t read(std::istream& file);
		std::size_t write(std::ostream& file) const;

		const uint32_t& raw() const;
		const std::string& str() const;

	protected:
		uint32_t _raw;
		std::string _str;
	};

	class base
	{
	public:
		base() {};
		virtual ~base() {};
		virtual bool isRightType(std::istream& file) { return false; }
		bool isOfType(std::istream& file, const std::string& Type);
		static std::string getType(std::istream& file);
		std::size_t readBASE() { return 0; }
		virtual bool canWrite() const { return false; }

		static std::size_t readUnknown(std::istream& file,
			const std::size_t size);

		static std::size_t read(std::istream& file, bool& data);
		static std::size_t write(std::ostream& file, const bool& data);
		static std::size_t read(std::istream& file, int8_t& data);
		static std::size_t write(std::ostream& file, const int8_t& data);
		static std::size_t read(std::istream& file, char& data);
		static std::size_t write(std::ostream& file, const char& data);
		static std::size_t read(std::istream& file, unsigned char& data);
		static std::size_t write(std::ostream& file, const unsigned char& data);
		static std::size_t read(std::istream& file, short& data);
		static std::size_t write(std::ostream& file, const short& data);
		static std::size_t read(std::istream& file, unsigned short& data);
		static std::size_t write(std::ostream& file, const unsigned short& data);
		static std::size_t read(std::istream& file, int& data);
		static std::size_t write(std::ostream& file, const int& data);
		static std::size_t read(std::istream& file, unsigned int& data);
		static std::size_t write(std::ostream& file, const unsigned int& data);
		static std::size_t read(std::istream& file, float& data);
		static std::size_t write(std::ostream& file, const float& data);
		static std::size_t read(std::istream& file, std::string& data, const std::size_t& length);
		static std::size_t read(std::istream& file, std::string& data);
		static std::size_t write(std::ostream& file, const std::string& data);
		static std::size_t read(std::istream& file, vector3& mat);
		static std::size_t write(std::ostream& file, const vector3& mat);
		static std::size_t read(std::istream& file, matrix3x3& mat);
		static std::size_t write(std::ostream& file, const matrix3x3& mat);
		static std::size_t read(std::istream& file, matrix3x4& mat);
		static std::size_t write(std::ostream& file, const matrix3x4& mat);
		static std::size_t read(std::istream& file, tag& t);
		static std::size_t write(std::ostream& file, const tag& t);

		static std::size_t skip(std::istream& file, const std::size_t& skipBytes);

		// ******************** String based Form headers ********************
		static void peekHeader(std::istream& file,
			std::string& form,
			std::size_t& size,
			std::string& type);

		static std::size_t readFormHeader(std::istream& file,
			std::string& form,
			std::size_t& size,
			std::string& type);

		static std::size_t readFormHeader(std::istream& file,
			std::string& type,
			std::size_t& size);

		static std::size_t readFormHeader(std::istream& file,
			const std::string& expectedType,
			std::size_t& size);

		// ******************** uint32 based Form headers ********************
		static void peekHeader(std::istream& file,
			uint32_t& form,
			std::size_t& size,
			uint32_t& type);

		static std::size_t readFormHeader(std::istream& file,
			uint32_t& form,
			std::size_t& size,
			uint32_t& type);

		static std::size_t readFormHeader(std::istream& file,
			uint32_t& type,
			std::size_t& size);

		static std::size_t readFormHeader(std::istream& file,
			const uint32_t& expectedType,
			std::size_t& size);

		// ******************** String based Record headers ********************

		static std::size_t readRecordHeader(std::istream& file,
			std::string& type,
			std::size_t& size);

		static std::size_t readRecordHeader(std::istream& file,
			std::string& type);

		static std::size_t readRecordHeader(std::istream& file,
			const std::string& expectedType,
			std::size_t& size);

		static std::size_t readRecordHeader(std::istream& file,
			const std::string& expectedType);

		// ******************** uint32_t based Record headers ********************
		static std::size_t readRecordHeader(std::istream& file,
			uint32_t& type,
			std::size_t& size);

		static std::size_t readRecordHeader(std::istream& file,
			uint32_t& type);

		static std::size_t readRecordHeader(std::istream& file,
			const uint32_t& expectedType,
			std::size_t& size);

		static std::size_t readRecordHeader(std::istream& file,
			const uint32_t& expectedType);



		

		static std::size_t writeFormHeader(std::ostream& file,
			const std::size_t& size,
			const std::string& type);

		static std::size_t writeRecordHeader(std::ostream& file,
			const std::string& type,
			const std::size_t& size);

		static std::size_t readBigEndian(std::istream& file,
			const std::size_t& size,
			char* buffer);

		static std::size_t writeBigEndian(std::ostream& file,
			const std::size_t& size,
			char* buffer);

		static bool fixSlash(std::string& filename);

		static uint32_t tagToVersion(const std::string& tag);
		static uint32_t tagToVersion(const uint32_t& tag);
		static std::string tagToStr(const uint32_t& tag);

		static uint32_t typeToNumber(const std::string& type);

	protected:
		std::string basePath;

	private:
	};
}

std::ostream& operator<<(std::ostream& os, const ml::tag& t);

#endif
