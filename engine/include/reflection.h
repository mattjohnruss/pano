#pragma once

#include <vector>
#include <cassert>

#ifdef PANO_DEBUG
#include <iostream>
#endif

#include <glad/glad.h>

struct ReflectorBase
{
    std::vector<std::size_t> sizes;
    std::vector<std::size_t> offsets;
    std::vector<std::size_t> components;
    std::vector<GLenum> gl_types;
    //static std::vector<std::string> names;

    void register_field_impl_(std::size_t size_bytes, std::size_t offset, std::size_t component, GLenum gl_type /* , std::string &name */)
    {
        sizes.push_back(size_bytes);
        offsets.push_back(offset);
        components.push_back(component);
        gl_types.push_back(gl_type); 
        //names.push_back(name);
    }

    const std::size_t size() const
    {
        unsigned n_field = sizes.size();
        assert(n_field == offsets.size());
        assert(n_field == components.size());
        assert(n_field == gl_types.size());

        // add the offset and the size of the last member to get the total size
        // of the relevant members of the struct
        return offsets[n_field-1] + sizes[n_field-1];
    }
};

// Reflector helper class which keeps track of the sizes and offsets etc and provides register_field(...)
template<typename T>
struct Reflector : public ReflectorBase
{
    Reflector()
    {
#ifdef PANO_DEBUG
        std::cout << "Reflector: Setting up reflection for \"" << typeid(T).name() << "\"\n";
        std::cout << "Reflector: Should happen only once per type!" << std::endl;
#endif

        // perform the reflection setup
        T::setup();

        // check the size is correct
        assert(size() == sizeof(T));
    }
};

// intermediate class to provide the static Reflector variable, templated so we
// can provide each derived class with its own static Reflector rather than one
// shared between all derived classes
template<typename T>
struct ReflectiveStruct
{
    static Reflector<T> r;
};

// instantiate the static variables
// since they are templated, this is automatically instantiated for each derived class
template<typename T> Reflector<T> ReflectiveStruct<T>::r;

#define register_field(T, x, c, gl_T) r.register_field_impl_(sizeof(x), offsetof(T, x), c, gl_T)
