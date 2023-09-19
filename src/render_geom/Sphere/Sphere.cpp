#include "Sphere.hpp"
#include <iostream>

Sphere::Sphere(float radius)
	: m_Radius(radius), m_VAO(), m_VBO(), m_IBO(), m_VBL(), m_Vertices(), m_Indices()
{
	GenerateSphereVertices();
	GenerateSphereIndices();
	m_IBO = IndexBuffer(&m_Indices[0], m_Indices.size());
	m_VBO = VertexBuffer(&m_Vertices[0], m_Vertices.size() * sizeof(float));

	for (int i = 0; i < m_Vertices.size(); i+=3)
	{
		std::cout << m_Vertices[i] << ", " << m_Vertices[i + 1] << ", " << m_Vertices[i + 2] <<", " << std::endl;
	}
	m_VBL.Push<float>(3);
	m_VAO.AddBuffer(m_VBO, m_VBL);
	std::cout << "Inside sphere" << std::endl;
}

void Sphere::GenerateSphereVertices()
{
	//We'll define the surface coordinates UV as phi and theta, then create a function to map them to XYZ coordinates
	//phi is in the range [0, 2pi], theta is in the range [0, pi]
	float numRows = 50;
	float numCols = 50;
	float dphi = 2 * PI / numCols;		//Normalized phi
	float dtheta = PI / numRows;		//Normalized theta
	float phi = 0;
	float theta = 0;

	for(int i = 0; i < numRows; i++)
	{
		theta = i * dtheta;
		for(int j = 0; j < numCols; j++)
		{
			phi = j * dphi;
			float x = m_Radius * sin(theta) * cos(phi);
			float y = m_Radius * sin(theta) * sin(phi);
			float z = m_Radius * cos(theta);
			m_Vertices.push_back(x);		//x
			m_Vertices.push_back(y);		//y
			m_Vertices.push_back(z);		//z
		}
	}
}

void Sphere::GenerateSphereIndices()
{
	int numRows = 50;
	int numCols = 50;
	for(int i = 0; i < numRows - 1; i++) 
	{			//For each row
		for(int j = 0; j < numCols; j++) 
		{		//For each column
			int currentVertex = i * numCols + j;
			int nextVertex = currentVertex + numCols;
			m_Indices.push_back(currentVertex);
			m_Indices.push_back(nextVertex);
		}
		// Add triangles to connect rows
		if(i < numRows - 2) 
		{
			m_Indices.push_back(m_Indices.back());
			m_Indices.push_back((i + 2) * numCols);
		}
	}
}

void Sphere::Draw()
{
	m_VAO.Bind();
	m_IBO.Bind();
	glDrawElements(GL_TRIANGLE_STRIP, m_Indices.size(), GL_UNSIGNED_INT, 0);
}