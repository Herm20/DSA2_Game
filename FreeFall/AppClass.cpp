#include "AppClass.h"
void Application::InitVariables(void)
{
	m_sProgrammer = "Herman McElveen, Edward Opich, Brandon Rodriguez, James Meeks";
	playerMat = IDENTITY_M4;
	playerMovement = vector3(0.0f, 2.0f, 2.0f);
	//playerMat = glm::translate(IDENTITY_M4, vector3(0.0f, 20.0f, 2.0f));

	player = new Model();
	player->Load("FFmodel\\cube.obj");
	playerRB = new MyRigidBody(player->GetVertexList());

	//steve
	for (uint i = 0; i < 1; i++)
	{
		crateOb.push_back(Model());
		crateOb[i].Load("FFmodel\\BoxCrate.obj");
		crateObRB = new MyRigidBody(crateOb[0].GetVertexList());
		mCrateMat.push_back(IDENTITY_M4);
	}	
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	player->SetModelMatrix(playerMat);
	playerRB->SetModelMatrix(playerMat);
	//m_pMeshMngr->AddAxisToRenderList(playerMat);

	/// If a collision happens at 0,0,0 the program will break no idea why

	for (uint i = 0; i < 1; i++)
	{
		mCrateMat[0] = glm::translate(vector3(0.0f, 0.0f, -7.0f));

		crateOb[i].SetModelMatrix(mCrateMat[i]);
		crateObRB->SetModelMatrix(mCrateMat[i]);
	}
	
	//m_pMeshMngr->AddAxisToRenderList(mCrate);

	bool bColliding = playerRB->IsColliding(crateObRB);

	player->AddToRenderList();
	playerRB->AddToRenderList();

	for (uint i = 0; i < 1; i++)
	{
		crateOb[i].AddToRenderList();
	}
	
	crateObRB->AddToRenderList();

	m_pMeshMngr->Print("Colliding: ");
	if (bColliding)
		m_pMeshMngr->PrintLine("YES!", C_RED);
	else
		m_pMeshMngr->PrintLine("no", C_YELLOW);
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