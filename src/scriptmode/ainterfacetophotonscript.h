#ifndef AINTERFACETOPHOTONSCRIPT_H
#define AINTERFACETOPHOTONSCRIPT_H

#include "scriptinterfaces.h"

#include <QVector>

class AConfiguration;
class EventsDataClass;
class DetectorClass;
class APhotonTracer;
class TrackHolderClass;
class OneEventClass;
class TH1;

class AInterfaceToPhotonScript : public AScriptInterface
{
  Q_OBJECT
public:
    AInterfaceToPhotonScript (AConfiguration* Config, EventsDataClass *EventsDataHub);
    ~AInterfaceToPhotonScript();

public slots:
    void ClearData();
    bool TracePhotons(int copies, double x, double y, double z, double vx, double vy, double vz, int iWave, double time);

    void SetBuildTracks(bool flag) {bBuildTracks = flag;}
    void SetTrackColor(int color) {TrackColor = color;}
    void SetTrackWidth(int width) {TrackWidth = width;}
    void SetMaxNumberTracks(int maxNumber) {MaxNumberTracks = maxNumber;}

    //photon loss statistics
    long GetBulkAbsorbed() const;  //Absorbed and BulkAbosrbed are the same
    long GetOverrideLoss() const;
    long GetHitPM() const;
    long GetHitDummy() const;
    long GetEscaped() const;
    long GetLossOnGrid() const;
    long GetTracingSkipped() const;
    long GetMaxCyclesReached() const;
    long GetGeneratedOutsideGeometry() const;
    //optical processes
    long GetFresnelTransmitted() const;
    long GetFresnelReflected() const;
    long GetRayleigh() const;
    long GetReemitted() const;

private:
    AConfiguration* Config;
    EventsDataClass *EventsDataHub;
    DetectorClass* Detector;

    APhotonTracer* Tracer;

    bool bBuildTracks = false;
    int TrackColor = 7;
    int TrackWidth = 1;
    int MaxNumberTracks = 1000;
    QVector<TrackHolderClass *> Tracks;
    OneEventClass* Event;

    void clearTrackHolder();
    void normalizeVector(double *arr);
};

#endif // AINTERFACETOPHOTONSCRIPT_H
