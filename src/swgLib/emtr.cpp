/** -*-c++-*-
 *  \class  emtr
 *  \file   emtr.cpp
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
#include <swgLib/emtr.hpp>
#include <swgLib/base.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

emtr::emtr()
{
}

emtr::~emtr()
{
}

std::size_t emtr::read(std::istream& file)
{
	std::size_t emtrSize;
	std::size_t total = base::readFormHeader(file, "EMTR", emtrSize);
	emtrSize += 8;
	std::cout << "Found EMTR: " << emtrSize - 12 << " bytes\n";

	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);
	std::cout << form << ":" << type << ":" << size << "\n";
	_version = base::tagToVersion(type);
	if (_version > 14) {
		std::cout << "Expected type [0000..0014]: " << type << "\n";
		exit(0);
	}
	std::cout << "Emitter Description Version: " << _version << "\n";

	switch (_version) {
	case 0: total += readV0(file); break;
	case 1: total += readV1(file); break;
	case 2: total += readV2(file); break;
	case 3: total += readV3(file); break;
	case 4: total += readV4(file); break;
	case 5: total += readV5(file); break;
	case 6: total += readV6(file); break;
	case 7: total += readV7(file); break;
	case 8: total += readV8(file); break;
	case 9: total += readV9(file); break;
	case 10: total += readV10(file); break;
	case 11: total += readV11(file); break;
	case 12: total += readV12(file); break;
	case 13: total += readV13(file); break;
	case 14: total += readV14(file); break;
	default:
		std::cout << "Unhandled Emitter Description version: " << _version << "\n";
		exit(0);
	}

	if (emtrSize == total) {
		std::cout << "Finished reading EMTR\n";
	}
	else {
		std::cout << "FAILED in reading EMTR\n"
			<< "Read " << total << " out of " << emtrSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t emtr::readV0(std::istream& file)
{
	std::size_t size0000;
	std::size_t total = base::readFormHeader(file, "0000", size0000);
	size0000 += 8;

	total += _emitterTranslationX.read(file);
	total += _emitterTranslationY.read(file);
	total += _emitterTranslationZ.read(file);

	total += _emitterRotationX.read(file);
	total += _emitterRotationY.read(file);
	total += _emitterRotationZ.read(file);

	total += _emitterDistance.read(file);
	total += _emitterShapeSize.read(file);
	total += _emitterSpread.read(file);

	total += _particleGenerationRate.read(file);
	total += _particleEmitSpeed.read(file);
	total += _particleInheritVelocityPercent.read(file);
	total += _particleClusterCount.read(file);
	total += _particleClusterRadius.read(file);
	total += _particleLifeTime.read(file);
	total += _particleWeight.read(file);

	_particleWeight.scale(0.28f);

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);

	total += base::read(file, _emitterEmitDirection);
	std::cout << "Emit direction: " << _emitterEmitDirection << "\n";

	total += base::read(file, _emitterShape);
	std::cout << "Emitter shape: " << _emitterShape << "\n";

	total += base::read(file, _emitterLoopImmediately);
	std::cout << "Loop immediately: " << std::boolalpha << _emitterLoopImmediately << "\n";

	total += base::read(file, _emitterLifetimeMin);
	_emitterLifetimeMax = _emitterLifetimeMin;
	std::cout << "Lifetime min/max: [" << _emitterLifetimeMin << ".." << _emitterLifetimeMax << "]\n";

	total += base::read(file, _emitterMaxParticles);
	std::cout << "Max particles: " << _emitterMaxParticles << "\n";

	total += base::read(file, _emitterOneShot);
	std::cout << "One shot: " << std::boolalpha << _emitterOneShot << "\n";

	_emitterOneShotMin = int32_t(_emitterMaxParticles) - 1;
	_emitterOneShotMax = int32_t(_emitterMaxParticles) - 1;
	std::cout << "One shot min/max: [" << _emitterOneShotMin << ".." << _emitterOneShotMax << "]\n";

	total += base::read(file, _particleRandomInitialRotation);
	std::cout << "Random initial rotation: " << std::boolalpha << _particleRandomInitialRotation << "\n";

	bool particleOrientWithVelocity;
	total += base::read(file, particleOrientWithVelocity);

	if (particleOrientWithVelocity) {
		_particleOrientation = 2;
	}
	else {
		_particleOrientation = 0;
	}

	std::cout << "Particle orientation: " << _particleOrientation << "\n";

	bool junk; total += base::read(file, junk);
	total += base::read(file, _particleVisible);
	std::cout << "Particle visible: " << std::boolalpha << _particleVisible << "\n";
	uint32_t junk2;	total += base::read(file, junk2);

	//particle description quad
	_ptcl = ptqdPtr(new ptqd());
	total += _ptcl->read(file);

	if (size0000 != total) {
		std::cout << "FAILED in reading 0000\n"
			<< "Read " << total << " out of " << size0000 << "\n";
		exit(0);
	}

	return total;
}

std::size_t emtr::readV1(std::istream& file)
{
	std::size_t size0001;
	std::size_t total = base::readFormHeader(file, "0001", size0001);
	size0001 += 8;

	total += _emitterTranslationX.read(file);
	total += _emitterTranslationY.read(file);
	total += _emitterTranslationZ.read(file);

	total += _emitterRotationX.read(file);
	total += _emitterRotationY.read(file);
	total += _emitterRotationZ.read(file);

	total += _emitterDistance.read(file);
	total += _emitterShapeSize.read(file);
	total += _emitterSpread.read(file);

	total += _particleGenerationRate.read(file);
	total += _particleEmitSpeed.read(file);
	total += _particleInheritVelocityPercent.read(file);
	total += _particleClusterCount.read(file);
	total += _particleClusterRadius.read(file);
	total += _particleLifeTime.read(file);
	total += _particleWeight.read(file);

	_particleWeight.scale(0.28f);

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);

	total += base::read(file, _emitterEmitDirection);
	total += base::read(file, _emitterShape);
	total += base::read(file, _emitterLoopImmediately);
	total += base::read(file, _emitterLifetimeMin);
	_emitterLifetimeMax = _emitterLifetimeMin;
	total += base::read(file, _emitterMaxParticles);
	total += base::read(file, _emitterOneShot);
	_emitterOneShotMin = int32_t(_emitterMaxParticles) - 1;
	_emitterOneShotMax = int32_t(_emitterMaxParticles) - 1;
	total += base::read(file, _particleRandomInitialRotation);
	total += base::read(file, _particleOrientation);
	bool junk; total += base::read(file, junk);
	total += base::read(file, _particleVisible);
	uint32_t junk2;	total += base::read(file, junk2);

	//particle description quad
	_ptcl = ptqdPtr(new ptqd());
	total += _ptcl->read(file);

	if (size0001 != total) {
		std::cout << "FAILED in reading 0001\n"
			<< "Read " << total << " out of " << size0001 << "\n";
		exit(0);
	}

	return total;
}

std::size_t emtr::readV2(std::istream& file)
{
	std::size_t size0002;
	std::size_t total = base::readFormHeader(file, "0002", size0002);
	size0002 += 8;

	total += _emitterTranslationX.read(file);
	total += _emitterTranslationY.read(file);
	total += _emitterTranslationZ.read(file);

	total += _emitterRotationX.read(file);
	total += _emitterRotationY.read(file);
	total += _emitterRotationZ.read(file);

	total += _emitterDistance.read(file);
	total += _emitterShapeSize.read(file);
	total += _emitterSpread.read(file);

	total += _particleGenerationRate.read(file);
	total += _particleEmitSpeed.read(file);
	total += _particleInheritVelocityPercent.read(file);
	total += _particleClusterCount.read(file);
	total += _particleClusterRadius.read(file);
	total += _particleLifeTime.read(file);
	total += _particleWeight.read(file);

	_particleWeight.scale(0.28f);

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);

	total += base::read(file, _emitterEmitDirection);
	total += base::read(file, _emitterShape);
	total += base::read(file, _emitterLoopImmediately);
	total += base::read(file, _emitterLifetimeMin);
	_emitterLifetimeMax = _emitterLifetimeMin;
	total += base::read(file, _emitterMaxParticles);
	total += base::read(file, _emitterOneShot);
	_emitterOneShotMin = int32_t(_emitterMaxParticles) - 1;
	_emitterOneShotMax = int32_t(_emitterMaxParticles) - 1;
	total += base::read(file, _particleRandomInitialRotation);
	total += base::read(file, _particleOrientation);
	if (3 == _particleOrientation) {
		_particleOrientation = 2;
	}
	bool junk; total += base::read(file, junk);
	total += base::read(file, _particleVisible);
	uint32_t junk2;	total += base::read(file, junk2);

	//particle description quad
	_ptcl = ptqdPtr(new ptqd());
	total += _ptcl->read(file);

	if (size0002 != total) {
		std::cout << "FAILED in reading 0002\n"
			<< "Read " << total << " out of " << size0002 << "\n";
		exit(0);
	}

	return total;
}

std::size_t emtr::readV3(std::istream& file)
{
	std::size_t size0003;
	std::size_t total = base::readFormHeader(file, "0003", size0003);
	size0003 += 8;

	total += _timing.read(file);

	total += _emitterTranslationX.read(file);
	total += _emitterTranslationY.read(file);
	total += _emitterTranslationZ.read(file);

	total += _emitterRotationX.read(file);
	total += _emitterRotationY.read(file);
	total += _emitterRotationZ.read(file);

	total += _emitterDistance.read(file);
	total += _emitterShapeSize.read(file);
	total += _emitterSpread.read(file);

	total += _particleGenerationRate.read(file);
	total += _particleEmitSpeed.read(file);
	total += _particleInheritVelocityPercent.read(file);
	total += _particleClusterCount.read(file);
	total += _particleClusterRadius.read(file);
	total += _particleLifeTime.read(file);
	total += _particleWeight.read(file);

	_particleWeight.scale(0.28f);

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _emitterName);
	total += base::read(file, _emitterEmitDirection);
	total += base::read(file, _emitterShape);
	total += base::read(file, _emitterLoopImmediately);
	total += base::read(file, _emitterLifetimeMin);
	_emitterLifetimeMax = _emitterLifetimeMin;
	total += base::read(file, _emitterMaxParticles);
	total += base::read(file, _emitterOneShot);
	_emitterOneShotMin = int32_t(_emitterMaxParticles) - 1;
	_emitterOneShotMax = int32_t(_emitterMaxParticles) - 1;
	total += base::read(file, _particleRandomInitialRotation);
	total += base::read(file, _particleOrientation);
	bool junk; total += base::read(file, junk);
	total += base::read(file, _particleVisible);
	uint32_t junk2;	total += base::read(file, junk2);

	//particle description quad
	_ptcl = ptqdPtr(new ptqd());
	total += _ptcl->read(file);

	if (size0003 != total) {
		std::cout << "FAILED in reading 0003\n"
			<< "Read " << total << " out of " << size0003 << "\n";
		exit(0);
	}

	return total;
}

std::size_t emtr::readV4(std::istream& file)
{
	std::size_t size0004;
	std::size_t total = base::readFormHeader(file, "0004", size0004);
	size0004 += 8;

	total += _timing.read(file);

	total += _emitterTranslationX.read(file);
	total += _emitterTranslationY.read(file);
	total += _emitterTranslationZ.read(file);

	total += _emitterRotationX.read(file);
	total += _emitterRotationY.read(file);
	total += _emitterRotationZ.read(file);

	total += _emitterDistance.read(file);
	total += _emitterShapeSize.read(file);
	total += _emitterSpread.read(file);

	total += _particleGenerationRate.read(file);
	total += _particleEmitSpeed.read(file);
	total += _particleInheritVelocityPercent.read(file);
	total += _particleClusterCount.read(file);
	total += _particleClusterRadius.read(file);
	total += _particleLifeTime.read(file);
	total += _particleWeight.read(file);

	_particleWeight.scale(0.28f);

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _emitterName);
	total += base::read(file, _emitterEmitDirection);
	total += base::read(file, _emitterShape);
	total += base::read(file, _emitterLoopImmediately);
	total += base::read(file, _emitterLifetimeMin);
	_emitterLifetimeMax = _emitterLifetimeMin;
	total += base::read(file, _emitterMaxParticles);
	total += base::read(file, _emitterOneShot);
	_emitterOneShotMin = int32_t(_emitterMaxParticles) - 1;
	_emitterOneShotMax = int32_t(_emitterMaxParticles) - 1;
	total += base::read(file, _particleRandomInitialRotation);
	total += base::read(file, _particleOrientation);
	bool junk; total += base::read(file, junk);
	total += base::read(file, _particleVisible);
	uint32_t junk2;	total += base::read(file, junk2);

	total += base::read(file, _particleType);

	//particle description quad
	if (0 == _particleType) {
		_ptcl = ptqdPtr(new ptqd());
	}
	else if (1 == _particleType) {
		_ptcl = ptmhPtr(new ptmh());
	}
	else {
		std::cout << "Unhandled particle type: " << _particleType << "\n";
		exit(0);
	}
	total += _ptcl->read(file);

	if (size0004 != total) {
		std::cout << "FAILED in reading 0004\n"
			<< "Read " << total << " out of " << size0004 << "\n";
		exit(0);
	}

	return total;
}

std::size_t emtr::readV5(std::istream& file)
{
	std::size_t size0005;
	std::size_t total = base::readFormHeader(file, "0005", size0005);
	size0005 += 8;

	total += _timing.read(file);

	total += _emitterTranslationX.read(file);
	total += _emitterTranslationY.read(file);
	total += _emitterTranslationZ.read(file);

	total += _emitterRotationX.read(file);
	total += _emitterRotationY.read(file);
	total += _emitterRotationZ.read(file);

	total += _emitterDistance.read(file);
	total += _emitterShapeSize.read(file);
	total += _emitterSpread.read(file);

	total += _particleGenerationRate.read(file);
	total += _particleEmitSpeed.read(file);
	total += _particleInheritVelocityPercent.read(file);
	total += _particleClusterCount.read(file);
	total += _particleClusterRadius.read(file);
	total += _particleLifeTime.read(file);
	total += _particleWeight.read(file);

	_particleWeight.scale(0.28f);

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _emitterName);
	total += base::read(file, _emitterEmitDirection);
	total += base::read(file, _emitterShape);
	total += base::read(file, _emitterLoopImmediately);
	total += base::read(file, _emitterLifetimeMin);
	_emitterLifetimeMax = _emitterLifetimeMin;
	total += base::read(file, _emitterMaxParticles);
	total += base::read(file, _emitterOneShot);
	_emitterOneShotMin = int32_t(_emitterMaxParticles) - 1;
	_emitterOneShotMax = int32_t(_emitterMaxParticles) - 1;
	total += base::read(file, _particleRandomInitialRotation);
	total += base::read(file, _particleOrientation);
	bool junk; total += base::read(file, junk);
	total += base::read(file, _particleVisible);
	uint32_t junk2;	total += base::read(file, junk2);

	total += base::read(file, _particleType);
	total += base::read(file, _soundPath);

	//particle description quad
	if (0 == _particleType) {
		_ptcl = ptqdPtr(new ptqd());
	}
	else if (1 == _particleType) {
		_ptcl = ptmhPtr(new ptmh());
	}
	else {
		std::cout << "Unhandled particle type: " << _particleType << "\n";
		exit(0);
	}
	total += _ptcl->read(file);

	if (size0005 != total) {
		std::cout << "FAILED in reading 0005\n"
			<< "Read " << total << " out of " << size0005 << "\n";
		exit(0);
	}

	return total;
}

std::size_t emtr::readV6(std::istream& file)
{
	std::size_t size0006;
	std::size_t total = base::readFormHeader(file, "0006", size0006);
	size0006 += 8;

	total += _timing.read(file);

	total += _emitterTranslationX.read(file);
	total += _emitterTranslationY.read(file);
	total += _emitterTranslationZ.read(file);

	total += _emitterRotationX.read(file);
	total += _emitterRotationY.read(file);
	total += _emitterRotationZ.read(file);

	total += _emitterDistance.read(file);
	total += _emitterShapeSize.read(file);
	total += _emitterSpread.read(file);

	total += _particleGenerationRate.read(file);
	total += _particleEmitSpeed.read(file);
	total += _particleInheritVelocityPercent.read(file);
	total += _particleClusterCount.read(file);
	total += _particleClusterRadius.read(file);
	total += _particleLifeTime.read(file);
	total += _particleWeight.read(file);

	_particleWeight.scale(0.28f);

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _emitterName);
	total += base::read(file, _emitterEmitDirection);
	total += base::read(file, _emitterShape);
	total += base::read(file, _emitterLoopImmediately);
	total += base::read(file, _emitterLifetimeMin);
	_emitterLifetimeMax = _emitterLifetimeMin;
	total += base::read(file, _emitterMaxParticles);
	total += base::read(file, _emitterOneShot);
	_emitterOneShotMin = int32_t(_emitterMaxParticles) - 1;
	_emitterOneShotMax = int32_t(_emitterMaxParticles) - 1;
	total += base::read(file, _particleRandomInitialRotation);
	total += base::read(file, _particleOrientation);
	total += base::read(file, _particleVisible);
	uint32_t junk2;	total += base::read(file, junk2);

	total += base::read(file, _particleType);
	total += base::read(file, _soundPath);

	total += base::read(file, _localSpaceParticles);

	//particle description quad
	if (0 == _particleType) {
		_ptcl = ptqdPtr(new ptqd());
	}
	else if (1 == _particleType) {
		_ptcl = ptmhPtr(new ptmh());
	}
	else {
		std::cout << "Unhandled particle type: " << _particleType << "\n";
		exit(0);
	}
	total += _ptcl->read(file);

	if (size0006 != total) {
		std::cout << "FAILED in reading 0006\n"
			<< "Read " << total << " out of " << size0006 << "\n";
		exit(0);
	}

	return total;
}

std::size_t emtr::readV7(std::istream& file)
{
	std::size_t size0007;
	std::size_t total = base::readFormHeader(file, "0007", size0007);
	size0007 += 8;

	total += _timing.read(file);

	total += _emitterTranslationX.read(file);
	total += _emitterTranslationY.read(file);
	total += _emitterTranslationZ.read(file);

	total += _emitterRotationX.read(file);
	total += _emitterRotationY.read(file);
	total += _emitterRotationZ.read(file);

	total += _emitterDistance.read(file);
	total += _emitterShapeSize.read(file);
	total += _emitterSpread.read(file);

	total += _particleGenerationRate.read(file);
	total += _particleEmitSpeed.read(file);
	total += _particleInheritVelocityPercent.read(file);
	total += _particleClusterCount.read(file);
	total += _particleClusterRadius.read(file);
	total += _particleLifeTime.read(file);
	total += _particleWeight.read(file);

	_particleWeight.scale(0.28f);

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _emitterName);
	total += base::read(file, _emitterEmitDirection);
	total += base::read(file, _generationType);
	total += base::read(file, _emitterShape);
	total += base::read(file, _emitterLoopImmediately);
	total += base::read(file, _emitterLifetimeMin);
	_emitterLifetimeMax = _emitterLifetimeMin;
	total += base::read(file, _emitterMaxParticles);
	total += base::read(file, _emitterOneShot);
	total += base::read(file, _emitterOneShotMin);
	total += base::read(file, _emitterOneShotMax);
	total += base::read(file, _particleRandomInitialRotation);
	total += base::read(file, _particleOrientation);
	total += base::read(file, _particleVisible);
	uint32_t junk2;	total += base::read(file, junk2); // Skip 32-bit

	total += base::read(file, _particleType);
	total += base::read(file, _soundPath);

	total += base::read(file, _localSpaceParticles);

	total += base::read(file, _particleGroundCollision);
	total += base::read(file, junk2); // Skip 32-bit
	total += base::read(file, _particleCollisionHeightAboveGround);
	total += base::read(file, _particleForwardVelocityMaintainedMin);
	total += base::read(file, _particleForwardVelocityMaintainedMax);
	total += base::read(file, _particleUpVelocityMaintainedMin);
	total += base::read(file, _particleUpVelocityMaintainedMax);
	total += base::read(file, _windResistenceGlobalPercent);

	//particle description quad
	if (0 == _particleType) {
		_ptcl = ptqdPtr(new ptqd());
	}
	else if (1 == _particleType) {
		_ptcl = ptmhPtr(new ptmh());
	}
	else {
		std::cout << "Unhandled particle type: " << _particleType << "\n";
		exit(0);
	}
	total += _ptcl->read(file);

	if (size0007 != total) {
		std::cout << "FAILED in reading 0007\n"
			<< "Read " << total << " out of " << size0007 << "\n";
		exit(0);
	}

	return total;
}

std::size_t emtr::readV8(std::istream& file)
{
	std::size_t size0008;
	std::size_t total = base::readFormHeader(file, "0008", size0008);
	size0008 += 8;

	total += _timing.read(file);

	total += _emitterTranslationX.read(file);
	total += _emitterTranslationY.read(file);
	total += _emitterTranslationZ.read(file);

	total += _emitterRotationX.read(file);
	total += _emitterRotationY.read(file);
	total += _emitterRotationZ.read(file);

	total += _emitterDistance.read(file);
	total += _emitterShapeSize.read(file);
	total += _emitterSpread.read(file);

	total += _particleGenerationRate.read(file);
	total += _particleEmitSpeed.read(file);
	total += _particleInheritVelocityPercent.read(file);
	total += _particleClusterCount.read(file);
	total += _particleClusterRadius.read(file);
	total += _particleLifeTime.read(file);
	total += _particleWeight.read(file);

	_particleWeight.scale(0.28f);

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _emitterName);
	total += base::read(file, _emitterEmitDirection);
	total += base::read(file, _generationType);
	total += base::read(file, _emitterShape);
	total += base::read(file, _emitterLoopImmediately);
	total += base::read(file, _emitterLifetimeMin);
	_emitterLifetimeMax = _emitterLifetimeMin;
	total += base::read(file, _emitterMaxParticles);
	total += base::read(file, _emitterOneShot);
	total += base::read(file, _emitterOneShotMin);
	total += base::read(file, _emitterOneShotMax);
	total += base::read(file, _particleRandomInitialRotation);
	total += base::read(file, _particleOrientation);
	total += base::read(file, _particleVisible);
	uint32_t junk2;	total += base::read(file, junk2); // Skip 32-bit

	total += base::read(file, _particleType);
	total += base::read(file, _soundPath);

	total += base::read(file, _localSpaceParticles);

	total += base::read(file, _particleGroundCollision);
	total += base::read(file, _particleKillParticlesOnCollision);
	total += base::read(file, junk2); // Skip 32-bit
	total += base::read(file, _particleCollisionHeightAboveGround);
	total += base::read(file, _particleForwardVelocityMaintainedMin);
	total += base::read(file, _particleForwardVelocityMaintainedMax);
	total += base::read(file, _particleUpVelocityMaintainedMin);
	total += base::read(file, _particleUpVelocityMaintainedMax);
	total += base::read(file, _windResistenceGlobalPercent);

	//particle description quad
	if (0 == _particleType) {
		_ptcl = ptqdPtr(new ptqd());
	}
	else if (1 == _particleType) {
		_ptcl = ptmhPtr(new ptmh());
	}
	else {
		std::cout << "Unhandled particle type: " << _particleType << "\n";
		exit(0);
	}
	total += _ptcl->read(file);

	if (size0008 != total) {
		std::cout << "FAILED in reading 0008\n"
			<< "Read " << total << " out of " << size0008 << "\n";
		exit(0);
	}

	return total;
}

std::size_t emtr::readV9(std::istream& file)
{
	std::size_t size0009;
	std::size_t total = base::readFormHeader(file, "0009", size0009);
	size0009 += 8;

	total += _timing.read(file);

	total += _emitterTranslationX.read(file);
	total += _emitterTranslationY.read(file);
	total += _emitterTranslationZ.read(file);

	total += _emitterRotationX.read(file);
	total += _emitterRotationY.read(file);
	total += _emitterRotationZ.read(file);

	total += _emitterDistance.read(file);
	total += _emitterShapeSize.read(file);
	total += _emitterSpread.read(file);

	total += _particleGenerationRate.read(file);
	total += _particleEmitSpeed.read(file);
	total += _particleInheritVelocityPercent.read(file);
	total += _particleClusterCount.read(file);
	total += _particleClusterRadius.read(file);
	total += _particleLifeTime.read(file);
	total += _particleWeight.read(file);

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _emitterName);
	total += base::read(file, _emitterEmitDirection);
	total += base::read(file, _generationType);
	total += base::read(file, _emitterShape);
	total += base::read(file, _emitterLoopImmediately);
	total += base::read(file, _emitterLifetimeMin);
	total += base::read(file, _emitterLifetimeMax);
	total += base::read(file, _emitterMaxParticles);
	total += base::read(file, _emitterOneShot);
	total += base::read(file, _emitterOneShotMin);
	total += base::read(file, _emitterOneShotMax);
	total += base::read(file, _particleRandomInitialRotation);
	total += base::read(file, _particleOrientation);
	total += base::read(file, _particleVisible);
	uint32_t junk2;	total += base::read(file, junk2); // Skip 32-bit

	total += base::read(file, _particleType);
	total += base::read(file, _soundPath);

	total += base::read(file, _localSpaceParticles);

	total += base::read(file, _particleGroundCollision);
	total += base::read(file, _particleKillParticlesOnCollision);
	total += base::read(file, junk2); // Skip 32-bit
	total += base::read(file, _particleCollisionHeightAboveGround);
	total += base::read(file, _particleForwardVelocityMaintainedMin);
	total += base::read(file, _particleForwardVelocityMaintainedMax);
	total += base::read(file, _particleUpVelocityMaintainedMin);
	total += base::read(file, _particleUpVelocityMaintainedMax);
	total += base::read(file, _windResistenceGlobalPercent);
	total += base::read(file, _emitterLODDistanceMin);
	total += base::read(file, _emitterLODDistanceMax);

	//particle description quad
	if (0 == _particleType) {
		_ptcl = ptqdPtr(new ptqd());
	}
	else if (1 == _particleType) {
		_ptcl = ptmhPtr(new ptmh());
	}
	else {
		std::cout << "Unhandled particle type: " << _particleType << "\n";
		exit(0);
	}
	total += _ptcl->read(file);

	if (size0009 != total) {
		std::cout << "FAILED in reading 0009\n"
			<< "Read " << total << " out of " << size0009 << "\n";
		exit(0);
	}

	return total;
}

std::size_t emtr::readV10(std::istream& file)
{
	std::size_t size0010;
	std::size_t total = base::readFormHeader(file, "0010", size0010);
	size0010 += 8;

	total += _timing.read(file);

	total += _emitterTranslationX.read(file);
	total += _emitterTranslationY.read(file);
	total += _emitterTranslationZ.read(file);

	total += _emitterRotationX.read(file);
	total += _emitterRotationY.read(file);
	total += _emitterRotationZ.read(file);

	total += _emitterDistance.read(file);
	total += _emitterShapeSize.read(file);
	total += _emitterSpread.read(file);

	total += _particleGenerationRate.read(file);
	total += _particleEmitSpeed.read(file);
	total += _particleInheritVelocityPercent.read(file);
	total += _particleClusterCount.read(file);
	total += _particleClusterRadius.read(file);
	total += _particleLifeTime.read(file);
	total += _particleWeight.read(file);

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _emitterName);
	total += base::read(file, _emitterEmitDirection);
	total += base::read(file, _generationType);
	total += base::read(file, _emitterShape);
	total += base::read(file, _emitterLoopImmediately);
	total += base::read(file, _emitterLifetimeMin);
	total += base::read(file, _emitterLifetimeMax);
	total += base::read(file, _emitterMaxParticles);
	total += base::read(file, _emitterOneShot);
	total += base::read(file, _emitterOneShotMin);
	total += base::read(file, _emitterOneShotMax);
	total += base::read(file, _particleRandomInitialRotation);
	total += base::read(file, _particleOrientation);
	total += base::read(file, _particleVisible);
	uint32_t junk2;	total += base::read(file, junk2); // Skip 32-bit

	total += base::read(file, _particleType);
	total += base::read(file, _soundPath);

	total += base::read(file, _localSpaceParticles);

	total += base::read(file, _particleGroundCollision);
	total += base::read(file, _particleKillParticlesOnCollision);
	total += base::read(file, junk2); // Skip 32-bit
	total += base::read(file, _particleCollisionHeightAboveGround);
	total += base::read(file, _particleForwardVelocityMaintainedMin);
	total += base::read(file, _particleForwardVelocityMaintainedMax);
	total += base::read(file, _particleUpVelocityMaintainedMin);
	total += base::read(file, _particleUpVelocityMaintainedMax);
	total += base::read(file, _windResistenceGlobalPercent);
	total += base::read(file, _emitterLODDistanceMin);
	total += base::read(file, _emitterLODDistanceMax);
	uint8_t flockingTypeTemp;
	total += base::read(file, flockingTypeTemp);
	if (flockingTypeTemp > 0) { _flockingType = 1; }
	else { _flockingType = 0; }
	total += base::read(file, _flockingSeperationDistance);
	total += base::read(file, _flockingAlignmentGain);
	total += base::read(file, _flockingCohesionGain);
	total += base::read(file, _flockingSeperationGain);
	total += base::read(file, _flockingCageWidth);
	float cageMin, cageMax;
	total += base::read(file, cageMin);
	total += base::read(file, cageMax);
	_flockingCageHeight = cageMax - cageMin;

	//particle description quad
	if (0 == _particleType) {
		_ptcl = ptqdPtr(new ptqd());
	}
	else if (1 == _particleType) {
		_ptcl = ptmhPtr(new ptmh());
	}
	else {
		std::cout << "Unhandled particle type: " << _particleType << "\n";
		exit(0);
	}
	total += _ptcl->read(file);

	if (size0010 != total) {
		std::cout << "FAILED in reading 0010\n"
			<< "Read " << total << " out of " << size0010 << "\n";
		exit(0);
	}

	return total;
}

std::size_t emtr::readV11(std::istream& file)
{
	std::size_t size0011;
	std::size_t total = base::readFormHeader(file, "0011", size0011);
	size0011 += 8;

	total += _timing.read(file);

	total += _emitterTranslationX.read(file);
	total += _emitterTranslationY.read(file);
	total += _emitterTranslationZ.read(file);

	total += _emitterRotationX.read(file);
	total += _emitterRotationY.read(file);
	total += _emitterRotationZ.read(file);

	total += _emitterDistance.read(file);
	total += _emitterShapeSize.read(file);
	total += _emitterSpread.read(file);

	total += _particleGenerationRate.read(file);
	total += _particleEmitSpeed.read(file);
	total += _particleInheritVelocityPercent.read(file);
	total += _particleClusterCount.read(file);
	total += _particleClusterRadius.read(file);
	total += _particleLifeTime.read(file);
	total += _particleWeight.read(file);

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _emitterName);
	total += base::read(file, _emitterEmitDirection);
	total += base::read(file, _generationType);
	total += base::read(file, _emitterShape);
	total += base::read(file, _emitterLoopImmediately);
	total += base::read(file, _emitterLifetimeMin);
	total += base::read(file, _emitterLifetimeMax);
	total += base::read(file, _emitterMaxParticles);
	total += base::read(file, _emitterOneShot);
	total += base::read(file, _emitterOneShotMin);
	total += base::read(file, _emitterOneShotMax);
	total += base::read(file, _particleRandomInitialRotation);
	total += base::read(file, _particleOrientation);
	total += base::read(file, _particleVisible);
	uint32_t junk2;	total += base::read(file, junk2); // Skip 32-bit

	total += base::read(file, _particleType);
	total += base::read(file, _soundPath);

	total += base::read(file, _localSpaceParticles);

	total += base::read(file, _particleGroundCollision);
	total += base::read(file, _particleKillParticlesOnCollision);
	total += base::read(file, junk2); // Skip 32-bit
	total += base::read(file, _particleCollisionHeightAboveGround);
	total += base::read(file, _particleForwardVelocityMaintainedMin);
	total += base::read(file, _particleForwardVelocityMaintainedMax);
	total += base::read(file, _particleUpVelocityMaintainedMin);
	total += base::read(file, _particleUpVelocityMaintainedMax);
	total += base::read(file, _windResistenceGlobalPercent);
	total += base::read(file, _emitterLODDistanceMin);
	total += base::read(file, _emitterLODDistanceMax);
	uint8_t flockingTypeTemp;
	total += base::read(file, flockingTypeTemp);
	if (flockingTypeTemp > 0) { _flockingType = 1; }
	else { _flockingType = 0; }
	total += base::read(file, _flockingSeperationDistance);
	total += base::read(file, _flockingAlignmentGain);
	total += base::read(file, _flockingCohesionGain);
	total += base::read(file, _flockingSeperationGain);
	total += base::read(file, _flockingCageWidth);
	float cageMin, cageMax;
	total += base::read(file, cageMin);
	total += base::read(file, cageMax);
	_flockingCageHeight = cageMax - cageMin;
	total += base::read(file, _particleTimeOfDayColorPercent);

	//particle description quad
	if (0 == _particleType) {
		_ptcl = ptqdPtr(new ptqd());
	}
	else if (1 == _particleType) {
		_ptcl = ptmhPtr(new ptmh());
	}
	else {
		std::cout << "Unhandled particle type: " << _particleType << "\n";
		exit(0);
	}
	total += _ptcl->read(file);

	if (size0011 != total) {
		std::cout << "FAILED in reading 0011\n"
			<< "Read " << total << " out of " << size0011 << "\n";
		exit(0);
	}

	return total;
}

std::size_t emtr::readV12(std::istream& file)
{
	std::size_t size0012;
	std::size_t total = base::readFormHeader(file, "0012", size0012);
	size0012 += 8;

	total += _timing.read(file);

	total += _emitterTranslationX.read(file);
	total += _emitterTranslationY.read(file);
	total += _emitterTranslationZ.read(file);

	total += _emitterRotationX.read(file);
	total += _emitterRotationY.read(file);
	total += _emitterRotationZ.read(file);

	total += _emitterDistance.read(file);
	total += _emitterShapeSize.read(file);
	total += _emitterSpread.read(file);

	total += _particleGenerationRate.read(file);
	total += _particleEmitSpeed.read(file);
	total += _particleInheritVelocityPercent.read(file);
	total += _particleClusterCount.read(file);
	total += _particleClusterRadius.read(file);
	total += _particleLifeTime.read(file);
	total += _particleWeight.read(file);

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _emitterName);
	total += base::read(file, _emitterEmitDirection);
	total += base::read(file, _generationType);
	total += base::read(file, _emitterShape);
	total += base::read(file, _emitterLoopImmediately);
	total += base::read(file, _emitterLifetimeMin);
	total += base::read(file, _emitterLifetimeMax);
	total += base::read(file, _emitterMaxParticles);
	total += base::read(file, _emitterOneShot);
	total += base::read(file, _emitterOneShotMin);
	total += base::read(file, _emitterOneShotMax);
	total += base::read(file, _particleRandomInitialRotation);
	total += base::read(file, _particleOrientation);
	total += base::read(file, _particleVisible);
	uint32_t junk2;	total += base::read(file, junk2); // Skip 32-bit

	total += base::read(file, _particleType);
	total += base::read(file, _soundPath);

	total += base::read(file, _localSpaceParticles);

	total += base::read(file, _particleGroundCollision);
	total += base::read(file, _particleKillParticlesOnCollision);
	total += base::read(file, _particleCollisionHeightAboveGround);
	total += base::read(file, _particleForwardVelocityMaintainedMin);
	total += base::read(file, _particleForwardVelocityMaintainedMax);
	total += base::read(file, _particleUpVelocityMaintainedMin);
	total += base::read(file, _particleUpVelocityMaintainedMax);
	total += base::read(file, _windResistenceGlobalPercent);
	total += base::read(file, _emitterLODDistanceMin);
	total += base::read(file, _emitterLODDistanceMax);
	total += base::read(file, _flockingType);
	total += base::read(file, _flockingSeperationDistance);
	total += base::read(file, _flockingAlignmentGain);
	total += base::read(file, _flockingCohesionGain);
	total += base::read(file, _flockingSeperationGain);
	total += base::read(file, _flockingCageWidth);
	total += base::read(file, _flockingCageHeight);
	total += base::read(file, _flockingCageShape);
	total += base::read(file, _flockingCageEdgeType);
	total += base::read(file, _flockingHeadingChangeRadian);
	total += base::read(file, _flockingHeadingChangeTime);
	total += base::read(file, _particleTimeOfDayColorPercent);
	total += base::read(file, _particleSnapToTerrainOnCreation);
	total += base::read(file, _particleAlignToTerrainNormalOnCreation);
	total += base::read(file, _particleSnapToTerrainOnCreationHeight);
	total += base::read(file, _particleChangeDirectionRadian);
	total += base::read(file, _particleChangeDirectionTime);

	//particle description quad
	if (0 == _particleType) {
		_ptcl = ptqdPtr(new ptqd());
	}
	else if (1 == _particleType) {
		_ptcl = ptmhPtr(new ptmh());
	}
	else {
		std::cout << "Unhandled particle type: " << _particleType << "\n";
		exit(0);
	}
	total += _ptcl->read(file);

	if (size0012 != total) {
		std::cout << "FAILED in reading 0012\n"
			<< "Read " << total << " out of " << size0012 << "\n";
		exit(0);
	}

	return total;
}

std::size_t emtr::readV13(std::istream& file)
{
	std::size_t size0013;
	std::size_t total = base::readFormHeader(file, "0013", size0013);
	size0013 += 8;

	total += _timing.read(file);

	total += _emitterTranslationX.read(file);
	total += _emitterTranslationY.read(file);
	total += _emitterTranslationZ.read(file);

	total += _emitterRotationX.read(file);
	total += _emitterRotationY.read(file);
	total += _emitterRotationZ.read(file);

	total += _emitterDistance.read(file);
	total += _emitterShapeSize.read(file);
	total += _emitterSpread.read(file);

	total += _particleGenerationRate.read(file);
	total += _particleEmitSpeed.read(file);
	total += _particleInheritVelocityPercent.read(file);
	total += _particleClusterCount.read(file);
	total += _particleClusterRadius.read(file);
	total += _particleLifeTime.read(file);
	total += _particleWeight.read(file);

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _emitterName);
	total += base::read(file, _emitterEmitDirection);
	total += base::read(file, _generationType);
	total += base::read(file, _emitterShape);
	total += base::read(file, _emitterLoopImmediately);
	total += base::read(file, _emitterLifetimeMin);
	total += base::read(file, _emitterLifetimeMax);
	total += base::read(file, _emitterMaxParticles);
	total += base::read(file, _emitterOneShot);
	total += base::read(file, _emitterOneShotMin);
	total += base::read(file, _emitterOneShotMax);
	total += base::read(file, _particleRandomInitialRotation);
	total += base::read(file, _particleOrientation);
	total += base::read(file, _particleVisible);
	uint32_t junk2;	total += base::read(file, junk2); // Skip 32-bit

	total += base::read(file, _particleType);
	total += base::read(file, _soundPath);

	total += base::read(file, _localSpaceParticles);

	total += base::read(file, _particleGroundCollision);
	total += base::read(file, _particleKillParticlesOnCollision);
	total += base::read(file, _particleCollisionHeightAboveGround);
	total += base::read(file, _particleForwardVelocityMaintainedMin);
	total += base::read(file, _particleForwardVelocityMaintainedMax);
	total += base::read(file, _particleUpVelocityMaintainedMin);
	total += base::read(file, _particleUpVelocityMaintainedMax);
	total += base::read(file, _windResistenceGlobalPercent);
	total += base::read(file, _emitterLODDistanceMin);
	total += base::read(file, _emitterLODDistanceMax);
	total += base::read(file, _flockingType);
	total += base::read(file, _flockingSeperationDistance);
	total += base::read(file, _flockingAlignmentGain);
	total += base::read(file, _flockingCohesionGain);
	total += base::read(file, _flockingSeperationGain);
	total += base::read(file, _flockingCageWidth);
	total += base::read(file, _flockingCageHeight);
	total += base::read(file, _flockingCageShape);
	total += base::read(file, _flockingCageEdgeType);
	total += base::read(file, _flockingHeadingChangeRadian);
	total += base::read(file, _flockingHeadingChangeTime);
	total += base::read(file, _particleTimeOfDayColorPercent);
	total += base::read(file, _particleSnapToTerrainOnCreation);
	total += base::read(file, _particleAlignToTerrainNormalOnCreation);
	total += base::read(file, _particleSnapToTerrainOnCreationHeight);
	total += base::read(file, _particleChangeDirectionRadian);
	total += base::read(file, _particleChangeDirectionTime);
	total += base::read(file, _firstParticleImmediately);

	//particle description quad
	if (0 == _particleType) {
		_ptcl = ptqdPtr(new ptqd());
	}
	else if (1 == _particleType) {
		_ptcl = ptmhPtr(new ptmh());
	}
	else {
		std::cout << "Unhandled particle type: " << _particleType << "\n";
		exit(0);
	}
	total += _ptcl->read(file);

	if (size0013 != total) {
		std::cout << "FAILED in reading 0013\n"
			<< "Read " << total << " out of " << size0013 << "\n";
		exit(0);
	}

	return total;
}

std::size_t emtr::readV14(std::istream& file)
{
	std::size_t size0014;
	std::size_t total = base::readFormHeader(file, "0014", size0014);
	size0014 += 8;

	total += _timing.read(file);

	total += _emitterTranslationX.read(file);
	total += _emitterTranslationY.read(file);
	total += _emitterTranslationZ.read(file);

	total += _emitterRotationX.read(file);
	total += _emitterRotationY.read(file);
	total += _emitterRotationZ.read(file);

	total += _emitterDistance.read(file);
	total += _emitterShapeSize.read(file);
	total += _emitterSpread.read(file);

	total += _particleGenerationRate.read(file);
	total += _particleEmitSpeed.read(file);
	total += _particleInheritVelocityPercent.read(file);
	total += _particleClusterCount.read(file);
	total += _particleClusterRadius.read(file);
	total += _particleLifeTime.read(file);
	total += _particleWeight.read(file);

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _emitterName);
	total += base::read(file, _emitterEmitDirection);
	total += base::read(file, _generationType);
	total += base::read(file, _emitterShape);
	total += base::read(file, _emitterLoopImmediately);
	total += base::read(file, _emitterLifetimeMin);
	total += base::read(file, _emitterLifetimeMax);
	total += base::read(file, _emitterMaxParticles);
	total += base::read(file, _emitterOneShot);
	total += base::read(file, _emitterOneShotMin);
	total += base::read(file, _emitterOneShotMax);
	total += base::read(file, _particleRandomInitialRotation);
	total += base::read(file, _particleOrientation);
	total += base::read(file, _particleVisible);
	uint32_t junk2;	total += base::read(file, junk2); // Skip 32-bit

	total += base::read(file, _particleType);
	total += base::read(file, _soundPath);

	total += base::read(file, _localSpaceParticles);

	total += base::read(file, _particleGroundCollision);
	total += base::read(file, _particleKillParticlesOnCollision);
	total += base::read(file, _particleCollisionHeightAboveGround);
	total += base::read(file, _particleForwardVelocityMaintainedMin);
	total += base::read(file, _particleForwardVelocityMaintainedMax);
	total += base::read(file, _particleUpVelocityMaintainedMin);
	total += base::read(file, _particleUpVelocityMaintainedMax);
	total += base::read(file, _windResistenceGlobalPercent);
	total += base::read(file, _emitterLODDistanceMin);
	total += base::read(file, _emitterLODDistanceMax);
	total += base::read(file, _flockingType);
	total += base::read(file, _flockingSeperationDistance);
	total += base::read(file, _flockingAlignmentGain);
	total += base::read(file, _flockingCohesionGain);
	total += base::read(file, _flockingSeperationGain);
	total += base::read(file, _flockingCageWidth);
	total += base::read(file, _flockingCageHeight);
	total += base::read(file, _flockingCageShape);
	total += base::read(file, _flockingCageEdgeType);
	total += base::read(file, _flockingHeadingChangeRadian);
	total += base::read(file, _flockingHeadingChangeTime);
	total += base::read(file, _particleTimeOfDayColorPercent);
	total += base::read(file, _particleSnapToTerrainOnCreation);
	total += base::read(file, _particleAlignToTerrainNormalOnCreation);
	total += base::read(file, _particleSnapToTerrainOnCreationHeight);
	total += base::read(file, _particleChangeDirectionRadian);
	total += base::read(file, _particleChangeDirectionTime);
	total += base::read(file, _firstParticleImmediately);
	total += base::read(file, _usePriorityParticles);

	//particle description quad
	if (0 == _particleType) {
		_ptcl = ptqdPtr(new ptqd());
	}
	else if (1 == _particleType) {
		_ptcl = ptmhPtr(new ptmh());
	}
	else {
		std::cout << "Unhandled particle type: " << _particleType << "\n";
		exit(0);
	}
	total += _ptcl->read(file);

	if (size0014 != total) {
		std::cout << "FAILED in reading 0014\n"
			<< "Read " << total << " out of " << size0014 << "\n";
		exit(0);
	}

	return total;
}

const wvfm& emtr::getEmitterTranslationX() const { return _emitterTranslationX; }
const wvfm& emtr::getEmitterTranslationY() const { return _emitterTranslationY; }
const wvfm& emtr::getEmitterTranslationZ() const { return _emitterTranslationZ; }

const wvfm& emtr::getEmitterRotationX() const { return _emitterRotationX; }
const wvfm& emtr::getEmitterRotationY() const { return _emitterRotationY; }
const wvfm& emtr::getEmitterRotationZ() const { return _emitterRotationZ; }

const wvfm& emtr::getEmitterDistance() const { return _emitterDistance; }
const wvfm& emtr::getEmitterShapeSize() const { return _emitterShapeSize; }
const wvfm& emtr::getEmitterSpread() const { return _emitterSpread; }

const wvfm& emtr::getParticleGenerationRate() const { return _particleGenerationRate; }
const wvfm& emtr::getParticleEmitSpeed() const { return _particleEmitSpeed; }
const wvfm& emtr::getParticleInheritVelocityPercent() const { return _particleInheritVelocityPercent; }
const wvfm& emtr::getParticleClusterCount() const { return _particleClusterCount; }
const wvfm& emtr::getParticleClusterRadius() const { return _particleClusterRadius; }
const wvfm& emtr::getParticleLifeTime() const { return _particleLifeTime; }
const wvfm& emtr::getParticleWeight() const { return _particleWeight; }

const std::string& emtr::getEmitterName() const { return _emitterName; }
const int32_t& emtr::getEmitterEmitDirection() const { return _emitterEmitDirection; }
const int32_t& emtr::getGenerationType() const { return _generationType; }
const uint32_t& emtr::getEmitterShape() const { return _emitterShape; }
const bool& emtr::emitterLoopImmediately() const { return _emitterLoopImmediately; }
const float& emtr::getEmitterLifetimeMin() const { return _emitterLifetimeMin; }
const float& emtr::getEmitterLifetimeMax() const { return _emitterLifetimeMax; }
const float& emtr::getEmitterMaxParticles() const { return _emitterMaxParticles; }
const bool& emtr::emitterOneShot() const { return _emitterOneShot; }
const int32_t& emtr::getEmitterOneShotMin() const { return _emitterOneShotMin; }
const int32_t& emtr::getEmitterOneShotMax() const { return _emitterOneShotMax; }
const bool& emtr::particleRandomInitialRotation() const { return _particleRandomInitialRotation; }
const uint32_t& emtr::getParticleOrientation() const { return _particleOrientation; }
const bool& emtr::particleVisible() const { return _particleVisible; }
const int32_t& emtr::getParticleType() const { return _particleType; }
const std::string& emtr::getSoundPath() const { return _soundPath; }
const bool& emtr::localSpaceParticles() const { return _localSpaceParticles; }
const bool& emtr::particleGroundCollision() const { return _particleGroundCollision; }
const bool& emtr::particleKillParticlesOnCollision() const { return _particleKillParticlesOnCollision; }
const float& emtr::getParticleCollisionHeightAboveGround() const { return _particleCollisionHeightAboveGround; }
const float& emtr::getParticleForwardVelocityMaintainedMin() const { return _particleForwardVelocityMaintainedMin; }
const float& emtr::getParticleForwardVelocityMaintainedMax() const { return _particleForwardVelocityMaintainedMax; }
const float& emtr::getParticleUpVelocityMaintainedMin() const { return _particleUpVelocityMaintainedMin; }
const float& emtr::getParticleUpVelocityMaintainedMax() const { return _particleUpVelocityMaintainedMax; }
const float& emtr::getWindResistenceGlobalPercent() const { return _windResistenceGlobalPercent; }
const float& emtr::getEmitterLODDistanceMin() const { return _emitterLODDistanceMin; }
const float& emtr::getEmitterLODDistanceMax() const { return _emitterLODDistanceMax; }
const uint32_t& emtr::getFlockingType() const { return _flockingType; }
const float& emtr::getFlockingSeperationDistance() const { return _flockingSeperationDistance; }
const float& emtr::getFlockingAlignmentGain() const { return _flockingAlignmentGain; }
const float& emtr::getFlockingCohesionGain() const { return _flockingCohesionGain; }
const float& emtr::getFlockingSeperationGain() const { return _flockingSeperationGain; }
const float& emtr::getFlockingCageWidth() const { return _flockingCageWidth; }
const float& emtr::getFlockingCageHeight() const { return _flockingCageHeight; }
const int32_t& emtr::getFlockingCageShape() const { return _flockingCageShape; }
const int32_t& emtr::getFlockingCageEdgeType() const { return _flockingCageEdgeType; }
const float& emtr::getFlockingHeadingChangeRadian() const { return _flockingHeadingChangeRadian; }
const float& emtr::getFlockingHeadingChangeTime() const { return _flockingHeadingChangeTime; }
const float& emtr::getParticleTimeOfDayColorPercent() const { return _particleTimeOfDayColorPercent; }
const bool& emtr::particleSnapToTerrainOnCreation() const { return _particleSnapToTerrainOnCreation; }
const bool& emtr::particleAlignToTerrainNormalOnCreation() const { return _particleAlignToTerrainNormalOnCreation; }
const float& emtr::getParticleSnapToTerrainOnCreationHeight() const { return _particleSnapToTerrainOnCreationHeight; }
const float& emtr::getParticleChangeDirectionRadian() const { return _particleChangeDirectionRadian; }
const float& emtr::getParticleChangeDirectionTime() const { return _particleChangeDirectionTime; }
const bool& emtr::firstParticleImmediately() const { return _firstParticleImmediately; }
const bool& emtr::usePriorityParticles() const { return _usePriorityParticles; }
