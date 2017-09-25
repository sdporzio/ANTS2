#ifndef INTERFACETOCHECKERSCRIPT_H
#define INTERFACETOCHECKERSCRIPT_H

#include "scriptinterfaces.h"
#include "ahistoryrecords.h"

#include <QVector>

class MainWindow;
class EventsDataClass;
struct AEnergyDepositionCell;

struct DepoNode
{
  double R[3];                  // x,y,z -coordinates of the cell
  double CellLength;            // cell length for charged particles
  double Energy;                // energy deposited in this cell
};

class MaterialRecord
{
public:
  int MatId;                    // material Id
  double StartPosition[3];      // position at which particle entered this material
  QVector<DepoNode> ByMaterial; // detailed depo info in this material
};

class ParticleRecord
{
public:
  EventHistoryStructure* History;     // pointers to external EventHistory, does not own them!
  QVector<MaterialRecord> Deposition; // extracted data per material - from EnergyVector
};

class InterfaceToInteractionScript : public AScriptInterface
{
  Q_OBJECT
public:
  InterfaceToInteractionScript(MainWindow* MW, EventsDataClass* EventsDataHub);
  ~InterfaceToInteractionScript(){ClearExtractedData();}

private: 
  MainWindow* MW;
  EventsDataClass* EventsDataHub;
  QVector<ParticleRecord> PR;

public slots:
  void ClearStack();
  void AddParticleToStack(int particleID, double X, double Y, double Z, double dX, double dY, double dZ, double time, double Energy, int numCopies);
  void TrackStack();  //run simulationwith particles in the stack
  void ClearExtractedData();

  int count() {return PR.length();} // total number of records

  int termination(int i);
  //1 - escaped
  //2 - all energy dissipated
  //3 - photoeffect
  //4 - compton
  //5 - capture
  //6 - error - undefined termination
  //7 - created outside defined geometry
  //8 - found untrackable material
  QString terminationStr(int i);
  double dX(int i);
  double dY(int i);
  double dZ(int i);
  int particleId(int i);
  int sernum(int i);
  int isSecondary(int i);

  // MaterialCrossed - info from DepositionHistory
  int MaterialsCrossed_count(int i);
  int MaterialsCrossed_matId(int i, int m);
  int MaterialsCrossed_energy(int i, int m);
  int MaterialsCrossed_distance(int i, int m);

  // Deposition - info from EnergyVector
  int Deposition_countMaterials(int i);
  int Deposition_matId(int i, int m);
  double Deposition_startX(int i, int m);
  double Deposition_startY(int i, int m);
  double Deposition_startZ(int i, int m);
  QString Deposition_volumeName(int i, int m);
  int Deposition_volumeIndex(int i, int m);
  double Deposition_energy(int i, int m);
    // +subindex - node index
  int Deposition_countNodes(int i, int m);
  double Deposition_X(int i, int m, int n);
  double Deposition_Y(int i, int m, int n);
  double Deposition_Z(int i, int m, int n);
  double Deposition_dE(int i, int m, int n);
  double Deposition_dL(int i, int m, int n);

private:
  void populateParticleRecords();

};

#endif // INTERFACETOCHECKERSCRIPT_H
