#include <stdio.h>
#include <string.h>
#include <vector>
#include <GL/gl.h>
#include "point3.h"
#include "mesh.h"

extern bool useWireframe; 

void Mesh::ComputeNormalsPerFace()
{
  for (size_t i=0; i<f.size(); i++)
  {
    f[i].ComputeNormal();
  }
}


void Mesh::ComputeNormalsPerVertex()
{
  for (size_t i=0; i<v.size(); i++) {
    v[i].n = Point3(0.f,0.f,0.f);
  }

  for (size_t i=0; i<f.size(); i++) {
    f[i].v[0]->n=f[i].v[0]->n + f[i].n;
    f[i].v[1]->n=f[i].v[1]->n + f[i].n;
    f[i].v[2]->n=f[i].v[2]->n + f[i].n;
  }

  for (size_t i=0; i<v.size(); i++) {
    v[i].n = -v[i].n.Normalize();
  }
  
}

void Mesh::RenderWire() const{
  glLineWidth(1.0);
  for (size_t i=0; i<f.size(); i++)
  {
    glBegin(GL_LINE_LOOP);
      f[i].n.SendAsNormal();    
      (f[i].v[0])->p.SendAsVertex();   
      (f[i].v[1])->p.SendAsVertex();   
      (f[i].v[2])->p.SendAsVertex();
    glEnd();
  }
}

Mesh::Mesh(const char* const filename, const TriangleWinding triangleWinding)
{
	bool isMeshLoaded = LoadFromObj(filename);
	assert(isMeshLoaded);

	if (triangleWinding == TriangleWinding::CCW)
	{
		for (Face& face : f)
		{
			std::swap(face.v[0], face.v[2]);
			std::swap(face.uv[0], face.uv[2]);
		}
	}

	ComputeNormalsPerFace();
	ComputeNormalsPerVertex();
	ComputeBoundingBox();
}
void Mesh::RenderNxF() const
{
  glBegin(GL_TRIANGLES);
  for (size_t i=0; i<f.size(); i++)
  {
    f[i].n.SendAsNormal(); // flat shading
    
    f[i].v[0]->p.SendAsVertex();    
    f[i].v[1]->p.SendAsVertex();    
    f[i].v[2]->p.SendAsVertex();
  }
  glEnd();
}

//  (GOURAUD SHADING)
void Mesh::RenderNxV(const bool useTexCoords) const
{
  
  glBegin(GL_TRIANGLES);


  for (size_t faceIndex=0; faceIndex<f.size(); faceIndex++)
  {
	const Face& face = f[faceIndex];
	for (size_t vertexIndex = 0; vertexIndex < 3; vertexIndex++)
	{
		const Vertex* vert = face.v[vertexIndex];
		const TexCoords* texCoords = face.uv[vertexIndex];

		assert(vert);
		vert->n.SendAsNormal(); // gouroud shading
		if (useTexCoords) {
			assert(texCoords);
			glTexCoord3f(texCoords->t, texCoords->s, texCoords->w);
		}
		vert->p.SendAsVertex();
	}
  }
  glEnd();
}

//ALIGNED BOUNDIG BOX
void Mesh::ComputeBoundingBox(){

  if (!v.size()) return;
  bbmin=bbmax=v[0].p;
  for (size_t i=0; i<v.size(); i++){
    for (size_t k=0; k<3; k++) {
      if (bbmin.coord[k]>v[i].p.coord[k]) bbmin.coord[k]=v[i].p.coord[k];
      if (bbmax.coord[k]<v[i].p.coord[k]) bbmax.coord[k]=v[i].p.coord[k];
    }
  }
}

#ifdef DO_LOG_OBJ
#define LOG_OBJ(stringFormat, ...) printf((stringFormat), __VA_ARGS__);
#else
#define LOG_OBJ(f_, ...)
#endif

// load the mesh from a file in Obj format
// Note: in the file, both quads and tris can be present
// but in the internal representation (Mesh class) we have only three of them.

bool Mesh::LoadFromObj(const char* const filename)
{
  
  FILE* file = fopen(filename, "rt"); 
  if (!file) {
	  LOG_OBJ("%s %s \n", "Error opening file:", filename);
	  return false;
  }

  LOG_OBJ("%s %s \n", "Parsing obj", filename);

  //first pass through the file to get a count of the number
  //of vertices, normals, texcoords & triangles 
  char buf[128];
  int nv,nf,nt;
  float x,y,z;
  int va,vb,vc,vd;
  int na,nb,nc,nd;
  int ta,tb,tc,td;

  nv=0; nf=0; nt=0;
  while(fscanf(file, "%s", buf) != EOF) {
        switch(buf[0]) {
        case '#':               
            fgets(buf, sizeof(buf), file);
            break;
        case 'v':               // v, vn, vt
            switch(buf[1]) {
            case '\0':          // vertex

                fgets(buf, sizeof(buf), file);
                nv++;
                break;
            default:
                break;
            }
            break;
         case 'f':               // face
                fscanf(file, "%s", buf);
                // can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d
                if (strstr(buf, "//")) {
                    // v//n
                    sscanf(buf, "%d//%d", &va, &na);
                    fscanf(file, "%d//%d", &vb, &nb);
                    fscanf(file, "%d//%d", &vc, &nc);
                    nf++;
                    nt++;
                    while(fscanf(file, "%d//%d", &vd, &nd) > 0) {
                        nt++;
                    }
                } else if (sscanf(buf, "%d/%d/%d", &va, &ta, &na) == 3) {
                    // v/t/n
                    fscanf(file, "%d/%d/%d", &vb, &tb, &nb);
                    fscanf(file, "%d/%d/%d", &vc, &tc, &nc);
                    nf++;
                    nt++;
                    while(fscanf(file, "%d/%d/%d", &vd, &td, &nd) > 0) {
                        nt++;
                    }
                 } else if (sscanf(buf, "%d/%d", &va, &ta) == 2) {
                    // v/t
                    fscanf(file, "%d/%d", &vb, &tb);
                    fscanf(file, "%d/%d", &vc, &tc);
                    nf++;
                    nt++;
                    while(fscanf(file, "%d/%d", &vd, &td) > 0) {
                        nt++;
                    }
                } else {
                    // v
                    fscanf(file, "%d", &va);
                    fscanf(file, "%d", &vb);
                    nf++;
                    nt++;
                    while(fscanf(file, "%d", &vc) > 0) {
                        nt++;
                    }
                }
                break;

            default:
                fgets(buf, sizeof(buf), file);
                break;
        }
   }
 
 LOG_OBJ("dopo FirstPass nv=%d nf=%d nt=%d\n",nv,nf,nt);

  v.resize(nv);

  texCoords.resize(nt);

  rewind(file);
  
  //on the second pass through the file, read all the data into the
  //allocated arrays

  nv = 0;
  int tcCounter = 0;
  while(fscanf(file, "%s", buf) != EOF) {
      switch(buf[0]) {
      case '#':               // comment
            // eat up rest of line
            fgets(buf, sizeof(buf), file);
            break;
      case 'v':               // v, vn, vt
            switch(buf[1]) {
            case '\0':          // vertex
                fscanf(file, "%f %f %f", &x, &y, &z);
                v.at(nv).p = Point3( x,y,z );
                nv++;
                break;
			case 't': // texture coords
				{
					float s;
					float t;
					float w;
					int result = fscanf(file, "%f %f %f", &s, &t, &w);
					if (result == 2)
					{
						// le coordinate sono flipped
						texCoords.at(tcCounter) = TexCoords{ 1.f-s, 1.f-t, 1.f };
					}
					else if (result == 3)
					{
						texCoords.at(tcCounter) = TexCoords{ 1.f-s, 1.f-t, w};
					}
					else
					{
						assert(false);
					}
					tcCounter++;
				}
				break;
            default:
                break;
            }
            break;


     case 'f':               // face
           fscanf(file, "%s", buf);
           // can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d
           if (strstr(buf, "//")) {
              // v//n
              sscanf(buf, "%d//%d", &va, &na);
              fscanf(file, "%d//%d", &vb, &nb);
              fscanf(file, "%d//%d", &vc, &nc);
              va--; vb--; vc--;
              Face newface( &v.at(va), &v.at(vc), &v.at(vb));
              f.push_back( newface );
              vb=vc;
              while(fscanf(file, "%d//%d", &vc, &nc) > 0) {
                 vc--;
                 Face newface( &v.at(va), &v.at(vc), &v.at(vb)); 
                 f.push_back( newface ); 
                 nt++;
                 vb=vc;
              }
           } else if (sscanf(buf, "%d/%d/%d", &va, &ta, &na) == 3) {
                // v/t/n
                fscanf(file, "%d/%d/%d", &vb, &tb, &nb);
                fscanf(file, "%d/%d/%d", &vc, &tc, &nc);
                va--; vb--; vc--; ta--;tb--;tc--;
                Face newface( &v.at(va), &v.at(vc), &v.at(vb), &texCoords.at(ta), &texCoords.at(tc), &texCoords.at(tb)); 
                f.push_back( newface ); 
				vb = vc;tb = tc;
                while(fscanf(file, "%d/%d/%d", &vc, &tc, &nc) > 0) {
					vc--;tc--;
                   Face newface( &v.at(va), &v.at(vc), &v.at(vb), &texCoords.at(ta), &texCoords.at(tc), &texCoords.at(tb)); 
                   f.push_back( newface ); 
                   nt++;
				   vb = vc;tb = tc;
                }
           } else if (sscanf(buf, "%d/%d", &va, &ta) == 2) {
                // v/t
                fscanf(file, "%d/%d", &vb, &tb);
                fscanf(file, "%d/%d", &vc, &tc);
				va--; vb--; vc--; ta--;tb--;tc--;
				
                Face newface( &v.at(va), &v.at(vc), &v.at(vb), &texCoords.at(ta), &texCoords.at(tc), &texCoords.at(tb)); 
                f.push_back( newface ); 
				vb = vc;tb = tc;
                while(fscanf(file, "%d/%d", &vc, &tc) > 0) {
					vc--;tc--;
                   Face newface( &v.at(va), &v.at(vc), &v.at(vb), &texCoords.at(ta), &texCoords.at(tc), &texCoords.at(tb)); 
                   f.push_back( newface ); 
				   vb = vc;tb = tc;
                }
           } else {
                // v
                sscanf(buf, "%d", &va);
                fscanf(file, "%d", &vb);
                fscanf(file, "%d", &vc);
                va--; vb--; vc--;
                Face newface( &v.at(va), &v.at(vc), &v.at(vb)); 
                f.push_back( newface ); 
                vb=vc;
                while(fscanf(file, "%d", &vc) > 0) {
                   vc--;
                   Face newface( &v.at(va), &v.at(vc), &v.at(vb));
                   f.push_back( newface );
                   vb=vc;
                }
            }
            break;

            default:
                // eat up rest of line
                fgets(buf, sizeof(buf), file);
                break;
    }
  }
  
  fclose(file);
  return true;
}
