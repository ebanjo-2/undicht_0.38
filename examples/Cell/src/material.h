#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include <string>

enum MaterialType {

    DrawAsNothing,      // wont get drawn
    DrawAsBlock,        // solid, textured cube
    DrawAsBumpBlock,    // block with normal map
    DrawAsLight,        // solid block, with texture, except it emits light
    DrawAsModel,        // no cube , forward to model renderer
    DrawAsCustom        // will get forwarded to renderer linked by material

};



class Material {
    /** properties shared by many cells
    * like texture, physical behaviour, ... */

    public:
        // all the materials, in the order in which they were constructed
        static std::vector<std::string> s_names;

        static std::vector<int> s_material_prefixes; // the prefixes used by the materials

        static std::vector<std::string> s_prefixes;

        const std::string& getName(unsigned short material_id);
        const std::string& getPrefix(unsigned short material_id);
        unsigned short getID(const std::string& material_prefix, const std::string& material_name);

    public:
        // string names
        // they should be universally unique
        // since these will be the ones identifying a material in a file

        std::string m_prefix; // example: default
        std::string m_name; // example: stone
        // complete name of this material would be default::stone

    public:
        // the material is going to be distinguished by an id during runtime, since this is faster than comparing strings
        // the id might change between runtimes

        unsigned short m_id = 0; // an unique id is assigned to a new material in its constructor

        MaterialType m_type = DrawAsBlock;

        unsigned short setName(const std::string& material_prefix, const std::string& material_name);
        const std::string& getPrefix();
        const std::string& getName();

        void setType(MaterialType type);
        MaterialType getType();

    public:
        // properties (mostly temporary, to be replaced with something actually meaningfull)

        int texture = 0;
        float density = 1.0f; // be water my friend

    public:

        Material();
        virtual ~Material();

};

#endif // MATERIAL_H
