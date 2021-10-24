/** -*-c++-*-
 *  \class  cldf
 *  \file   cldf.hpp
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

#include <istream>
#include <ostream>

#ifndef CLDF_HPP
#define CLDF_HPP 1

namespace ml
{
	// Client Data File
	class cldf
	{
	public:
		cldf();
		~cldf();

		std::size_t read(std::istream& file);
		void print(std::ostream& os) const;

	public:
		class damageLevel {
		public:
			damageLevel();
			~damageLevel();

			std::size_t read(std::istream& file);
			void print(std::ostream& os) const;

		private:
			float _minDamageLevel;
			float _maxDamageLevel;
			std::string _clientEffectName;
			std::string _appearanceName;
			bool _useHardpoint;
			float _positionX;
			float _positionY;
			float _positionZ;
			float _yaw;
			float _pitch;
			float _roll;
			std::string _hardpointName;
			std::string _ambientSoundName;
		};

	private:
		// Damage levels
		std::vector<class damageLevel> _damageLevel;

		// Ambient sound
		std::string _ambientSoundFilename;

		// Client Effect Template - CEFT
		// std::map<>;

		// Transform child object (CHLD / CHL2 / WING)
		//std::vector<>;

		// Clear Flora (CNCF)
		//std::vector<>

		// Cached Sound (CSND)
		//std::map<>

		// Event (EVNT)
		// std::map<>

		// Hardpoint child object (HOBJ)
		//std::vector<>

		// Initial hardpoint child object (IHOB)
		// std::vector<>

		// Transform light object (LOBJ)
		// std::vector<>

		// On/Off Object (ONOF)
		// std::vector<>

		// Sun tracking object (SUNT)
		//

		// Turret object data (TURR)

		// Wearable (WEAR)
		// std::vector<>

		// Flag (FLAG)
		// std::vector<>

		// Banner (BANR)
		// std::vector<>

		// Customization int (CSSI)
		// std::vector<>

		// Vehicle thruster data (VTHR)
		// std::vector<>

		// Vehicle ground effect data (VGEF)
		// std::vector<>

		// Vehicle lightning effect data (VLEF)
		// std::vector<>

		// Contrail data (CONT)
		// std::vector<>

		// Glow data (GLOW)
		// std::vector<>

		// Interpolated sound (INTS)

		// Engine sound (ENGS)
		// std::vector<>

		// Breakpoint (BRKP)
		// std::vector<>

		// Destruction effect(DSTR)

		// Destruction sequence (DSEF)

	};
}

#endif
