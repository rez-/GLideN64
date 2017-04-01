#include "opengl_WrappedFunctions.h"

namespace opengl {

	std::array<std::shared_ptr<std::vector<char>>, MaxAttribIndex> GlVertexAttribPointerUnbufferedCommand::m_attribsData;
}