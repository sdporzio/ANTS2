#ifndef LRFAXIAL3D_H
#define LRFAXIAL3D_H

#include "lrf2.h"

class Bspline2d;

class LRFaxial3d : public LRF2
{
public:
    LRFaxial3d(double r, int nint_, double z_min, double z_max, int n_intz, bool log = false);
    LRFaxial3d(QJsonObject &json);
    ~LRFaxial3d();
    virtual bool inDomain(double x, double y, double z) const;
    virtual bool errorDefined() const {return bse != 0;}
    virtual double getRmax() const {return rmax;}
    virtual double getXmin() const {return -getRmax();}
    virtual double getXmax() const {return +getRmax();}
    virtual double getYmin() const {return -getRmax();}
    virtual double getYmax() const {return +getRmax();}
    int getNint() const {return nint;}
    int getNintZ() const {return nintz;}
    virtual double eval(double x, double y, double z) const;
    virtual double evalErr(double x, double y, double z=0.) const;
    virtual double evalDrvX(double x, double y, double z) const;
    virtual double evalDrvY(double x, double y, double z) const;
    virtual double eval(double x, double y, double z, double *err) const;
    virtual double fit(int npts, const double *x, const double *y, const double *z, const double *data, bool grid);
    void setSpline(Bspline2d *bs, bool log);
    virtual const char *type() const { return "Axial3D"; }
    virtual void writeJSON(QJsonObject &json) const;
    virtual QJsonObject reportSettings() const;

    virtual double compress(double r) const { return r; }
    virtual double comprDev(double r) const;

    void SetFlatTop(bool val) {flat_top = val;}
    void SetNonNegative(bool val) {non_negative = val;}
    void SetNonIncreasing(bool val) {non_increasing = val;}
    void SetForcedZSlope(int val) {z_slope = val;}

private:
    double rmax;	// domain
    int nint;		// intervals
    int nintz;	// intervals
    Bspline2d *bsr; 	// spline describing radial dependence
    Bspline2d *bse; 	// spline describing radial error dependence
    bool flat_top;
    bool non_negative;
    bool non_increasing;
    int z_slope;
};

#endif // LRFAXIAL3D_H
