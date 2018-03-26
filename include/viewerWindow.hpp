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
        ////////////////////////////////////////////////////////////////////////
        /// Constructor
        ///
        /// \param slice_pos_in The locations of each slice in real space
        /// \param arrowOn Are spin arros on?
        /// \param allArrow Are all spin arrows on?
        /// \param slices Which slices of spin arrows should be shown?
        /// \param isoOn Are isosurfaces on?
        /// \param isoVal What is the value of the isosurfaces?
        /// \param parent The parent of this object.
        ////////////////////////////////////////////////////////////////////////
        viewerWindow(std::vector<double> slice_pos_in,
            const bool arrowOn,
            const bool allArrow,
            const std::vector<int> slices,
            const bool isoOn,
            const double isoVal,
            QWidget *parent=nullptr);
        ////////////////////////////////////////////////////////////////////////
        /// Destructor
        ////////////////////////////////////////////////////////////////////////
        virtual ~viewerWindow();
        ////////////////////////////////////////////////////////////////////////
        /// Update the vector field data
        ///
        /// \param geometry The positions of the sites
        /// \param vectors The vectors associated with the geometry
        ////////////////////////////////////////////////////////////////////////
        void update(const VFRendering::Geometry& geometry,
            const std::vector<glm::vec3>& vectors);
        ////////////////////////////////////////////////////////////////////////
        /// Update the vector field data
        ///
        /// \param vectors The vectors associated with the geometry
        ////////////////////////////////////////////////////////////////////////
        void updateVectors(const std::vector<glm::vec3>& vectors);
        ////////////////////////////////////////////////////////////////////////
        /// Update the viewer options
        ///
        /// \param options The viewer options
        ////////////////////////////////////////////////////////////////////////
        void updateOptions(const VFRendering::Options& options);
        ////////////////////////////////////////////////////////////////////////
        /// Setup the renderers based on input from options window
        ///
        /// \param arrowOn Are spin arros on?
        /// \param allArrow Are all spin arrows on?
        /// \param slices Which slices of spin arrows should be shown?
        /// \param isoOn Are isosurfaces on?
        /// \param isoVal What is the value of the isosurfaces?
        ////////////////////////////////////////////////////////////////////////
        void set_renderers(const bool arrowOn,
            const bool allArrow,
            const std::vector<int> slices,
            const bool isoOn,
            const double isoVal);
        ////////////////////////////////////////////////////////////////////////
        /// Get the framerate of the window
        ////////////////////////////////////////////////////////////////////////
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
