#include <QApplication>
#include <QFileDialog>
#include <iostream>
#include "../include/h5Input.hpp"
#include "../include/selectLatt.hpp"

int main(int argc, char **argv)
{
    // Start Application
    QApplication app (argc, argv);

    // File Dialog
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::ExistingFile);
    openDialog.setViewMode(QFileDialog::Detail);
    openDialog.setNameFilter("HDF5 Files (*.h5)");

    // Get File Names from dialog
    nMagWindows::selectTHWindow *sWindow;

    if(openDialog.exec())
    {
        QStringList fileNames = openDialog.selectedFiles();

        std::string fileName = fileNames.at(0).toLocal8Bit().constData();

        std::cout << fileName << std::endl;

        // Create the slider window
        sWindow = new nMagWindows::selectTHWindow(fileName);
        sWindow->show();
    }

    // Return the Application
    return app.exec();
}
