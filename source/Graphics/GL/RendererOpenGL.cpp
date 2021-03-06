#include <Nephilim/Platform.h>

#ifdef NEPHILIM_DESKTOP
#include <Nephilim/Graphics/GL/RendererOpenGL.h>
#include <Nephilim/Graphics/View.h>
#include <Nephilim/Graphics/GL/GLHelpers.h>
#include <Nephilim/Graphics/Shader.h>

#include <Nephilim/Foundation/Logging.h>
#include <Nephilim/Foundation/Math.h>
#include <Nephilim/Foundation/Matrix.h>

#include <iostream>

NEPHILIM_NS_BEGIN

static const char gVertexSource[] =
	"#version 120\n"
	"attribute vec4 vertex;\n"
	"attribute vec4 color;\n"
	"attribute vec2 texCoord;\n"
	"uniform mat4 projection = mat4(1);\n"
	"uniform mat4 model = mat4(1);\n"
	"uniform mat4 view = mat4(1);\n"
	"varying vec4 outColor;\n"
	"varying vec2 texUV;\n"
	"void main() {\n"
	"  gl_Position = projection * view * model * vertex;\n"
	"  outColor = color;\n"
	"  texUV = texCoord;\n"
	"}\n";

static const char gFragmentSource[] =
"#version 120\n"
"uniform sampler2D texture;\n"
"varying vec4 outColor;\n"
"varying vec2 texUV;\n"
"void main() {\n"
" if(texture2D(texture,texUV).a == 0) discard;"
	"   gl_FragColor = texture2D(texture, texUV) * outColor;\n"
	"}\n";


RendererOpenGL::RendererOpenGL()
: GraphicsDevice()
{
	m_shaderUsageHint = true;
	m_type = OpenGL;
	m_name = "OpenGL";

	m_defaultShader.loadShader(GLShader::VertexUnit, gVertexSource);
	m_defaultShader.loadShader(GLShader::FragmentUnit, gFragmentSource);
	m_defaultShader.addAttributeLocation(0, "vertex");
	m_defaultShader.addAttributeLocation(1, "color");
	m_defaultShader.addAttributeLocation(2, "texCoord");
	m_defaultShader.create();
	m_defaultShader.bind();
};

/// This function will create a new shader program from the raw code
/// It runs a preprocessing step to identify what portions belong to what shader
GDI_ShaderProgram* RendererOpenGL::createShader(const String& code)
{
	GLShader* shaderProgram = new GLShader;

	String _vertex;
	String _frag;

	int current_type = 0;
	std::size_t i = 0;
	while (i < code.size())
	{
		if (code[i] == '#' && code.substr(i + 1, 6) == "VERTEX")
		{
			current_type = 1;
			i += 7;
		}
		else if (code[i] == '#' && code.substr(i + 1, 5) == "PIXEL")
		{
			current_type = 2;
			i += 6;
		}
		else if (current_type == 0)
		{
			++i;
		}
		else
		{
			// add the char to the sources
			if (current_type == 1)
			{
				_vertex += code[i++];
			}
			else if (current_type == 2)
			{
				_frag += code[i++];
			}
		}
	}


	shaderProgram->loadShader(GLShader::VertexUnit, _vertex);
	shaderProgram->loadShader(GLShader::FragmentUnit, _frag);
	shaderProgram->addAttributeLocation(0, "vertex");
	shaderProgram->addAttributeLocation(1, "color");
	shaderProgram->addAttributeLocation(2, "texCoord");
	shaderProgram->addAttributeLocation(3, "normal");
	if (shaderProgram->create())
	{
		Log("SUCCESS CREATESHADER");
	}

	std::cout << "Vertex " << std::endl
		<< _vertex << std::endl;

	std::cout << "Pixel " << std::endl
		<< _frag << std::endl;

	_shaderPrograms.push_back(shaderProgram);

	return shaderProgram;
}

/// Push client-side geometry to the GPU
/// This is usually slower than using a VBO because the data is uploaded to the GPU every time
/// Note: This will eventually be the only way to render client-side data
void RendererOpenGL::draw(const VertexArray& vertexData)
{

}

/// Draw a vertex array
void RendererOpenGL::draw(const VertexArray2D& varray, const RenderState& state)
{
	const char* data  = reinterpret_cast<const char*>(&varray.m_vertices[0]);

	if(m_activeShader)
	{
		enableVertexAttribArray(0);
		enableVertexAttribArray(1);
		enableVertexAttribArray(2);

		setVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(VertexArray2D::Vertex), data + 0);
		setVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, true, sizeof(VertexArray2D::Vertex), data + 8);
		setVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(VertexArray2D::Vertex), data + 12);

		drawArrays(varray.geometryType, 0, varray.m_vertices.size());

		disableVertexAttribArray(0);
		disableVertexAttribArray(1);
		disableVertexAttribArray(2);
	}
	else
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(2, GL_FLOAT, sizeof(VertexArray2D::Vertex), data + 0);
		glColorPointer(4, GL_UNSIGNED_BYTE,sizeof(VertexArray2D::Vertex), data + 8);
		glTexCoordPointer(2, GL_FLOAT, sizeof(VertexArray2D::Vertex), data + 12);

		drawArrays(varray.geometryType, 0, varray.m_vertices.size());

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

/// This will cancel all shader-related settings and activate the default shader/fixed pipeline
void RendererOpenGL::setDefaultShader()
{
	if(m_shaderUsageHint)
	{
		m_activeShader = &m_defaultShader;
		m_defaultShader.bind();
	}
	else
	{
		m_activeShader = NULL;
		glUseProgram(0);
	}
}

/// Activates the shader for the next drawing calls
void RendererOpenGL::setShader(Shader& shader)
{
	m_activeShader = (GLShader*)shader.shaderImpl;
	static_cast<GLShader*>(shader.shaderImpl)->bind();
}

void RendererOpenGL::setProjectionMatrix(const mat4& projection)
{
	GraphicsDevice::setProjectionMatrix(projection);
	if(m_activeShader) m_activeShader->setUniformMatrix("projection", projection.get());
}

void RendererOpenGL::setViewMatrix(const mat4& view)
{
	GraphicsDevice::setViewMatrix(view);
	if(m_activeShader) m_activeShader->setUniformMatrix("view", view.get());
}

void RendererOpenGL::setModelMatrix(const mat4& model)
{
	GraphicsDevice::setModelMatrix(model);
	if(m_activeShader) m_activeShader->setUniformMatrix("model", model.get());
}
/*
void RendererOpenGL::applyView(const View &view){
	if(!m_renderTarget) return;

	IntRect viewport = ((Window*)m_renderTarget)->getViewport(view);
	int top = m_renderTarget->getSize().y - (viewport.top + viewport.height);
	glViewport(viewport.left, top, viewport.width, viewport.height);

	if(m_shader)
	{
		m_shader->setUniformMatrix("projection", view.getTransform().getMatrix());
	}
	else
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//gluPerspective(45, (float)width / height, 0.5f, 150);
		FloatRect rect = view.getRect();
		//glOrtho(rect.left, rect.width , rect.height ,rect.top, -0.1f , 0.1f);
		glLoadMatrixf(view.getTransform().getMatrix());
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
};*/

//void RendererOpenGL::enableClipping(FloatRect rect)
//{
	/*FloatRect current;
	if(m_clipRegionStack.empty())
	{
		glEnable(GL_SCISSOR_TEST);

		// full window test
		current = FloatRect(0,0,m_renderTarget->getSize().x, m_renderTarget->getSize().y);
	}
	else
	{
		current = m_clipRegionStack.top();
	}

	// Crop rect if needed and push
	if(rect.left < current.left)
	{
		rect.width = rect.width - current.left - rect.left;
		rect.left = current.left;
	}
	if(rect.top < current.top)
	{
		rect.height = rect.height - current.top - rect.top;
		rect.top = current.top;
	}
	if(rect.left + rect.width > current.left + current.width)
	{
		rect.width = current.left + current.width - rect.left;
	}
	if(rect.top + rect.height > current.top + current.height)
	{
		rect.height = current.top + current.height - rect.top;
	}

	m_clipRegionStack.push(rect);

	activateClipRegion(m_clipRegionStack.top());*/
//}

//void RendererOpenGL::activateClipRegion(FloatRect rect)
//{
//	glScissor(rect.left, m_renderTarget->getSize().y - (rect.top + rect.height), rect.width, rect.height);
//}


NEPHILIM_NS_END
#endif
