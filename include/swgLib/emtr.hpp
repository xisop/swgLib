/** -*-c++-*-
 *  \class  emtr
 *  \file   emtr.hpp
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

#include <swgLib/ptcl.hpp>
#include <swgLib/ptim.hpp>
#include <swgLib/ptmh.hpp>
#include <swgLib/ptqd.hpp>
#include <swgLib/wvfm.hpp>

#include <istream>
#include <string>

#ifndef EMTR_HPP
#define EMTR_HPP 1

namespace ml
{
	class emtr
	{
	public:
		emtr();
		~emtr();

		std::size_t read(std::istream& file);

		const wvfm& getEmitterTranslationX() const;
		const wvfm& getEmitterTranslationY() const;
		const wvfm& getEmitterTranslationZ() const;

		const wvfm& getEmitterRotationX() const;
		const wvfm& getEmitterRotationY() const;
		const wvfm& getEmitterRotationZ() const;

		const wvfm& getEmitterDistance() const;
		const wvfm& getEmitterShapeSize() const;
		const wvfm& getEmitterSpread() const;

		const wvfm &getParticleGenerationRate() const;
		const wvfm &getParticleEmitSpeed() const;
		const wvfm &getParticleInheritVelocityPercent() const;
		const wvfm &getParticleClusterCount() const;
		const wvfm &getParticleClusterRadius() const;
		const wvfm &getParticleLifeTime() const;
		const wvfm &getParticleWeight() const;

		const std::string &getEmitterName() const;
		const int32_t &getEmitterEmitDirection() const;
		const int32_t &getGenerationType() const;
		const uint32_t &getEmitterShape() const;
		const bool &emitterLoopImmediately() const;
		const float &getEmitterLifetimeMin() const;
		const float &getEmitterLifetimeMax() const;
		const float &getEmitterMaxParticles() const;
		const bool &emitterOneShot() const;
		const int32_t &getEmitterOneShotMin() const;
		const int32_t &getEmitterOneShotMax() const;
		const bool &particleRandomInitialRotation() const;
		const uint32_t &getParticleOrientation() const;
		const bool &particleVisible() const;
		const int32_t &getParticleType() const;
		const std::string &getSoundPath() const;
		const bool &localSpaceParticles() const;
		const bool &particleGroundCollision() const;
		const bool &particleKillParticlesOnCollision() const;
		const float &getParticleCollisionHeightAboveGround() const;
		const float &getParticleForwardVelocityMaintainedMin() const;
		const float &getParticleForwardVelocityMaintainedMax() const;
		const float &getParticleUpVelocityMaintainedMin() const;
		const float &getParticleUpVelocityMaintainedMax() const;
		const float &getWindResistenceGlobalPercent() const;
		const float &getEmitterLODDistanceMin() const;
		const float &getEmitterLODDistanceMax() const;
		const uint32_t &getFlockingType() const;
		const float &getFlockingSeperationDistance() const;
		const float &getFlockingAlignmentGain() const;
		const float &getFlockingCohesionGain() const;
		const float &getFlockingSeperationGain() const;
		const float &getFlockingCageWidth() const;
		const float &getFlockingCageHeight() const;
		const int32_t &getFlockingCageShape() const;
		const int32_t &getFlockingCageEdgeType() const;
		const float &getFlockingHeadingChangeRadian() const;
		const float &getFlockingHeadingChangeTime() const;
		const float &getParticleTimeOfDayColorPercent() const;
		const bool &particleSnapToTerrainOnCreation() const;
		const bool &particleAlignToTerrainNormalOnCreation() const;
		const float &getParticleSnapToTerrainOnCreationHeight() const;
		const float &getParticleChangeDirectionRadian() const;
		const float &getParticleChangeDirectionTime() const;
		const bool &firstParticleImmediately() const;
		const bool &usePriorityParticles() const;

	protected:
		std::size_t readV0(std::istream& file);
		std::size_t readV1(std::istream& file);
		std::size_t readV2(std::istream& file);
		std::size_t readV3(std::istream& file);
		std::size_t readV4(std::istream& file);
		std::size_t readV5(std::istream& file);
		std::size_t readV6(std::istream& file);
		std::size_t readV7(std::istream& file);
		std::size_t readV8(std::istream& file);
		std::size_t readV9(std::istream& file);
		std::size_t readV10(std::istream& file);
		std::size_t readV11(std::istream& file);
		std::size_t readV12(std::istream& file);
		std::size_t readV13(std::istream& file);
		std::size_t readV14(std::istream& file);

	protected:
		uint32_t _version;
		ptim _timing;
		wvfm _emitterTranslationX;
		wvfm _emitterTranslationY;
		wvfm _emitterTranslationZ;

		wvfm _emitterRotationX;
		wvfm _emitterRotationY;
		wvfm _emitterRotationZ;

		wvfm _emitterDistance;
		wvfm _emitterShapeSize;
		wvfm _emitterSpread;

		wvfm _particleGenerationRate;
		wvfm _particleEmitSpeed;
		wvfm _particleInheritVelocityPercent;
		wvfm _particleClusterCount;
		wvfm _particleClusterRadius;
		wvfm _particleLifeTime;
		wvfm _particleWeight;

		std::string _emitterName;
		int32_t _emitterEmitDirection;
		int32_t _generationType;
		uint32_t _emitterShape;
		bool _emitterLoopImmediately;
		float _emitterLifetimeMin;
		float _emitterLifetimeMax;
		float _emitterMaxParticles;
		bool _emitterOneShot;
		int32_t _emitterOneShotMin;
		int32_t _emitterOneShotMax;
		bool _particleRandomInitialRotation;
		uint32_t _particleOrientation;
		bool _particleVisible;
		int32_t _particleType;
		std::string _soundPath;
		bool _localSpaceParticles;
		bool _particleGroundCollision;
		bool _particleKillParticlesOnCollision;
		float _particleCollisionHeightAboveGround;
		float _particleForwardVelocityMaintainedMin;
		float _particleForwardVelocityMaintainedMax;
		float _particleUpVelocityMaintainedMin;
		float _particleUpVelocityMaintainedMax;
		float _windResistenceGlobalPercent;
		float _emitterLODDistanceMin;
		float _emitterLODDistanceMax;
		uint32_t _flockingType;
		float _flockingSeperationDistance;
		float _flockingAlignmentGain;
		float _flockingCohesionGain;
		float _flockingSeperationGain;
		float _flockingCageWidth;
		float _flockingCageHeight;
		int32_t	_flockingCageShape;
		int32_t _flockingCageEdgeType;
		float _flockingHeadingChangeRadian;
		float _flockingHeadingChangeTime;
		float _particleTimeOfDayColorPercent;
		bool _particleSnapToTerrainOnCreation;
		bool _particleAlignToTerrainNormalOnCreation;
		float _particleSnapToTerrainOnCreationHeight;
		float _particleChangeDirectionRadian;
		float  _particleChangeDirectionTime;
		bool _firstParticleImmediately;
		bool _usePriorityParticles;

		ptclPtr _ptcl;

	private:
	};
}
#endif
