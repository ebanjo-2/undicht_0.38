#include "material.h"




Material::Material() {
    //ctor
}

Material::~Material() {
    //dtor
}


unsigned short Material::setName(const std::string& material_prefix, const std::string& material_name) {
    /** @return the new id assigned to the material */

	m_name = material_name;
	m_prefix = material_prefix;

	return m_id;
}

const std::string& Material::getPrefix() {

	return m_prefix;
}

const std::string& Material::getName() {

	return m_name;
}

void Material::setType(MaterialType type) {

	m_type = type;
}

MaterialType Material::getType() {

	return m_type;
}
