#include "resource.hpp"

resource::resource() {
}

texture& resource::tex(const std::string& path) {
	if (m_texs.find(path) == m_texs.end()) {
		m_texs[path].load_from_file(path.c_str());
	}
	return m_texs[path];
}
