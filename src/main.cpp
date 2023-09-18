#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>

#include "Renderer.hpp"

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Controls.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

const unsigned int windowWidth = 1920;
const unsigned int windowHeight = 1080;

float CalculateWaveHeight(float x, float z, float currentTime)
{
	float y = 0.0f;

	// Calculate the wave height at the given position

		y = sinf(0.5 * (x + z + 10.0 * currentTime)) * 3.0;
	
	return y;
}


int main(void)
{

    const int numRows = 100;
    const int numCols = 100;
    const float planeSizeX = 100.0f;
    const float planeSizeZ = 100.0f;
    const float cellWidth = planeSizeX / (numCols - 1);
    const float cellDepth = planeSizeZ / (numRows - 1);

    // Create an array to hold vertex positions
    float vertices[numRows * numCols * 3];

    // Calculate heights and update vertex positions

    const int numIndices = (numRows - 1) * (numCols - 1) * 6;

    // Step 3: Allocate memory for indices
    unsigned int indices2[numIndices];

    // Step 4: Generate indices
    int index = 0;
    for (int i = 0; i < numRows - 1; i++) {
        for (int j = 0; j < numCols - 1; j++) {
            // Define indices for the two triangles of each grid cell
            int topLeft = i * numCols + j;
            int topRight = topLeft + 1;
            int bottomLeft = (i + 1) * numCols + j;
            int bottomRight = bottomLeft + 1;

            // Triangle 1
            indices2[index++] = topLeft;
            indices2[index++] = topRight;
            indices2[index++] = bottomLeft;

            // Triangle 2
            indices2[index++] = topRight;
            indices2[index++] = bottomRight;
            indices2[index++] = bottomLeft;
        }
    }









    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	//set the major version of OpenGL to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	//set the minor version of OpenGL to 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//set the OpenGL profile to core

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowWidth, windowHeight, ";-; this is so painful", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;
    {

        float positions[] = {
            -50.0f,  -20.0f, -50.0f,
             50.0f,  -20.0f, -50.0f,
             50.0f,  -20.0f,  50.0f,
            -50.0f,  -20.0f,  50.0f,
        };


        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        
        VertexArray va; //create a vertex array
        VertexBuffer vb(positions, 4 * 3 * sizeof(float)); //create a vertex buffer

        VertexBufferLayout layout;  //create a vertex buffer layout
        layout.Push<float>(3);  //push a float with 2 elements to the layout
        va.AddBuffer(vb, layout);   //add the vertex buffer and the layout to the vertex array
        IndexBuffer ib(indices, 6); //create an index buffer
        
        Shader shader("res/shaders/Basic.shader"); //create a shader

        
        //GLCall(glUseProgram(1));
        //std::cout << "LOL" << std::endl;

        //shader.Bind(); //bind the shader
       
        //Texture texture("res/textures/img.png"); //create a texture)
        //texture.Bind();
        //shader.SetUniform1i("u_Texture", 0); //set the uniform

        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer; //create a renderer

        ImGui::CreateContext();    
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        glm::vec3 translationA(0, -50.0f, 0);
        glm::vec3 translationB(0, -20.0f, 0.0f);
        glm::vec3 rotationB(0, 1, 0);

        Camera camera(glm::radians(90.0f), (float)windowWidth / (float)windowHeight, 0.1f, 900.0f, window);
        camera.SetPosition(glm::vec3(0, 0, 3));

        Controls controls(window);
        camera.BindControls(&controls);
        float fov = 4000.0f;
        float speed = 0.1f;
        float sensitivity = 0.1f;
        /* Loop until the user closes the window */
        glViewport(0, 0, windowWidth, windowHeight);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            float time = glfwGetTime(); // Get the current time in seconds



            // Define the properties of the rainbow effect
            float frequency = 1.0f; // Adjust the frequency to control the speed of the rainbow effect
            float amplitude = 0.5f; // Adjust the amplitude to control the intensity of the effect

            // Calculate the RGB values based on time
            float r = amplitude * sin(frequency * time + 0.0f) + 0.5f;   // Red channel
            float g = amplitude * sin(frequency * time + 2.0f) + 0.5f; // Green channel
            float b = amplitude * sin(frequency * time + 4.0f) + 0.5f;  // Blue channel

            // Ensure that RGB values are in the range [0, 1]
            r = std::max(0.0f, std::min(1.0f, r));
            g = std::max(0.0f, std::min(1.0f, g));
            b = std::max(0.0f, std::min(1.0f, b));

            glm::mat4 viewMatrix = camera.GetViewMatrix();
            glm::mat4 projectionMatrix = camera.GetProjectionMatrix();
            /* Render here */
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            {   //Draw object A
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA); //create a model matrix
                glm::mat4 mvp = projectionMatrix * viewMatrix * model;
                shader.Bind();
                shader.SetUniform4f("u_Color", r, g, b, 1.0f); //set the uniform
                shader.SetUniformMat4f("u_MVP", mvp); //set the uniform
                renderer.Draw(va, ib, shader);
            }
            {
                for (int i = 0; i < numRows; i++) {
                    for (int j = 0; j < numCols; j++) {
                        float x = j * cellWidth;
                        float z = i * cellDepth;
                        float y = CalculateWaveHeight(x, z, time); // Calculate wave height for this vertex
                        vertices[(i * numCols + j) * 3 + 0] = x;
                        vertices[(i * numCols + j) * 3 + 1] = y;
                        vertices[(i * numCols + j) * 3 + 2] = z;
                    }
                }
                VertexArray va2 = VertexArray();
                IndexBuffer ib2(indices2, numIndices);
                VertexBuffer vb2(vertices, numRows * numCols * 3 * sizeof(float));
                VertexBufferLayout layout;
                layout.Push<float>(3);
                va2.AddBuffer(vb2, layout);
                shader.Bind();
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB); //create a model matrix
				glm::mat4 mvp = projectionMatrix * viewMatrix * model;
				shader.SetUniform4f("u_Color", r, g, b, 1.0f); //set the uniform
				shader.SetUniformMat4f("u_MVP", mvp); //set the uniform
				renderer.Draw(va2, ib2, shader);

            }






            {
                ImGui::SliderFloat3("Translation A", &translationA.x, -960.f, 960.0f);
                ImGui::SliderFloat3("Translation B", &translationB.x, -960.f, 960.0f);
                ImGui::SliderFloat("FOV", &fov, 0.0f, 10000.0f);
                ImGui::SliderFloat("Flying Speed", &speed, 0.0f, 10.0f);
                ImGui::SliderFloat("Mouse Sensitivity", &sensitivity, 0.0f, 10.0f);  
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }
            camera.SetFOV(glm::radians(fov));
            camera.setSpeed(speed);
            camera.setSensitivity(sensitivity);

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
            camera.ProcessControls();
        }
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
