/** -*-c++-*-
 *  \class  spaceTerrain
 *  \file   spaceTerrain.hpp
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
#include <string>
#include <vector>

#ifndef SPACETERRAIN_HPP
#define SPACETERRAIN_HPP 1

namespace ml
{
	// Space Terrain Appearance Template (SPACETERRAIN)
	class spaceTerrain
	{
	public:
		spaceTerrain();
		~spaceTerrain();

		std::size_t read(std::istream& file);
		void print() const;

	public:
		class light {
		public:
			light();
			~light();

			std::size_t read(std::istream& file);
			void print(std::ostream& os) const;

		protected:
			// DOT3 bump mapping
			bool _lightDOT3;

			// Light diffuse color
			float _lightDiffuseAlpha;
			float _lightDiffuseRed;
			float _lightDiffuseGreen;
			float _lightDiffuseBlue;

			// Light specular color
			float _lightSpecularAlpha;
			float _lightSpecularRed;
			float _lightSpecularGreen;
			float _lightSpecularBlue;

			// Light direction 
			float _lightDirX;
			float _lightDirY;
			float _lightDirZ;
		};

		class celestial {
		public:
			celestial();
			~celestial();

			std::size_t read(std::istream& file);
			void print(std::ostream& os) const;

		protected:
			std::string _backShaderTemplateName;
			float _backSize;
			std::string _frontShaderTemplateName;
			float _frontSize;
			float _directionX;
			float _directionY;
			float _directionZ;
		};

		class distance {
		public:
			distance();
			~distance();

			std::size_t readDIST(std::istream& file);
			std::size_t readPLAN(std::istream& file);
			void print(std::ostream& os) const;

		protected:
			std::string _templateName;
			float _directionX;
			float _directionY;
			float _directionZ;
			float _orienationYaw;
			float _orienationPitch;
			float _orienationRoll;
			float _haloRoll;
			float _haloScale;
			bool _infiniteDistance;
		};

	private:
		float _mapWidthInMeters;

		// Clear color
		float _clearColorRed;
		float _clearColorGreen;
		float _clearColorBlue;

		// Ambient color
		float _ambientAlpha;
		float _ambientRed;
		float _ambientGreen;
		float _ambientBlue;

		// List of lights
		std::vector<light> _light;

		std::string _environmentTextureName;

		// Fog information
		bool _fogEnabled;
		float _fogAlpha;
		float _fogRed;
		float _fogGreen;
		float _fogBlue;
		float _fogDensity;

		// Star information
		std::string _starColorRampName;
		int32_t _numberOfStars;

		// Dust information
		int32_t _numberOfDust;
		float _dustRadius;

		// SkyBox information
		bool _skyboxCubeMap;
		std::string _skyboxTextureName;

		// Distant objects and planets
		std::vector<distance> _distance;

		// Celestial information
		std::vector<celestial> _celestial;

	};
}
#endif
