#include "opengl_Wrapper.h"

namespace opengl {

	bool FunctionWrapper::m_threaded_wrapper = false;
	bool FunctionWrapper::m_shutdown = false;
	std::thread FunctionWrapper::m_commandExecutionThread;
	BlockingQueue<std::shared_ptr<OpenGlCommand>> FunctionWrapper::m_commandQueue;

	void FunctionWrapper::executeCommand(std::shared_ptr<OpenGlCommand> _command)
	{
		if (m_threaded_wrapper) {
			m_commandQueue.push(_command);
			_command->waitOnCommand();
		} else {
			_command->performCommand();
		}
	}

	void FunctionWrapper::commandLoop(void)
	{
		while(!m_shutdown)
		{
			std::shared_ptr<OpenGlCommand> command = m_commandQueue.pop();
			command->performCommand();
		}
	}

	void FunctionWrapper::setThreadedMode(void)
	{
		m_threaded_wrapper = true;

		m_commandExecutionThread = std::thread(FunctionWrapper::commandLoop);
	}

	void FunctionWrapper::glBlendFunc(GLenum sfactor, GLenum dfactor)
	{
		executeCommand(std::make_shared<GlBlendFuncCommand>(sfactor, dfactor));
	}

	void FunctionWrapper::glPixelStorei(GLenum pname, GLint param)
	{
		executeCommand(std::make_shared<GlPixelStoreiCommand>(pname, param));
	}

	void FunctionWrapper::glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{
		executeCommand(std::make_shared<GlClearColorCommand>(red, green, blue, alpha));
	}

	void FunctionWrapper::glCullFace(GLenum mode)
	{
		executeCommand(std::make_shared<GlCullFaceCommand>(mode));
	}

	void FunctionWrapper::glDepthFunc(GLenum func)
	{
		executeCommand(std::make_shared<GlDepthFuncCommand>(func));
	}

	void FunctionWrapper::glDepthMask(GLboolean flag)
	{
		executeCommand(std::make_shared<GlDepthMaskCommand>(flag));
	}

	void FunctionWrapper::glDisable(GLenum cap)
	{
		executeCommand(std::make_shared<GlDisableCommand>(cap));
	}

	void FunctionWrapper::glEnable(GLenum cap)
	{
		executeCommand(std::make_shared<GlEnableCommand>(cap));
	}

	void FunctionWrapper::glPolygonOffset(GLfloat factor, GLfloat units)
	{
		executeCommand(std::make_shared<GlPolygonOffsetCommand>(factor, units));
	}

	void FunctionWrapper::glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
	{
		executeCommand(std::make_shared<GlScissorCommand>(x, y, width, height));
	}

	void FunctionWrapper::glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
	{
		executeCommand(std::make_shared<GlViewportCommand>(x, y, width, height));
	}

	void FunctionWrapper::glBindTexture(GLenum target, GLuint texture)
	{
		executeCommand(std::make_shared<GlBindTextureCommand>(target, texture));
	}

	void FunctionWrapper::glTexParameteri(GLenum target, GLenum pname, GLint param)
	{
		executeCommand(std::make_shared<GlTexParameteriCommand>(target, pname, param));
	}

	void FunctionWrapper::glGetIntegerv(GLenum pname, GLint *data)
	{
		executeCommand(std::make_shared<GlGetIntegervCommand>(pname, data));
	}

	const GLubyte* FunctionWrapper::glGetString(GLenum name)
	{
		const GLubyte* returnValue;
		executeCommand(std::make_shared<GlGetStringCommand>(name, returnValue));

		return returnValue;
	}

	void FunctionWrapper::glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
	{
		executeCommand(std::make_shared<GlReadPixelsCommand>(x, y, width, height, format, type, pixels));
	}

	void FunctionWrapper::glTexSubImage2DBuffered(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, std::size_t offset)
	{
		executeCommand(std::make_shared<GlTexSubImage2DBufferedCommand>(target, level, xoffset, yoffset, width, height, format, type, offset));
	}

	void FunctionWrapper::glDrawArrays(GLenum mode, GLint first, GLsizei count)
	{
		executeCommand(std::make_shared<GlDrawArraysCommand>(mode, first, count));
	}

	GLenum FunctionWrapper::glGetError(void)
	{
		GLenum returnValue;
		executeCommand(std::make_shared<GlGetErrorCommand>(returnValue));
		return returnValue;
	}

	void FunctionWrapper::glLineWidth(GLfloat width)
	{
		executeCommand(std::make_shared<GlLineWidthCommand>(width));
	}

	void FunctionWrapper::glClear(GLbitfield mask)
	{
		executeCommand(std::make_shared<GlClearCommand>(mask));
	}

	void FunctionWrapper::glGetFloatv(GLenum pname, GLfloat *data)
	{
		executeCommand(std::make_shared<GlGetFloatvCommand>(pname, data));
	}

	void FunctionWrapper::glDeleteTextures(GLsizei n, std::unique_ptr<GLuint[]> textures)
	{
		executeCommand(std::make_shared<GlDeleteTexturesCommand>(n, std::move(textures)));
	}

	void FunctionWrapper::glGenTextures(GLsizei n, GLuint *textures)
	{
		executeCommand(std::make_shared<GlGenTexturesCommand>(n, textures));
	}

	void FunctionWrapper::glTexParameterf(GLenum target, GLenum pname, GLfloat param)
	{
		executeCommand(std::make_shared<GlTexParameterfCommand>(target, pname, param));
	}

	void FunctionWrapper::glActiveTexture(GLenum texture)
	{
		executeCommand(std::make_shared<GlActiveTextureCommand>(texture));
	}

	void FunctionWrapper::glBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{
		executeCommand(std::make_shared<GlBlendColorCommand>(red, green, blue, alpha));
	}

	void FunctionWrapper::glReadBuffer(GLenum src)
	{
		executeCommand(std::make_shared<GlReadBufferCommand>(src));
	}

	GLuint FunctionWrapper::glCreateShader(GLenum type)
	{
		GLuint returnValue;
		executeCommand(std::make_shared<GlCreateShaderCommand>(type, returnValue));
		return returnValue;
	}

	void FunctionWrapper::glCompileShader(GLuint shader)
	{
		executeCommand(std::make_shared<GlCompileShaderCommand>(shader));
	}

	void FunctionWrapper::glShaderSource(GLuint shader, const std::string& string)
	{
		executeCommand(std::make_shared<GlShaderSourceCommand>(shader, string));
	}

	GLuint FunctionWrapper::glCreateProgram(void)
	{
		GLuint returnValue;
		executeCommand(std::make_shared<GlCreateProgramCommand>(returnValue));
		return returnValue;
	}

	void FunctionWrapper::glAttachShader(GLuint program, GLuint shader)
	{
		executeCommand(std::make_shared<GlAttachShaderCommand>(program, shader));
	}

	void FunctionWrapper::glLinkProgram(GLuint program)
	{
		executeCommand(std::make_shared<GlLinkProgramCommand>(program));
	}

	void FunctionWrapper::glUseProgram(GLuint program)
	{
		executeCommand(std::make_shared<GlUseProgramCommand>(program));
	}

	GLint FunctionWrapper::glGetUniformLocation(GLuint program, const GLchar *name)
	{
		GLint returnValue;
		executeCommand(std::make_shared<GlGetUniformLocationCommand>(program, name, returnValue));
		return returnValue;
	}

	void FunctionWrapper::glUniform1i(GLint location, GLint v0)
	{
		executeCommand(std::make_shared<GlUniform1iCommand>(location, v0));
	}

	void FunctionWrapper::glUniform1f(GLint location, GLfloat v0)
	{
		executeCommand(std::make_shared<GlUniform1fCommand>(location, v0));
	}

	void FunctionWrapper::glUniform2f(GLint location, GLfloat v0, GLfloat v1)
	{
		executeCommand(std::make_shared<GlUniform2fCommand>(location, v0, v1));
	}

	void FunctionWrapper::glUniform2i(GLint location, GLint v0, GLint v1)
	{
		executeCommand(std::make_shared<GlUniform2iCommand>(location, v0, v1));
	}

	void FunctionWrapper::glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
	{
		executeCommand(std::make_shared<GlUniform4iCommand>(location, v0, v1, v2, v3));
	}


	void FunctionWrapper::glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		executeCommand(std::make_shared<GlUniform4fCommand>(location, v0, v1, v2, v3));
	}

	void FunctionWrapper::glUniform3fv(GLint location, GLsizei count, std::unique_ptr<GLfloat[]> value)
	{
		executeCommand(std::make_shared<GlUniform3fvCommand>(location, count, std::move(value)));
	}

	void FunctionWrapper::glUniform4fv(GLint location, GLsizei count, std::unique_ptr<GLfloat[]> value)
	{
		executeCommand(std::make_shared<GlUniform4fvCommand>(location, count, std::move(value)));
	}

	void FunctionWrapper::glDetachShader(GLuint program, GLuint shader)
	{
		executeCommand(std::make_shared<GlDetachShaderCommand>(program, shader));
	}

	void FunctionWrapper::glDeleteShader(GLuint shader)
	{
		executeCommand(std::make_shared<GlDeleteShaderCommand>(shader));
	}

	void FunctionWrapper::glDeleteProgram(GLuint program)
	{
		executeCommand(std::make_shared<GlDeleteProgramCommand>(program));
	}

	void FunctionWrapper::glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
	{
		executeCommand(std::make_shared<GlGetProgramInfoLogCommand>(program, bufSize, length, infoLog));
	}

	void FunctionWrapper::glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
	{
		executeCommand(std::make_shared<GlGetShaderInfoLogCommand>(shader, bufSize, length, infoLog));
	}

	void FunctionWrapper::glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
	{
		executeCommand(std::make_shared<GlGetShaderivCommand>(shader, pname, params));
	}

	void FunctionWrapper::glGetProgramiv(GLuint program, GLenum pname, GLint *params)
	{
		executeCommand(std::make_shared<GlGetProgramivCommand>(program, pname, params));
	}


	void FunctionWrapper::glEnableVertexAttribArray(GLuint index)
	{
		executeCommand(std::make_shared<GlEnableVertexAttribArrayCommand>(index));
	}

	void FunctionWrapper::glDisableVertexAttribArray(GLuint index)
	{
		executeCommand(std::make_shared<GlDisableVertexAttribArrayCommand>(index));
	}

	void FunctionWrapper::glVertexAttribPointerBuffered(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, std::size_t offset)
	{
		executeCommand(std::make_shared<GlVertexAttribPointerBufferedCommand>(index, size, type, normalized, stride, offset));
	}

	void FunctionWrapper::glVertexAttribPointerNotThreadSafe(GLuint index, GLint size, GLenum type, GLboolean normalized,
		GLsizei stride, const void *pointer)
	{
		executeCommand(std::make_shared<GlVertexAttribPointerNotThreadSafeCommand>(index, size, type, normalized, stride, pointer));
	}

	void FunctionWrapper::glVertexAttribPointerUnbuffered(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
		std::size_t offset, std::shared_ptr<std::vector<char>> data)
	{
		executeCommand(std::make_shared<GlVertexAttribPointerUnbufferedCommand>(index, size, type, normalized, stride, offset, data));
	}

	void FunctionWrapper::glBindAttribLocation(GLuint program, GLuint index, const std::string& name)
	{
		executeCommand(std::make_shared<GlBindAttribLocationCommand>(program, index, std::move(name)));
	}

	void FunctionWrapper::glVertexAttrib1f(GLuint index, GLfloat x)
	{
		executeCommand(std::make_shared<GlVertexAttrib1fCommand>(index, x));
	}

	void FunctionWrapper::glVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{
		executeCommand(std::make_shared<GlVertexAttrib4fCommand>(index, x, y, z, w));
	}

	void FunctionWrapper::glVertexAttrib4fv(GLuint index, std::unique_ptr<GLfloat[]> v)
	{
		executeCommand(std::make_shared<GlVertexAttrib4fvCommand>(index, std::move(v)));
	}


	void FunctionWrapper::glDepthRangef(GLfloat n, GLfloat f)
	{
		executeCommand(std::make_shared<GlDepthRangefCommand>(n, f));
	}

	void FunctionWrapper::glClearDepthf(GLfloat d)
	{
		executeCommand(std::make_shared<GlClearDepthfCommand>(d));
	}


	void FunctionWrapper::glDrawBuffers(GLsizei n, std::unique_ptr<GLenum[]> bufs)
	{
		executeCommand(std::make_shared<GlDrawBuffersCommand>(n, std::move(bufs)));
	}

	void FunctionWrapper::glGenFramebuffers(GLsizei n, GLuint *framebuffers)
	{
		executeCommand(std::make_shared<GlGenFramebuffersCommand>(n, framebuffers));
	}

	void FunctionWrapper::glBindFramebuffer(GLenum target, GLuint framebuffer)
	{
		executeCommand(std::make_shared<GlBindFramebufferCommand>(target, framebuffer));
	}

	void FunctionWrapper::glDeleteFramebuffers(GLsizei n, std::unique_ptr<GLuint[]> framebuffers)
	{
		executeCommand(std::make_shared<GlDeleteFramebuffersCommand>(n, std::move(framebuffers)));
	}

	void FunctionWrapper::glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
	{
		executeCommand(std::make_shared<GlFramebufferTexture2DCommand>(target, attachment, textarget, texture, level));
	}

	void FunctionWrapper::glTexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
	{
		executeCommand(std::make_shared<GlTexImage2DMultisampleCommand>(target, samples, internalformat, width, height, fixedsamplelocations));
	}

	void FunctionWrapper::glTexStorage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
	{
		executeCommand(std::make_shared<GlTexStorage2DMultisampleCommand>(target, samples, internalformat, width, height, fixedsamplelocations));
	}

	void FunctionWrapper::glGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
	{
		executeCommand(std::make_shared<GlGenRenderbuffersCommand>(n, renderbuffers));
	}

	void FunctionWrapper::glBindRenderbuffer(GLenum target, GLuint renderbuffer)
	{
		executeCommand(std::make_shared<GlBindRenderbufferCommand>(target, renderbuffer));
	}

	void FunctionWrapper::glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
	{
		executeCommand(std::make_shared<GlRenderbufferStorageCommand>(target, internalformat, width, height));
	}

	void FunctionWrapper::glDeleteRenderbuffers(GLsizei n, std::unique_ptr<GLuint[]> renderbuffers)
	{
		executeCommand(std::make_shared<GlDeleteRenderbuffersCommand>(n, std::move(renderbuffers)));
	}

	void FunctionWrapper::glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
	{
		executeCommand(std::make_shared<GlFramebufferRenderbufferCommand>(target, attachment, renderbuffertarget, renderbuffer));
	}

	GLenum FunctionWrapper::glCheckFramebufferStatus(GLenum target)
	{
		GLenum returnValue;
		executeCommand(std::make_shared<GlCheckFramebufferStatusCommand>(target, returnValue));
		return returnValue;
	}

	void FunctionWrapper::glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
	{
		executeCommand(std::make_shared<GlBlitFramebufferCommand>(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter));
	}

	void FunctionWrapper::glGenVertexArrays(GLsizei n, GLuint *arrays)
	{
		executeCommand(std::make_shared<GlGenVertexArraysCommand>(n, arrays));
	}

	void FunctionWrapper::glBindVertexArray(GLuint array)
	{
		executeCommand(std::make_shared<GlBindVertexArrayCommand>(array));
	}

	void FunctionWrapper::glDeleteVertexArrays(GLsizei n, std::unique_ptr<GLuint[]> arrays)
	{
		executeCommand(std::make_shared<GlDeleteVertexArraysCommand>(n, std::move(arrays)));
	}

	void FunctionWrapper::glGenBuffers(GLsizei n, GLuint *buffers)
	{
		executeCommand(std::make_shared<GlGenBuffersCommand>(n, buffers));
	}

	void FunctionWrapper::glBindBuffer(GLenum target, GLuint buffer)
	{
		executeCommand(std::make_shared<GlBindBufferCommand>(target, buffer));
	}

	void FunctionWrapper::glMapBuffer(GLenum target, GLenum access)
	{
		executeCommand(std::make_shared<GlMapBufferCommand>(target, access));
	}

	void* FunctionWrapper::glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
	{
		GLubyte* returnValue;
		executeCommand(std::make_shared<GlMapBufferRangeCommand>(target, offset, length, access, returnValue));
		return returnValue;
	}

	GLboolean FunctionWrapper::glUnmapBuffer(GLenum target)
	{
		GLboolean returnValue;
		executeCommand(std::make_shared<GlUnmapBufferCommand>(target, returnValue));
		return returnValue;
	}

	void FunctionWrapper::glDeleteBuffers(GLsizei n, std::unique_ptr<GLuint[]> buffers)
	{
		executeCommand(std::make_shared<GlDeleteBuffersCommand>(n, std::move(buffers)));
	}

	void FunctionWrapper::glBindImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
	{
		executeCommand(std::make_shared<GlBindImageTextureCommand>(unit, texture, level, layered, layer, access, format));
	}

	void FunctionWrapper::glMemoryBarrier(GLbitfield barriers)
	{
		executeCommand(std::make_shared<GlMemoryBarrierCommand>(barriers));
	}

	const GLubyte* FunctionWrapper::glGetStringi(GLenum name, GLuint index)
	{
		const GLubyte* returnValue;
		executeCommand(std::make_shared<GlGetStringiCommand>(name, index, returnValue));
		return returnValue;
	}

	void FunctionWrapper::glInvalidateFramebuffer(GLenum target, GLsizei numAttachments, std::unique_ptr<GLenum[]> attachments)
	{
		executeCommand(std::make_shared<GlInvalidateFramebufferCommand>(target, numAttachments, std::move(attachments)));
	}

	GLsync FunctionWrapper::glFenceSync(GLenum condition, GLbitfield flags)
	{
		GLsync returnValue;
		executeCommand(std::make_shared<GlFenceSyncCommand>(condition, flags, returnValue));
		return returnValue;
	}

	void FunctionWrapper::glClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
	{
		executeCommand(std::make_shared<GlClientWaitSyncCommand>(sync, flags, timeout));
	}

	void FunctionWrapper::glDeleteSync(GLsync sync)
	{
		executeCommand(std::make_shared<GlDeleteSyncCommand>(sync));
	}

	GLuint FunctionWrapper::glGetUniformBlockIndex(GLuint program, const GLchar *uniformBlockName)
	{
		GLuint returnValue;
		executeCommand(std::make_shared<GlGetUniformBlockIndexCommand>(program, uniformBlockName, returnValue));
		return returnValue;
	}

	void FunctionWrapper::glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
	{
		executeCommand(std::make_shared<GlUniformBlockBindingCommand>(program, uniformBlockIndex, uniformBlockBinding));
	}

	void FunctionWrapper::glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params)
	{
		executeCommand(std::make_shared<GlGetActiveUniformBlockivCommand>(program, uniformBlockIndex, pname, params));
	}

	void FunctionWrapper::glGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices)
	{
		executeCommand(std::make_shared<GlGetUniformIndicesCommand>(program, uniformCount, uniformNames, uniformIndices));
	}

	void FunctionWrapper::glGetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params)
	{
		executeCommand(std::make_shared<GlGetActiveUniformsivCommand>(program, uniformCount, uniformIndices, pname, params));
	}

	void FunctionWrapper::glBindBufferBase(GLenum target, GLuint index, GLuint buffer)
	{
		executeCommand(std::make_shared<GlBindBufferBaseCommand>(target, index, buffer));
	}

	void FunctionWrapper::glGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary)
	{
		executeCommand(std::make_shared<GlGetProgramBinaryCommand>(program, bufSize, length, binaryFormat, binary));
	}

	void FunctionWrapper::glProgramParameteri(GLuint program, GLenum pname, GLint value)
	{
		executeCommand(std::make_shared<GlProgramParameteriCommand>(program, pname, value));
	}

	void FunctionWrapper::glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
	{
		executeCommand(std::make_shared<GlTexStorage2DCommand>(target, levels, internalformat, width, height));
	}

	void FunctionWrapper::glTextureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
	{
		executeCommand(std::make_shared<GlTextureStorage2DCommand>(texture, levels, internalformat, width, height));
	}

	void FunctionWrapper::glTextureSubImage2DBuffered(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, std::size_t offset)
	{
		executeCommand(std::make_shared<GlTextureSubImage2DBufferedCommand>(texture, level, xoffset, yoffset, width, height, format, type, offset));
	}

	void FunctionWrapper::glTextureStorage2DMultisample(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
	{
		executeCommand(std::make_shared<GlTextureStorage2DMultisampleCommand>(texture, target, samples, internalformat, width, height, fixedsamplelocations));
	}

	void FunctionWrapper::glTextureParameteri(GLuint texture, GLenum pname, GLint param)
	{
		executeCommand(std::make_shared<GlTextureParameteriCommand>(texture, pname, param));
	}

	void FunctionWrapper::glTextureParameterf(GLuint texture, GLenum pname, GLfloat param)
	{
		executeCommand(std::make_shared<GlTextureParameterfCommand>(texture, pname, param));
	}

	void FunctionWrapper::glCreateTextures(GLenum target, GLsizei n, GLuint *textures)
	{
		executeCommand(std::make_shared<GlCreateTexturesCommand>(target, n, textures));
	}

	void FunctionWrapper::glCreateBuffers(GLsizei n, GLuint *buffers)
	{
		executeCommand(std::make_shared<GlCreateBuffersCommand>(n, buffers));
	}

	void FunctionWrapper::glCreateFramebuffers(GLsizei n, GLuint *framebuffers)
	{
		executeCommand(std::make_shared<GlCreateFramebuffersCommand>(n, framebuffers));
	}

	void FunctionWrapper::glNamedFramebufferTexture(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level)
	{
		executeCommand(std::make_shared<GlNamedFramebufferTextureCommand>(framebuffer, attachment, texture, level));
	}

	void FunctionWrapper::glDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const char* indices, GLint basevertex)
	{
		executeCommand(std::make_shared<GlDrawElementsBaseVertexCommand>(mode, count, type, std::move(indices), basevertex));
	}

	void FunctionWrapper::glFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length)
	{
		executeCommand(std::make_shared<GlFlushMappedBufferRangeCommand>(target, offset, length));
	}

	void FunctionWrapper::glFinish(void)
	{
		executeCommand(std::make_shared<GlFinishCommand>());
	}

	void FunctionWrapper::CoreVideo_Init(void)
	{
		executeCommand(std::make_shared<CoreVideoInitCommand>());
	}

	void FunctionWrapper::CoreVideo_Quit(void)
	{
		m_shutdown = true;
		executeCommand(std::make_shared<CoreVideoQuitCommand>());
		m_commandExecutionThread.join();
	}

	m64p_error FunctionWrapper::CoreVideo_SetVideoMode(int screenWidth, int screenHeight, int bitsPerPixel, m64p_video_mode mode, m64p_video_flags flags)
	{
		m64p_error returnValue;
		executeCommand(std::make_shared<CoreVideoSetVideoModeCommand>(screenWidth, screenHeight, bitsPerPixel, mode, flags, returnValue));
		return returnValue;
	}

	void FunctionWrapper::CoreVideo_GL_SetAttribute(m64p_GLattr attribute, int value)
	{
		executeCommand(std::make_shared<CoreVideoGLSetAttributeCommand>(attribute, value));
	}

	void FunctionWrapper::CoreVideo_GL_GetAttribute(m64p_GLattr attribute, int *value)
	{
		executeCommand(std::make_shared<CoreVideoGLGetAttributeCommand>(attribute, value));
	}

	void FunctionWrapper::CoreVideo_GL_SwapBuffers(void)
	{
		executeCommand(std::make_shared<CoreVideoGLSwapBuffersCommand>());
	}

}
