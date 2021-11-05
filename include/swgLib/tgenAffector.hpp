/** -*-c++-*-
 *  \class  tgenAffector
 *  \file   tgenAffector.hpp
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
#include <memory>
#include <vector>

#include <swgLib/tgenBaseLayer.hpp>
#include <swgLib/mfrc.hpp>
#include <swgLib/hdta.hpp>

#ifndef TGENAFFECTOR_HPP
#define TGENAFFECTOR_HPP 1

namespace ml
{
	typedef std::shared_ptr<class tgenAffector> tgenAffectorPtr;

	class tgenAffector : public tgenBaseLayer {
	public:
		struct point2D {
			float x, y;
		};

		struct point3D {
			float x, y, height;
		};
	public:
		static bool peekAffector(std::istream& file);
		virtual std::size_t read(std::istream& file);

		static std::size_t read(std::istream& file, tgenAffectorPtr& affectorPtr);

	};

	// AENV
	class affectorEnvironment : public tgenAffector {
	public:
		affectorEnvironment();
		~affectorEnvironment();

		std::size_t read(std::istream& file);

	protected:
		int32_t _familyId;
		bool _useFeatherClampOverride;
		float _featherClampOverride;
	};
	typedef std::shared_ptr<class affectorEnvironment> affectorEnvironmentPtr;

	// AHTR
	class affectorHeightTerrace : public tgenAffector {
	public:
		affectorHeightTerrace();
		~affectorHeightTerrace();

		std::size_t read(std::istream& file);

	protected:
		float _fraction;
		float _height;
		mfrc _multifractal;
	};
	typedef std::shared_ptr<class affectorHeightTerrace> affectorHeightTerracePtr;

	// AHCN
	class affectorHeightConstant : public tgenAffector {
	public:
		affectorHeightConstant();
		~affectorHeightConstant();

		std::size_t read(std::istream& file);

	protected:
		int32_t _operation;
		float   _height;
	};
	typedef std::shared_ptr<class affectorHeightConstant> affectorHeightConstantPtr;

	// AHFR
	class affectorHeightFractal : public tgenAffector {
	public:
		affectorHeightFractal();
		~affectorHeightFractal();

		std::size_t read(std::istream& file);

	protected:
		std::size_t readV0(std::istream& file);
		std::size_t readV1(std::istream& file);
		std::size_t readV2(std::istream& file);
		std::size_t readV3(std::istream& file);
	protected:
		int32_t _familyId;
		int32_t _operation;
		mfrc    _multifractal;
		float   _scaleVertical;

	};
	typedef std::shared_ptr<class affectorHeightFractal> affectorHeightFractalPtr;

	// ACCN
	class affectorColorConstant : public tgenAffector {
	public:
		affectorColorConstant();
		~affectorColorConstant();

		std::size_t read(std::istream& file);

	protected:
		int32_t _operation;
		uint8_t _r;
		uint8_t _g;
		uint8_t _b;
	};
	typedef std::shared_ptr<class affectorColorConstant> affectorColorConstantPtr;

	// ACRH
	class affectorColorRampHeight : public tgenAffector {
	public:
		affectorColorRampHeight();
		~affectorColorRampHeight();

		std::size_t read(std::istream& file);

	protected:
		int32_t     _operation;
		float       _low;
		float       _high;
		std::string _imageName;
	};
	typedef std::shared_ptr<class affectorColorRampHeight> affectorColorRampHeightPtr;

	// ACRF
	class affectorColorFractal : public tgenAffector {
	public:
		affectorColorFractal();
		~affectorColorFractal();

		std::size_t read(std::istream& file);

	protected:
		mfrc _multifractal;
		int32_t     _familyId;
		int32_t     _operation;
		std::string _imageName;
	};
	typedef std::shared_ptr<class affectorColorFractal> affectorColorFractalPtr;

	// ASCN
	class affectorShaderConstant : public tgenAffector {
	public:
		affectorShaderConstant();
		~affectorShaderConstant();

		std::size_t read(std::istream& file);

	protected:
		int32_t _familyId;
		bool _useFeatherClampOverride;
		float _featherClampOverride;
	};
	typedef std::shared_ptr<class affectorShaderConstant> affectorShaderConstantPtr;

	// ASRP
	class affectorShaderReplace : public tgenAffector {
	public:
		affectorShaderReplace();
		~affectorShaderReplace();

		std::size_t read(std::istream& file);

	protected:
		int32_t _sourceFamilyId;
		int32_t _destinationFamilyId;
		bool    _useFeatherClampOverride;
		float   _featherClampOverride;
	};
	typedef std::shared_ptr<class affectorShaderReplace> affectorShaderReplacePtr;

	// AFCN or AFSC
	class affectorFloraSCC : public tgenAffector {
	public:
		affectorFloraSCC();
		~affectorFloraSCC();

		std::size_t read(std::istream& file);

	protected:
		int32_t _familyId;
		int32_t _operation;
		bool    _removeAll;
		bool    _useDensityOverride;
		float   _densityOverride;
	};
	typedef std::shared_ptr<class affectorFloraSCC> affectorFloraSCCPtr;

	// AFSN
	class affectorFloraSNCC : public tgenAffector {
	public:
		affectorFloraSNCC();
		~affectorFloraSNCC();

		std::size_t read(std::istream& file);

	protected:
		int32_t _familyId;
		int32_t _operation;
		bool    _removeAll;
		bool    _useDensityOverride;
		float   _densityOverride;
	};
	typedef std::shared_ptr<class affectorFloraSNCC> affectorFloraSNCCPtr;

	// ARCN or AFDN
	class affectorFDNC : public tgenAffector {
	public:
		affectorFDNC();
		~affectorFDNC();

		std::size_t read(std::istream& file);

	protected:
		int32_t _familyId;
		int32_t _operation;
		bool    _removeAll;
		bool    _useDensityOverride;
		float   _densityOverride;
	};
	typedef std::shared_ptr<class affectorFDNC> affectorFDNCPtr;

	// AFDF
	class affectorFDFC : public tgenAffector {
	public:
		affectorFDFC();
		~affectorFDFC();

		std::size_t read(std::istream& file);

	protected:
		int32_t _familyId;
		int32_t _operation;
		bool    _removeAll;
		bool    _useDensityOverride;
		float   _densityOverride;
	};
	typedef std::shared_ptr<class affectorFDFC> affectorFDFCPtr;

	// ARIB
	class affectorRibbon : public tgenAffector {
	public:
		affectorRibbon();
		~affectorRibbon();

		std::size_t read(std::istream& file);

	protected:
		std::size_t readV0(std::istream& file);
		std::size_t readV1(std::istream& file);
		std::size_t readV2(std::istream& file);
		std::size_t readV3(std::istream& file);
		std::size_t readV4(std::istream& file);
		std::size_t readV5(std::istream& file);

	protected:
		hdta _heightData;
		std::vector<point3D> _points;
		float _width;
		float _waterShaderSize;
		float _velocity;
		float _capWidth;
		int32_t _terrainShaderFamilyId;
		float _featherDistanceTerrainShader;
		std::string _waterShaderName;
		int32_t _waterType;
	};
	typedef std::shared_ptr<class affectorRibbon> affectorRibbonPtr;

	// AEXC
	class affectorExclude : public tgenAffector {
	public:
		affectorExclude();
		~affectorExclude();

		std::size_t read(std::istream& file);

	protected:
	};
	typedef std::shared_ptr<class affectorExclude> affectorExcludePtr;

	// APAS
	class affectorPassable : public tgenAffector {
	public:
		affectorPassable();
		~affectorPassable();

		std::size_t read(std::istream& file);

	protected:
		bool _passable;
		float _featherThreshold;
	};
	typedef std::shared_ptr<class affectorPassable> affectorPassablePtr;

	// AROA
	class affectorRoad : public tgenAffector {
	public:
		affectorRoad();
		~affectorRoad();

		std::size_t read(std::istream& file);

	protected:
		std::size_t readV0(std::istream& file);
		std::size_t readV1(std::istream& file);
		std::size_t readV2(std::istream& file);
		std::size_t readV3(std::istream& file);
		std::size_t readV4(std::istream& file);
		std::size_t readV5(std::istream& file);
		std::size_t readV6(std::istream& file);

	protected:
		std::vector<point2D> _points;
		std::vector<float> _height;
		float _width;
		hdta _heightData;
		int32_t _familyId;
		int32_t _featherFunction;
		float _featherDistance;
		int32_t _featherFunctionShader;
		float _featherDistanceShader;
		bool _hasFixedHeights;

	};
	typedef std::shared_ptr<class affectorRoad> affectorRoadPtr;

	// ARIV
	class affectorRiver : public tgenAffector {
	public:
		affectorRiver();
		~affectorRiver();

		std::size_t read(std::istream& file);

	protected:
		std::size_t readV0(std::istream& file);
		std::size_t readV1(std::istream& file);
		std::size_t readV2(std::istream& file);
		std::size_t readV3(std::istream& file);
		std::size_t readV4(std::istream& file);
		std::size_t readV5(std::istream& file);
		std::size_t readV6(std::istream& file);

	protected:
		hdta _heightData;
		std::vector<point2D> _points;
		float _width;
		int32_t _bankFamilyId;
		int32_t _bottomFamilyId;
		int32_t _featherFunction;
		float _featherDistance;
		float _trenchDepth;
		float _velocity;
		bool _hasLocalWaterTable;
		float _localWaterTableDepth;
		float _localWaterTableShaderSize;
		float _localWaterTableWidth;
		std::string _localWaterTableShaderName;
		int32_t _waterType;

	};
	typedef std::shared_ptr<class affectorRiver> affectorRiverPtr;
}

#endif
