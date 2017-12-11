#include "EntityManagerFF.h"
using namespace Simplex;

//  EntityManagerFF
EntityManagerFF* EntityManagerFF::instance = nullptr;
void EntityManagerFF::Init(void)
{
	entityCount = 0;
	entityList.clear();
}
void EntityManagerFF::Release(void)
{
	for (int x = 0; x < entityCount; x++)
	{
		EntityFF* pEntity = entityList[x];
		SafeDelete(pEntity);
	}
	entityCount = 0;
	entityList.clear();
}
EntityManagerFF* EntityManagerFF::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new EntityManagerFF();
	}
	return instance;
}
void EntityManagerFF::ReleaseInstance()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}
int Simplex::EntityManagerFF::GetEntityIndex(String uID)
{
	// look for the specified unique ID by going through each of them
	for (int x = 0; x < entityCount; x++)
	{
		if (uID == entityList[x]->GetUniqueID())
		{
			return x;
		}
	}
	return -1;
}
//Accessors
Model* Simplex::EntityManagerFF::GetModel(uint index)
{
	if (entityList.size() == 0)
	{
		return nullptr;
	}

	// check if the model is out of bounds
	if (index >= entityCount)
	{
		index = entityCount - 1;
	}

	return entityList[index]->GetModel();
}
Model* Simplex::EntityManagerFF::GetModel(String uID)
{
	EntityFF* pTemp = EntityFF::GetEntity(uID);

	if (pTemp)
	{
		return pTemp->GetModel();
	}
	return nullptr;
}
MyRigidBody* Simplex::EntityManagerFF::GetRigidBody(uint index)
{
	if (entityList.size() == 0)
	{
		return nullptr;
	}

	if (index >= entityCount)
	{
		index = entityCount - 1;
	}

	return entityList[index]->GetRigidBody();
}
MyRigidBody* Simplex::EntityManagerFF::GetRigidBody(String uID)
{
	EntityFF* pTemp = EntityFF::GetEntity(uID);

	if (pTemp)
	{
		return pTemp->GetRigidBody();
	}

	return nullptr;
}
matrix4 Simplex::EntityManagerFF::GetModelMatrix(uint index)
{
	if (entityList.size() == 0)
	{
		return IDENTITY_M4;
	}

	if (index >= entityCount)
	{
		index = entityCount - 1;
	}

	return entityList[index]->GetModelMatrix();
}
matrix4 Simplex::EntityManagerFF::GetModelMatrix(String uID)
{
	EntityFF* pTemp = EntityFF::GetEntity(uID);

	if (pTemp)
	{
		return pTemp->GetModelMatrix();
	}
	return IDENTITY_M4;
}
void Simplex::EntityManagerFF::SetModelMatrix(matrix4 m4ToWorld, String uID)
{
	// get entity
	EntityFF* pTemp = EntityFF::GetEntity(uID);

	// if it exists set the model matrix
	if (pTemp)
	{
		pTemp->SetModelMatrix(m4ToWorld);
	}
}
void Simplex::EntityManagerFF::SetAxisVisibility(bool visibility, uint index)
{
	//if the list is empty return blank
	if (entityList.size() == 0)
		return;

	// if out of bounds
	if (index >= entityCount)
		index = entityCount - 1;

	return entityList[index]->SetAxisVisible(visibility);
}
void Simplex::EntityManagerFF::SetAxisVisibility(bool visibility, String uID)
{
	//Get the entity
	EntityFF* pTemp = EntityFF::GetEntity(uID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->SetAxisVisible(visibility);
	}
}
void Simplex::EntityManagerFF::SetModelMatrix(matrix4 m4ToWorld, uint index)
{
	if (entityList.size() == 0)
	{
		return;
	}

	if (index >= entityCount)
	{
		index = entityCount - 1;
	}

	entityList[index]->SetModelMatrix(m4ToWorld);
}
//The big 3
EntityManagerFF::EntityManagerFF() { Init(); }
EntityManagerFF::EntityManagerFF(EntityManagerFF const& other) { }
EntityManagerFF& EntityManagerFF::operator=(EntityManagerFF const& other) { return *this; }
EntityManagerFF::~EntityManagerFF() { Release(); };
// other methods
void Simplex::EntityManagerFF::Update(void)
{
	bool myBool = false;

	// check collisions
	for (int x = 0; x < entityCount -1; x++)
	{
		for (int y = x + 1; y < entityCount; y++)
		{
			// don't check the ground plane
			if (y == 1 || x == 1)
			{
				continue;
			}

			//if the player collides with anything
			if (entityList[0]->IsColliding(entityList[y]) && colliding == false)
			{
				lives--;
				colliding = true;
				myBool = true;

				//move colliding object
				entityList[y]->move = -200.0f;
				entityList[y]->horiStart = (rand() % 30) - 15;
				entityList[y]->vertStart = (rand() % 30) - 10;
			}

			//if any objects collide with each other excluding the player
			else if (entityList[x]->IsColliding(entityList[y]))
			{
				entityList[y]->move = (rand() % -200) - 10 * y;
				entityList[y]->horiStart = (rand() % 30) - 15;
				entityList[y]->vertStart = (rand() % 30) - 10;
			}

			entityList[x]->IsColliding(entityList[y]);
		}
	}

	if (!myBool)
	{
		colliding = false;
	}
}
void Simplex::EntityManagerFF::AddEntity(String a_sFileName, String uID)
{
	EntityFF* pTemp = new EntityFF(a_sFileName, uID);

	if (pTemp->IsInitialized())
	{
		entityList.push_back(pTemp);
		entityCount = entityList.size();
	}
}
void Simplex::EntityManagerFF::RemoveEntity(uint index)
{
	if (entityList.size() == 0)
	{
		return;
	}

	if (index >= entityCount)
	{
		index = entityCount - 1;
	}

	if (index != entityCount - 1)
	{
		std::swap(entityList[index], entityList[entityCount - 1]);
	}

	EntityFF* pTemp = entityList[entityCount - 1];
	SafeDelete(pTemp);

	entityList.pop_back();
	entityCount--;
	return;
}
void Simplex::EntityManagerFF::RemoveEntity(String uID)
{
	int nIndex = GetEntityIndex(uID);
	RemoveEntity((uint)nIndex);
}
String Simplex::EntityManagerFF::GetUniqueID(uint index)
{
	if (entityList.size() == 0)
	{
		return "";
	}

	if (index >= entityList.size())
	{
		index = entityList.size() - 1;
	}

	return entityList[index]->GetUniqueID();
}
EntityFF* Simplex::EntityManagerFF::GetEntity(uint index)
{
	if (entityList.size() == 0)
	{
		return nullptr;
	}

	if (index >= entityList.size())
	{
		index = entityList.size() - 1;
	}

	return entityList[index];
}
uint Simplex::EntityManagerFF::GetEntityCount(void)
{
	return entityCount;
}
void Simplex::EntityManagerFF::AddEntityToRenderList(uint index, bool rigidBody)
{
	if (index >= entityCount)
	{
		for (index = 0; index < entityCount; index++)
		{
			entityList[index]->AddToRenderList(rigidBody);
		}
	}
	else
	{
		entityList[index]->AddToRenderList(rigidBody);
	}
}
void Simplex::EntityManagerFF::AddEntityToRenderList(String uID, bool rigidBody)
{
	EntityFF* pTemp = EntityFF::GetEntity(uID);

	if (pTemp)
	{
		pTemp->AddToRenderList(rigidBody);
	}
}

void Simplex::EntityManagerFF::RemoveDimension(uint a_uIndex, uint a_uDimension)
{
	//if the list is empty return
	if (entityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= entityCount)
	{
		a_uIndex = entityCount - 1;
	}
		
	return entityList[a_uIndex]->RemoveDimension(a_uDimension);
}

void Simplex::EntityManagerFF::RemoveDimension(String a_sUniqueID, uint a_uDimension)
{
	//Get the entity
	EntityFF* pTemp = EntityFF::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->RemoveDimension(a_uDimension);
	}
}

void Simplex::EntityManagerFF::ClearDimensionSetAll(void)
{
	for (uint i = 0; i < entityCount; ++i)
	{
		ClearDimensionSet(i);
	}
}
void Simplex::EntityManagerFF::ClearDimensionSet(uint a_uIndex)
{
	//if the list is empty return
	if (entityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= entityCount)
		a_uIndex = entityCount - 1;

	return entityList[a_uIndex]->ClearDimensionSet();
}
void Simplex::EntityManagerFF::ClearDimensionSet(String a_sUniqueID)
{
	//Get the entity
	EntityFF* pTemp = EntityFF::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->ClearDimensionSet();
	}
}

void Simplex::EntityManagerFF::AddDimension(String a_sUniqueID, uint a_uDimension)
{
	//Get the entity
	EntityFF* pTemp = EntityFF::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->AddDimension(a_uDimension);
	}
}

void Simplex::EntityManagerFF::AddDimension(uint a_uIndex, uint a_uDimension)
{
	//if the list is empty return
	if (entityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= entityCount)
		a_uIndex = entityCount - 1;

	return entityList[a_uIndex]->AddDimension(a_uDimension);
}

bool Simplex::EntityManagerFF::IsInDimension(String a_sUniqueID, uint a_uDimension)
{
	//Get the entity
	EntityFF* pTemp = EntityFF::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		return pTemp->IsInDimension(a_uDimension);
	}
	return false;
}

bool Simplex::EntityManagerFF::IsInDimension(uint a_uIndex, uint a_uDimension)
{
	//if the list is empty return
	if (entityCount == 0)
	{
		return false;
	}

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= entityCount)
	{
		a_uIndex = entityCount - 1;
	}
		

	return entityList[a_uIndex]->IsInDimension(a_uDimension);
}

bool Simplex::EntityManagerFF::SharesDimension(uint a_uIndex, EntityFF * const a_pOther)
{
	//if the list is empty return
	if (entityCount == 0)
		return false;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= entityCount)
	{
		a_uIndex = entityCount - 1;
	}
		
	return entityList[a_uIndex]->SharesDimension(a_pOther);
}

bool Simplex::EntityManagerFF::SharesDimension(String a_sUniqueID, EntityFF * const a_pOther)
{
	//Get the entity
	EntityFF* pTemp = EntityFF::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		return pTemp->SharesDimension(a_pOther);
	}
	return false;
}
