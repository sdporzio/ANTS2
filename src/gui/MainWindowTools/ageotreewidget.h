#ifndef AGEOTREEWIDGET_H
#define AGEOTREEWIDGET_H

#include <QTreeWidget>
#include <QSyntaxHighlighter>

class AGeoObject;
class AGeoWidget;
class ASandwich;

class AGeoTreeWidget : public QTreeWidget
{
  Q_OBJECT

public:
  AGeoTreeWidget(ASandwich* Sandwich);
  AGeoWidget* GetEditWidget() {return EditWidget;}

  ASandwich *Sandwich;

  void SelectObjects(QStringList ObjectNames);

public slots:
  void UpdateGui(QString selected = "");
  void onGridReshapeRequested(QString objName);

private slots:
  void onItemSelectionChanged();
  void customMenuRequested(const QPoint &pos);  // CONTEXT MENU
  void onItemClicked(); //only to return to normal geo view mode!

  void onItemExpanded(QTreeWidgetItem* item);
  void onItemCollapsed(QTreeWidgetItem* item);

protected:
  void dropEvent(QDropEvent *event);
  void dragEnterEvent(QDragEnterEvent* event);
//  void dragMoveEvent(QDragMoveEvent* event);

private:  
  AGeoObject *World;
  AGeoWidget *EditWidget;

  //base images for icons
  QImage Lock;
  QImage GroupStart, GroupMid, GroupEnd;
  QImage StackStart, StackMid, StackEnd;

  QColor BackgroundColor;
  bool fSpecialGeoViewMode;

  void populateTreeWidget(QTreeWidgetItem *parent, AGeoObject *Container, bool fDisabled = false);
  void updateExpandState(QTreeWidgetItem* item); //recursive!
  void updateIcon(QTreeWidgetItem *item, AGeoObject *obj);
  void formSet(QList<QTreeWidgetItem *> selected, int option);
  void addLightguide(bool upper);

  void menuActionAddNewObject(QString ContainerName);
  void menuActionCopyObject(QString ObjToCopyName);
  void SetLineAttributes(QString ObjectName);
  void ShowObject(QString ObjectName);
  void ShowObjectRecursive(QString ObjectName);
  void ShowObjectOnly(QString ObjectName);
  void menuActionEnableDisable(QString ObjectName);
  void menuActionLock();
  void menuActionUnlock();
  void menuActionLockAllInside(QString ObjectName);
  void menuActionUnlockAllInside(QString ObjectName);
  void menuActionRemove();
  void menuActionRemoveHostedObjects(QString ObjectName);
  void menuActionRemoveRecursively(QString ObjectName);
  void menuActionAddNewComposite(QString ContainerName);
  void menuActionAddNewArray(QString ContainerName);
  void menuActionAddNewGrid(QString ContainerName);
  void menuActionAddNewMonitor(QString ContainerName);

signals:
  void ObjectSelectionChanged(const QString); // should be fired with empty string if selection does not contain a single item  
  void RequestRebuildDetector();
  void RequestHighlightObject(QString name);
  void RequestShowObjectRecursive(QString name);
  void RequestNormalDetectorDraw();
  void RequestListOfParticles(QStringList &definedParticles);
  void RequestShowMonitor(const AGeoObject* mon);
};

// GeoWidget
class QLineEdit;
class QPlainTextEdit;
class QComboBox;
class QSpinBox;
class QVBoxLayout;
class QGridLayout;
class QPushButton;
class AGeoObjectDelegate;
class AGridElementDelegate;
class AMonitorDelegate;
class QLabel;
class ASlabDelegate;
class ATypeCompositeObject;
class TVector3;
class AMonitorDelegateForm;
class QHBoxLayout;
class AGeoShape;

class AGeoWidget : public QWidget  // ***!!! can get rid of the Widget here?
{
  Q_OBJECT

public:
  AGeoWidget(AGeoObject* World, AGeoTreeWidget* tw);
  //destructor does not delete Widget - it is handled by the layout

private:
  AGeoObject* World;
  AGeoTreeWidget* tw;

  AGeoObject* CurrentObject;

  AGeoObjectDelegate* GeoObjectDelegate;
  ASlabDelegate* SlabDelegate;
  AGridElementDelegate* GridDelegate;
  AMonitorDelegate* MonitorDelegate;

  QVBoxLayout *lMain;
  QVBoxLayout *ObjectLayout;
  QFrame      *frBottom;
  QPushButton *pbConfirm, *pbCancel;

  bool fIgnoreSignals;
  bool fEditingMode;

  void ClearGui();
  void UpdateGui();

public slots:
  void onObjectSelectionChanged(const QString SelectedObject); //starts GUI update
  void onStartEditing();
  void onRequestChangeShape(AGeoShape * NewShape);
  void OnCustomContextMenuTriggered_forMainObject(QPoint pos);
  void onMonitorRequestsShowSensitiveDirection();

  void onRequestShowCurrentObject();
  void onRequestScriptLineToClipboard();
  void onRequestSetVisAttributes();

private slots:
  void onConfirmPressed();
  void onCancelPressed();

private:
  void rotate(TVector3* v, double dPhi, double dTheta, double dPsi);

  void getValuesFromNonSlabDelegates(AGeoObject *objMain);
  void confirmChangesForSlab();
  void confirmChangesForGridDelegate();
  void confirmChangesForMonitorDelegate();

  void exitEditingMode();
  QString getSuffix(AGeoObject *objCont);
  //void convertToLG(int UpperLower); //0 - upper, 1 - lower
  QLabel * addInfoLabel(QString text);
  AGeoObjectDelegate   *createAndAddGeoObjectDelegate(AGeoObject *obj, QWidget *parent);
  ASlabDelegate        *createAndAddSlabDelegate(AGeoObject *obj);
  AGridElementDelegate *createAndAddGridElementDelegate(AGeoObject *obj);
  AMonitorDelegate     *createAndAddMonitorDelegate(AGeoObject *obj, QStringList particles);
  bool checkNonSlabObjectDelegateValidity(AGeoObject *obj);

signals:
  void showMonitor(const AGeoObject* mon);
};

class AGeoObjectDelegate : public QWidget
{
    Q_OBJECT

public:
    AGeoObjectDelegate(const QStringList & materials, QWidget * ParentWidget = nullptr);
    virtual ~AGeoObjectDelegate(){}

    QFrame* Widget;

    QFrame* frMainFrame;
    QLineEdit* leName;
    QComboBox* cobMat;
    QPlainTextEdit* pteShape;
    QWidget* PosOrient;
    QWidget* ArrayWid;
    QLabel* lMat;

    QSpinBox *sbNumX, *sbNumY, *sbNumZ;
    QLineEdit *ledStepX, *ledStepY, *ledStepZ;

    QLineEdit *ledX, *ledY, *ledZ;
    QLineEdit *ledPhi, *ledTheta, *ledPsi;

private:
    QVBoxLayout * lMF = nullptr;      //main layout

    QPushButton * pbShapeInfo = nullptr;

    QPushButton * pbShow = nullptr;
    QPushButton * pbChangeAtt = nullptr;
    QPushButton * pbScriptLine = nullptr;

protected:
    QWidget * ParentWidget = nullptr;
    const AGeoObject * CurrentObject = nullptr;
    QLabel * labType = nullptr;
    QString DelegateTypeName = "Object";
    QString ShapeHelp;

public slots:
    virtual void Update(const AGeoObject * obj);

private slots:
    void onContentChanged();          // only to enter the editing mode! Object update is performed only on confirm button click!
    void onHelpRequested();           // dialog with AGeoShape list can be accessed here
    void onCursorPositionChanged();
    void onChangeShapePressed();

protected slots:
    virtual void onLocalParameterChange() {}

protected:
    void addLocalLayout(QLayout * lay);
    void updatePteShape(const QString & text);

signals:
    void ContentChanged();
    void RequestChangeShape(AGeoShape * newShape);
    void RequestShow();
    void RequestChangeVisAttributes();
    void RequestScriptToClipboard();
};

class AGeoBoxDelegate : public AGeoObjectDelegate
{
    Q_OBJECT

public:
    AGeoBoxDelegate(const QStringList & materials, QWidget * parent);

    QLineEdit * ex = nullptr;
    QLineEdit * ey = nullptr;
    QLineEdit * ez = nullptr;

public slots:
    virtual void Update(const AGeoObject * obj) override;

private slots:
    void onLocalParameterChange() override;
};

class AGeoTubeDelegate : public AGeoObjectDelegate
{
    Q_OBJECT

public:
    AGeoTubeDelegate(const QStringList & materials, QWidget * parent);

    QLineEdit * ei = nullptr;
    QLineEdit * eo = nullptr;
    QLineEdit * ez = nullptr;

protected:
    QGridLayout * gr = nullptr;

public slots:
    virtual void Update(const AGeoObject * obj) override;

private slots:
    void onLocalParameterChange() override;
};

class AGeoTubeSegDelegate : public AGeoTubeDelegate
{
    Q_OBJECT

public:
    AGeoTubeSegDelegate(const QStringList & materials, QWidget * parent);

    QLineEdit * ep1 = nullptr;
    QLineEdit * ep2 = nullptr;

public slots:
    virtual void Update(const AGeoObject * obj) override;

private slots:
    void onLocalParameterChange() override;
};

class AGeoTubeSegCutDelegate : public AGeoTubeSegDelegate
{
    Q_OBJECT

public:
    AGeoTubeSegCutDelegate(const QStringList & materials, QWidget * parent);

    QLineEdit * elnx = nullptr;
    QLineEdit * elny = nullptr;
    QLineEdit * elnz = nullptr;
    QLineEdit * eunx = nullptr;
    QLineEdit * euny = nullptr;
    QLineEdit * eunz = nullptr;

public slots:
    virtual void Update(const AGeoObject * obj) override;

private slots:
    void onLocalParameterChange() override;
};

class AGeoElTubeDelegate : public AGeoObjectDelegate
{
    Q_OBJECT

public:
    AGeoElTubeDelegate(const QStringList & materials, QWidget * parent);

    QLineEdit * ex = nullptr;
    QLineEdit * ey = nullptr;
    QLineEdit * ez = nullptr;

protected:
    QGridLayout * gr = nullptr;

public slots:
    virtual void Update(const AGeoObject * obj) override;

private slots:
    void onLocalParameterChange() override;
};

class AGeoParaDelegate : public AGeoObjectDelegate
{
    Q_OBJECT

public:
    AGeoParaDelegate(const QStringList & materials, QWidget * parent);

    QLineEdit * ex = nullptr;
    QLineEdit * ey = nullptr;
    QLineEdit * ez = nullptr;
    QLineEdit * ea = nullptr;
    QLineEdit * et = nullptr;
    QLineEdit * ep = nullptr;

public slots:
    virtual void Update(const AGeoObject * obj) override;

private slots:
    void onLocalParameterChange() override;
};

class AGeoSphereDelegate : public AGeoObjectDelegate
{
    Q_OBJECT

public:
    AGeoSphereDelegate(const QStringList & materials, QWidget * parent);

    QLineEdit * eod = nullptr;
    QLineEdit * eid = nullptr;
    QLineEdit * et1 = nullptr;
    QLineEdit * et2 = nullptr;
    QLineEdit * ep1 = nullptr;
    QLineEdit * ep2 = nullptr;

public slots:
    virtual void Update(const AGeoObject * obj) override;

private slots:
    void onLocalParameterChange() override;
};

class AGeoConeDelegate : public AGeoObjectDelegate
{
    Q_OBJECT

public:
    AGeoConeDelegate(const QStringList & materials, QWidget * parent);

    QLineEdit * ez  = nullptr;
    QLineEdit * eli = nullptr;
    QLineEdit * elo = nullptr;
    QLineEdit * eui = nullptr;
    QLineEdit * euo = nullptr;

public slots:
    virtual void Update(const AGeoObject * obj) override;

protected slots:
    void onLocalParameterChange() override;
};

class AGeoTrapXDelegate : public AGeoObjectDelegate
{
    Q_OBJECT

public:
    AGeoTrapXDelegate(const QStringList & materials, QWidget * parent);

    QLineEdit * exl = nullptr;
    QLineEdit * exu = nullptr;
    QLineEdit * ey = nullptr;
    QLineEdit * ez = nullptr;

public slots:
    virtual void Update(const AGeoObject * obj) override;

private slots:
    void onLocalParameterChange() override;
};

class AGeoTrapXYDelegate : public AGeoObjectDelegate
{
    Q_OBJECT

public:
    AGeoTrapXYDelegate(const QStringList & materials, QWidget * parent);

    QLineEdit * exl = nullptr;
    QLineEdit * exu = nullptr;
    QLineEdit * eyl = nullptr;
    QLineEdit * eyu = nullptr;
    QLineEdit * ez  = nullptr;

public slots:
    virtual void Update(const AGeoObject * obj) override;

private slots:
    void onLocalParameterChange() override;
};

// ---------------- Grid delegate ----------------

class AGridElementDelegate : public QWidget
{
  Q_OBJECT

public:
   AGridElementDelegate();

   QFrame* Widget;

   QFrame* frMainFrame;
   QLineEdit *ledDX, *ledDY, *ledDZ;
   QComboBox *cobShape;
   QLabel *lSize1, *lSize2;

private:
   const AGeoObject* CurrentObject;

   void updateVisibility();

public slots:
  void Update(const AGeoObject* obj);

private slots:
  void onContentChanged();  //only to enter editing mode! Object update only on confirm button!
  void StartDialog();
  void onInstructionsForGridRequested();

signals:
  void ContentChanged();
  void RequestReshapeGrid(QString);
};

// ---------------- Monitor delegate ----------------

class AMonitorDelegate : public QWidget
{
  Q_OBJECT

public:
   AMonitorDelegate(const QStringList & definedParticles);

   QFrame* Widget;
   AMonitorDelegateForm* del;

   QString getName() const;
   void updateObject(AGeoObject* obj);

private:
   const AGeoObject* CurrentObject;

public slots:
  void Update(const AGeoObject* obj);

private slots:
  void onContentChanged();  //only to enter editing mode! Object update only on confirm button!

signals:
  void ContentChanged();
  void requestShowSensitiveFaces();
};

class AShapeHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    AShapeHighlighter(QTextDocument *parent = 0);
protected:
    void highlightBlock(const QString &text);
private:
    struct HighlightingRule
      {
        QRegExp pattern;
        QTextCharFormat format;
      };
    QVector<HighlightingRule> highlightingRules;
    QTextCharFormat ShapeFormat;
};

#endif // AGEOTREEWIDGET_H
