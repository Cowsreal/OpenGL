#include "VertexArray.hpp"
#include "Renderer.hpp"
#include "VertexBufferLayout.hpp"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID)); //generate a vertex array object)
}
VertexArray::~VertexArray() 
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));   //enable the vertex attribute
		GLCall(glVertexAttribPointer(i, element.count , element.type, element.normalized, layout.GetStride(), (const void*)offset)); //0 is the index of the attribute, 2 is the size of the attribute, GL_FLOAT is the type of the attribute, GL_FALSE is whether the data should be normalized, sizeof(float) * 2 is the size of the vertex, 0 is the offset of the 
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	
}
void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}
void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}