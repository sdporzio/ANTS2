#ifndef AOPTICALOVERRIDETESTER_H
#define AOPTICALOVERRIDETESTER_H

#include "atracerstateful.h"

#include <QMainWindow>

namespace Ui {
class AOpticalOverrideTester;
}

class AMaterialParticleCollection;
class AOpticalOverride;
class GraphWindowClass;
class GeometryWindowClass;
class TRandom2;

class AOpticalOverrideTester : public QMainWindow
{
    Q_OBJECT

public:
    explicit AOpticalOverrideTester(AOpticalOverride ** ovLocal, GraphWindowClass *GraphWindow, GeometryWindowClass* GeometryWindow, AMaterialParticleCollection *MPcollection, int matFrom, int matTo, QWidget *parent = 0);
    ~AOpticalOverrideTester();

private slots:
    void on_pbDirectionHelp_clicked();

    void on_pbST_RvsAngle_clicked();

    void on_pbCSMtestmany_clicked();

    void on_pbST_showTracks_clicked();

    void on_pbST_uniform_clicked();

    void on_pbST_AngleCos_clicked();

private:
    Ui::AOpticalOverrideTester *ui;
    AMaterialParticleCollection* MPcollection;
    int MatFrom;
    int MatTo;
    AOpticalOverride ** pOV;
    GraphWindowClass* GraphWindow;
    GeometryWindowClass* GeometryWindow;
    TRandom2* RandGen;
    ATracerStateful* Resources;

    bool testOverride();
};

#endif // AOPTICALOVERRIDETESTER_H
