// programmer: Preston Meeks - jpm4447@g.rit.edu
// Date: 12/4/17
#ifndef __ENTITYFF_H_
#define __ENTITYFF_H_

#include "Simplex\Physics\Solver.h"
#include "MyRigidBody.h"

namespace Simplex
{
	class EntityFF
	{
		// variables
		bool inMemory = false;	// checks if loaded
		bool setAxis = false;	// checks if axis has been set
		String uniqueID = "";	// unique object identifier name

		uint m_nDimensionCount = 0; //tells how many dimensions this entity lives in
		uint* m_DimensionArray = nullptr; //Dimensions on which this entity is located

		Model* model = nullptr;	// model associated with this Entity
		MyRigidBody* rigidBody = nullptr;	// rigidbody associated with this Entity

		matrix4 m4World = IDENTITY_M4;	// Model matrix for the entity
		MeshManager* meshMngr = nullptr;	

		static std::map<String, EntityFF*> IDMap;	// a map of the unique IDs

		bool physicsSolver = false;
		//Solver* solver = nullptr;	// physics solver

	public:
		// variables 
		float move;
		float horiStart;
		float vertStart;

		// rule of three
		EntityFF(String filename, String UniqueID = "NA");
		EntityFF(EntityFF const& other);
		EntityFF& operator=(EntityFF const& other);
		~EntityFF(void);
		
										// use -> to call
		void Swap(EntityFF& other);		// swaps models with another entity
		matrix4 GetModelMatrix(void);	// gets the model matrix for the entity
		void SetModelMatrix(matrix4 m4World);	// sets the model matrix for the entity
		Model* GetModel(void);	// returns the model associated with this entity
		MyRigidBody* GetRigidBody(void);	// returns the Rigidbody associated with a model
		
		bool IsInitialized(void);	// checks if the entity has been initialized
		void AddToRenderList(bool drawRigidBody = false);	// adds entity to the render list
		bool IsColliding(EntityFF* const other);	// tells if this entity and the given entity are currently colliding
		static EntityFF* GetEntity(String UniqueID);	// gets entity specified by unique ID, nullptr if it does not exist
		void GenUniqueID(String& UniqueID);	// will generate a unique id based on a provided name string
		String GetUniqueID(void);	// returns the unique ID of this entity
		void SetAxisVisible(bool axis = true);	// set the axis visibility of the entity
		
		void AddDimension(uint dimension);	// will set an entity to the given integer
		void RemoveDimension(uint dimension);	// will remove the entity from the specified dimension
		void ClearDimensionSet(void);	// remove all dimensions from entity
		bool IsInDimension(uint dimension);	// checks if an entity is in a given dimension
		bool SharesDimension(EntityFF* const other);	// checks if this entity and the other entity share the same dimensional space
		void SortDimensions(void);	// sorts the array of dimensions
		
		void ClearCollisionList(void);	// clear collision list of the entity
		//MyRigidBody::PRigidBody* GetColliderArray(void);	// gets the array of rigid bodies pointer this entity is colliding with
		uint GetsCollidingCount(void);	// returns the number of colliding objects with this one
		bool HasThisRigidBody(MyRigidBody* rigidBody);	// returns if the given rigidbody is the same as the one being used by the entity
		void ResolveCollision(EntityFF* other);	// asks the entity to resolve the collision with the incoming entity
		
		Solver* GetSolver(void);	// gets the solver applied to this entity
		void ApplyForce(vector3 v3Force);	// applies a force to the solver
		void SetPosition(vector3 v3Position);	// sets the position of the solver
		vector3 GetPosition(void);	// gets the position of the solver
		void SetVelocity(vector3 v3Velocity);	// sets the velocity of the solver
		vector3 GetVelocity(void);	// gets the velocity of the solver
		void SetMass(float mass);	// sets mass ofthe solver
		float GetMass(void);	// gets mass of the solver
		void UsePhysicsSolver(bool use = true);	// resolves using physics solver or not in the update

		void Update(void);	// updates the entity

	private:
		void Release(void);	// deallocate member fields
		void Init(void);	// allocates member fields
	};

}

#endif	//__ENTITYFF_H_