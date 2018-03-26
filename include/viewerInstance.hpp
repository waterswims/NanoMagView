#ifndef _VINST
#define _VINST

#include "renderOptions.hpp"
#include "viewerWindow.hpp"

#include <string>

namespace nMagWindows
{
    class viewerInstance: public QObject
    {
        Q_OBJECT
    public:
        ////////////////////////////////////////////////////////////////////////
        /// Constructor
        ///
        /// \param h5NameIn The name of the chosen HDF5 file
        /// \param TInd The index of the chosen temperature.
        /// \param HInd The index of the chosen field.
        /// \param parent The parent QT object.
        ////////////////////////////////////////////////////////////////////////
        viewerInstance(std::string h5Name, int TInd,
            int HInd, QObject *parent = Q_NULLPTR);

    private:
        std::vector<glm::vec3> directions;
        VFRendering::Geometry geometry;
        std::vector<double> z_pos;
        viewerWindow *vWind;
        renderOptionsWindow *oWind;
        ////////////////////////////////////////////////////////////////////////
        /// Creates a new viewer window within this instance
        ///
        /// \param arrowOn Are spin arros on?
        /// \param allArrow Are all spin arrows on?
        /// \param slices Which slices of spin arrows should be shown?
        /// \param isoOn Are isosurfaces on?
        /// \param isoVal What is the value of the isosurfaces?
        ////////////////////////////////////////////////////////////////////////
        void newView(const bool arrowOn,
            const bool allArrow,
            const std::vector<int> slices,
            const bool isoOn,
            const double isoVal);

    private slots:
        ////////////////////////////////////////////////////////////////////////
        /// Slot which creates a new viewer window within this instance (calls
        /// newView)
        ///
        /// \param arrowOn Are spin arros on?
        /// \param allArrow Are all spin arrows on?
        /// \param slices Which slices of spin arrows should be shown?
        /// \param isoOn Are isosurfaces on?
        /// \param isoVal What is the value of the isosurfaces?
        ////////////////////////////////////////////////////////////////////////
        void recvUpdate(bool arrowOn,
            bool allArrow,
            std::vector<int> slices,
            bool isoOn,
            double isoVal);
    };
}

#endif
