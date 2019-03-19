#pragma once

#include "point3.h"
#include <vector>

class Vertex
{ 
public: 
  Point3 p; // position

  Vector3 n; // normal (per vertice)
};

struct TexCoords
{
	float t;
	float s;
	float w;
};


class Edge
{
public: 
  Vertex* v[2]; 
};

class Face
{
public:
  Vertex *v[3]; 

  TexCoords *uv[3]; 

	// Constructor for face without texture coords
  Face(Vertex *a, Vertex *b, Vertex *c) {
    v[0] = a;
    v[1] = b;
    v[2] = c;
    uv[0] = nullptr;
    uv[1] = nullptr;
    uv[2] = nullptr;
  }

	// Constructor for face with texture coords
  Face(Vertex *a, Vertex *b, Vertex *c, TexCoords *at, TexCoords *bt,
       TexCoords *ct) {
    v[0] = a;
    v[1] = b;
    v[2] = c;
    uv[0] = at;
    uv[1] = bt;
    uv[2] = ct;
  }

  Vector3 n; // normal (per face)

  void ComputeNormal() {
    n= -( (v[1]->p - v[0]->p) % (v[2]->p - v[0]->p) ).Normalize();
  }

  
};

enum class TriangleWinding
{
	CCW, 
	CW 
};


class Mesh
{
  std::vector<Vertex> v; 
  std::vector<Face> f;   
  std::vector<Edge> e;   
  std::vector<TexCoords> texCoords; 
                           
public:  
  
	explicit Mesh(const char* const filename, const TriangleWinding triangleWinding = TriangleWinding::CW);


  void RenderNxF() const; 
  void RenderNxV(const bool useTexCoords = false) const; 
  void RenderWire() const; 
  
  bool LoadFromObj(const char* const filename); 
  
  void ComputeNormalsPerFace();
  void ComputeNormalsPerVertex();
  
  void ComputeBoundingBox();

  Point3 Center() const { return (bbmin+bbmax)/2.0; };
  
  Point3 bbmin,bbmax; // bounding box 
};
