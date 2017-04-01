#include <Config.h>
#include "GLFunctions.h"
#include "opengl_Attributes.h"
#include "opengl_CachedFunctions.h"
#include "opengl_UnbufferedDrawerThreadSafe.h"
#include "opengl_Wrapper.h"
#include <algorithm>

using namespace opengl;

UnbufferedDrawerThreadSafe::UnbufferedDrawerThreadSafe(const GLInfo & _glinfo, CachedVertexAttribArray * _cachedAttribArray)
: m_glInfo(_glinfo)
, m_cachedAttribArray(_cachedAttribArray)
{
	m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::position, false);
	m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::color, false);
	m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::texcoord, false);
	m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::numlights, false);
	m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::modify, false);

	m_cachedAttribArray->enableVertexAttribArray(rectAttrib::position, false);
	m_cachedAttribArray->enableVertexAttribArray(rectAttrib::texcoord0, false);
	m_cachedAttribArray->enableVertexAttribArray(rectAttrib::texcoord1, false);

	m_attribsData.fill(nullptr);
}

UnbufferedDrawerThreadSafe::~UnbufferedDrawerThreadSafe()
{
}

void UnbufferedDrawerThreadSafe::drawTriangles(const graphics::Context::DrawTriangleParameters & _params)
{
	const char* verticesRawData = reinterpret_cast<char*>(_params.vertices);
	std::shared_ptr<std::vector<char>> verticesCopy = std::make_shared<std::vector<char>>(verticesRawData,
		verticesRawData + _params.verticesCount*sizeof(SPVertex));

	{
		m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::position, true);
		FunctionWrapper::glVertexAttribPointerUnbuffered(triangleAttrib::position, 4, GL_FLOAT, GL_FALSE,
			sizeof(SPVertex), offsetof(SPVertex, x), verticesCopy);
	}

	if (_params.combiner->usesShade()) {
		m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::color, true);
		std::size_t offset = _params.flatColors ? offsetof(SPVertex, flat_r) : offsetof(SPVertex, r);

		FunctionWrapper::glVertexAttribPointerUnbuffered(triangleAttrib::color, 4, GL_FLOAT, GL_FALSE,
			sizeof(SPVertex), offset, verticesCopy);
	}
	else
		m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::color, false);

	if (_params.combiner->usesTexture()) {
		m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::texcoord, true);
		FunctionWrapper::glVertexAttribPointerUnbuffered(triangleAttrib::texcoord, 2, GL_FLOAT, GL_FALSE,
			sizeof(SPVertex), offsetof(SPVertex, s), verticesCopy);
	} else
		m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::texcoord, false);

	{
		m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::modify, true);
		FunctionWrapper::glVertexAttribPointerUnbuffered(triangleAttrib::modify, 4, GL_BYTE, GL_FALSE,
			sizeof(SPVertex), offsetof(SPVertex, modify), verticesCopy);
	}

	if (config.generalEmulation.enableHWLighting != 0)
		FunctionWrapper::glVertexAttrib1f(triangleAttrib::numlights, GLfloat(_params.vertices[0].HWLight));

	m_cachedAttribArray->enableVertexAttribArray(rectAttrib::position, false);
	m_cachedAttribArray->enableVertexAttribArray(rectAttrib::texcoord0, false);
	m_cachedAttribArray->enableVertexAttribArray(rectAttrib::texcoord1, false);

	if (_params.elements == nullptr) {
		FunctionWrapper::glDrawArrays(GLenum(_params.mode), 0, _params.verticesCount);
		return;
	}

	if (config.frameBufferEmulation.N64DepthCompare == 0) {
		std::unique_ptr<char[]> elementsCopy(new char[_params.elementsCount]);
		std::copy_n(reinterpret_cast<char*>(_params.elements), _params.elementsCount, elementsCopy.get());

		FunctionWrapper::glDrawElements(GLenum(_params.mode), _params.elementsCount, GL_UNSIGNED_BYTE, std::move(elementsCopy));
		return;
	}

	// Draw polygons one by one
	for (GLint i = 0; i < _params.elementsCount; i += 3) {
		FunctionWrapper::glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		std::unique_ptr<char[]> elementsCopy(new char[3]);
		std::copy_n(reinterpret_cast<char*>(_params.elements) + i, 3, elementsCopy.get());

		FunctionWrapper::glDrawElements(GLenum(_params.mode), 3, GL_UNSIGNED_BYTE, std::move(elementsCopy));
	}
}

void UnbufferedDrawerThreadSafe::drawRects(const graphics::Context::DrawRectParameters & _params)
{
	const char* verticesRawData = reinterpret_cast<char*>(_params.vertices);
	std::shared_ptr<std::vector<char>> verticesCopy = std::make_shared<std::vector<char>>(verticesRawData,
		verticesRawData + _params.verticesCount*sizeof(RectVertex));

	{
		m_cachedAttribArray->enableVertexAttribArray(rectAttrib::position, true);
		FunctionWrapper::glVertexAttribPointerUnbuffered(rectAttrib::position, 4, GL_FLOAT, GL_FALSE, sizeof(RectVertex),
			offsetof(RectVertex, x), verticesCopy);
	}

	if (_params.texrect && _params.combiner->usesTile(0)) {
		m_cachedAttribArray->enableVertexAttribArray(rectAttrib::texcoord0, true);
		FunctionWrapper::glVertexAttribPointerUnbuffered(rectAttrib::texcoord0, 2, GL_FLOAT, GL_FALSE, sizeof(RectVertex),
			offsetof(RectVertex, s0), verticesCopy);
	} else
		m_cachedAttribArray->enableVertexAttribArray(rectAttrib::texcoord0, false);

	if (_params.texrect && _params.combiner->usesTile(1)) {
		m_cachedAttribArray->enableVertexAttribArray(rectAttrib::texcoord1, true);
		FunctionWrapper::glVertexAttribPointerUnbuffered(rectAttrib::texcoord1, 2, GL_FLOAT, GL_FALSE, sizeof(RectVertex),
			offsetof(RectVertex, s1), verticesCopy);
	} else
		m_cachedAttribArray->enableVertexAttribArray(rectAttrib::texcoord1, false);

	m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::position, false);
	m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::color, false);
	m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::texcoord, false);
	m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::modify, false);

	FunctionWrapper::glDrawArrays(GLenum(_params.mode), 0, _params.verticesCount);
}

void UnbufferedDrawerThreadSafe::drawLine(f32 _width, SPVertex * _vertices)
{
	const char* verticesRawData = reinterpret_cast<char*>(_vertices);
	std::shared_ptr<std::vector<char>> verticesCopy = std::make_shared<std::vector<char>>(verticesRawData,
		verticesRawData + 2*sizeof(SPVertex));

	{
		m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::position, true);
		FunctionWrapper::glVertexAttribPointerUnbuffered(triangleAttrib::position, 4, GL_FLOAT, GL_FALSE, sizeof(SPVertex),
			offsetof(SPVertex, x), verticesCopy);
	}

	{
		m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::color, true);
		FunctionWrapper::glVertexAttribPointerUnbuffered(triangleAttrib::color, 4, GL_FLOAT, GL_FALSE, sizeof(SPVertex),
			offsetof(SPVertex, r), verticesCopy);
	}

	m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::texcoord, false);
	m_cachedAttribArray->enableVertexAttribArray(triangleAttrib::modify, false);

	m_cachedAttribArray->enableVertexAttribArray(rectAttrib::position, false);
	m_cachedAttribArray->enableVertexAttribArray(rectAttrib::texcoord0, false);
	m_cachedAttribArray->enableVertexAttribArray(rectAttrib::texcoord1, false);

	FunctionWrapper::glLineWidth(_width);
	FunctionWrapper::glDrawArrays(GL_LINES, 0, 2);
}
