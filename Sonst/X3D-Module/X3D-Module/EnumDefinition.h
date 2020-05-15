#pragma once

namespace X3D
{
	enum e_profile
	{
		Interchange,
		Interactive,
		Immersive,
		Full
	};

	static const char* profileValues[4] = { "Interchange", "Interactive", "Immersive", "Full" };

	enum e_inputType
	{
		button,
		checkbox
	};

	static const char* inputTypeValues[2] = { "button", "checkbox" };

	enum e_geomType
	{
		point,
		line,
		face
	};

	enum e_dataType
	{
		x3d,
		sceneGraph,
		all
	};
}