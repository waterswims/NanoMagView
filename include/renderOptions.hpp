#ifndef _ROPTS
#define _ROPTS

#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <vector>

namespace nMagWindows
{
    class renderOptionsWindow: public QWidget
    {
        Q_OBJECT
    public:
        ////////////////////////////////////////////////////////////////////////
        /// Constructor
        ////////////////////////////////////////////////////////////////////////
        renderOptionsWindow();

    private:
        QLineEdit *sliceChoiceText;
        QLineEdit *isoValText;
        QCheckBox *arrowCheck;
        QCheckBox *allCheck;
        QCheckBox *isoCheck;
        QPushButton *updateButton;

    signals:
        ////////////////////////////////////////////////////////////////////////
        /// Sends the chosen slices and iso value
        ///
        /// \param arrOn Arrow switch
        /// \param allOn Turn on all arrows
        /// \param slices Slices for the arrow
        /// \param isoOn Isosurface switch
        /// \param isoVal Value at which isosurface is drawn
        ////////////////////////////////////////////////////////////////////////
        void sendUpdate(bool arrOn, bool allOn, std::vector<int> slices,
            bool isoOn, double isoVal);

    private slots:
        ////////////////////////////////////////////////////////////////////////
        /// Triggers sendUpdate signal
        ////////////////////////////////////////////////////////////////////////
        void updateHit();
    };
}

#endif
