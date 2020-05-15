#pragma once

class PolygonGeo;
class Triangle;
class Matrix4;
///////////////////////////////////////////////////////////////////////////////
//  Triangulation ebener Polygone mit Innenkonturen                          //
///////////////////////////////////////////////////////////////////////////////
class Triangulation
{
  public:
    Triangulation();
    virtual ~Triangulation();

  private:
    PolygonGeo * pPolyToTriangulate;
    Matrix4    * pTransformation;

    unsigned int anzContours;       // Anzahl der Innenkonturen + 1
    unsigned int anzVertices;       // Anzahl der Kanten
    unsigned int anzTriangles;      // Gesamtzahl Dreiecke
    unsigned int anzOverflowTriangles; // Sicherheit gegen Fehler im Alg.

    int    * contours;
    double (* vertices)[2];
    int    (* triangles)[3];

  public:
    void triangulate ( PolygonGeo * pPoly, std::vector<Triangle*> &vTriangles );
    
};