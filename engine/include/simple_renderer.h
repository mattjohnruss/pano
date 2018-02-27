#pragma once

#include <renderer.h>

#include <deque>

class SimpleRenderer : public Renderer
{
public:
    virtual ~SimpleRenderer();

    void submit(const Renderable2D *const renderable);
    void flush();

private:
    std::deque<Renderable2D const*> render_queue_;
};
