// programmer: Preston Meeks - jpm4447@g.rit.edu
// Date: 12/4/17
#ifndef __ENTITYMANAGERFF_H_
#define __ENTITYMANAGERFF_H_

#include "Simplex\Physics\Solver.h"
#include "EntityFF.h"

namespace Simplex
{
	class EntityManagerFF
	{
		// variables
		bool colliding = false;
		typedef EntityFF* PEntity;	// entity pointer
		uint entityCount = 0;		// number of elements in the list
		PEntity* entityArray = nullptr;	// array of entity pointers
		static EntityManagerFF* instance;	// singleton pointer
		//std::vector<EntityFF*> entityList;

	public:
		// variables
		std::vector<EntityFF*> entityList;
		int lives = 5;

		// use -> to call
		// helper methods
		static EntityManagerFF* GetInstance();	// gets the singleton pointer
		static void ReleaseInstance(void);		// releases the content of the singleton
		int GetEntityIndex(String uID);	// gets the index of the entity specified, if it is in the list
		void AddEntity(String filename, String uID = "NA");	// will add an entity to the list
		void RemoveEntity(uint index);	// will remove the entity given its index in the list
		void RemoveEntity(String uID);	// will remove the entity given its unique ID in the list
		String GetUniqueID(uint index = -1);	// gets the unique id of the entity indexed
		EntityFF* GetEntity(uint index = -1);	// gets the unique id of the entity indexed
		uint GetEntityCount(void);	// returns the entity count

		void Update(void);	// updates the Entity Manager

		// individual entity methods
		Model* GetModel(uint index = -1);	// returns the model associated with this entity at this index
		Model* GetModel(String uID);	// returns the model associated with this entity with this uID
		MyRigidBody* GetRigidBody(uint index = -1);	// returns the rigidbody associated with this entity at this index
		MyRigidBody* GetRigidBody(String uID);	// returns the rigidbody associated with this entity with this uID
		matrix4 GetModelMatrix(uint index = -1);	// gets the model matrix for the entity
		matrix4 GetModelMatrix(String uID);	// gets the model matrix for the entity
		void SetModelMatrix(matrix4 m4World, uint index = -1);	// sets the model matrix for the entity
		void SetModelMatrix(matrix4 m4World, String uID);	// sets the model matrix for the entity
		void SetAxisVisibility(bool visibility, uint index = -1);	// sets the axis visibility based on the index
		void SetAxisVisibility(bool visibility, String uID);	// sets the axis visibility based on the uID
		void AddEntityToRenderList(uint index = -1, bool rigidBody = false);	// will add the entity to the render list
		void AddEntityToRenderList(String uID, bool rigidBody = false);	// will add the entity to the render list

		void RemoveDimension(uint a_uIndex, uint a_uDimension);

		void RemoveDimension(String a_sUniqueID, uint a_uDimension);

		void ClearDimensionSetAll(void);

		void ClearDimensionSet(uint a_uIndex);

		void ClearDimensionSet(String a_sUniqueID);

		void AddDimension(String a_sUniqueID, uint a_uDimension);

		void AddDimension(uint a_uIndex, uint a_uDimension);

		bool IsInDimension(String a_sUniqueID, uint a_uDimension);

		bool IsInDimension(uint a_uIndex, uint a_uDimension);

		bool SharesDimension(uint a_uIndex, EntityFF* const a_pOther);

		bool SharesDimension(String a_sUniqueID, EntityFF* const a_pOther);

	private:
		// rule of three
		EntityManagerFF(void);	// constructor
		EntityManagerFF(EntityManagerFF const& other);
		EntityManagerFF& operator=(EntityManagerFF const& other);
		~EntityManagerFF(void);

		void Release(void);	// deallocate member fields
		void Init(void);	// allocates member fields
	};

}

#endif	//__ENTITYMANAGERFF_H_