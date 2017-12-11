#include "Octree.h"
using namespace Simplex;

//declaring static variables
uint Octree::numOctant;
uint Octree::numLeaves;
uint Octree::maxLevel;
uint Octree::entityCount;

//constructor for root
Octree::Octree(uint p_MaxLevel, uint p_EntityCount)
{
	//creates the root octant
	Init();
	octRoot = this;
	maxLevel = p_MaxLevel;
	entityCount = p_EntityCount;

	//sets the max/min to center
	octantMax = octantMin = m_pEntityMngrFF->GetRigidBody()->GetCenterGlobal();
	currEntityCount = m_pEntityMngrFF->GetEntityCount();
	for (uint i = 0; i < currEntityCount; ++i)
	{
		entityList.push_back(i);

		//get min/max
		vector3 rb_min = m_pEntityMngrFF->GetRigidBody(i)->GetMinGlobal();
		vector3 rb_max = m_pEntityMngrFF->GetRigidBody(i)->GetMaxGlobal();

		// Setting min and max for X
		if (rb_min.x < octantMin.x)
		{
			octantMin.x = rb_min.x;
		}
		if (rb_max.x > octantMax.x)
		{
			octantMax.x = rb_max.x;
		}

		// Setting min and max for Y
		if (rb_min.y < octantMin.y)
		{
			octantMin.y = rb_min.y;
		}
		if (rb_max.y > octantMax.y)
		{
			octantMax.y = rb_max.y;
		}

		// Setting min and max for Z
		if (rb_min.z < octantMin.z)
		{
			octantMin.z = rb_min.z;
		}
		if (rb_max.z > octantMax.z)
		{
			octantMax.z = rb_max.z;
		}
	}

	//calculate center/size
	octantCenter = (octantMin + octantMax) / 2.f;
	octantSize = octantMax - octantMin;

	//create appropriate children
	Subdivide();

	//add leaf dimensions
	ConfigureDimensions();
}

//constructor for branch/leaf
Octree::Octree(vector3 p_Center, vector3 p_Size)
{
	Init();
	octantCenter = p_Center;
	octantSize = p_Size;
	octantMax =	p_Center + octantSize / 2.f;
	octantMin = p_Center - octantSize / 2.f;
}

//copy constructor
Octree::Octree(Octree const & other)
{
	Init();

	//copying data over
	currLevel = other.currLevel;
	octantSize = other.octantSize;
	octantCenter = other.octantCenter;
	octantMin = other.octantMin;
	octantMax = other.octantMax;
	octParent = other.octParent;
	Release();

	//loops through and recursively copy/create more nodes
	numChildren = other.numChildren;
	for (uint i = 0; i < numChildren; ++i)
	{
		octChildren[i] = new Octree(*other.octChildren[i]);
	}

	//create a new entity list
	currEntityCount = other.currEntityCount;
	for (uint i = 0; i < currEntityCount; ++i)
	{
		entityList.push_back(other.entityList[i]);
	}

	//if it's root, copy m_lChildren over
	octRoot = other.octRoot;
	if (this == octRoot)
	{
		float childCount = other.rootChildren.size();
		for (uint i = 0; i < childCount; ++i)
		{
			rootChildren.push_back(other.rootChildren[i]);
		}
	}
}

//destructor
Octree::~Octree(void)
{
	Release();
}

//Getters
vector3 Octree::GetSize(void) 
{ 
	return octantSize;
}

vector3 Octree::GetCenterGlobal(void)
{ 
	return octantCenter; 
}

vector3 Octree::GetMinGlobal(void)
{ 
	return octantMin;
}

vector3 Octree::GetMaxGlobal(void)
{ 
	return octantMax; 
}

uint Octree::GetOctantCount(void)
{ 
	return numOctant; 
}

uint Simplex::Octree::GetLeafCount(void)
{ 
	return numLeaves;
}

Octree * Octree::GetParent(void)
{
	return octParent;
}

Octree * Octree::GetChild(uint a_nChild)
{
	if (numChildren == 0)
	{
		return nullptr;
	}
	else
	{
		return octChildren[a_nChild];
	}
}
bool Octree::IsLeaf(void) 
{ 
	return numChildren == 0; 
}

bool Octree::ContainsMoreThan(uint a_nEntities) 
{ 
	return currEntityCount > a_nEntities;
}
bool Octree::IsColliding(uint a_uRBIndex)
{
	MyRigidBody* rb = m_pEntityMngrFF->GetRigidBody(a_uRBIndex);
	vector3 rb_max = rb->GetMaxGlobal();
	vector3 rb_min = rb->GetMinGlobal();
	if (rb_max.x > octantMin.x &&
		rb_max.y > octantMin.y &&
		rb_max.z > octantMin.z &&
		rb_min.x < octantMax.x &&
		rb_min.y < octantMax.y &&
		rb_min.z < octantMax.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//function to display selected octant(s)
void Simplex::Octree::Display(uint a_uIndex, vector3 a_v3Color)
{
	if (a_uIndex >= numOctant && wireFrameSwitch == true)
	{
		DisplayAll();
		return;
	}
	rootChildren[a_uIndex]->DisplayCurrent(a_v3Color);
}
//helper function to only display an octant
void Octree::DisplayCurrent(vector3 a_v3Color)
{
	if (wireFrameSwitch == true)
	{
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, octantCenter) * glm::scale(IDENTITY_M4, octantSize), a_v3Color);
	}
	else
	{
	}
}
//helper function to display all octants
void Simplex::Octree::DisplayAll(vector3 a_v3Color)
{
	if (wireFrameSwitch == true)
	{
		if (IsLeaf())
		{
			DisplayCurrent(a_v3Color);
		}
		else
		{
			for (uint i = 0; i < numChildren; ++i)
			{
				octChildren[i]->DisplayAll(a_v3Color);
			}
		}
	}
}
//recursively clears all child nodes
void Octree::ClearEntityList(void)
{
	for (uint i = 0; i < numChildren; ++i)
	{
		octChildren[i]->ClearEntityList();
	}
	entityList.clear();
}
//recursively subdivides
void Octree::Subdivide(void)
{
	//stops subdividing if at desired level of there's 5 or less entity in the octant
	if (currLevel >= maxLevel || !ContainsMoreThan(entityCount))
	{
		octRoot->rootChildren.push_back(this);
		numLeaves += 1;
		return;
	}

	//prints out if someone is trying to subdivide an octant that's not a leaf
	if (numChildren == 8)
	{
		std::cout << "Octree is cannot go over Max Subdivisions" << std::endl;
		return;
	}

	//creating each octant at the right position
	octChildren[0] = new Octree(octantCenter + vector3(-octantSize.x / 4, octantSize.y / 4, -octantSize.z / 4), octantSize / 2.f);
	octChildren[1] = new Octree(octantCenter + vector3(-octantSize.x / 4, octantSize.y / 4, octantSize.z / 4), octantSize / 2.f);
	octChildren[2] = new Octree(octantCenter + vector3(-octantSize.x / 4, -octantSize.y / 4, -octantSize.z / 4), octantSize / 2.f);
	octChildren[3] = new Octree(octantCenter + vector3(-octantSize.x / 4, -octantSize.y / 4, octantSize.z / 4), octantSize / 2.f);
	octChildren[4] = new Octree(octantCenter + vector3(octantSize.x / 4, -octantSize.y / 4, -octantSize.z / 4), octantSize / 2.f);
	octChildren[5] = new Octree(octantCenter + vector3(octantSize.x / 4, -octantSize.y / 4, octantSize.z / 4), octantSize / 2.f);
	octChildren[6] = new Octree(octantCenter + vector3(octantSize.x / 4, octantSize.y / 4, -octantSize.z / 4), octantSize / 2.f);
	octChildren[7] = new Octree(octantCenter + vector3(octantSize.x / 4, octantSize.y / 4, octantSize.z / 4), octantSize / 2.f);
	numChildren = 8;

	//loop through and initialize children
	for (uint i = 0; i < numChildren; ++i)
	{
		octChildren[i]->octParent = this;
		octChildren[i]->currLevel = currLevel + 1;
		octChildren[i]->octRoot = octRoot;
		//loops through and adds colliding rigid bodies
		for (uint j = 0; j < currEntityCount; ++j)
		{
			if (octChildren[i]->IsColliding(entityList[j]))
			{
				octChildren[i]->entityList.push_back(entityList[j]);
			}
		}
		//update entity count
		octChildren[i]->currEntityCount = octChildren[i]->entityList.size();
		//recursive call
		octChildren[i]->Subdivide();
	}
}

//recursively kills all nodes except root
void Octree::KillBranches(void)
{
	if (IsLeaf())
	{
		return;
	}
	else
	{
		for (uint i = 0; i < numChildren; ++i)
		{
			octChildren[i]->KillBranches();
			SafeDelete(octChildren[i]);
		}
	}
}

//recursive call to configure dimensions for all leaves
void Simplex::Octree::ConfigureDimensions()
{
	if (IsLeaf())
	{
		for (uint i = 0; i < currEntityCount; ++i)
		{
			m_pEntityMngrFF->AddDimension(entityList[i], octID);
		}
	}
	else
	{
		for (uint i = 0; i < numChildren; ++i)
		{
			octChildren[i]->ConfigureDimensions();
		}
	}
}

//release
void Octree::Release(void)
{
	if (this == octRoot)
	{
		KillBranches();
	}
}

//init
void Octree::Init(void)
{
	m_pEntityMngrFF = EntityManagerFF::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();

	octID = numOctant;
	numOctant += 1;
}