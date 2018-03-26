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
        viewerInstance(std::string h5Name, int TInd,
            int HInd, QObject *parent = Q_NULLPTR);

    private:
        std::vector<glm::vec3> directions;
        VFRendering::Geometry geometry;
        std::vector<double> z_pos;
        viewerWindow *vWind;
        renderOptionsWindow *oWind;
        void newView(const bool arrowOn,
            const bool allArrow,
            const std::vector<int> slices,
            const bool isoOn,
            const double isoVal);

    private slots:
        void recvUpdate(bool arrowOn,
            bool allArrow,
            std::vector<int> slices,
            bool isoOn,
            double isoVal);
    };
}

#endif
