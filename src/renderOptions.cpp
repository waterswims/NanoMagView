#include "../include/renderOptions.hpp"

#include <sstream>

nMagWindows::renderOptionsWindow::renderOptionsWindow()
{
    // Set the size of the window
    this->setFixedSize(250, 230);

    // Create the tick boxes
    arrowCheck = new QCheckBox("Show Spin Arrows", this);
    arrowCheck->setGeometry(10, 10, 180, 30);
    arrowCheck->setChecked(true);

    allCheck = new QCheckBox("Show All Slices", this);
    allCheck->setGeometry(10, 40, 180, 30);
    allCheck->setChecked(true);

    isoCheck = new QCheckBox("Show Iso Surfaces", this);
    isoCheck->setGeometry(10, 100, 180, 30);
    isoCheck->setChecked(true);

    // Create Text Boxes
    sliceChoiceText = new QLineEdit("Slices e.g. '0, 20, 39'", this);
    sliceChoiceText->setGeometry(10, 70, 230, 30);

    isoValText = new QLineEdit("Isosurface Value", this);
    isoValText->setGeometry(10, 130, 230, 30);

    // Create Update Box
    updateButton = new QPushButton("Update", this);
    updateButton->setGeometry(50, 170, 150, 50);
    QObject::connect(updateButton, SIGNAL (clicked()), this,
        SLOT (updateHit()));
}

void nMagWindows::renderOptionsWindow::updateHit()
{
    bool arrowBool = arrowCheck->isChecked();
    bool isoBool = isoCheck->isChecked();
    bool allBool = allCheck->isChecked();

    std::vector<int> slices;
    if(arrowBool && !allBool)
    {
        QString sliceString = sliceChoiceText->text();
        std::string stdSliceString = sliceString.toLocal8Bit().constData();
        std::istringstream stream(stdSliceString);
        std::string subString;
        while (getline(stream, subString, ','))
        {
            slices.push_back(stoi(subString));
        }
    }

    double isoVal=0;
    if(isoBool)
    {
        QString isoString = isoValText->text();
        isoVal = isoString.toDouble();
    }

    emit sendUpdate(arrowBool, allBool, slices, isoBool, isoVal);
}
