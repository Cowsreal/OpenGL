#pragma once

class IndexBuffer
{
public:
	IndexBuffer(){}; //default constructor
	IndexBuffer(const unsigned int* data, unsigned int count); //constructor
	~IndexBuffer(); //destructor

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
};