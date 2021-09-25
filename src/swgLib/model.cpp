/** -*-c++-*-
 *  \class  model
 *  \file   model.cpp
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

#include <swgLib/model.hpp>
#include <swgLib/box.hpp>
#include <iostream>
#include <cstdlib>

using namespace ml;

unsigned int model::readSPHR(std::istream& file,
	float& cx, float& cy, float& cz,
	float& radius
)
{
	std::string form;
	std::size_t size;
	std::string type;

	std::size_t total = readRecordHeader(file, type, size);
	size += 8; // Size of header
	if (type != "SPHR")
	{
		std::cout << "Expected record of type SPHR: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found SPHR record" << std::endl;

	// Read center and radius of bounding sphere
	total += base::read(file, cx);
	total += base::read(file, cy);
	total += base::read(file, cz);
	total += base::read(file, radius);

	std::cout << "Bounding sphere center and radius: " << std::endl;
	std::cout << "  ( " << cx << ", " << cy << ", " << cz
		<< " ) ( " << radius << " ) " << std::endl;

	if (total == size)
	{
		std::cout << "Finished reading SPHR." << std::endl;
	}
	else
	{
		std::cout << "Error reading SPHR!" << std::endl;
		std::cout << "Read " << total << " out of " << size
			<< std::endl;
	}

	return total;
}

unsigned int model::readCYLN(std::istream& file,
	float& u1, float& u2, float& u3,
	float& u4, float& u5
)
{
	std::string form;
	std::size_t size;
	std::string type;

	std::size_t total = readRecordHeader(file, type, size);
	size += 8; // Size of header
	if (type != "CYLN")
	{
		std::cout << "Expected record of type CYLN: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found CYLN record" << std::endl;

	// Read 
	total += read(file, u1);
	total += read(file, u2);
	total += read(file, u3);
	total += read(file, u4);
	total += read(file, u5);
	std::cout << u1 << " "
		<< u2 << " "
		<< u3 << " "
		<< u4 << " "
		<< u5 << std::endl;

	if (total == size)
	{
		std::cout << "Finished reading CYLN." << std::endl;
	}
	else
	{
		std::cout << "Error reading CYLN!" << std::endl;
		std::cout << "Read " << total << " out of " << size
			<< std::endl;
	}

	return total;
}

unsigned int model::readEXSP(std::istream& file,
	float& cx, float& cy, float& cz,
	float& radius
)
{
	std::string form;
	std::size_t exspSize;
	std::string type;

	std::size_t total = readFormHeader(file, "EXSP", exspSize);
	exspSize += 8;
	std::cout << "Found EXSP form" << std::endl;

	std::size_t size;
	total += readFormHeader(file, "0001", size);
	std::cout << "Found 0001 form" << std::endl;

	total += model::readSPHR(file, cx, cy, cz, radius);

	if (total == exspSize)
	{
		std::cout << "Finished reading EXSP." << std::endl;
	}
	else
	{
		std::cout << "Error reading EXSP!" << std::endl;
		std::cout << "Read " << total << " out of " << exspSize
			<< std::endl;
	}

	return total;
}

unsigned int model::readEXBX(std::istream& file,
	float& cx, float& cy, float& cz,
	float& radius,
	box& box1
)
{
	std::string form;
	std::size_t exbxSize;
	std::string type;

	std::size_t total = readFormHeader(file, "EXBX", exbxSize);
	exbxSize += 8;
	std::cout << "Found EXBX form" << std::endl;

	std::size_t size;
	total += readFormHeader(file, "0001", size);
	std::cout << "Found 0001 form" << std::endl;

	total += model::readEXSP(file, cx, cy, cz, radius);
	total += box1.readBOX(file);

	if (total == exbxSize)
	{
		std::cout << "Finished reading EXBX." << std::endl;
	}
	else
	{
		std::cout << "Error reading EXBX!" << std::endl;
		std::cout << "Read " << total << " out of " << exbxSize
			<< std::endl;
	}

	return total;
}

unsigned int model::readXCYL(std::istream& file,
	float& u1, float& u2, float& u3,
	float& u4, float& u5
)
{
	std::string form;
	std::size_t xcylSize;
	std::string type;

	std::size_t total = readFormHeader(file, "XCYL", xcylSize);
	xcylSize += 8;
	std::cout << "Found XCYL form" << std::endl;

	std::size_t size;
	total += readFormHeader(file, "0000", size);
	std::cout << "Found 0000 form" << std::endl;

	total += model::readCYLN(file, u1, u2, u3, u4, u5);

	if (total == xcylSize)
	{
		std::cout << "Finished reading XCYL." << std::endl;
	}
	else
	{
		std::cout << "Error reading XCYL!" << std::endl;
		std::cout << "Read " << total << " out of " << xcylSize
			<< std::endl;
	}

	return total;
}

unsigned int model::readTransformMatrix(std::istream& file, matrix3x4& matrix) {
	return matrix.read(file);
}

unsigned int model::readIDTL(std::istream& file,
	std::vector<vector3>& vec,
	std::vector<int32_t>& index
)
{
	std::size_t idtlSize;
	std::size_t total = readFormHeader(file, "IDTL", idtlSize);
	idtlSize += 8;
	std::cout << "Found IDTL FORM\n";

	std::size_t size;
	std::string form, type;
	total += readFormHeader(file, "0000", size);
	std::cout << "Found 0000 FORM\n";

	total += readVERT(file, vec);
	total += readINDX(file, index);

	if (idtlSize == total)
	{
		std::cout << "Finished reading IDTL" << std::endl;
	}
	else
	{
		std::cout << "Failed in reading IDTL" << std::endl;
		std::cout << "Read " << total << " out of " << idtlSize
			<< std::endl;
	}

	return total;
}


unsigned int model::readVERT(std::istream& file,
	std::vector<vector3>& vec
)
{
	std::size_t vertSize;

	std::size_t total = readRecordHeader(file, "VERT", vertSize);
	vertSize += 8;
	std::cout << "Found VERT record" << std::endl;

	float x, y, z;
	unsigned int numVerts = (vertSize - 8) / (sizeof(float) * 3);
	std::cout << "Number of vertices: " << numVerts << std::endl;
	for (unsigned int i = 0; i < numVerts; ++i)
	{
		total += read(file, x);
		total += read(file, y);
		total += read(file, z);
#if 1
		std::cout << "Vert: " << std::fixed
			<< x << ", "
			<< y << ", "
			<< z << std::endl;
#endif
		vec.push_back(vector3(x, y, z));
	}

	if (vertSize == total)
	{
		std::cout << "Finished reading VERT" << std::endl;
	}
	else
	{
		std::cout << "Failed in reading VERT" << std::endl;
		std::cout << "Read " << total << " out of " << vertSize
			<< std::endl;
	}

	return total;

}

unsigned int model::readINDX(std::istream& file,
	std::vector<int32_t>& index)
{
	std::size_t indxSize;

	std::size_t total = readRecordHeader(file, "INDX", indxSize);
	indxSize += 8;
	std::cout << "Found INDX record" << std::endl;

	int32_t x;
	unsigned int numIndex = (indxSize - 8) / sizeof(x);
	for (unsigned int i = 0; i < numIndex; ++i)
	{
		total += read(file, x);
		std::cout << "Index: " << x << std::endl;
		index.push_back(x);
	}

	if (indxSize == total)
	{
		std::cout << "Finished reading INDX" << std::endl;
	}
	else
	{
		std::cout << "Failed in reading INDX" << std::endl;
		std::cout << "Read " << total << " out of " << indxSize
			<< std::endl;
	}

	return total;
}

unsigned int model::readAPPR(std::istream& file)
{
	std::size_t apprSize;
	std::string type;

	std::size_t total = readFormHeader(file, "APPR", apprSize);
	apprSize += 8;
	std::cout << "Found APPR form" << std::endl;

	std::size_t size;
	total += readFormHeader(file, "0003", size);
	std::cout << "Found 0003 form" << std::endl;

	std::string form;
	while (total < apprSize)
	{
		peekHeader(file, form, size, type);
		if ("FORM" == form)
		{
			if ("EXBX" == type)
			{
				// Read bounding box - needs work, overwriting
				// previous.
				box box1;
				total += readEXBX(file, bsX, bsY, bsZ, bsRadius, box1);
				bbox.push_back(box1);
			}
			else if ("EXSP" == type)
			{
				float cx, cy, cz, radius;
				total += model::readEXSP(file, cx, cy, cz, radius);
			}
			else if ("XCYL" == type)
			{
				// Needs work.
				float u1, u2, u3, u4, u5;
				total += readXCYL(file, u1, u2, u3, u4, u5);
			}
			else if ("CMPT" == type)
			{
				// Needs work.
				total += readCMPT(file);
			}
			else if ("CMSH" == type)
			{
				// Needs work.
				total += readCMSH(file);
			}
			else if ("NULL" == type)
			{
				total += readNULL(file);
			}
			else if ("DTAL" == type)
			{
				total += readDTAL(file);
			}
			else if ("HPTS" == type)
			{
				total += readHPTS(file);
			}
			else if ("FLOR" == type)
			{
				total += readFLOR(file);
			}
			else if ("INFO" == type)
			{
				total += readUnknown(file, size + 8);
			}
			else
			{
				std::cout << "Expected form of type NULL, EXBX, EXSP, XCYL, CMPT, CMSH, DTAL, HPTS, FLOR or INFO: "
					<< type
					<< std::endl;
				exit(0);
			}
		}
		else
		{
			std::cout << "Expected FORM: " << form
				<< std::endl;
			exit(0);
		}
	}

	if (total == apprSize)
	{
		std::cout << "Finished reading APPR." << std::endl;
	}
	else
	{
		std::cout << "Error reading APPR!" << std::endl;
		std::cout << "Read " << total << " out of " << apprSize
			<< std::endl;
	}

	return total;
}

unsigned int model::readHPNT(std::istream& file)
{
	std::size_t size;
	std::string type;

	std::size_t total = readRecordHeader(file, type, size);
	size += 8; // Size of header
	if (type != "HPNT")
	{
		std::cout << "Expected record of type HPNT: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found HPNT record" << std::endl;

	float u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11, u12;
	total += read(file, u1);
	total += read(file, u2);
	total += read(file, u3);
	total += read(file, u4);
	total += read(file, u5);
	total += read(file, u6);
	total += read(file, u7);
	total += read(file, u8);
	total += read(file, u9);
	total += read(file, u10);
	total += read(file, u11);
	total += read(file, u12);

	std::string name;
	total += read(file, name);

	std::cout << u1 << " " << u2 << " " << u3 << " " << u4 << std::endl;
	std::cout << u5 << " " << u6 << " " << u7 << " " << u8 << std::endl;
	std::cout << u9 << " " << u10 << " " << u11 << " " << u12 << std::endl;
	std::cout << name << std::endl;

	if (total == size)
	{
		std::cout << "Finished reading HPNT." << std::endl;
	}
	else
	{
		std::cout << "Error reading HPNT!" << std::endl;
		std::cout << "Read " << total << " out of " << size
			<< std::endl;
	}

	return total;
}

unsigned int model::readNULL(std::istream& file)
{
	std::size_t nullSize;
	std::string type;

	std::size_t total = readFormHeader(file, "NULL", nullSize);
	nullSize += 8;
	std::cout << "Found NULL form" << std::endl;

	total += readUnknown(file, nullSize - total);

	if (total == nullSize)
	{
		std::cout << "Finished reading NULL." << std::endl;
	}
	else
	{
		std::cout << "Error reading NULL!" << std::endl;
		std::cout << "Read " << total << " out of " << nullSize
			<< std::endl;
	}

	return total;
}

unsigned int model::readHPTS(std::istream& file)
{
	std::size_t hptsSize;
	std::string type;

	std::size_t total = readFormHeader(file, "HPTS", hptsSize);
	hptsSize += 8;
	std::cout << "Found HPTS form" << std::endl;

	std::string form;
	std::size_t size;
	while (total < hptsSize)
	{
		peekHeader(file, form, size, type);

		if ("HPNT" == form)
		{
			total += readHPNT(file);
		}
		else if ("DYN " == form)
		{
			total += readDYN(file);
		}
	}

	if (total == hptsSize)
	{
		std::cout << "Finished reading HPTS." << std::endl;
	}
	else
	{
		std::cout << "Error reading HPTS!" << std::endl;
		std::cout << "Read " << total << " out of " << hptsSize
			<< std::endl;
	}

	return total;
}

unsigned int model::readFLOR(std::istream& file)
{
	std::size_t florSize;
	std::string type;

	std::size_t total = readFormHeader(file, "FLOR", florSize);
	florSize += 8;
	std::cout << "Found FLOR form" << std::endl;

	// Read DATA record
	std::size_t size;
	total += readRecordHeader(file, type, size);
	if (type != "DATA")
	{
		std::cout << "Expected record of type DATA: " << type << std::endl;
		exit(0);
	}
	std::cout << "Size: " << size << std::endl;

	unsigned char numFloor = 0;
	total += read(file, numFloor);

	for (unsigned char i = 0; i < numFloor; ++i)
	{
		std::string name;
		total += read(file, name);
		std::cout << name << std::endl;
	}

	if (total == florSize)
	{
		std::cout << "Finished reading FLOR." << std::endl;
	}
	else
	{
		std::cout << "Error reading FLOR!" << std::endl;
		std::cout << "Read " << total << " out of " << florSize
			<< std::endl;
	}

	return total;
}

unsigned int model::readCMPT(std::istream& file)
{
	std::size_t cmptSize;
	std::string type;

	std::size_t total = readFormHeader(file, "CMPT", cmptSize);
	cmptSize += 8;
	std::cout << "Found CMPT form" << std::endl;

	std::size_t size;
	total += readFormHeader(file, "0000", size);
	std::cout << "Found FORM " << type
		<< ": " << size - 4 << " bytes"
		<< std::endl;

	total += readCPST(file);

	if (cmptSize == total)
	{
		std::cout << "Finished reading CMPT" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading CMPT" << std::endl;
		std::cout << "Read " << total << " out of " << cmptSize
			<< std::endl;
	}

	return total;
}

unsigned int model::readCPST(std::istream& file)
{
	std::size_t cpstSize;

	std::size_t total = readFormHeader(file, "CPST", cpstSize);
	cpstSize += 8;
	std::cout << "Found CPST form" << std::endl;

	std::size_t size;
	std::string form;
	std::string type;
#if 0
	total += readFormHeader(file, form, size, type);
	if (form != "FORM")
	{
		std::cout << "Expected FORM not: " << form << std::endl;
		exit(0);
	}
	std::cout << "Found " << form << " " << type
		<< ": " << size - 4 << " bytes"
		<< std::endl;
#else
	total += readFormHeader(file, "0000", size);
#endif
	while (total < cpstSize)
	{
		// Peek at next record, but keep file at same place.
		peekHeader(file, form, size, type);

		if ("CMSH" == type)
		{
			total += readCMSH(file);
		}
		else if ("CMPT" == type)
		{
			total += readCMPT(file);
		}
		else if ("EXBX" == type)
		{
			float cx, cy, cz, radius;
			box box1;
			total += readEXBX(file, cx, cy, cz, radius, box1);
		}
		else if ("EXSP" == type)
		{
			float cx, cy, cz, radius;
			total += model::readEXSP(file, cx, cy, cz, radius);
		}
		else if ("XCYL" == type)
		{
			float u1, u2, u3, u4, u5;
			total += readXCYL(file, u1, u2, u3, u4, u5);
		}
		else
		{
			std::cout << "Unexpected type: " << type << std::endl;
			exit(0);
		}
	}

	if (cpstSize == total)
	{
		std::cout << "Finished reading CPST" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading CPST" << std::endl;
		std::cout << "Read " << total << " out of " << cpstSize
			<< std::endl;
	}

	return total;
}

unsigned int model::readCMSH(std::istream& file)
{
	std::size_t cmshSize;

	std::size_t total = readFormHeader(file, "CMSH", cmshSize);
	cmshSize += 8;
	std::cout << "Found FORM CMSH: "
		<< cmshSize - 12 << " bytes"
		<< std::endl;

	std::string form;
	std::string type;
	std::size_t size;
	total += readFormHeader(file, form, size, type);
	if (form != "FORM")
	{
		std::cout << "Expected FORM not: " << form << std::endl;
		exit(0);
	}
	std::cout << "Found " << form << " " << type
		<< ": " << size - 4 << " bytes"
		<< std::endl;

	std::vector<vector3> vec;
	std::vector<int32_t> index;

	total += readIDTL(file, vec, index);

	if (cmshSize == total)
	{
		std::cout << "Finished reading CMSH" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading CMSH" << std::endl;
		std::cout << "Read " << total << " out of " << cmshSize
			<< std::endl;
	}

	return total;
}

unsigned int model::readDTAL(std::istream& file)
{
	std::string form;
	std::size_t dtalSize;
	std::string type;

	std::size_t total = readFormHeader(file, "DTAL", dtalSize);
	dtalSize += 8;
	std::cout << "Found " << form << " " << type
		<< ": " << dtalSize - 12 << " bytes"
		<< std::endl;

	std::size_t size;
	total += readFormHeader(file, form, size, type);
	if (form != "FORM")
	{
		std::cout << "Expected FORM not: " << form << std::endl;
		exit(0);
	}
	std::cout << "Found " << form << " " << type
		<< ": " << size - 4 << " bytes"
		<< std::endl;

	total += readCPST(file);

	if (dtalSize == total)
	{
		std::cout << "Finished reading DTAL" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading DTAL" << std::endl;
		std::cout << "Read " << total << " out of " << dtalSize
			<< std::endl;
	}

	return total;
}

unsigned int model::readDYN(std::istream& file)
{
	std::size_t size;
	std::string type;
	std::size_t total = readRecordHeader(file, type, size);
	size += 8; // Size of header
	if (type != "DYN ")
	{
		std::cout << "Expected record of type DYN : " << type << std::endl;
		exit(0);
	}
	std::cout << "Found DYN  record" << std::endl;

	total += readUnknown(file, size - total);

	if (total == size)
	{
		std::cout << "Finished reading DYN ." << std::endl;
	}
	else
	{
		std::cout << "Error reading DYN !" << std::endl;
		std::cout << "Read " << total << " out of " << size
			<< std::endl;
	}

	return total;
}
