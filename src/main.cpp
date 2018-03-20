#include <QApplication>
#include <QPushButton>
#include <QFileDialog>
#include "../include/VFRenderingWidget.hpp"

int main(int argc, char **argv)
{
    // Start Application
    QApplication app (argc, argv);

    // New Window inside the Application
    QWidget window;
    window.setFixedSize(1000, 1000);

    // New Button inside the window
    QPushButton *closeButton = new QPushButton("Exit", &window);
    closeButton->setGeometry(175, 50, 150, 50);

    // Connect the button to the quit of the application
    QObject::connect(closeButton, SIGNAL (clicked()), QApplication::instance(),
        SLOT (quit()));

    // Open button
    QPushButton *openButton = new QPushButton("Open", &window);
    openButton->setGeometry(175, 150, 150, 50);

    // File Dialog
    QFileDialog openDialog(&window);
    openDialog.setFileMode(QFileDialog::ExistingFile);
    openDialog.setViewMode(QFileDialog::Detail);
    openDialog.setNameFilter("HDF5 Files (*.h5)");

    // Connect File Dialog to Button
    QObject::connect(openButton, SIGNAL (clicked()), &openDialog, SLOT(exec()));

    // Get File Names from dialog
    QStringList fileNames;
    fileNames = openDialog.selectedFiles();

    // Create a Surface for the rendering
    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    VFRenderingWidget window2;

    // Generate the vector data
    std::vector<glm::vec3> directions;
    VFRendering::Geometry geometry = VFRendering::Geometry::cartesianGeometry({21, 21, 21}, {-20, -20, -20}, {20, 20, 20});
    for (const auto& position : geometry.positions()) {
        directions.push_back(glm::normalize(position));
    }
    window2.update(geometry, directions);

    // Setup Camera
    VFRendering::Options options;
    options.set<VFRendering::View::Option::CAMERA_POSITION>({0, 0, 30});
    options.set<VFRendering::View::Option::CENTER_POSITION>({0, 0, 0});
    options.set<VFRendering::View::Option::UP_VECTOR>({0, 1, 0});
    options.set<VFRendering::View::Option::SYSTEM_CENTER>((geometry.min() + geometry.max()) * 0.5f);
    options.set<VFRendering::View::Option::COLORMAP_IMPLEMENTATION>(VFRendering::Utilities::getColormapImplementation(VFRendering::Utilities::Colormap::HSV));
    window2.updateOptions(options);

    // Show the surface
    window2.resize(1000, 1000);
    window2.setWindowTitle("VFRenderingWidgetExample");
    window2.show();

    // Show the window
    window.show();

    // Return the Application
    return app.exec();
}
