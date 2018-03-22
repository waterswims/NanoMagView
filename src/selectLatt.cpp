#include "../include/selectLatt.hpp"
#include "../include/h5Input.hpp"
#include "../include/VFRenderingWidget.hpp"

nMagWindows::selectTHWindow::selectTHWindow(std::string h5NameIn)
{
    h5Name = h5NameIn;

    // Set the size of the window
    this->setFixedSize(250, 140);

    // Get the fields and temps
    h5Input::getFieldTemps(h5Name, Ts, Hs);

    // Create the sliders
    tempSlider = new QSlider(this);
    tempSlider->setOrientation(Qt::Horizontal);
    tempSlider->setRange(0, Ts.size()-1);
    tempSlider->setValue(0);
    tempSlider->setGeometry(10, 15, 180, 30);

    fieldSlider = new QSlider(this);
    fieldSlider->setOrientation(Qt::Horizontal);
    fieldSlider->setRange(0, Hs.size()-1);
    fieldSlider->setValue(0);
    fieldSlider->setGeometry(10, 45, 180, 30);

    // Create the Labels
    currTInd = 0;
    tempLabel = new QLabel(this);
    tempLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    tempLabel->setText(QString::number(Ts[0]));
    tempLabel->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    tempLabel->setGeometry(200, 10, 40, 25);

    currHInd = 0;
    fieldLabel = new QLabel(this);
    fieldLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    fieldLabel->setText(QString::number(Hs[0]));
    fieldLabel->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    fieldLabel->setGeometry(200, 40, 40, 25);

    // Connect the sliders to the labels
    QObject::connect(tempSlider, SIGNAL (valueChanged(int)),
        this, SLOT (changeTempLabel(int)));
    QObject::connect(fieldSlider, SIGNAL (valueChanged(int)),
        this, SLOT (changeFieldLabel(int)));

    // Create the button
    openButton = new QPushButton("View Lattice", this);
    openButton->setGeometry(50, 80, 150, 50);

    // Connect the button
    QObject::connect(openButton, SIGNAL (clicked()), this, SLOT (openViewer()));
}

void nMagWindows::selectTHWindow::changeTempLabel(int tInd)
{
    currTInd = tInd;
    tempLabel->setText(QString::number(Ts[tInd]));
}

void nMagWindows::selectTHWindow::changeFieldLabel(int hInd)
{
    currHInd = hInd;
    fieldLabel->setText(QString::number(Hs[hInd]));
}

void nMagWindows::selectTHWindow::openViewer()
{
    // Create a Surface for the rendering
    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    VFRenderingWidget *viewWindow = new VFRenderingWidget;

    // Extract the vector data
    std::vector<glm::vec3> directions;
    VFRendering::Geometry geometry;
    h5Input::getSpins(h5Name, currTInd, currHInd, geometry, directions);
    viewWindow->update(geometry, directions);

    // Setup Camera
    VFRendering::Options options;
    options.set<VFRendering::View::Option::CAMERA_POSITION>({0, 0, 30});
    options.set<VFRendering::View::Option::CENTER_POSITION>({0, 0, 0});
    options.set<VFRendering::View::Option::UP_VECTOR>({0, 1, 0});
    options.set<VFRendering::View::Option::SYSTEM_CENTER>((geometry.min() + geometry.max()) * 0.5f);
    options.set<VFRendering::View::Option::COLORMAP_IMPLEMENTATION>(VFRendering::Utilities::getColormapImplementation(VFRendering::Utilities::Colormap::HSV));
    viewWindow->updateOptions(options);

    // Show the surface
    viewWindow->resize(1000, 1000);
    viewWindow->setWindowTitle("VFRenderingWidgetExample");
    viewWindow->show();

    this->close();
}
