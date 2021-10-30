/** -*-c++-*-
 *  \class  ptcl
 *  \file   ptcl.hpp
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

#include <swgLib/clrr.hpp>
#include <swgLib/patt.hpp>
#include <swgLib/wvfm.hpp>

#include <istream>
#include <memory>
#include <string>

#ifndef PTCL_HPP
#define PTCL_HPP 1

namespace ml
{
	class ptcl
	{
	public:
		ptcl();
		virtual ~ptcl();

		virtual std::size_t read(std::istream& file);

		virtual const uint32_t& getVersion() const;
		const std::string& getName() const;
		const bool& randomRotationDirection() const;
		const int32_t& getNumAttachments() const;
		const clrr& getColorRamp() const;
		const wvfm& getAlpha() const;
		const wvfm& getSpeedScale() const;

		const std::vector<patt>& getAttachments() const;

		const wvfm& getParticleRelativeRotationX() const;
		const wvfm& getParticleRelativeRotationY() const;
		const wvfm& getParticleRelativeRotationZ() const;

		const bool& particleUsesRelativeRotation() const;

	protected:
		uint32_t _version;
		std::string _name;
		bool _randomRotationDirection;
		int32_t _numAttachments;

		clrr _colorRamp;
		wvfm _alpha;
		wvfm _speedScale;

		std::vector<patt> _attachments;

		wvfm _particleRelativeRotationX;
		wvfm _particleRelativeRotationY;
		wvfm _particleRelativeRotationZ;

		bool _particleUsesRelativeRotation;

	private:
	};
}
typedef std::shared_ptr<ml::ptcl> ptclPtr;
#endif
