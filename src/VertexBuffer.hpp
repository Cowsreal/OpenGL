#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size); //constructor
	~VertexBuffer(); //destructor
	
	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_RendererID;
};