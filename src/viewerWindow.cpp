#include "../include/viewerWindow.hpp"

#include <vector>
#include <QWheelEvent>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <VFRendering/IsosurfaceRenderer.hxx>
#include <VFRendering/ArrowRenderer.hxx>
#include <VFRendering/CombinedRenderer.hxx>

nMagWindows::viewerWindow::viewerWindow(std::vector<double> slice_pos_in,
    QWidget *parent) : QOpenGLWidget(parent)
{
    slice_pos = slice_pos_in;
    this->set_renderers(true, false, {0, 20, 39}, true, 0);
}

nMagWindows::viewerWindow::~viewerWindow()
{

}

void nMagWindows::viewerWindow::initializeGL()
{
    setMouseTracking(true);
}

void nMagWindows::viewerWindow::resizeGL(int width, int height)
{
    arrow_view.setFramebufferSize(width, height);
}

void nMagWindows::viewerWindow::update(const VFRendering::Geometry& geometry,
    const std::vector<glm::vec3>& vectors)
{
    arrow_view.update(geometry, vectors);
}


void nMagWindows::viewerWindow::updateVectors(
    const std::vector<glm::vec3>& vectors)
{
    arrow_view.updateVectors(vectors);
}

void nMagWindows::viewerWindow::updateOptions(
    const VFRendering::Options& options)
{
    arrow_view.updateOptions(options);
}

void nMagWindows::viewerWindow::paintGL()
{
    arrow_view.draw();
}

float nMagWindows::viewerWindow::getFramerate() const
{
    return arrow_view.getFramerate();
}

void nMagWindows::viewerWindow::wheelEvent(QWheelEvent *event)
{
    float delta = event->angleDelta().y()*0.1;
    arrow_view.mouseScroll(delta);
    ((QWidget *)this)->update();
}

void nMagWindows::viewerWindow::mousePressEvent(QMouseEvent *event)
{
    m_previous_mouse_position = event->pos();
}

void nMagWindows::viewerWindow::mouseMoveEvent(QMouseEvent *event)
{
    glm::vec2 current_mouse_position(event->pos().x(), event->pos().y());
    glm::vec2 previous_mouse_position(m_previous_mouse_position.x(),
        m_previous_mouse_position.y());
    m_previous_mouse_position = event->pos();

    if (event->buttons() & Qt::LeftButton) {
    auto movement_mode = VFRendering::CameraMovementModes::ROTATE;
    if (event->modifiers() & Qt::AltModifier) {
      movement_mode = VFRendering::CameraMovementModes::TRANSLATE;
    }
    arrow_view.mouseMove(previous_mouse_position, current_mouse_position,
        movement_mode);
    ((QWidget *)this)->update();
    }
}

void nMagWindows::viewerWindow::set_renderers(const bool arrowOn,
    const bool allArrow,
    const std::vector<int> slices,
    const bool isoOn,
    const double isoVal)
{
    auto arrow_renderer_ptr = std::make_shared<VFRendering::ArrowRenderer>(arrow_view);
    auto isosurface_renderer_ptr = std::make_shared<VFRendering::IsosurfaceRenderer>(arrow_view);
    isosurface_renderer_ptr->setOption<VFRendering::IsosurfaceRenderer::Option::VALUE_FUNCTION>([] (const glm::vec3& position, const glm::vec3& direction) -> VFRendering::IsosurfaceRenderer::isovalue_type {
        (void)position;
        return direction.z;
    });
    isosurface_renderer_ptr->setOption<VFRendering::IsosurfaceRenderer::Option::ISOVALUE>(isoVal);
    std::vector<std::shared_ptr<VFRendering::RendererBase>> renderers = {arrow_renderer_ptr, isosurface_renderer_ptr};
    arrow_view.renderers({{std::make_shared<VFRendering::CombinedRenderer>(arrow_view, renderers), {{0, 0, 1, 1}}}});
    if(arrowOn)
    {
        if(!allArrow)
        {
            double sliceBuff = (slice_pos[1] - slice_pos[0]) / 2;
            std::stringstream funcStream;
            funcStream << "bool is_visible(vec3 position, vec3 direction) {bool isVis = false;";

            for (unsigned int i = 0; i < slices.size(); i++)
            {
                funcStream << "if (position.z > ";
                funcStream << slice_pos[slices[i]] - sliceBuff;
                funcStream << " && position.z < ";
                funcStream << slice_pos[slices[i]] + sliceBuff;
                funcStream << "){isVis = true;}";
            }

            funcStream << "return isVis;}";

            std::string funcString;
            funcString = funcStream.str();
            arrow_renderer_ptr->setOption<VFRendering::View::Option::IS_VISIBLE_IMPLEMENTATION>(funcString);
        }
    }
    else
    {
        arrow_renderer_ptr->setOption<VFRendering::View::Option::IS_VISIBLE_IMPLEMENTATION>("bool is_visible(vec3 position, vec3 direction) { return false; }");
    }

    if(!isoOn)
    {
        isosurface_renderer_ptr->setOption<VFRendering::View::Option::IS_VISIBLE_IMPLEMENTATION>("bool is_visible(vec3 position, vec3 direction) { return false; }");
    }
}
