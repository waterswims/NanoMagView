#include "../include/viewerInstance.hpp"
#include "../include/h5Input.hpp"

nMagWindows::viewerInstance::viewerInstance(std::string h5Name, int TInd,
    int HInd, QObject *parent) : QObject(parent)
{
    // Extract the vector data
    h5Input::getSpins(h5Name, TInd, HInd, geometry, directions, z_pos);

    // Create an options window
    oWind = new nMagWindows::renderOptionsWindow();
    oWind->show();

    // Create viewer window
    this->newView(true, true, {0}, true, 0);

    QObject::connect(oWind,
        SIGNAL (sendUpdate(bool, bool, std::vector<int>, bool, double)),
        this,
        SLOT (recvUpdate(bool, bool, std::vector<int>, bool, double)));
}

void nMagWindows::viewerInstance::newView(const bool arrowOn,
    const bool allArrow,
    const std::vector<int> slices,
    const bool isoOn,
    const double isoVal)
{
    // Create a Surface for the rendering
    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    vWind = new viewerWindow(z_pos, arrowOn, allArrow, slices, isoOn, isoVal);
    vWind->update(geometry, directions);

    // Setup Camera
    VFRendering::Options options;
    options.set<VFRendering::View::Option::CAMERA_POSITION>({-50, -50, 50});
    options.set<VFRendering::View::Option::CENTER_POSITION>({0, 0, 0});
    options.set<VFRendering::View::Option::UP_VECTOR>({0, 0, 1});
    options.set<VFRendering::View::Option::SYSTEM_CENTER>((geometry.min() + geometry.max()) * 0.5f);
    options.set<VFRendering::View::Option::COLORMAP_IMPLEMENTATION>(VFRendering::Utilities::getColormapImplementation(VFRendering::Utilities::Colormap::HSV));
    vWind->updateOptions(options);

    // Show the surface
    vWind->resize(1000, 1000);
    vWind->setWindowTitle("VFRenderingWidgetExample");
    vWind->show();
}

void nMagWindows::viewerInstance::recvUpdate(bool arrowOn,
    bool allArrow,
    std::vector<int> slices,
    bool isoOn,
    double isoVal)
{
    vWind->close();
    this->newView(arrowOn, allArrow, slices, isoOn, isoVal);
}
