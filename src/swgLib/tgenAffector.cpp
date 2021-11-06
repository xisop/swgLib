/** -*-c++-*-
 *  \class  tgenAffector
 *  \file   tgenAffector.cpp
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

#include <swgLib/tgenAffector.hpp>
#include <swgLib/base.hpp>

using namespace ml;

bool tgenAffector::peekAffector(std::istream& file) {
	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);

	return (("AHSM" == type) || // Affector Height Smoother (unused)
		("AHBM" == type) || // Affector Height Bitmap (unused)
		("ACBM" == type) || // Affector Color Bitmap (unused)
		("ASBM" == type) || // Affector Shader Bitmap (unused)
		("AFBM" == type) || // Affector Flora Bitmap (unused)
		("AENV" == type) || // Affector Environment
		("AHTR" == type) || // Affector Height Terrace
		("AHCN" == type) || // Affector Height Constant
		("AHFR" == type) || // Affector Height Fractal
		("ACCN" == type) || // Affector Color Constant
		("ACRH" == type) || // Affector Color Ramp Height
		("ACRF" == type) || // Affector Color Ramp Fractal
		("ASCN" == type) || // Affector Shader Constant
		("ASRP" == type) || // Affector Shader Replace
		("AFCN" == type) || // Affector Flora Static Collidable Constant - Same as AFSC?
		("AFSC" == type) || // Affector Flora Static Collidable Constant - Same as AFCN?
		("AFSN" == type) || // Affector Flora Static Non Collidable Constant
		("ARCN" == type) || // Affector Flora Dynamic Near Constant - Same as AFDN
		("AFDN" == type) || // Affector Flora Dynamic Near Constant - Same as ARCN
		("AFDF" == type) || // Affector Flora Dynamic Far Constant
		("ARIB" == type) || // Affector Ribbon
		("AEXC" == type) || // Affector Exclude
		("APAS" == type) || // Affector Passable
		("AROA" == type) || // Affector Road
		("ARIV" == type)); // Affector River
}

std::size_t tgenAffector::read(std::istream& file) {
	return 0;
}

std::size_t tgenAffector::read(std::istream& file, tgenAffectorPtr& affectorPtr) {
	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);

	if ("FORM" == form) {
		if ("AENV" == type) {
			affectorPtr = affectorEnvironmentPtr(new ml::affectorEnvironment);
		}
		else if ("AHTR" == type) {
			affectorPtr = affectorHeightTerracePtr(new ml::affectorHeightTerrace);
		}
		else if ("AHCN" == type) {
			affectorPtr = affectorHeightConstantPtr(new ml::affectorHeightConstant);
		}
		else if ("AHFR" == type) {
			affectorPtr = affectorHeightFractalPtr(new ml::affectorHeightFractal);
		}
		else if ("ACCN" == type) {
			affectorPtr = affectorColorConstantPtr(new ml::affectorColorConstant);
		}
		else if ("ACRH" == type) {
			affectorPtr = affectorColorRampHeightPtr(new ml::affectorColorRampHeight);
		}
		else if ("ACRF" == type) {
			affectorPtr = affectorColorFractalPtr(new ml::affectorColorFractal);
		}
		else if ("ASCN" == type) {
			affectorPtr = affectorShaderConstantPtr(new ml::affectorShaderConstant);
		}
		else if ("ASRP" == type) {
			affectorPtr = affectorShaderReplacePtr(new ml::affectorShaderReplace);
		}
		else if (("AFCN" == type) || ("AFSC" == type)) {
			affectorPtr = affectorFloraSCCPtr(new ml::affectorFloraSCC);
		}
		else if ("AFSN" == type) {
			affectorPtr = affectorFloraSNCCPtr(new ml::affectorFloraSNCC);
		}
		else if (("ARCN" == type) || ("AFDN" == type)) {
			affectorPtr = affectorFDNCPtr(new ml::affectorFDNC);
		}
		else if ("AFDF" == type) {
			affectorPtr = affectorFDFCPtr(new ml::affectorFDFC);
		}
		else if ("ARIB" == type) {
			affectorPtr = affectorRibbonPtr(new ml::affectorRibbon);
		}
		else if ("AEXC" == type) {
			affectorPtr = affectorExcludePtr(new ml::affectorExclude);
		}
		else if ("APAS" == type) {
			affectorPtr = affectorPassablePtr(new ml::affectorPassable);
		}
		else if ("AROA" == type) {
			affectorPtr = affectorRoadPtr(new ml::affectorRoad);
		}
		else if ("ARIV" == type) {
			affectorPtr = affectorRiverPtr(new ml::affectorRiver);
		}

		if (affectorPtr) {
			return affectorPtr->read(file);
		}
	}
	else {
		std::cout << "Expected FORM instead of record: " << type << "\n";
		return 0;
	}

	return 0;
}

// ****************************** Environment ******************************

affectorEnvironment::affectorEnvironment() :
	tgenAffector() {
}

affectorEnvironment::~affectorEnvironment() {
}

std::size_t affectorEnvironment::read(std::istream& file) {
	std::size_t aenvSize;
	std::size_t total = base::readFormHeader(file, "AENV", aenvSize);
	aenvSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 0) {
		std::cout << "Unexpected AENV version: " << version << "\n";
		exit(0);
	}
	std::cout << "AENV version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _familyId);
	int32_t temp; total += base::read(file, temp); _useFeatherClampOverride = (0 != temp);
	total += base::read(file, _featherClampOverride);

	if (aenvSize != total) {
		std::cout << "Failed in reading AENV\n";
		std::cout << "Read " << total << " out of " << aenvSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Height Terrace ******************************

affectorHeightTerrace::affectorHeightTerrace() :
	tgenAffector() {
}

affectorHeightTerrace::~affectorHeightTerrace() {
}

std::size_t affectorHeightTerrace::read(std::istream& file) {
	std::size_t ahtrSize;
	std::size_t total = base::readFormHeader(file, "AHTR", ahtrSize);
	ahtrSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 4) {
		std::cout << "Unexpected AHTR version: " << version << "\n";
		exit(0);
	}
	std::cout << "AHTR version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	if (3 == version) {
		total += _multifractal.read(file);
	}

	total += base::read(file, _fraction);
	total += base::read(file, _height);

	if (3 == version) {
		total += base::skip(file, sizeof(int32_t));
	}

	if (ahtrSize != total) {
		std::cout << "Failed in reading AHTR\n";
		std::cout << "Read " << total << " out of " << ahtrSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Height Constant ******************************

affectorHeightConstant::affectorHeightConstant() :
	tgenAffector() {
}

affectorHeightConstant::~affectorHeightConstant() {
}

std::size_t affectorHeightConstant::read(std::istream& file) {
	std::size_t ahcnSize;
	std::size_t total = base::readFormHeader(file, "AHCN", ahcnSize);
	ahcnSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 0) {
		std::cout << "Unexpected AHCN version: " << version << "\n";
		exit(0);
	}
	std::cout << "AHCN version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _operation);
	total += base::read(file, _height);

	if (ahcnSize != total) {
		std::cout << "Failed in reading AHCN\n";
		std::cout << "Read " << total << " out of " << ahcnSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Height Fractal ******************************

affectorHeightFractal::affectorHeightFractal() :
	tgenAffector() {
}

affectorHeightFractal::~affectorHeightFractal() {
}

std::size_t affectorHeightFractal::read(std::istream& file) {
	std::size_t ahfrSize;
	std::size_t total = base::readFormHeader(file, "AHFR", ahfrSize);
	ahfrSize += 8;

	std::size_t size;
	std::string form, type;
	base::peekHeader(file, form, size, type);

	uint32_t version = base::tagToVersion(type);
	if (version > 3) {
		std::cout << "Unexpected AHFR version: " << version << "\n";
		exit(0);
	}
	std::cout << "Affector Height Fractal version: " << version << "\n";

	switch (version) {
	case 0: total += readV0(file); break;
	case 1: total += readV1(file); break;
	case 2: total += readV2(file); break;
	case 3: total += readV3(file); break;
	}

	if (ahfrSize != total) {
		std::cout << "Failed in reading AHFR\n";
		std::cout << "Read " << total << " out of " << ahfrSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorHeightFractal::readV0(std::istream& file) {
	std::size_t size0000;
	std::size_t total = base::readFormHeader(file, "0000", size0000);
	size0000 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _operation);

	int32_t fractalType;
	total += base::read(file, fractalType);
	_multifractal.setCombinationRule(fractalType);

	uint32_t seed;
	total += base::read(file, seed);
	_multifractal.setSeed(seed);

	float scaleX, scaleZ;
	total += base::read(file, scaleX);
	total += base::read(file, _scaleVertical);
	total += base::read(file, scaleZ);

	_multifractal.setScale(scaleX, scaleZ);

	if (size0000 != total) {
		std::cout << "Failed in reading 0000\n";
		std::cout << "Read " << total << " out of " << size0000 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorHeightFractal::readV1(std::istream& file) {
	std::size_t size0001;
	std::size_t total = base::readFormHeader(file, "0001", size0001);
	size0001 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _operation);

	int32_t fractalType;
	total += base::read(file, fractalType);
	_multifractal.setCombinationRule(fractalType);

	int32_t numOctaves;
	total += base::read(file, numOctaves);
	_multifractal.setNumOctaves(numOctaves);

	float frequency;
	total += base::read(file, frequency);
	_multifractal.setFrequency(frequency);

	float scaleX, scaleZ;
	int32_t count;
	total += base::read(file, count);

	for (int32_t i = 0; i < count; ++i) {
		total += base::skip(file, sizeof(int32_t));
		total += base::read(file, scaleX);
		total += base::read(file, scaleZ);
	}

	uint32_t seed;
	total += base::read(file, seed);
	_multifractal.setSeed(seed);

	for (int32_t i = 0; i < numOctaves; ++i) {
		total += base::skip(file, sizeof(int32_t));
		total += base::skip(file, sizeof(int32_t));
	}

	total += base::read(file, _scaleVertical);
	_multifractal.setScale(scaleX, scaleZ);

	if (size0001 != total) {
		std::cout << "Failed in reading 0001\n";
		std::cout << "Read " << total << " out of " << size0001 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorHeightFractal::readV2(std::istream& file) {
	std::size_t size0002;
	std::size_t total = base::readFormHeader(file, "0002", size0002);
	size0002 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _multifractal.read(file);

	total += base::readRecordHeader(file, "PARM", size);

	total += base::read(file, _operation);

	total += base::read(file, _scaleVertical);

	if (size0002 != total) {
		std::cout << "Failed in reading 0002\n";
		std::cout << "Read " << total << " out of " << size0002 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorHeightFractal::readV3(std::istream& file) {
	std::size_t size0003;
	std::size_t total = base::readFormHeader(file, "0003", size0003);
	size0003 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += base::readRecordHeader(file, "PARM", size);

	total += base::read(file, _familyId);
	total += base::read(file, _operation);
	total += base::read(file, _scaleVertical);

	if (size0003 != total) {
		std::cout << "Failed in reading 0003\n";
		std::cout << "Read " << total << " out of " << size0003 << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Color Constant ******************************

affectorColorConstant::affectorColorConstant() :
	tgenAffector() {
}

affectorColorConstant::~affectorColorConstant() {
}

std::size_t affectorColorConstant::read(std::istream& file) {
	std::size_t accnSize;
	std::size_t total = base::readFormHeader(file, "ACCN", accnSize);
	accnSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 0) {
		std::cout << "Unexpected ACCN version: " << version << "\n";
		exit(0);
	}
	std::cout << "ACCN version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _operation);
	total += base::read(file, _r);
	total += base::read(file, _g);
	total += base::read(file, _b);

	if (accnSize != total) {
		std::cout << "Failed in reading ACCN\n";
		std::cout << "Read " << total << " out of " << accnSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Color Constant ******************************

affectorColorRampHeight::affectorColorRampHeight() :
	tgenAffector() {
}

affectorColorRampHeight::~affectorColorRampHeight() {
}

std::size_t affectorColorRampHeight::read(std::istream& file) {
	std::size_t acrhSize;
	std::size_t total = base::readFormHeader(file, "ACRH", acrhSize);
	acrhSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 0) {
		std::cout << "Unexpected ACRH version: " << version << "\n";
		exit(0);
	}
	std::cout << "ACRH version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _operation);
	total += base::read(file, _low);
	total += base::read(file, _high);
	total += base::read(file, _imageName);

	if (acrhSize != total) {
		std::cout << "Failed in reading ACRH\n";
		std::cout << "Read " << total << " out of " << acrhSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Color Fractal ******************************

affectorColorFractal::affectorColorFractal() :
	tgenAffector() {
}

affectorColorFractal::~affectorColorFractal() {
}

std::size_t affectorColorFractal::read(std::istream& file) {
	std::size_t acrfSize;
	std::size_t total = base::readFormHeader(file, "ACRF", acrfSize);
	acrfSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 1) {
		std::cout << "Unexpected ACRF version: " << version << "\n";
		exit(0);
	}
	std::cout << "ACRF version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readFormHeader(file, "DATA", size);

	if (0 == version) {
		total += _multifractal.read(file);
	}

	total += base::readRecordHeader(file, "PARM", size);

	if (1 == version) {
		total += base::read(file, _familyId);
	}

	total += base::read(file, _operation);
	total += base::read(file, _imageName);

	if (acrfSize != total) {
		std::cout << "Failed in reading ACRF\n";
		std::cout << "Read " << total << " out of " << acrfSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Shader Constant ******************************

affectorShaderConstant::affectorShaderConstant() :
	tgenAffector() {
}

affectorShaderConstant::~affectorShaderConstant() {
}

std::size_t affectorShaderConstant::read(std::istream& file) {
	std::size_t ascnSize;
	std::size_t total = base::readFormHeader(file, "ASCN", ascnSize);
	ascnSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 1) {
		std::cout << "Unexpected ASCN version: " << version << "\n";
		exit(0);
	}
	std::cout << "ASCN version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _familyId);
	int32_t temp; total += base::read(file, temp); _useFeatherClampOverride = (0 != temp);
	total += base::read(file, _featherClampOverride);

	if (ascnSize != total) {
		std::cout << "Failed in reading ASCN\n";
		std::cout << "Read " << total << " out of " << ascnSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Shader Replace ******************************

affectorShaderReplace::affectorShaderReplace() :
	tgenAffector() {
}

affectorShaderReplace::~affectorShaderReplace() {
}

std::size_t affectorShaderReplace::read(std::istream& file) {
	std::size_t asrpSize;
	std::size_t total = base::readFormHeader(file, "ASRP", asrpSize);
	asrpSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 1) {
		std::cout << "Unexpected ASRP version: " << version << "\n";
		exit(0);
	}
	std::cout << "ASRP version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _sourceFamilyId);
	total += base::read(file, _destinationFamilyId);
	int32_t temp; total += base::read(file, temp); _useFeatherClampOverride = (0 != temp);
	total += base::read(file, _featherClampOverride);

	if (asrpSize != total) {
		std::cout << "Failed in reading ASRP\n";
		std::cout << "Read " << total << " out of " << asrpSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Flora Static Collidable Constant ******************************

affectorFloraSCC::affectorFloraSCC() :
	tgenAffector(),
	_operation(0) {
}

affectorFloraSCC::~affectorFloraSCC() {
}

std::size_t affectorFloraSCC::read(std::istream& file) {
	std::size_t fsccSize;
	std::string fsccType;
	std::size_t total = base::readFormHeader(file, fsccType, fsccSize);
	fsccSize += 8;
	if (("AFCN" != fsccType) && ("AFSC" != fsccType)) {
		std::cout << "Expected types AFCN or AFSC. Found type: " << fsccType << "\n";
		exit(0);
	}

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 4) {
		std::cout << "Unexpected AFCN or AFSC version: " << version << "\n";
		exit(0);
	}
	std::cout << "AFCN version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	if (version > 0) {
		total += base::read(file, _familyId);
	}

	if (version < 2) {
		_operation = 0;
	}
	else {
		total += base::read(file, _operation);
	}

	if (version > 2) {
		int32_t temp; total += base::read(file, temp); _removeAll = (0 != temp);
	}

	if (version > 3) {
		int32_t temp; total += base::read(file, temp); _useDensityOverride = (0 != temp);
		total += base::read(file, _densityOverride);
	}

	if (fsccSize != total) {
		std::cout << "Failed in reading AFCN or AFSC\n";
		std::cout << "Read " << total << " out of " << fsccSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Flora Static Non-Collidable Constant ******************************

affectorFloraSNCC::affectorFloraSNCC() :
	tgenAffector(),
	_operation(0) {
}

affectorFloraSNCC::~affectorFloraSNCC() {
}

std::size_t affectorFloraSNCC::read(std::istream& file) {
	std::size_t afsnSize;
	std::size_t total = base::readFormHeader(file, "AFSN", afsnSize);
	afsnSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 4) {
		std::cout << "Unexpected AFSN version: " << version << "\n";
		exit(0);
	}
	std::cout << "AFSN version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	if (version > 0) {
		total += base::read(file, _familyId);
	}

	if (version < 2) {
		_operation = 0;
	}
	else {
		total += base::read(file, _operation);
	}

	if (version > 2) {
		int32_t temp; total += base::read(file, temp); _removeAll = (0 != temp);
	}

	if (version > 3) {
		int32_t temp; total += base::read(file, temp); _useDensityOverride = (0 != temp);
		total += base::read(file, _densityOverride);
	}

	if (afsnSize != total) {
		std::cout << "Failed in reading AFSN\n";
		std::cout << "Read " << total << " out of " << afsnSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Flora Dynamic Near Constant ******************************

affectorFDNC::affectorFDNC() :
	tgenAffector(),
	_operation(0) {
}

affectorFDNC::~affectorFDNC() {
}

std::size_t affectorFDNC::read(std::istream& file) {
	std::string fdncType;
	std::size_t fdncSize;
	std::size_t total = base::readFormHeader(file, fdncType, fdncSize);
	fdncSize += 8;
	if (("ARCN" != fdncType) && ("AFDN" != fdncType)) {
		std::cout << "Expected types ARCN or AFDN. Found type: " << fdncType << "\n";
		exit(0);
	}

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 3) {
		std::cout << "Unexpected ARCN or AFDN version: " << version << "\n";
		exit(0);
	}
	std::cout << "ARCN/AFDN version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _familyId);

	if (version > 0) {
		total += base::read(file, _operation);
		int32_t temp; total += base::read(file, temp); _removeAll = (0 != temp);
	}

	if (version > 1) {
		int32_t temp; total += base::read(file, temp); _useDensityOverride = (0 != temp);
		total += base::read(file, _densityOverride);
	}

	if (fdncSize != total) {
		std::cout << "Failed in reading ARCN or AFDN\n";
		std::cout << "Read " << total << " out of " << fdncSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Flora Dynamic Far Constant ******************************

affectorFDFC::affectorFDFC() :
	tgenAffector(),
	_operation(0) {
}

affectorFDFC::~affectorFDFC() {
}

std::size_t affectorFDFC::read(std::istream& file) {
	std::size_t afdfSize;
	std::size_t total = base::readFormHeader(file, "AFDF", afdfSize);
	afdfSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 3) {
		std::cout << "Unexpected AFDF version: " << version << "\n";
		exit(0);
	}
	std::cout << "AFDF version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _familyId);

	if (version > 0) {
		total += base::read(file, _operation);
		int32_t temp; total += base::read(file, temp); _removeAll = (0 != temp);
	}

	if (version > 1) {
		int32_t temp; total += base::read(file, temp); _useDensityOverride = (0 != temp);
		total += base::read(file, _densityOverride);
	}

	if (afdfSize != total) {
		std::cout << "Failed in reading AFDF\n";
		std::cout << "Read " << total << " out of " << afdfSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Ribbon ******************************

affectorRibbon::affectorRibbon() :
	tgenAffector() {
}

affectorRibbon::~affectorRibbon() {
}

std::size_t affectorRibbon::read(std::istream& file) {
	std::size_t aribSize;
	std::size_t total = base::readFormHeader(file, "ARIB", aribSize);
	aribSize += 8;

	std::size_t size;
	std::string form, type;
	base::peekHeader(file, form, size, type);

	uint32_t version = base::tagToVersion(type);
	if (version > 5) {
		std::cout << "Unexpected ARIB version: " << version << "\n";
		exit(0);
	}
	std::cout << "ARIB version: " << version << "\n";

	switch (version) {
	case 0: total += readV0(file); break;
	case 1: total += readV1(file); break;
	case 2: total += readV2(file); break;
	case 3: total += readV3(file); break;
	case 4: total += readV4(file); break;
	case 5: total += readV5(file); break;
	}

	if (aribSize != total) {
		std::cout << "Failed in reading ARIB\n";
		std::cout << "Read " << total << " out of " << aribSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRibbon::readV0(std::istream& file) {

	std::size_t size0000;
	std::size_t total = base::readFormHeader(file, "0000", size0000);
	size0000 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _heightData.read(file);

	total += base::readRecordHeader(file, "DATA", size);

	int32_t count;
	total += base::read(file, count);
	_points.resize(count);
	for (int32_t i = 0; i < count; ++i) {
		total += base::read(file, _points[i].x);
		total += base::read(file, _points[i].y);
		_points[i].height = 0.0f;
	}

	total += base::read(file, _width);
	total += base::skip(file, sizeof(float));
	_waterShaderSize = 64.0f;
	_velocity = 1.0f;
	_capWidth = _width;
	total += base::skip(file, sizeof(int32_t));

	total += base::read(file, _waterShaderName);

	if (size0000 != total) {
		std::cout << "Failed in reading 0000\n";
		std::cout << "Read " << total << " out of " << size0000 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRibbon::readV1(std::istream& file) {

	std::size_t size0001;
	std::size_t total = base::readFormHeader(file, "0001", size0001);
	size0001 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _heightData.read(file);

	total += base::readRecordHeader(file, "DATA", size);

	int32_t count;
	total += base::read(file, count);
	_points.resize(count);
	for (int32_t i = 0; i < count; ++i) {
		total += base::read(file, _points[i].x);
		total += base::read(file, _points[i].y);
		_points[i].height = 0.0f;
	}

	total += base::read(file, _width);
	total += base::skip(file, sizeof(float));
	total += base::read(file, _waterShaderSize);
	total += base::read(file, _velocity);
	_capWidth = _width;
	total += base::skip(file, sizeof(int32_t));

	total += base::read(file, _waterShaderName);

	if (size0001 != total) {
		std::cout << "Failed in reading 0001\n";
		std::cout << "Read " << total << " out of " << size0001 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRibbon::readV2(std::istream& file) {

	std::size_t size0002;
	std::size_t total = base::readFormHeader(file, "0002", size0002);
	size0002 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _heightData.read(file);

	total += base::readRecordHeader(file, "DATA", size);

	int32_t count;
	total += base::read(file, count);
	_points.resize(count);
	for (int32_t i = 0; i < count; ++i) {
		total += base::read(file, _points[i].x);
		total += base::read(file, _points[i].y);
		_points[i].height = 0.0f;
	}

	total += base::read(file, _width);
	total += base::skip(file, sizeof(float));
	total += base::read(file, _waterShaderSize);
	total += base::read(file, _velocity);
	_capWidth = _width;
	total += base::skip(file, sizeof(int32_t));

	total += base::read(file, _waterShaderName);
	total += base::read(file, _waterType);

	if (size0002 != total) {
		std::cout << "Failed in reading 0002\n";
		std::cout << "Read " << total << " out of " << size0002 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRibbon::readV3(std::istream& file) {

	std::size_t size0003;
	std::size_t total = base::readFormHeader(file, "0003", size0003);
	size0003 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);
	total += base::readRecordHeader(file, "DATA", size);

	int32_t count;
	total += base::read(file, count);
	_points.resize(count);
	for (int32_t i = 0; i < count; ++i) {
		total += base::read(file, _points[i].x);
		total += base::read(file, _points[i].y);
		total += base::read(file, _points[i].height);
	}

	total += base::read(file, _width);
	total += base::read(file, _waterShaderSize);
	total += base::read(file, _velocity);
	total += base::read(file, _capWidth);
	total += base::skip(file, sizeof(int32_t));

	total += base::read(file, _waterShaderName);
	total += base::read(file, _waterType);

	if (size0003 != total) {
		std::cout << "Failed in reading 0003\n";
		std::cout << "Read " << total << " out of " << size0003 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRibbon::readV4(std::istream& file) {

	std::size_t size0004;
	std::size_t total = base::readFormHeader(file, "0004", size0004);
	size0004 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);
	total += base::readRecordHeader(file, "DATA", size);

	int32_t count;
	total += base::read(file, count);
	_points.resize(count);
	for (int32_t i = 0; i < count; ++i) {
		total += base::read(file, _points[i].x);
		total += base::read(file, _points[i].y);
		total += base::read(file, _points[i].height);
	}

	total += base::read(file, _width);
	total += base::read(file, _waterShaderSize);
	total += base::read(file, _velocity);
	total += base::read(file, _capWidth);

	total += base::read(file, _waterShaderName);
	total += base::read(file, _waterType);

	if (size0004 != total) {
		std::cout << "Failed in reading 0004\n";
		std::cout << "Read " << total << " out of " << size0004 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRibbon::readV5(std::istream& file) {

	std::size_t size0005;
	std::size_t total = base::readFormHeader(file, "0005", size0005);
	size0005 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);
	total += base::readRecordHeader(file, "DATA", size);

	int32_t count;
	total += base::read(file, count);
	_points.resize(count);
	for (int32_t i = 0; i < count; ++i) {
		total += base::read(file, _points[i].x);
		total += base::read(file, _points[i].y);
		total += base::read(file, _points[i].height);
	}

	total += base::read(file, _width);
	total += base::read(file, _waterShaderSize);
	total += base::read(file, _velocity);
	total += base::read(file, _capWidth);
	total += base::read(file, _terrainShaderFamilyId);
	total += base::read(file, _featherDistanceTerrainShader);
	total += base::read(file, _waterShaderName);
	total += base::read(file, _waterType);

	if (size0005 != total) {
		std::cout << "Failed in reading 0005\n";
		std::cout << "Read " << total << " out of " << size0005 << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Exclude ******************************

affectorExclude::affectorExclude() :
	tgenAffector() {
}

affectorExclude::~affectorExclude() {
}

std::size_t affectorExclude::read(std::istream& file) {
	std::size_t aexcSize;
	std::size_t total = base::readFormHeader(file, "AEXC", aexcSize);
	aexcSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 0) {
		std::cout << "Unexpected AEXC version: " << version << "\n";
		exit(0);
	}
	std::cout << "AEXC version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	if (aexcSize != total) {
		std::cout << "Failed in reading AEXC\n";
		std::cout << "Read " << total << " out of " << aexcSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Passable ******************************

affectorPassable::affectorPassable() :
	tgenAffector() {
}

affectorPassable::~affectorPassable() {
}

std::size_t affectorPassable::read(std::istream& file) {
	std::size_t apasSize;
	std::size_t total = base::readFormHeader(file, "APAS", apasSize);
	apasSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 0) {
		std::cout << "Unexpected APAS version: " << version << "\n";
		exit(0);
	}
	std::cout << "APAS version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);
	total += base::read(file, _passable);
	total += base::read(file, _featherThreshold);

	if (apasSize != total) {
		std::cout << "Failed in reading APAS\n";
		std::cout << "Read " << total << " out of " << apasSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Road ******************************

affectorRoad::affectorRoad() :
	tgenAffector() {
}

affectorRoad::~affectorRoad() {
}

std::size_t affectorRoad::read(std::istream& file) {
	std::size_t aroaSize;
	std::size_t total = base::readFormHeader(file, "AROA", aroaSize);
	aroaSize += 8;

	std::size_t size;
	std::string form, type;
	base::peekHeader(file, form, size, type);

	uint32_t version = base::tagToVersion(type);
	if (version > 6) {
		std::cout << "Unexpected AROA version: " << version << "\n";
		exit(0);
	}
	std::cout << "AROA version: " << version << "\n";

	switch (version) {
	case 0: total += readV0(file); break;
	case 1: total += readV1(file); break;
	case 2: total += readV2(file); break;
	case 3: total += readV3(file); break;
	case 4: total += readV4(file); break;
	case 5: total += readV5(file); break;
	case 6: total += readV6(file); break;
	}

	if (aroaSize != total) {
		std::cout << "Failed in reading AROA\n";
		std::cout << "Read " << total << " out of " << aroaSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRoad::readV0(std::istream& file) {

	std::size_t size0000;
	std::size_t total = base::readFormHeader(file, "0000", size0000);
	size0000 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readRecordHeader(file, "DATA", size);

	if (size0000 != total) {
		std::cout << "Failed in reading 0000\n";
		std::cout << "Read " << total << " out of " << size0000 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRoad::readV1(std::istream& file) {

	std::size_t size0001;
	std::size_t total = base::readFormHeader(file, "0001", size0001);
	size0001 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readRecordHeader(file, "DATA", size);

	_points.resize(2);
	total += base::read(file, _points[0].x);
	total += base::read(file, _points[0].y);
	total += base::read(file, _points[1].x);
	total += base::read(file, _points[1].y);

	total += base::read(file, _width);

	if (size0001 != total) {
		std::cout << "Failed in reading 0001\n";
		std::cout << "Read " << total << " out of " << size0001 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRoad::readV2(std::istream& file) {

	std::size_t size0002;
	std::size_t total = base::readFormHeader(file, "0002", size0002);
	size0002 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _heightData.read(file);

	total += base::readRecordHeader(file, "DATA", size);

	_points.resize(2);
	total += base::read(file, _points[0].x);
	total += base::read(file, _points[0].y);
	total += base::read(file, _points[1].x);
	total += base::read(file, _points[1].y);

	total += base::read(file, _width);

	if (size0002 != total) {
		std::cout << "Failed in reading 0002\n";
		std::cout << "Read " << total << " out of " << size0002 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRoad::readV3(std::istream& file) {

	std::size_t size0003;
	std::size_t total = base::readFormHeader(file, "0003", size0003);
	size0003 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _heightData.read(file);

	total += base::readRecordHeader(file, "DATA", size);

	_points.resize(2);
	total += base::read(file, _points[0].x);
	total += base::read(file, _points[0].y);
	total += base::read(file, _points[1].x);
	total += base::read(file, _points[1].y);

	total += base::read(file, _width);
	total += base::read(file, _familyId);
	total += base::read(file, _featherFunction);
	total += base::read(file, _featherDistance);
	_featherFunctionShader = _featherFunction;
	_featherDistanceShader = _featherDistance;

	if (size0003 != total) {
		std::cout << "Failed in reading 0003\n";
		std::cout << "Read " << total << " out of " << size0003 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRoad::readV4(std::istream& file) {

	std::size_t size0004;
	std::size_t total = base::readFormHeader(file, "0004", size0004);
	size0004 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _heightData.read(file);

	total += base::readRecordHeader(file, "DATA", size);

	int32_t count;
	total += base::read(file, count);
	_points.resize(count);
	for (auto& p : _points) {
		total += base::read(file, p.x);
		total += base::read(file, p.y);
	}

	total += base::read(file, _width);
	total += base::read(file, _familyId);
	total += base::read(file, _featherFunction);
	total += base::read(file, _featherDistance);
	_featherFunctionShader = _featherFunction;
	_featherDistanceShader = _featherDistance;

	if (size0004 != total) {
		std::cout << "Failed in reading 0004\n";
		std::cout << "Read " << total << " out of " << size0004 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRoad::readV5(std::istream& file) {

	std::size_t size0005;
	std::size_t total = base::readFormHeader(file, "0005", size0005);
	size0005 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _heightData.read(file);

	total += base::readRecordHeader(file, "DATA", size);

	int32_t count;
	total += base::read(file, count);
	_points.resize(count);
	for (auto& p : _points) {
		total += base::read(file, p.x);
		total += base::read(file, p.y);
	}

	total += base::read(file, _width);
	total += base::read(file, _familyId);
	total += base::read(file, _featherFunction);
	total += base::read(file, _featherDistance);
	total += base::read(file, _featherFunctionShader);
	total += base::read(file, _featherDistanceShader);

	if (size0005 != total) {
		std::cout << "Failed in reading 0005\n";
		std::cout << "Read " << total << " out of " << size0005 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRoad::readV6(std::istream& file) {

	std::size_t size0006;
	std::size_t total = base::readFormHeader(file, "0006", size0006);
	size0006 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _heightData.read(file);

	total += base::readRecordHeader(file, "DATA", size);

	int32_t count;
	total += base::read(file, count);
	_points.resize(count);
	for (auto& p : _points) {
		total += base::read(file, p.x);
		total += base::read(file, p.y);
	}

	total += base::read(file, count);
	_height.resize(count);
	for (auto& h : _height) {
		total += base::read(file, h);
	}

	total += base::read(file, _width);
	total += base::read(file, _familyId);
	total += base::read(file, _featherFunction);
	total += base::read(file, _featherDistance);
	total += base::read(file, _featherFunctionShader);
	total += base::read(file, _featherDistanceShader);
	int32_t temp; total += base::read(file, temp); _hasFixedHeights = (0 != temp);

	if (size0006 != total) {
		std::cout << "Failed in reading 0006\n";
		std::cout << "Read " << total << " out of " << size0006 << "\n";
		exit(0);
	}

	return total;
}

// ****************************** River ******************************

affectorRiver::affectorRiver() :
	tgenAffector() {
}

affectorRiver::~affectorRiver() {
}

std::size_t affectorRiver::read(std::istream& file) {
	std::size_t arivSize;
	std::size_t total = base::readFormHeader(file, "ARIV", arivSize);
	arivSize += 8;

	std::size_t size;
	std::string form, type;
	base::peekHeader(file, form, size, type);

	uint32_t version = base::tagToVersion(type);
	if (version > 6) {
		std::cout << "Unexpected ARIV version: " << version << "\n";
		exit(0);
	}
	std::cout << "ARIV version: " << version << "\n";

	switch (version) {
	case 0: total += readV0(file); break;
	case 1: total += readV1(file); break;
	case 2: total += readV2(file); break;
	case 3: total += readV3(file); break;
	case 4: total += readV4(file); break;
	case 5: total += readV5(file); break;
	case 6: total += readV6(file); break;
	}

	if (arivSize != total) {
		std::cout << "Failed in reading ARIV\n";
		std::cout << "Read " << total << " out of " << arivSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRiver::readV0(std::istream& file) {

	std::size_t size0000;
	std::size_t total = base::readFormHeader(file, "0000", size0000);
	size0000 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _heightData.read(file);

	total += base::readRecordHeader(file, "DATA", size);

	_points.resize(2);
	total += base::read(file, _points[0].x);
	total += base::read(file, _points[0].y);
	total += base::read(file, _points[1].x);
	total += base::read(file, _points[1].y);

	total += base::read(file, _width);
	total += base::read(file, _bankFamilyId);
	_bottomFamilyId = _bankFamilyId;
	total += base::read(file, _featherFunction);
	total += base::read(file, _featherDistance);

	if (size0000 != total) {
		std::cout << "Failed in reading 0000\n";
		std::cout << "Read " << total << " out of " << size0000 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRiver::readV1(std::istream& file) {

	std::size_t size0001;
	std::size_t total = base::readFormHeader(file, "0001", size0001);
	size0001 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _heightData.read(file);

	total += base::readRecordHeader(file, "DATA", size);

	_points.resize(2);
	total += base::read(file, _points[0].x);
	total += base::read(file, _points[0].y);
	total += base::read(file, _points[1].x);
	total += base::read(file, _points[1].y);

	total += base::read(file, _width);
	total += base::read(file, _bankFamilyId);
	_bottomFamilyId = _bankFamilyId;
	total += base::read(file, _featherFunction);
	total += base::read(file, _featherDistance);

	total += base::read(file, _trenchDepth);
	total += base::read(file, _velocity);
	total += base::read(file, _localWaterTableShaderSize);

	total += base::read(file, _localWaterTableShaderName);

	if (size0001 != total) {
		std::cout << "Failed in reading 0001\n";
		std::cout << "Read " << total << " out of " << size0001 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRiver::readV2(std::istream& file) {

	std::size_t size0002;
	std::size_t total = base::readFormHeader(file, "0002", size0002);
	size0002 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _heightData.read(file);

	total += base::readRecordHeader(file, "DATA", size);

	_points.resize(2);
	total += base::read(file, _points[0].x);
	total += base::read(file, _points[0].y);
	total += base::read(file, _points[1].x);
	total += base::read(file, _points[1].y);

	total += base::read(file, _width);
	total += base::read(file, _bankFamilyId);
	_bottomFamilyId = _bankFamilyId;
	total += base::read(file, _featherFunction);
	total += base::read(file, _featherDistance);

	total += base::read(file, _trenchDepth);
	total += base::read(file, _velocity);

	int32_t temp; total += base::read(file, temp); _hasLocalWaterTable = (0 != temp);
	total += base::read(file, _localWaterTableDepth);
	total += base::read(file, _localWaterTableShaderSize);
	total += base::read(file, _localWaterTableWidth);

	total += base::read(file, _localWaterTableShaderName);

	if (size0002 != total) {
		std::cout << "Failed in reading 0002\n";
		std::cout << "Read " << total << " out of " << size0002 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRiver::readV3(std::istream& file) {

	std::size_t size0003;
	std::size_t total = base::readFormHeader(file, "0003", size0003);
	size0003 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _heightData.read(file);

	total += base::readRecordHeader(file, "DATA", size);

	_points.resize(2);
	total += base::read(file, _points[0].x);
	total += base::read(file, _points[0].y);
	total += base::read(file, _points[1].x);
	total += base::read(file, _points[1].y);

	total += base::read(file, _width);
	total += base::read(file, _bankFamilyId);
	total += base::read(file, _bottomFamilyId);
	total += base::read(file, _featherFunction);
	total += base::read(file, _featherDistance);

	total += base::read(file, _trenchDepth);
	total += base::read(file, _velocity);

	int32_t temp; total += base::read(file, temp); _hasLocalWaterTable = (0 != temp);
	total += base::read(file, _localWaterTableDepth);
	total += base::read(file, _localWaterTableShaderSize);
	total += base::read(file, _localWaterTableWidth);

	total += base::read(file, _localWaterTableShaderName);

	if (size0003 != total) {
		std::cout << "Failed in reading 0003\n";
		std::cout << "Read " << total << " out of " << size0003 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRiver::readV4(std::istream& file) {

	std::size_t size0004;
	std::size_t total = base::readFormHeader(file, "0004", size0004);
	size0004 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _heightData.read(file);

	total += base::readRecordHeader(file, "DATA", size);

	int32_t count;
	total += base::read(file, count);
	_points.resize(count);
	for (auto& p : _points) {
		total += base::read(file, p.x);
		total += base::read(file, p.y);
	}

	total += base::read(file, _width);
	total += base::read(file, _bankFamilyId);
	total += base::read(file, _bottomFamilyId);
	total += base::read(file, _featherFunction);
	total += base::read(file, _featherDistance);

	total += base::read(file, _trenchDepth);
	total += base::read(file, _velocity);

	int32_t temp; total += base::read(file, temp); _hasLocalWaterTable = (0 != temp);
	total += base::read(file, _localWaterTableDepth);
	total += base::read(file, _localWaterTableShaderSize);
	total += base::read(file, _localWaterTableWidth);

	total += base::read(file, _localWaterTableShaderName);

	if (size0004 != total) {
		std::cout << "Failed in reading 0004\n";
		std::cout << "Read " << total << " out of " << size0004 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRiver::readV5(std::istream& file) {

	std::size_t size0005;
	std::size_t total = base::readFormHeader(file, "0005", size0005);
	size0005 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _heightData.read(file);

	total += base::readRecordHeader(file, "DATA", size);

	int32_t count;
	total += base::read(file, count);
	_points.resize(count);
	for (auto& p : _points) {
		total += base::read(file, p.x);
		total += base::read(file, p.y);
	}

	total += base::read(file, _width);
	total += base::read(file, _bankFamilyId);
	total += base::read(file, _bottomFamilyId);
	total += base::read(file, _featherFunction);
	total += base::read(file, _featherDistance);

	total += base::read(file, _trenchDepth);
	total += base::read(file, _velocity);

	int32_t temp; total += base::read(file, temp); _hasLocalWaterTable = (0 != temp);
	total += base::read(file, _localWaterTableDepth);
	total += base::read(file, _localWaterTableWidth);
	total += base::read(file, _localWaterTableShaderSize);

	total += base::read(file, _localWaterTableShaderName);

	if (size0005 != total) {
		std::cout << "Failed in reading 0005\n";
		std::cout << "Read " << total << " out of " << size0005 << "\n";
		exit(0);
	}

	return total;
}

std::size_t affectorRiver::readV6(std::istream& file) {

	std::size_t size0006;
	std::size_t total = base::readFormHeader(file, "0006", size0006);
	size0006 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _heightData.read(file);

	total += base::readRecordHeader(file, "DATA", size);

	int32_t count;
	total += base::read(file, count);
	_points.resize(count);
	for (auto& p : _points) {
		total += base::read(file, p.x);
		total += base::read(file, p.y);
	}

	total += base::read(file, _width);
	total += base::read(file, _bankFamilyId);
	total += base::read(file, _bottomFamilyId);
	total += base::read(file, _featherFunction);
	total += base::read(file, _featherDistance);

	total += base::read(file, _trenchDepth);
	total += base::read(file, _velocity);

	int32_t temp; total += base::read(file, temp); _hasLocalWaterTable = (0 != temp);
	total += base::read(file, _localWaterTableDepth);
	total += base::read(file, _localWaterTableWidth);
	total += base::read(file, _localWaterTableShaderSize);

	total += base::read(file, _localWaterTableShaderName);

	total += base::read(file, _waterType);

	if (size0006 != total) {
		std::cout << "Failed in reading 0006\n";
		std::cout << "Read " << total << " out of " << size0006 << "\n";
		exit(0);
	}

	return total;
}

