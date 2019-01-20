#include "Laborator9.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#define SECTION 100 /* ----- Dimensiune harta (n x n) ----- */

using namespace std;
glm::mat4 faceMatrix = glm::mat4(1);

Laborator9::Laborator9()
{
}

Laborator9::~Laborator9()
{
}

void Laborator9::Init()
{
	const string textureLoc = "Source/Laboratoare/Laborator9/Textures/";

	/* ----- Load textures ----- */ 
	{
		/* ----- Sosea ----- */
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "road.jpg").c_str(), GL_REPEAT);
			mapTextures["road"] = texture;
		}

		/* ----- Soare ----- */
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "sun.jpg").c_str(), GL_REPEAT);
			mapTextures["sun"] = texture;
		}

		/* ----- Iarba ----- */
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "grass.jpg").c_str(), GL_REPEAT);
			mapTextures["grass"] = texture;
		}

		/* ----- Cladiri ----- */ 
		{
			{
				Texture2D* texture = new Texture2D();
				texture->Load2D((textureLoc + "random.jpg").c_str(), GL_REPEAT);
				mapTextures["randomB"] = texture;
			}

			{
				Texture2D* texture = new Texture2D();
				texture->Load2D((textureLoc + "random2.jpg").c_str(), GL_REPEAT);
				mapTextures["randomB2"] = texture;
			}

			{
				Texture2D* texture = new Texture2D();
				texture->Load2D((textureLoc + "modern.jpg").c_str(), GL_REPEAT);
				mapTextures["modernB"] = texture;
			}

			{
				Texture2D* texture = new Texture2D();
				texture->Load2D((textureLoc + "modern2.jpg").c_str(), GL_REPEAT);
				mapTextures["modernB2"] = texture;
			}

			{
				Texture2D* texture = new Texture2D();
				texture->Load2D((textureLoc + "old.jpg").c_str(), GL_REPEAT);
				mapTextures["oldB"] = texture;
			}

			{
				Texture2D* texture = new Texture2D();
				texture->Load2D((textureLoc + "common.jpg").c_str(), GL_REPEAT);
				mapTextures["commonB"] = texture;
			}

			{
				Texture2D* texture = new Texture2D();
				texture->Load2D((textureLoc + "grey.jpg").c_str(), GL_REPEAT);
				mapTextures["grey"] = texture;
			}
		}

		/* ----- Nori ----- */
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "cloud.png").c_str(), GL_REPEAT);
			mapTextures["cloud"] = texture;
		}
	}
	
	/* ----- Load meshes ----- */
	{
		{
			Mesh* mesh = new Mesh("box");
			mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
			meshes[mesh->GetMeshID()] = mesh;
		}

		{
			Mesh* mesh = new Mesh("sphere");
			mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
			meshes[mesh->GetMeshID()] = mesh;
		}

		{
			Mesh* mesh = new Mesh("wind_turbine");
			mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "wind_turbine.obj");
			meshes[mesh->GetMeshID()] = mesh;
		}
	}

	/* ----- Create a simple quad ----- */
	{
		vector<glm::vec3> vertices
		{
			glm::vec3(0.5f,   0.5f, 0.0f),	// Top Right
			glm::vec3(0.5f,  -0.5f, 0.0f),	// Bottom Right
			glm::vec3(-0.5f, -0.5f, 0.0f),	// Bottom Left
			glm::vec3(-0.5f,  0.5f, 0.0f),	// Top Left
		};

		vector<glm::vec3> normals
		{
			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0)
		};

		// TODO : Complete texture coordinates for the square
		vector<glm::vec2> textureCoords
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f)
		};

		vector<unsigned short> indices =
		{
			0, 1, 3,
			1, 2, 3
		};

		Mesh* mesh = new Mesh("quad");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderLab9");
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	faceMatrix = glm::translate(faceMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
	faceMatrix = glm::scale(faceMatrix, glm::vec3(1.0f));
	faceMatrix = glm::rotate(faceMatrix, RADIANS(-25), glm::vec3(1, 0, 0));


	/* ----- Generate road matrix ----- */
	{
		/* ----- Initializare matrice drum ----- */
		for (int i = 0; i < SECTION; i++)
			for (int j = 0; j < SECTION; j++)
				road_matrix[i][j] = 0;

		/* ----- Alegerea unei directii initiale ----- */
		if (rand() % 2 == 0) { 
			for (int j = 0; j < SECTION; j += (rand() % 3 + 1)) 
				for (int i = 0; i < SECTION; i++) 
					road_matrix[i][j] = 1;

			/* ----- Generarea unor eventuale intersectii ----- */
			if (rand() % 2 == 1)
				for (int i = 0; i < SECTION; i += (rand() % 5 + 1))
					for (int j = 0; j < SECTION; j++)
						road_matrix[i][j] = 1;

		} else {
			for (int i = 0; i < SECTION; i += (rand() % 3 + 1))
				for (int j = 0; j < SECTION; j++)
					road_matrix[i][j] = 1;

			/* ----- Generarea unor eventuale intersectii ----- */
			if (rand() % 2 == 1) 
				for (int j = 0; j < SECTION; j += (rand() % 5 + 1))
					for (int i = 0; i < SECTION; i++)
						road_matrix[i][j] = 1;
		}

		/* ----- "Alegerea" modelelor cladirilor ----- */
		for (int i = 0; i < SECTION; i++)
			for (int j = 0; j < SECTION; j++)
				if (road_matrix[i][j] == 0 && rand() % 3 == 1)
					road_matrix[i][j] = rand() % 5 + 2;
	}
	
	/* ----- "Alegerea" scalarii nivelului 2 al cladirilor ----- */
	for (int i = 0; i < 5; i++)
		scale[i] = rand() % 4 + 1.15f;

	//Light & material properties
	{
		lightPosition = glm::vec3(0, 2, 0);
		materialShininess = 30;
		materialKd = 0.5;
		materialKs = 0.5;
	}
}

void Laborator9::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);	
}

void Laborator9::Update(float deltaTimeSeconds)
{
	/* ----- Desenare ----- */
	{
		string building_textures[] = {"randomB", "randomB2", "modernB", "modernB2", "oldB", "commonB"};
		int k = 0;
		int deg = 10;

		for (int i = 0; i < SECTION; i++) 
			for (int j = 0; j < SECTION; j++) {
				if (road_matrix[i][j] == 0) { /* ----- Iarba ----- */
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(i + -50.0f, 0.0f, j + -50.0f));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));
					modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
					RenderSimpleMesh(meshes["quad"], shaders["ShaderLab9"], modelMatrix, mapTextures["grass"]);

					if (k == 0 && road_matrix[i][j + 1] != 0 && road_matrix[i][j - 1] != 0) {
						glm::mat4 modelMatrix = glm::mat4(1);
						modelMatrix = glm::translate(modelMatrix, glm::vec3(i + -50.25f, 0.0f, j + -50.25f));
						modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
						modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
						RenderSimpleMesh(meshes["wind_turbine"], shaders["ShaderLab9"], modelMatrix, mapTextures["grey"]);
					}

				} else if (road_matrix[i][j] == 1) { /* ----- Sosea ----- */
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(i + -50.0f, 0.0f, j + -50.0f));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));
					modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
					RenderSimpleMesh(meshes["quad"], shaders["ShaderLab9"], modelMatrix, mapTextures["road"]);

				} else { /* ----- Cladiri ----- */
					/* ----- Nivel 1 ----- */
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(i + -50.0f, 0.5f, j + -50.0f));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));
					if (k == 2) {
						modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f));
						modelMatrix = glm::rotate(modelMatrix, RADIANS(deg), glm::vec3(0, 1, 0));
					}
					RenderSimpleMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures[building_textures[road_matrix[i][j] - 2]]);

					/* ----- Nivel 2 ----- */
					if (k != 2) {
						glm::mat4 modelMatrix1 = glm::mat4(1);
						modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(i + -50.0f, 1.0f, j + -50.0f));
						modelMatrix1 = glm::scale(modelMatrix1, glm::vec3(1.0f / scale[k]));
						modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
						RenderSimpleMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix1, mapTextures[building_textures[road_matrix[i][j] - 2]]);
					}
					else {
						glm::mat4 modelMatrix = glm::mat4(1);
						modelMatrix = glm::translate(modelMatrix, glm::vec3(i + -50.0f, 0.0f, j + -50.0f));
						modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));
						modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
						RenderSimpleMesh(meshes["quad"], shaders["ShaderLab9"], modelMatrix, mapTextures["grass"]);
					}
				} 

				/* ----- Constanta alegere scalare - Nivel 2 cladiri ----- */
				if (k == 3)
					k = 0;
				else
					k++;

				/* ----- Constanta alegere rotatie - Cladiri ----- */
				if (deg < 360)
					deg += 10;
				else
					deg = 10;
			}
	}

	/* ----- Desenare nori => animatie ----- */
	{
		for (int i = 0; i < SECTION; i++)
			for (int j = 0; j < SECTION; j++) 
				if (rand() % 5 == 1) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(i + -50.0f, 2.0f, j + -50.0f));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));
					RenderSimpleMesh(meshes["quad"], shaders["ShaderLab9"], modelMatrix, mapTextures["cloud"]);
				}
	}

	// Render the point light in the scene
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, lightPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix, mapTextures["sun"]);
	}
}

void Laborator9::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator9::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	GLint loc_light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(loc_light_position, 1, glm::value_ptr(lightPosition));

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	GLint loc_eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(loc_eye_position, 1, glm::value_ptr(eyePosition));

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	GLint loc = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(loc, materialShininess);

	loc = glGetUniformLocation(shader->program, "material_kd");  // componenta difuza
	glUniform1f(loc, materialKd);

	loc = glGetUniformLocation(shader->program, "material_ks");  // componenta speculara
	glUniform1f(loc, materialKs);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	unsigned int gl_texture_object;

	glGenTextures(1, &gl_texture_object);

	if (texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

Texture2D* Laborator9::CreateRandomTexture(unsigned int width, unsigned int height)
{
	GLuint textureID = 0;
	unsigned int channels = 3;
	unsigned int size = width * height * channels;
	unsigned char* data = new unsigned char[size];

	// TODO: generate random texture data

	// Generate and bind the new texture ID

	// TODO: Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	CheckOpenGLError();

	// TODO: Use glTextImage2D to set the texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	// TODO: Generate texture mip-maps

	CheckOpenGLError();

	// Save the texture into a wrapper Texture2D class for using easier later during rendering phase
	Texture2D* texture = new Texture2D();
	texture->Init(textureID, width, height, channels);

	SAFE_FREE_ARRAY(data);
	return texture;
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator9::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 2;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	}
}

void Laborator9::OnKeyPress(int key, int mods)
{
	/* ----- Schimbare scalare - Nivel 2 cladiri ----- */
	if (window->KeyHold(GLFW_KEY_R)) 
		for (int i = 0; i < 5; i++)
			scale[i] = rand() % 4 + 1.15f;
}

void Laborator9::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator9::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator9::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator9::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator9::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator9::OnWindowResize(int width, int height)
{
}
