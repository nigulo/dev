#include <GL/gl.h>

#include "mesh.h"

using namespace engine3d;

// class constructor
Mesh::Mesh(int mode) : mElementBuffer(mode) {
}

void Mesh::Copy(const Mesh& rMesh)
{
    Shape::Copy(rMesh);
}

Mesh* Mesh::Clone()
{
    Debug("Mesh.Clone");
    Mesh* p_m = new Mesh(mElementBuffer.GetMode());
    p_m->Copy(*this);
    return p_m;
}

// class destructor
Mesh::~Mesh()
{
}

/** 
 * Adds new vertex to the set of vertices.
 * This method clones vertex internally so use it only
 * after all vertex attributes have been set.
 * Otherwise use Node::AddChild
 **/
void Mesh::AddVertex(const Vertex& v)
{
    Vertex* vertex = new Vertex(v);
	AddChild(vertex);
}

/**
 * Adds new vertex to the set of vertices.
 **/
void Mesh::AddVertex(const Vector& v, const Color& color)
{
    Vertex* vertex = new Vertex(v);
    vertex->SetColor(color);
	AddChild(vertex);
}

int Mesh::GetSize() const 
{
    return mChildren.size();
}

/**
 * Adds new vertex to the set of vertices
 **/
void Mesh::AddVertex(const Vector& v, const Vector& texCoords)
{
    Vertex* vertex = new Vertex(v);
    vertex->SetTexCoords(texCoords);
	AddChild(vertex);
}


void Mesh::UpdateBuffers() {
	if (mChildren.empty()) {
		return;
	}
	int dim = (dynamic_cast<Vertex*>(mChildren[0]))->GetCoords().GetDim();
	double vertex_coords[mChildren.size()][dim];
	//double vertex_tex_coords[mChildren.size()];
	int j = 0;
	int dim;
	for (auto i = mChildren.begin(); i != mChildren.end(); i++) {
        Vertex* p_vertex = dynamic_cast<Vertex*>(i);
        for (int k = 0; k < dim; k++) {
        	vertex_coords[j][k] = p_vertex->GetCoord(k);
        }
	}
	mVertexBuffer.SetData(mChildren.size() * dim * sizeof(double), &vertex_coords, GL_STATIC_DRAW);
}

void Mesh::Render() {
	mVertexBuffer.Render();
	mElementBuffer.Render();
}

// Sets texture coordinates for all vertices
void Mesh::SetTexCoords(vector<Vector*>& texCoords)
{
	assert(texCoords.size() == mChildren.size());
	for (unsigned i = 0; i < mChildren.size(); i++) {
        Vertex* p_child = dynamic_cast<Vertex*>(&GetChild(i));
        assert(p_child);
        p_child->SetTexCoords(*texCoords[i]);
    }
}

// Generates texture coordinates for the vertices
// Implement this method in sub-classes
void Mesh::GenTexCoords()
{
}

