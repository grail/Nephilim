#include <Nephilim/Graphics/Framebuffer.h>
#include <Nephilim/CGL.h>
#include <Nephilim/Logger.h>
#include <Nephilim/Graphics/GLTexture.h>

NEPHILIM_NS_BEGIN

/// Constructs a uninitialized framebuffer
Framebuffer::Framebuffer()
: RenderTarget() 
, m_id(0)
{

}

/// Releases the framebuffer
Framebuffer::~Framebuffer()
{

}

/// Get the size of the target
Vec2i Framebuffer::getSize() const
{
	return m_size;
}

/// Bind the framebuffer as the current one
void Framebuffer::activate()
{
	glBindFramebufferCGL(GL_FRAMEBUFFER_CGL, m_id);
}

/// Attaches a texture to the color attachment 0
void Framebuffer::attachTexture(const Texture& texture)
{
	if(m_id == 0)
	{
		//Log("This is the default framebuffer");
		return;
	}

	glBindFramebufferCGL(GL_FRAMEBUFFER_CGL, m_id);
	glFramebufferTexture2DCGL(GL_FRAMEBUFFER_CGL, GL_COLOR_ATTACHMENT0_CGL, GL_TEXTURE_2D, texture.getIdentifier(), 0);

	if(glCheckFramebufferStatusCGL(GL_FRAMEBUFFER_CGL) == GL_FRAMEBUFFER_COMPLETE_CGL)
	{
		//Log("Created a complete framebuffer");
	}
}

/// Attempts to create the framebuffer
bool Framebuffer::create()
{
	bool success = false;

	GLuint tid = 0;
	glGenFramebuffersCGL(1, static_cast<GLuint*>(&tid));
	glBindFramebufferCGL(GL_FRAMEBUFFER_CGL, tid);

	m_id = tid;
	if(m_id)
	{
		success = true;
	}
	else
		m_id = 0;

	return success;
}

/// Returns the internal id of the currently in-use program by OpenGL
unsigned int Framebuffer::getCurrentActiveFramebuffer()
{
	GLint id = 0;
//	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING_EXT, &id);
	return static_cast<unsigned int>(id);
}

NEPHILIM_NS_END