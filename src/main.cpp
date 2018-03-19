#include <QApplication>
#include <QPushButton>
#include <QFileDialog>
#include <VFRendering/View.hxx>

int main(int argc, char **argv)
{
    // Start Application
    QApplication app (argc, argv);

    // New Window inside the Application
    QWidget window;
    window.setFixedSize(500, 250);

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

    // // VF Geometry
    // auto geometry = VFRendering::Geometry::cartesianGeometry(
    //     {30, 30, 30},
    //     {-1.0, -1.0, -1.0},
    //     {1.0, 1.0, 1.0}
    // );
    // std::vector<glm::vec3> directions;
    // for (int iz = 0; iz < 10; iz++) {
    //     for (int iy = 0; iy < 10; iy++) {
    //         for (int ix = 0; ix < 10; ix++) {
    //             // calculate direction for ix, iy, iz
    //             directions.push_back(glm::normalize(glm::vec3(ix-4.5, iy-4.5, iz-4.5)));
    //         }
    //     }
    // }
    //
    // // Create a VF View
    // VFRendering::View view;
    // view.update(geometry, directions);
    // view.draw();

    // Show the window
    window.show();

    // Return the Application
    return app.exec();
}
