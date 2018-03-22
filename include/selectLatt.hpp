#ifndef _SLATT
#define _SLATT

#include <string>
#include <vector>
#include <QPushButton>
#include <QLabel>
#include <QSlider>

namespace nMagWindows
{
    class selectTHWindow: public QWidget
    {
        Q_OBJECT
    public:
        ////////////////////////////////////////////////////////////////////////
        /// Constructor
        ///
        /// /param h5NameIn The name of the chosen HDF5 file
        ////////////////////////////////////////////////////////////////////////
        selectTHWindow(std::string h5NameIn);

    private:
        std::string h5Name;
        int currTInd, currHInd;
        std::vector<float> Ts, Hs;
        QSlider *tempSlider;
        QSlider *fieldSlider;
        QPushButton *openButton;
        QLabel *tempLabel;
        QLabel *fieldLabel;

    private slots:
        ////////////////////////////////////////////////////////////////////////
        /// Slot which changes the temperature label
        ///
        /// /param tInd The index of the temperature in Ts
        ////////////////////////////////////////////////////////////////////////
        void changeTempLabel(int tInd);
        ////////////////////////////////////////////////////////////////////////
        /// Slot which changes the field label
        ///
        /// /param tInd The index of the field in Hs
        ////////////////////////////////////////////////////////////////////////
        void changeFieldLabel(int hInd);
        ////////////////////////////////////////////////////////////////////////
        /// Slot which opens the Lattice Viewer
        ////////////////////////////////////////////////////////////////////////
        void openViewer();
    };
}

#endif
