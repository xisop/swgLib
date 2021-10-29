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

		uint32_t _emitterEmitDirection;
		uint32_t _emitterShape;
		bool _emitterLoopImmediately;
		float _emitterLifetimeMin;
		float _emitterLifetimeMax;
		float _emitterMaxParticles;
		bool _emitterOneShot;
		int32_t _emitterOneShotMin;
		int32_t _emitterOneShotMax;
		bool _particleRandomInitialRotation;
		bool _particleOrientWithVelocity;
		bool _particleVisible;

	private:
	};
}
#endif
