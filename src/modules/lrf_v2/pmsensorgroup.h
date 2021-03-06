#ifndef PMSENSORGROUP_H
#define PMSENSORGROUP_H

#include "pmsensor.h"
#include "lrffactory.h"

#include <vector>
#include <QVector>
#include <QPair>

class LRF2;
class APmHub;
class SensorLRFs;
class SensorLocalCache;
class QJsonObject;

//class used to store info on a group of sensors
class PMsensorGroup
{
public:
    PMsensorGroup();
    ~PMsensorGroup();
    void clear();

    PMsensorGroup &operator=(const PMsensorGroup &copy);

    static QVector<PMsensorGroup> mkFromSensorsAndLRFs(QVector<int> gids, QVector<PMsensor> sensors, QVector<LRF2*> lrfs);
    static QVector<PMsensorGroup> mkIndividual(const APmHub *PMs);
    static QVector<PMsensorGroup> mkCommon(const APmHub *PMs);
    static QVector<PMsensorGroup> mkAxial(const APmHub *PMs);
    static QVector<PMsensorGroup> mkLattice(const APmHub *PMs, int N_gonal);
    static QVector<PMsensorGroup> mkHexagonal(const APmHub *PMs);
    static QVector<PMsensorGroup> mkSquare(const APmHub *PMs);

    void setLRF(LRF2 *lrf);
    LRF2* getLRF() { return lrf; }

    void setGains(const double *gains);

    std::vector<PMsensor> *getPMs() { return &sensors; }

    void writeJSON(QJsonObject &json) const;
    bool readJSON (QJsonObject &json);

private:
    void addSensor(const APmHub *PMs, int ipm, double phi, bool flip);

    std::vector<PMsensor> sensors;
    LRF2* lrf;
};

#endif // PMSENSORGROUP_H
