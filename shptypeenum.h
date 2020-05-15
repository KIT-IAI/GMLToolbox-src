#ifndef SHPTYPEENUM_H
#define SHPTYPEENUM_H

enum SHPTypeEnum
{
	SHPT_NULL			= 0,
	SHPT_POINT			= 1,
	SHPT_ARC			= 3,
	SHPT_POLYGON		= 5,
	SHPT_MULTIPOINT		= 8,
	SHPT_POINT_Z		= 11,
	SHPT_ARC_Z			= 13,
	SHPT_POLYGON_Z		= 15,
	SHPT_MULTIPOINT_Z	= 18,
	SHPT_POINT_M		= 21,
	SHPT_ARC_M			= 23,
	SHPT_POLYGON_M		= 25,
	SHPT_MULTIPOINT_M	= 28,
	SHPT_MULTIPATCH		= 31
};

enum SHPPartType
{
	SHPTriangleStrip	= 0,
	SHPTriangleFan		= 1,
	SHPOuterRing		= 2,
	SHPInnerRing		= 3,
	SHPFirstRing		= 4,
	SHPRing				= 5
};

#endif	// SHPTYPEENUM_H