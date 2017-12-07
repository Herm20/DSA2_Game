#include "AppClass.h"
void Application::InitVariables(void)
{
	m_sProgrammer = "Herman McElveen, Edward Opich, Brandon Rodriguez, James Meeks";
	playerMat = IDENTITY_M4;
	playerMovement = vector3(0.0f, 2.0f, 2.0f);
	//playerMat = glm::translate(IDENTITY_M4, vector3(0.0f, 20.0f, 2.0f));
	
	
	srand(std::time(NULL));
	//float horiStart = (rand() % 20) - 10;
	//float vertStart = (rand() % 8) - 4;
	
	// entity manager
	entMan = EntityManagerFF::GetInstance();

		//player = new Model();
		//player->Load("FFmodel\\FreeFallMan.obj");
		//playerRB = new MyRigidBody(player->GetVertexList());

	entMan->AddEntity("FFmodel\\FreeFallMan.obj", "FFMan");
	entMan->SetAxisVisibility(true, "FFMan");

	entMan->AddEntity("FFmodel\\BoxCrate.obj", "Debris_0");
	entMan->SetAxisVisibility(true, "Debris_0");
	mCrateMat.push_back(IDENTITY_M4);
	move.push_back(-80.0f);
	horiStart.push_back((rand() % 20) - 10);
	vertStart.push_back((rand() % 8) - 4);
	moveVec.push_back(vector3(0.0f, 0.0f, 0.0f));

	entMan->AddEntity("FFmodel\\BoxCrate.obj", "Debris_1");
	entMan->SetAxisVisibility(true, "Debris_1");
	mCrateMat.push_back(IDENTITY_M4);
	move.push_back(-100.0f);
	horiStart.push_back((rand() % 20) - 10);
	vertStart.push_back((rand() % 8) - 4);
	moveVec.push_back(vector3(0.0f, 0.0f, 0.0f));

	entMan->AddEntity("FFmodel\\BoxCrate.obj", "Debris_2");
	entMan->SetAxisVisibility(true, "Debris_2");
	mCrateMat.push_back(IDENTITY_M4);
	move.push_back(-120.0f);
	horiStart.push_back((rand() % 20) - 10);
	vertStart.push_back((rand() % 8) - 4);
	moveVec.push_back(vector3(0.0f, 0.0f, 0.0f));

	entMan->AddEntity("FFmodel\\RoundCrate.obj", "Debris_3");
	entMan->SetAxisVisibility(true, "Debris_3");
	mCrateMat.push_back(IDENTITY_M4);
	move.push_back(-150.0f);
	horiStart.push_back((rand() % 20) - 10);
	vertStart.push_back((rand() % 8) - 4);
	moveVec.push_back(vector3(0.0f, 0.0f, 0.0f));
	
		//crate movement
		//for (uint i = 0; i < 1; i++)
		//{
		//	//entMan->AddEntity("Minecraft\\BoxCrate.obj", "Crate_0");
		//	//entMan->SetAxisVisibility(true, "Crate_0");
		//	//crateOb.push_back(Model());
		//	//crateOb[i].Load("FFmodel\\BoxCrate.obj");
		//	//crateObRB = new MyRigidBody(crateOb[0].GetVertexList());
		//	//mCrateMat.push_back(IDENTITY_M4);
		//}	

		//mCrateMat[0] = glm::translate(vector3(0.0f, 0.0f, -100.0f));
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
	
	// character movement
	entMan->SetModelMatrix(playerMat, "FFMan");
	
	// debris movement
	for (int x = 0; x < mCrateMat.size(); x++)
	{
		mCrateMat[x] = glm::translate(moveVec[x])  * glm::scale(vector3(1.5f));
		entMan->SetModelMatrix(mCrateMat[x], "Debris_" + std::to_string(x));

		moveVec[x] = vector3(horiStart[x], vertStart[x], move[x]);
	}

	for (int x = 0; x < move.size(); x++)
	{
		// move debris objects forward
		move[x] += 1.0f;

		// randomize respawn location of debris
		if (move[x] >= 10)
		{
			horiStart[x] = (rand() % 20) - 10;
			vertStart[x] = (rand() % 8) - 4;
			move[x] = -100;
		}
	}

		/*m_pMeshMngr->Print("Colliding: ");
		if (bColliding)
			m_pMeshMngr->PrintLine("YES!", C_RED);
		else
			m_pMeshMngr->PrintLine("no", C_YELLOW);
			*/
	
	// check collisions in the entity manager after a certain point
	//if (time > 5)
	{
		entMan->Update();
	}

	// render the entities
	entMan->AddEntityToRenderList(-1, true);

	//if (entMan->lives < 0.5)
	{
		// end the game
	}
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release GUI
	ShutdownGUI();
}