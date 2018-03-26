#include "../include/selectLatt.hpp"
#include "../include/h5Input.hpp"
#include "../include/viewerInstance.hpp"

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
    viewerInstance *vInst = new viewerInstance(h5Name, currTInd, currHInd);

    this->close();
}
