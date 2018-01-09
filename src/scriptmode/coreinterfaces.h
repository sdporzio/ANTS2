#ifndef COREINTERFACES_H
#define COREINTERFACES_H

#include "ascriptinterface.h"

#include <QVariant>
#include <QString>

class AScriptManager;
class TRandom2;

class AInterfaceToCore : public AScriptInterface
{
  Q_OBJECT

public:
  AInterfaceToCore(AScriptManager *ScriptManager);

public slots:
  //abort execution of the script
  void abort(QString message = "Aborted!");

  QVariant evaluate(QString script);

  //sleep
  void sleep(int ms);

  //output part of the script window
  void print(QString text);
  void clearText();
  QString str(double value, int precision);

  //time stamps
  QString GetTimeStamp();
  QString GetDateTimeStamp();

  //save to file
  bool createFile(QString fileName, bool AbortIfExists = true);
  bool isFileExists(QString fileName);
  bool deleteFile(QString fileName);
  bool createDir(QString path);
  QString getCurrentDir();
  bool setCirrentDir(QString path);
  bool save(QString fileName, QString str);
  bool saveArray(QString fileName, QVariant array);
  bool saveObject(QString FileName, QVariant Object, bool CanOverride);

  //load from file
  QVariant loadColumn(QString fileName, int column = 0); //load column of doubles from file and return it as an array
  QVariant loadArray(QString fileName, int columns);     //load column of doubles from file and return it as an array
  QString loadText(QString fileName);

  //dirs
  QString GetWorkDir();
  QString GetScriptDir();
  QString GetExamplesDir();

private:
  AScriptManager* ScriptManager;
};

// ---- M A T H ----
class AInterfaceToMath : public AScriptInterface
{
  Q_OBJECT
  Q_PROPERTY(double pi READ pi)
  double pi() const { return 3.141592653589793238462643383279502884; }

public:
  AInterfaceToMath(TRandom2* RandGen);
  void setRandomGen(TRandom2* RandGen);

public slots:
  double abs(double val);
  double acos(double val);
  double asin(double val);
  double atan(double val);
  double atan2(double y, double x);
  double ceil(double val);
  double cos(double val);
  double exp(double val);
  double floor(double val);
  double log(double val);
  double max(double val1, double val2);
  double min(double val1, double val2);
  double pow(double val, double power);
  double sin(double val);
  double sqrt(double val);
  double tan(double val);
  double round(double val);
  double random();
  double gauss(double mean, double sigma);
  double poisson(double mean);
  double maxwell(double a);  // a is sqrt(kT/m)
  double exponential(double tau);

private:
  TRandom2* RandGen;
};

#endif // COREINTERFACES_H