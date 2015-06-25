#include "node.h"
#include "scene.h"
#include "camera.h"

#include <cassert>
#include <GL/glew.h>

using namespace engine3d;

Node::Node(const string& name) :
    Spatial(name),
    mpParent(nullptr),
    mpScene(nullptr),
    mpBound(nullptr), // no bounding volume by default
    mpCollisionBound(nullptr) // no bounding volume by default
{
}

void Node::Copy(const Node& n)
{

    Spatial::Copy(n);
    
    for (auto p_child : n.mChildren) {
        AddChild(p_child->Clone());
    }

    if (n.mpBound) {
        mpBound = n.mpBound->Clone();
    }
    if (n.mpCollisionBound) {
        mpCollisionBound = n.mpCollisionBound->Clone();
    }

}

Node* Node::Clone() const
{
    Debug("Node.Clone");
    Node* p_node;// = static_cast<Node*>(o);
    p_node= new Node();
    p_node->Copy(*this);
    return p_node;
}

Node::~Node()
{
	for (unsigned i = 0; i < mChildren.size(); i++) {
		assert(mChildren[i]);
        delete mChildren[i];
    }
    mChildren.clear();
}

bool Node::CheckCollisions(const Node& rNode1, const Node& rNode2) {
    if (!rNode1.mpCollisionBound) {
        return false;
    }
    if (rNode2.mpCollisionBound && rNode1.mpCollisionBound->Collides(*(rNode2.mpCollisionBound))) {
        return true;
    }
    for (auto&& p_node2_child : rNode2.mChildren) {
        if (&rNode1 == p_node2_child) {
            continue;
        }
        if (p_node2_child->mpCollisionBound && rNode1.mpCollisionBound->Collides(*p_node2_child->mpCollisionBound)) {
            return true;
        }
        if (CheckCollisions(rNode1, *p_node2_child)) {
            return true;
        }
    }
    return false;
}


void Node::CheckCollisions()
{
    long millis = GetMillis();
    Debug("Node::CheckCollisions 1");
    Debug("Node::CheckCollisions 5");
    for (auto&& p_child : mChildren) {
        Debug("Node::CheckCollisions 6");
        p_child->CheckCollisions();
        Debug("Node::CheckCollisions 7");
    }
    for (auto&& p_child : mChildren) {
        Debug("Node::CheckCollisions 8");
        Node* p_collision_with = nullptr;
        Debug("Node::CheckCollisions 9");
        for (auto&& p_other_child : mChildren) {
            Debug("Node::CheckCollisions 10");
            if (p_child == p_other_child) {
                Debug("Node::CheckCollisions 11");
                continue;
            }
            Debug("Node::CheckCollisions 12");
            //if (p_child->mpCollisionBound && (*j)->mpCollisionBound && p_child->mpCollisionBound->Collides(*(*j)->mpCollisionBound)) {
            if (CheckCollisions(*p_child, *p_other_child)) {
                Debug("Node::CheckCollisions 13");
                p_collision_with = p_other_child; // Currently only binary collisions are supported
                break;
            }
            Debug("Node::CheckCollisions 14");
        }
        if (!p_collision_with) {
            Debug("Node::CheckCollisions 15");
            p_child->Transform();
            if (p_child->mpCollisionBound) {
                p_child->mpCollisionBound->Transform();
            }
        } else {
            CollisionWith(*p_collision_with);
        }
    }
}

void Node::CollisionWith(const Node& rNode)
{
    Revert();
    if (mpCollisionBound) {
        mpCollisionBound->Revert();
    }
}

void Node::Init() {
    if (IsChanged()) {
    	Spatial::SetWorldTransformation(mpParent ? mpParent->GetWorldTransformation() * GetTransformation() : GetTransformation());
    }
    if (mpCollisionBound) {
        mpCollisionBound->SetTransformation(GetWorldTransformation());
    }
	for (auto&& p_child : mChildren) {
        if (p_child->IsChanged()) {
			p_child->Init();
        }
	}
}

void Node::Update() {
	for (auto&& p_child : mChildren) {
        Debug("Updating child");
        if (p_child->IsChanged()) {
        	p_child->Update();
        }
	}
}

void Node::Render()
{
    if (mpBound) {
        mpBound->SetTransformation(GetWorldTransformation());
        mpBound->Transform();
    }
    if (mpBound && GetScene().GetCamera().Cull(*mpBound)) {
        Debug("Object culled");
    }
    else {
		Debug("transforming");
		for (auto&& p_child : mChildren) {
	        Debug("Rendering child");
			p_child->Render();
		}
    }
    Spatial::Reset();
}

void Node::SetScene(Scene* pScene)
{
    assert(!mpParent);
	this->mpScene = pScene;
}

Node* Node::GetParent() const
{
    return mpParent;
}

Scene& Node::GetScene() const
{
    if (mpParent == nullptr) {
        assert(mpScene);
        return *mpScene;
    }
    else {
        return mpParent->GetScene();
    }
}

void Node::AddChild(Node* n) 
{
    assert(n);
    mChildren.push_back(n);
    n->mpParent = this;
}

Node& Node::GetChild(int i) const
{
    assert(i >= 0 && i < mChildren.size());
    return *mChildren[i];
}

/**
 * @return the child node with the given name or nullptr
 * if no such child element exists
 **/
Node* Node::GetChild(const string& name) const
{
    for (int i = 0; i < mChildren.size(); i++) {
        Debug(string("children[") + to_string(i) + "] = " + mChildren[i]->Name());
        if (mChildren[i]->Name() == name) {
            return mChildren[i];
        }
    }
    return nullptr;
}

void Node::RemoveChild(int i)
{
    assert(i >= 0 && i < mChildren.size());
    assert(mChildren[i]);
    delete mChildren[i];
    mChildren.erase(mChildren.begin() + i);
}

bool Node::IsChanged()
{
	if (Spatial::IsChanged()) {
		return true;
	}
    for (auto&& p_child : mChildren) {
        if (p_child->IsChanged()) {
            return true;
        }
    }
    return false;
}


bool Node::IsLeaf() const
{
    return mChildren.size() == 0;
}
