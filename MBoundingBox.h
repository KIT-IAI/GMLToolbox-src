#pragma once


///////////////////////////////////////////////////////////////////////////////
//  MBoundingBox                                                             //
//  Kleinster umschlieﬂender Quader einer bel. Geometrie                     //
///////////////////////////////////////////////////////////////////////////////
class  MBoundingBox
{
	// Konstruktor / Destruktor
	public:
		MBoundingBox();
		virtual ~MBoundingBox();

	// Public Operations
	public:
		MBoundingBox & operator= (	MBoundingBox & box );
	
		bool operator== (	MBoundingBox & box );
		bool equalXY ( MBoundingBox * box );

		void bbSet ( double sw[], double no[] );
		bool bbGet ( double sw[], double no[] );
		bool bbExist();

		void bbDelete();

		bool addBB ( double sw[], double no[] );
		bool addBB ( MBoundingBox * bb );

		bool addOffset ( double offset[] );

		bool inside (  MBoundingBox * bb );
		bool insideXY ( MBoundingBox * bb );

		bool fullInside ( MBoundingBox * bb );
		bool fullInsideXY (  MBoundingBox * bb );

    bool disjunct ( MBoundingBox * bb );

    static bool disjunct( double swA[], double noA[], double swB[], double noB[], double minPointDist );

	// Private Attributes
	private:
		double * BoundingBox;          //  6 Koordinaten des unteren linke und oberen rechten Eckpunkts
    double   minimalPointDistance; //  Minimaler Abstand zweier unterschiedlicher Koordinaten

	// Private Operations
		void newBB();
};