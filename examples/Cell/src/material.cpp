#include "material.h"

// all the materials, in the order in which they were constructed
std::vector<std::string> Material::s_names;

std::vector<int> Material::s_material_prefixes; // the prefixes used by the materials

std::vector<std::string> Material::s_prefixes;

const std::string& Material::getName(unsigned short material_id) {

    return s_names[material_id];
}


const std::string& Material::getPrefix(unsigned short material_id) {

    return s_prefixes[s_material_prefixes[material_id]];
}

unsigned short Material::getID(const std::string& material_prefix, const std::string& material_name) {

    for(int i = 0; i < s_names.size(); i++) {

        if(!s_names.at(i).compare(material_name)) {

            if(!s_prefixes.at(s_material_prefixes.at(i)).compare(material_prefix)) {

                return i;
            }

        }

    }


    return 0;
}


Material::Material() {
    //ctor
}

Material::~Material() {
    //dtor
}


unsigned short Material::setName(const std::string& material_prefix, const std::string& material_name) {
    /** @return the new id assigned to the material */
}

const std::string& Material::getPrefix() {

}

const std::string& Material::getName() {

}

void Material::setType(MaterialType type) {

}

MaterialType Material::getType() {


}
