// Class automatically generated by Dev-C++ New Class wizard

#include <GL/gl.h>

#include "vertex.h"

using namespace engine3d;

Vertex::Vertex() : coords(0, 0, 0), worldCoords(0, 0, 0)
{
    color = NULL;
    texCoords = NULL;
}

Vertex::Vertex(double x, double y, double z) : coords(x, y, z), worldCoords(x, y, z)
{
//    coords = new Vector(x, y, z);
    color = NULL;
    texCoords = NULL;
//    worldCoords = NULL;
    //Color c(1.0, 1.0, 1.0, 1.0);
    //SetColor(&c);
}

Vertex::Vertex(const Vector& v) : coords(v), worldCoords(v) {
    color = NULL;
    texCoords = NULL;
//    worldCoords = NULL;
}

Vertex::Vertex(const Vertex& v) : coords(v.coords), worldCoords(v.worldCoords) {
    color = NULL;
    texCoords = NULL;
//    worldCoords = NULL;
    if (v.texCoords) {
        texCoords = new Vector(*v.texCoords);
    }
    if (v.color) {
        color = new Color(v.color->red, v.color->green, v.color->blue, v.color->alpha);
    }
}

Vertex::Vertex(const Vertex& vertex, const Vector& vect) : coords(vect), worldCoords(vect) {
    color = NULL;
    texCoords = NULL;
    if (vertex.texCoords) {
        texCoords = new Vector(*vertex.texCoords);
    }
    if (vertex.color) {
        color = new Color(vertex.color->red, vertex.color->green, vertex.color->blue, vertex.color->alpha);
    }
    //color = new Color(1.0, 1.0, 1.0, 1.0);
}

Vertex* Vertex::Clone() const
{
    Debug("Vertex.Clone");
    return new Vertex(*this);
}

//Vertex::Vertex(int dim, fpn coords[]) : Vector(dim, coords)
//{
//}

// class destructor
Vertex::~Vertex()
{
//    delete coords;
    if (texCoords) {
        delete texCoords;
    }
    if (color) {
        delete color;
    }
//    if (worldCoords) {
//        delete worldCoords;
//    }
}

//double Vertex::GetCoord(int index)
//{
//    return coords->GetCoord(index);
//}
//
//const Vector& Vertex::GetCoords() const
//{
//    return *coords;
//}

void Vertex::SetColor(const Color& c)
{
    if (color) {
        delete color;
    }
    color = new Color(c.red, c.green, c.blue, c.alpha);
}

void Vertex::SetCoords(const Vector& coords)
{
    assert(coords.GetDim() == 3);
    this->coords = coords;
    this->worldCoords = coords;
}

void Vertex::SetCoords(double x, double y, double z)
{
    this->coords = Vector(x, y, z);
    this->worldCoords = this->coords;
}

void Vertex::SetTexCoords(const Vector& texCoords)
{
    assert(texCoords.GetDim() == 2);
    SetTexCoords(texCoords[0], texCoords[1]);
}

void Vertex::SetTexCoords(double s, double t)
{
    //Debug("set tex coords");
    if (texCoords) {
        //Debug("set tex coords0");
        delete texCoords;
    }
    //Debug("set tex coords1");
    if (s < 0.0f) {
        s = 0.0f;
    }
    if (t < 0.0f) {
        t = 0.0f;
    }
    if (s > 1.0f) {
        s = 1.0f;
    }
    if (t > 1.0f) {
        t = 1.0f;
    }
    //Debug("set tex coords2");
    this->texCoords = new Vector(s, t);
    //Debug("set tex coords3");
}

//const Vector& Vertex::GetTexCoords() const
//{
//    return *texCoords;
//}
//
//const Color& Vertex::GetColor() const
//{
//    return *color;
//}

void Vertex::Init()
{
    //long millis = GetMillis();
    Node::Init();
//    if (worldCoords) {
//        delete worldCoords;
//    }
    worldCoords = mWorldTransformation.Transform(coords);
    //out << "Vertex:worldCoords " << (*worldCoords)[0] << ", " << (*worldCoords)[1] << ", " << (*worldCoords)[2] << "\n";
    //Debug(String("Vertex::Init took ") + (GetMillis() - millis));
}

void Vertex::Render()
{
    //long millis = GetMillis();
    Node::Render();
    if (color) {
        glColor4d(color->red, color->green, color->blue, color->alpha);
    }
    if (texCoords) {
        //Debug(String("V::R tc ") + (*texCoords)[0] + ", " + (*texCoords)[1]);
        glTexCoord2d((*texCoords)[0], (*texCoords)[1]);
    }
    if (TEST_WORLD_COORDS) {
        ////Debug(String("TWC V::R ") + (worldCoords)[0] + ", " + (worldCoords)[1] + ", " + (worldCoords)[2]);
        glVertex3d((worldCoords)[0], (worldCoords)[1], (worldCoords)[2]);
    }
    else {
        ////Debug(String("WC V::R ") + (worldCoords)[0] + ", " + (worldCoords)[1] + ", " + (worldCoords)[2]);
        //Debug((String) "V::R " + (coords)[0] + ", " + (coords)[1] + ", " + (coords)[2]);
        glVertex3d((coords)[0], (coords)[1], (coords)[2]);
    }
    //Debug(String("Vertex::Render took ") + (GetMillis() - millis));
}

/**
 * Overrides Node::GetCenter
 * @returns the world coordinates of this vertex
 **/
//Vector Vertex::GetCenter() const
//{
//    //Debug("Vertex::GetCenter()");
//    return GetWorldCoords();
//}

