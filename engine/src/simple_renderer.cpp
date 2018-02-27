#include <simple_renderer.h>
#include <renderable2d.h>

SimpleRenderer::~SimpleRenderer()
{
}

void SimpleRenderer::submit(const Renderable2D *const renderable)
{
    render_queue_.push_back(renderable);
}

void SimpleRenderer::flush()
{
    while(!render_queue_.empty())
    {
        //render_queue_.front()->draw();
        render_queue_.pop_front();
    }
}
