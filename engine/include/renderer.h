#pragma once

#include <vector>

// forward declaration
class Renderable2D;

class Renderer
{
public:
    Renderer()
    {
    }

    virtual ~Renderer()
    {
    }

    virtual void submit(const Renderable2D *const renderable) = 0;

    inline void submit(const std::vector<Renderable2D> &renderables)
    {
        unsigned n_renderable = renderables.size();

        for(unsigned i = 0; i < n_renderable; ++i)
        {
            submit(&renderables[i]);
        }
    }

    //template<typename T>
    //inline void submit(const T &renderables)
    //{
        //for(auto const &renderable : renderables)
        //{
            //submit(&renderable);
        //}
    //}

    virtual void flush() = 0;
};
