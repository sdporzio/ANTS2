#ifndef NNMODULECLASS_H
#define NNMODULECLASS_H

#include "flann/flann.hpp"

#include <QVector>
#include <QJsonObject>
#include <QObject>
#include <QPair>

class EventsDataClass;
class APmHub;

class KNNfilterClass
{
public:
  KNNfilterClass();
  ~KNNfilterClass();

  void configure(EventsDataClass *EventsDataHub, APmHub* PMs) {this->EventsDataHub = EventsDataHub, this->PMs = PMs;}

  void clear(); //deletes/clears data and forces next filter call to recalculate data
  bool prepareNNfilter(int KNNfilterAverageOver); //MUST be used after events data changed
  float getAverageDist(int iev) {return averageDistNN[iev];} //assumes prepareNNfilter was already called

  int getLastSearchN() {return LastSearchN;} //-1 if data not ready
  int getLastAveragedN() {return LastAveragedN;} //-1 if data not ready

private:
  EventsDataClass *EventsDataHub;
  APmHub* PMs;

  flann::Matrix<int> *indices;
  flann::Matrix<float> *dists;
  flann::Matrix<float> *dataset;

  QVector<float> averageDistNN;

  int LastSearchN;   //-1 if not ready
  int LastAveragedN; //-1 if not ready
  int numEvents;
  int numPMs;

  bool Init();
  void kNNsearch(int n); //n = neighbours+1
  bool calculateAverageDists(int n); //n = neighbours+1
};

class KNNreconstructorClass : public QObject
{
  Q_OBJECT
public:
  KNNreconstructorClass();
  ~KNNreconstructorClass();

  void configure(EventsDataClass *EventsDataHub, APmHub* PMs) {this->EventsDataHub = EventsDataHub, this->PMs = PMs;}
  void setNumNeighbours(int num) {numNeighbours = num;}
  void setNumTrees(int num) {numTrees = num;}
  void setWeightMode(int mode) {weightMode = mode;}
  void clear();

  bool calibrate(char option); // 'x' or 'y' or 'b' - the last uses the same events dataset for X and Y (compact)
  bool reconstructEvents();

  bool isXready();
  bool isYready();
  bool isXYready();
  int countEventsXset() {return fTrainedXYwithSameEventSet ? 0 : Xpositions.size();}
  int countEventsYset() {return Ypositions.size();}
  int countEventsXYset() {return fTrainedXYwithSameEventSet ? Xpositions.size(): 0;}

  bool readFromJson(QJsonObject &json);

  QString ErrorString;

signals:
  void readyXchanged(bool ready, int events);
  void readyYchanged(bool ready, int events);

private:
  EventsDataClass *EventsDataHub;
  APmHub* PMs;
  int numNeighbours; //search for numNeighbours in calibration set and use their coordinates
  int useNeighbours; //number of neighbours used to reconstruct position. Cannot be larger than numNeighbours!
  int numTrees; //number of trees used in flann index
  int weightMode; //0-no weighting, 1- inverse dist, 2- inverse square dist

  bool fTrainedXYwithSameEventSet;
  flann::Matrix<float> *Xdataset, *Ydataset; //training events

  flann::Index<flann::L1<float> > *Xindex, *Yindex; //flann index data to be used in position reconstruction

  QVector<float> Xpositions, Ypositions; //knows positions of events used to construct Xindex and Yindex

  flann::Index<flann::L1<float> >* buildFlannIndex(flann::Matrix<float> *dataset);
  bool prepareData(char option, flann::Matrix<float> **dataset); // 'x' or 'y' or 'b' (both X and Y)
  void reconstructPositions(int icoord, flann::Matrix<int> *indices, flann::Matrix<float> *dists);
};

class AScriptInterfacer
{
public:
   AScriptInterfacer(EventsDataClass *EventsDataHub, APmHub* PMs);

   QVariant getNeighbours(int ievent, int numNeighbours);
   QVariant getNeighboursDirect(const QVector<float>& point, int numNeighbours);
   bool filterByDistance(int numNeighbours, float maxDistance, bool filterOutEventsWithSmallerDistance);

   void SetSignalNormalization(int type) {NormSwitch = type;}

   void clearCalibration();
   bool setCalibration(bool bUseScan);
   bool setCalibrationDirect(const QVector<QVector<float> >& data);

   int countCalibrationEvents() {return numCalibrationEvents;}

   QVector<float> evaluatePhPerPhE(int numNeighbours, float upperDistanceLimit, float maxSignal);
   int countPMs() const {return numPMs;}

   double getCalibrationEventX(int ievent);
   double getCalibrationEventY(int ievent);
   double getCalibrationEventZ(int ievent);
   double getCalibrationEventE(int ievent);
   QVariant getCalibrationEventSignals(int ievent);  //if norm is active, the signal values are normalized!

   QString ErrorString;

private:
   EventsDataClass* EventsDataHub;
   APmHub* PMs;

   bool bCalibrationReady;
   int numCalibrationEvents;
   int numPMs;

   int NormSwitch;  // 0 - no norm, 1 - sum signal, 2 - quadrature sum

   flann::Matrix<float>* CalibrationEvents;       //calibration events - signals
   QVector<float> X, Y, Z, E;                     //calibration events - positions and energy

   flann::Index<flann::L1<float> > *FlannIndex;   //flann index data

   bool isValidEventIndex(int ievent);

   float calculateNorm(const QVector<float>& data) const;
   const QVector<QPair<int, float> > neighbours(const QVector<float> &point, int numNeighbours);
};

class NNmoduleClass
{  
public:
  NNmoduleClass(EventsDataClass *EventsDataHub, APmHub* PMs);
  ~NNmoduleClass();

  KNNfilterClass Filter;
  KNNreconstructorClass Reconstructor;
  AScriptInterfacer* ScriptInterfacer;

private:
  EventsDataClass *EventsDataHub;
  APmHub* PMs;
};

#endif // NNMODULECLASS_H
