#ifndef GLWIDGET_H_INCLUDED
#define GLWIDGET_H_INCLUDED

#include <memory>
#include <vector>
#include <QWidget>
#include <QOpenGLWidget>

#include <glm/vec3.hpp>
#include <VFRendering/View.hxx>
#include <VFRendering/Geometry.hxx>

namespace VFRendering {
  class View;
}

namespace nMagWindows
{
    class viewerWindow : public QOpenGLWidget {
    Q_OBJECT

    public:
        viewerWindow(std::vector<double> slice_pos_in,
            const bool arrowOn,
            const bool allArrow,
            const std::vector<int> slices,
            const bool isoOn,
            const double isoVal,
            QWidget *parent=nullptr);
        virtual ~viewerWindow();
        void update(const VFRendering::Geometry& geometry,
            const std::vector<glm::vec3>& vectors);
        void updateVectors(const std::vector<glm::vec3>& vectors);
        void updateOptions(const VFRendering::Options& options);
        void set_renderers(const bool arrowOn,
            const bool allArrow,
            const std::vector<int> slices,
            const bool isoOn,
            const double isoVal);
        float getFramerate() const;

    protected:
        void initializeGL() override;
        void resizeGL(int width, int height) override;
        void paintGL() override;
        void wheelEvent(QWheelEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;

    private:
        std::vector<double> slice_pos;
        VFRendering::View arrow_view;
        QPoint m_previous_mouse_position;
    };
}

#endif
