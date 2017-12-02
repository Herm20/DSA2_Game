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
		MyEntity* pEntity = entityList[x];
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
	MyEntity* pTemp = MyEntity::GetEntity(uID);

	if (pTemp)
	{
		return pTemp->GetModel();
	}
	return nullptr;
}
RigidBody* Simplex::EntityManagerFF::GetRigidBody(uint index)
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
RigidBody* Simplex::EntityManagerFF::GetRigidBody(String uID)
{
	MyEntity* pTemp = MyEntity::GetEntity(uID);

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
	MyEntity* pTemp = MyEntity::GetEntity(uID);

	if (pTemp)
	{
		return pTemp->GetModelMatrix();
	}
	return IDENTITY_M4;
}
void Simplex::EntityManagerFF::SetModelMatrix(matrix4 m4ToWorld, String uID)
{
	// get entity
	MyEntity* pTemp = MyEntity::GetEntity(uID);

	// if it exists set the model matrix
	if (pTemp)
	{
		pTemp->SetModelMatrix(m4ToWorld);
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
	// check collisions
	for (int x = 0; x < entityCount; x++)
	{
		for (int y = 0; y < entityCount; y++)
		{
			entityList[x]->IsColliding(entityList[y]);
		}
	}
}
void Simplex::EntityManagerFF::AddEntity(String a_sFileName, String uID)
{
	MyEntity* pTemp = new MyEntity(a_sFileName, uID);

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

	MyEntity* pTemp = entityList[entityCount - 1];
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
MyEntity* Simplex::EntityManagerFF::GetEntity(uint index)
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
	MyEntity* pTemp = MyEntity::GetEntity(uID);

	if (pTemp)
	{
		pTemp->AddToRenderList(rigidBody);
	}
}