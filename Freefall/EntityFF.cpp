#include "EntityFF.h"
using namespace Simplex;

std::map<String, EntityFF*> EntityFF::IDMap;
//  Accessors
matrix4 Simplex::EntityFF::GetModelMatrix(void)
{
	return m4World; 
}

void Simplex::EntityFF::SetModelMatrix(matrix4 m4ToWorld)
{
	if (!inMemory)
		return;

	m4World = m4ToWorld;
	model->SetModelMatrix(m4World);
	rigidBody->SetModelMatrix(m4World);
}

Model* Simplex::EntityFF::GetModel(void)
{ 
	return model; 
}

MyRigidBody* Simplex::EntityFF::GetRigidBody(void)
{ 
	return rigidBody; 
}

bool Simplex::EntityFF::IsInitialized(void)
{ 
	return inMemory;
}
String Simplex::EntityFF::GetUniqueID(void)
{ 
	return uniqueID; 
}

void Simplex::EntityFF::SetAxisVisible(bool axis)
{ 
	setAxis = axis; 
}

void Simplex::EntityFF::AddDimension(uint dimension)
{
	//we need to check that this dimension is not already allocated in the list
	if (IsInDimension(dimension))
		return;//it is, so there is no need to add

			   //insert the entry
	uint* pTemp;
	pTemp = new uint[m_nDimensionCount + 1];
	if (m_DimensionArray)
	{
		memcpy(pTemp, m_DimensionArray, sizeof(uint) * m_nDimensionCount);
		delete[] m_DimensionArray;
		m_DimensionArray = nullptr;
	}
	pTemp[m_nDimensionCount] = dimension;
	m_DimensionArray = pTemp;

	++m_nDimensionCount;

	SortDimensions();
}

void Simplex::EntityFF::RemoveDimension(uint dimension)
{
	//if there are no dimensions return
	if (m_nDimensionCount == 0)
		return;

	//we look one by one if its the one wanted
	for (uint i = 0; i < m_nDimensionCount; i++)
	{
		if (m_DimensionArray[i] == dimension)
		{
			//if it is, then we swap it with the last one and then we pop
			std::swap(m_DimensionArray[i], m_DimensionArray[m_nDimensionCount - 1]);
			uint* pTemp;
			pTemp = new uint[m_nDimensionCount - 1];
			if (m_DimensionArray)
			{
				memcpy(pTemp, m_DimensionArray, sizeof(uint) * (m_nDimensionCount - 1));
				delete[] m_DimensionArray;
				m_DimensionArray = nullptr;
			}
			m_DimensionArray = pTemp;

			--m_nDimensionCount;
			SortDimensions();
			return;
		}
	}
}

void Simplex::EntityFF::ClearDimensionSet(void)
{
	if (m_DimensionArray)
	{
		delete[] m_DimensionArray;
		m_DimensionArray = nullptr;
	}
	m_nDimensionCount = 0;
}

bool Simplex::EntityFF::IsInDimension(uint dimension)
{
	//see if the entry is in the set
	for (uint i = 0; i < m_nDimensionCount; i++)
	{
		if (m_DimensionArray[i] == dimension)
			return true;
	}
	return false;
}

bool Simplex::EntityFF::SharesDimension(EntityFF * const other)
{
	//special case: if there are no dimensions on either MyEntity
	//then they live in the special global dimension
	if (0 == m_nDimensionCount)
	{
		//if no spatial optimization all cases should fall here as every 
		//entity is by default, under the special global dimension only
		if (0 == other->m_nDimensionCount)
			return true;
	}

	//for each dimension on both Entities we check if there is a common dimension
	for (uint i = 0; i < m_nDimensionCount; ++i)
	{
		for (uint j = 0; j < other->m_nDimensionCount; j++)
		{
			if (m_DimensionArray[i] == other->m_DimensionArray[j])
				return true; //as soon as we find one we know they share dimensionality
		}
	}
	//could not find a common dimension
	return false;
}

void Simplex::EntityFF::SortDimensions(void)
{
	std::sort(m_DimensionArray, m_DimensionArray + m_nDimensionCount);
}

//  Entity
void Simplex::EntityFF::Init(void)
{
	meshMngr = MeshManager::GetInstance();
	inMemory = false;
	setAxis = false;
	model = nullptr;
	rigidBody = nullptr;
	m4World = IDENTITY_M4;
	uniqueID = "";
}
void Simplex::EntityFF::Swap(EntityFF& other)
{
	inMemory = false;
	std::swap(model, other.model);
	std::swap(rigidBody, other.rigidBody);
	std::swap(m4World, other.m4World);
	std::swap(meshMngr, other.meshMngr);
	std::swap(inMemory, other.inMemory);
	std::swap(uniqueID, other.uniqueID);
	std::swap(setAxis, other.setAxis);
}
void Simplex::EntityFF::Release(void)
{
	meshMngr = nullptr;
	//it is not the job of the entity to release the model, 
	//it is for the mesh manager to do so.
	model = nullptr;
	SafeDelete(rigidBody);
	IDMap.erase(uniqueID);
}
//The big 3
Simplex::EntityFF::EntityFF(String fileName, String uID)
{
	Init();
	model = new Model();
	model->Load(fileName);
	//if the model is loaded
	if (model->GetName() != "")
	{
		GenUniqueID(uID);
		uniqueID = uID;
		IDMap[uID] = this;
		rigidBody = new MyRigidBody(model->GetVertexList()); //generate a rigid body
		inMemory = true; //mark this entity as viable
	}
}
Simplex::EntityFF::EntityFF(EntityFF const& other)
{
	inMemory = other.inMemory;
	model = other.model;
	//generate a new rigid body we do not share the same rigid body as we do the model
	rigidBody = new MyRigidBody(model->GetVertexList());
	m4World = other.m4World;
	meshMngr = other.meshMngr;
	uniqueID = other.uniqueID;
	setAxis = other.setAxis;
}
EntityFF& Simplex::EntityFF::operator=(EntityFF const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		EntityFF temp(other);
		Swap(temp);
	}
	return *this;
}
EntityFF::~EntityFF() 
{
	Release(); 
}

//--- Methods
void Simplex::EntityFF::AddToRenderList(bool drawRigidBody)
{
	//if not in memory return
	if (!inMemory)
		return;

	//draw model
	model->AddToRenderList();

	//draw rigid body
	if (drawRigidBody)
		rigidBody->AddToRenderList();

	if (setAxis)
		meshMngr->AddAxisToRenderList(m4World);
}
bool Simplex::EntityFF::IsColliding(EntityFF* const other)
{
	//if not in memory return
	if (!inMemory || !other->inMemory)
		return true;

	return rigidBody->IsColliding(other->GetRigidBody());
}
EntityFF* Simplex::EntityFF::GetEntity(String uID)
{
	//look the entity based on the unique id
	auto entity = IDMap.find(uID);
	//if not found return nullptr, if found return it
	return entity == IDMap.end() ? nullptr : entity->second;
}

void Simplex::EntityFF::GenUniqueID(String& uID)
{
	static uint index = 0;
	String sName = uID;
	EntityFF* pEntity = GetEntity(uID);
	//while Entity exists keep changing name
	while (pEntity)
	{
		uID = sName + "_" + std::to_string(index);
		index++;
		pEntity = GetEntity(uID);
	}
	return;
}