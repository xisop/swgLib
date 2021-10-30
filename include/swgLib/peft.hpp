/** -*-c++-*-
 *  \class  peft
 *  \file   peft.hpp
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
#include <swgLib/emgp.hpp>
#include <swgLib/ptim.hpp>

#include <istream>
#include <string>
#include <vector>

#ifndef PEFT_HPP
#define PEFT_HPP 1

namespace ml
{
	class peft
	{
	public:
		peft();
		~peft();

		std::size_t readPEFT(std::istream& file);

		const uint32_t& getVersion() const;
		const ptim& getTiming() const;
		const int32_t& getNumEmitterGroups() const;
		const float& getInitialPlaybackRate() const;
		const float& getInitialPlaybackRateTime() const;
		const float& getPlaybackRate() const;
		const float& getScale() const;
		const std::vector<emgp>& getEmitterGroups() const;

	protected:
		uint32_t _version;

		ptim _timing;
		int32_t _numEmitterGroups;
		float _initialPlaybackRate;
		float _initialPlaybackRateTime;
		float _playbackRate;
		float _scale;
		std::vector<emgp> _emitterGroups;
	};
}
#endif
