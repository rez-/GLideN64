#include <Graphics/Context.h>
#include "opengl_ColorBufferReaderWithPixelBuffer.h"
#include "opengl_Wrapper.h"

using namespace graphics;
using namespace opengl;

ColorBufferReaderWithPixelBuffer::ColorBufferReaderWithPixelBuffer(CachedTexture *_pTexture,
																   CachedBindBuffer *_bindBuffer)
	: ColorBufferReader(_pTexture), m_bindBuffer(_bindBuffer)
{
	_initBuffers();
}


ColorBufferReaderWithPixelBuffer::~ColorBufferReaderWithPixelBuffer()
{
	_destroyBuffers();
}

void ColorBufferReaderWithPixelBuffer::_destroyBuffers()
{
	auto buffers = std::unique_ptr<GLuint[]>(new GLuint[_numPBO]);

	for(unsigned int index = 0; index < _numPBO; ++index) {
		buffers[index] = m_PBO[index];
	}

	FunctionWrapper::glDeleteBuffers(_numPBO, std::move(buffers));

	for(int index = 0; index < _numPBO; ++index)
		m_PBO[index] = 0;
}

void ColorBufferReaderWithPixelBuffer::_initBuffers()
{
	// Generate Pixel Buffer Objects
	FunctionWrapper::glGenBuffers(_numPBO, m_PBO);
	m_curIndex = 0;

	// Initialize Pixel Buffer Objects
	for (u32 i = 0; i < _numPBO; ++i) {
		m_bindBuffer->bind(Parameter(GL_PIXEL_PACK_BUFFER), ObjectHandle(m_PBO[i]));
		FunctionWrapper::glBufferData(GL_PIXEL_PACK_BUFFER, m_pTexture->textureBytes, std::move(std::unique_ptr<u8[]>(nullptr)), GL_DYNAMIC_READ);
	}
	m_bindBuffer->bind(Parameter(GL_PIXEL_PACK_BUFFER), ObjectHandle::null);
}

u8 * ColorBufferReaderWithPixelBuffer::readPixels(s32 _x0, s32 _y0, u32 _width, u32 _height, u32 _size, bool _sync)
{
	const graphics::FramebufferTextureFormats & fbTexFormat = gfxContext.getFramebufferTextureFormats();
	GLenum colorFormat, colorType, colorFormatBytes;
	if (_size > G_IM_SIZ_8b) {
		colorFormat = GLenum(fbTexFormat.colorFormat);
		colorType = GLenum(fbTexFormat.colorType);
		colorFormatBytes = GLenum(fbTexFormat.colorFormatBytes);
	} else {
		colorFormat = GLenum(fbTexFormat.monochromeFormat);
		colorType = GLenum(fbTexFormat.monochromeType);
		colorFormatBytes = GLenum(fbTexFormat.monochromeFormatBytes);
	}

	// If Sync, read pixels from the buffer, copy them to RDRAM.
	// If not Sync, read pixels from the buffer, copy pixels from the previous buffer to RDRAM.
	if (!_sync) {
		m_curIndex ^= 1;
		const u32 nextIndex = m_curIndex ^ 1;
		m_bindBuffer->bind(Parameter(GL_PIXEL_PACK_BUFFER), ObjectHandle(m_PBO[m_curIndex]));
		FunctionWrapper::glReadPixelsAsync(_x0, _y0, m_pTexture->realWidth, _height, colorFormat, colorType);
		m_bindBuffer->bind(Parameter(GL_PIXEL_PACK_BUFFER), ObjectHandle(m_PBO[nextIndex]));
	} else {
		m_bindBuffer->bind(Parameter(GL_PIXEL_PACK_BUFFER), ObjectHandle(m_PBO[_numPBO -1]));
		FunctionWrapper::glReadPixels(_x0, _y0, m_pTexture->realWidth, _height, colorFormat, colorType, 0);
	}

	GLubyte* pixelData = (GLubyte*)FunctionWrapper::glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, m_pTexture->realWidth * _height * colorFormatBytes, GL_MAP_READ_BIT);
	if (pixelData == nullptr)
		return nullptr;

	int widthBytes = _width*colorFormatBytes;
	int strideBytes = m_pTexture->realWidth * colorFormatBytes;

	u8 * pixelDataAlloc = m_pixelData.data();
	for (unsigned int lnIndex = 0; lnIndex < _height; ++lnIndex) {
		memcpy(pixelDataAlloc + lnIndex*widthBytes, pixelData + (lnIndex*strideBytes), widthBytes);
	}
	return pixelDataAlloc;
}

void ColorBufferReaderWithPixelBuffer::cleanUp()
{
	FunctionWrapper::glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
	m_bindBuffer->bind(Parameter(GL_PIXEL_PACK_BUFFER), ObjectHandle::null);
}
