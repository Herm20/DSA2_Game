#ifndef __MYOCTANTCLASS_H_
#define __MYOCTANTCLASS_H_

#include "EntityManagerFF.h"

namespace Simplex
{
	//System Class
	class Octree
	{
		/// Variable used throughtout 
		static uint numOctant; 
		static uint numLeaves; 
		static uint maxLevel; 
		static uint entityCount; 

		uint octID = 0; //Will store the current ID for this octant
		uint currLevel = 0; //Will store the current level of the octant
		uint numChildren = 0;// Number of children on the octant (either 0 or 8)

		MeshManager* m_pMeshMngr = nullptr; //Mesh Manager singleton
		EntityManagerFF* m_pEntityMngrFF = nullptr; //Entity Manager Singleton

		vector3 octantSize = ZERO_V3; //size of the octant
		vector3 octantCenter = ZERO_V3; //Will store the center point of the octant
		vector3 octantMin = ZERO_V3; //Will store the minimum vector of the octant
		vector3 octantMax = ZERO_V3; //Will store the maximum vector of the octant

		Octree* octParent = nullptr; // Will store the parent of current octant
		Octree* octChildren[8]; //Will store the children of the current octant

		std::vector<uint> entityList; //List of Entities under this octant (Index in Entity Manager)
		uint currEntityCount= 0;

		Octree* octRoot = nullptr; //Root octant
		std::vector<Octree*> rootChildren; //list of nodes that contain objects (this will be applied to root only)
		
	public:
		bool wireFrameSwitch = true;

		Octree(uint p_MaxLevel = 2, uint p_EntityCount = 5);

		Octree(vector3 p_Center, vector3 p_Size);

		Octree(Octree const& other);


		~Octree(void);

		vector3 GetSize(void);

		vector3 GetCenterGlobal(void);

		vector3 GetMinGlobal(void);

		vector3 GetMaxGlobal(void);

		bool IsColliding(uint a_uRBIndex);

		void Display(uint a_uIndex, vector3 a_v3Color = C_GREEN);

		void DisplayCurrent(vector3 a_v3Color = C_GREEN);

		void DisplayAll(vector3 a_v3Color = C_GREEN);

		void ClearEntityList(void);

		void Subdivide(void);

		Octree* GetChild(uint a_nChild);

		Octree* GetParent(void);

		bool IsLeaf(void);

		bool ContainsMoreThan(uint a_nEntities);

		void KillBranches(void);

		uint GetOctantCount(void);

		uint GetLeafCount(void);

		void ConfigureDimensions(void);

	private:

		void Release(void);

		void Init(void);
	};
} 

#endif