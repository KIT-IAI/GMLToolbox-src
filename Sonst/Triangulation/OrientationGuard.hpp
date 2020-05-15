#pragma once

/// Corrects orientation before triangulation if necessary, and rolls that back
/// afterwards.
class OrientationGuard
{
public:
	OrientationGuard();

  void invertContour(IfcDB::Polyloop& contour);
	void invertHole(IfcDB::Polyloop& hole);

	void rollback(IfcDB::Polyloop& polyloop);

private:
	bool m_inverted;
};