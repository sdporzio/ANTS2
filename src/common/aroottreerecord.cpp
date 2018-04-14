#include "aroottreerecord.h"

#include "TObject.h"
#include "TTree.h"
#include "TFile.h"
#include "TKey.h"

#include <QVariant>
#include <QDebug>

ABranchBuffer::ABranchBuffer(const QString &branchName, const QString &branchType, TTree *tree) :
    name(branchName), type(branchType), treePtr(tree)
{
    tName = name.toLatin1().data();
    TString f = tName + "/" + type.toLatin1().data();

    if (type.length() == 1)
    {
        bVector = false;
        cType   = type.at(0).toLatin1();

        switch (cType)
        {
            case 'C' : branchPtr = tree->Branch(tName, &C, f); break;
            case 'I' : branchPtr = tree->Branch(tName, &I, f); break;
            case 'F' : branchPtr = tree->Branch(tName, &F, f); break;
            case 'D' : branchPtr = tree->Branch(tName, &D, f); break;
            case 'O' : branchPtr = tree->Branch(tName, &O, f); break;
            default  : qWarning() << "Unknown tree branch type:" << type;
        }
    }
    else
    {
        bVector = true;
        cType   = type.at(1).toLatin1();

        switch (cType)
        {
            case 'C' : branchPtr = tree->Branch(tName, &AC); break;
            case 'I' : branchPtr = tree->Branch(tName, &AI); break;
            case 'F' : branchPtr = tree->Branch(tName, &AF); break;
            case 'D' : branchPtr = tree->Branch(tName, &AD); break;
            case 'O' : branchPtr = tree->Branch(tName, &AO); break;
            default  : qWarning() << "Unknown tree branch type:" << type;
        }
    }
}

ABranchBuffer::ABranchBuffer(const QString &branchName, const QString &branchType, TBranch *branch)
{
    name  = branchName;
    type  = branchType;
    tName = branchName.toLatin1().data();
    treePtr = branch->GetTree();

    if (type.size() == 1)
    {
        if ( ABranchBuffer::getAllTypes().contains(type) )
        {
            //this is one of the basic types            
            bVector = false;
            cType   = type.at(0).toLatin1();
            switch (cType)
            {
                case 'C' : treePtr->SetBranchAddress(tName, &C); break;
                case 'I' : treePtr->SetBranchAddress(tName, &I); break;
                case 'F' : treePtr->SetBranchAddress(tName, &F); break;
                case 'D' : treePtr->SetBranchAddress(tName, &D); break;
                case 'O' : treePtr->SetBranchAddress(tName, &O); break;
                default:;
            }
            branchPtr = branch; // non 0 -> indicates that the branch is valid
        }
    }
    else
    {
        if (type.contains("vector"))
        {
            type.remove("vector");
            type.remove("<");
            type.remove(">");

            if      (type == "char")   { type = "AC"; treePtr->SetBranchAddress(tName, &AC); }
            else if (type == "int")    { type = "AI"; treePtr->SetBranchAddress(tName, &AI); }
            else if (type == "float")  { type = "AF"; treePtr->SetBranchAddress(tName, &AF); }
            else if (type == "double") { type = "AD"; treePtr->SetBranchAddress(tName, &AD); }
            else if (type == "bool")   { type = "AO"; treePtr->SetBranchAddress(tName, &AO); }

            if ( ABranchBuffer::getAllTypes().contains(type) && type.size() == 2)
            {
                bVector = true;
                cType = type.at(1).toLatin1();
                branchPtr = branch; // non 0 -> indicates that the branch is valid
            }
        }
    }
}

//#include "TString.h"
void ABranchBuffer::write(const QVariant &val)
{
    if (!bVector)
    {
        switch (cType)
        {
            //case 'C' : C = val.toString().toLatin1().data(); break;
            case 'C' :
            {
                TString tmp = val.toString().toLatin1().data();
                for (int i = 0; i < tmp.Length()+1; i++) C[i] = tmp[i]; //including termination
                break;
            }
            case 'I' : I = val.toInt();    break;
            case 'F' : F = val.toFloat();  break;
            case 'D' : D = val.toDouble(); break;
            case 'O' : O = val.toBool();   break;
            default  : qWarning() << "write - unknown tree branch type:" << type;
        }
    }
    else
    {
        const QVariantList vl = val.toList();
        switch (cType)
        {
            case 'C' : AC.resize(vl.size()); for (int i=0; i<vl.size(); i++) AC[i] = vl.at(i).toString().toLatin1().data(); break;
            case 'I' : AI.resize(vl.size()); for (int i=0; i<vl.size(); i++) AI[i] = vl.at(i).toInt();    break;
            case 'F' : AF.resize(vl.size()); for (int i=0; i<vl.size(); i++) AF[i] = vl.at(i).toFloat();  break;
            case 'D' : AD.resize(vl.size()); for (int i=0; i<vl.size(); i++) AD[i] = vl.at(i).toDouble(); break;
            case 'O' : AO.resize(vl.size()); for (int i=0; i<vl.size(); i++) AO[i] = vl.at(i).toBool();   break;
            default  : qWarning() << "write - unknown tree branch type:" << type;
        }
    }
}

const QVariant ABranchBuffer::read()
{
    if (!bVector)
    {
        switch (cType)
        {
            case 'C' : return QString(C);
            case 'I' : return I;
            case 'F' : return F;
            case 'D' : return D;
            case 'O' : return O;
            default  : qWarning() << "read - unknown tree branch type:" << type;
        }
    }
    else
    {
        QVariantList vl;
        switch (cType)
        {
            case 'C' : for (const TString& e : AC) vl << QString(e); return vl;
            case 'I' : for (const int& e : AI) vl << e; return vl;
            case 'F' : for (const float& e : AF) vl << e; return vl;
            case 'D' : for (const double& e : AD) vl << e; return vl;
            case 'O' : for (const bool& e : AO) vl << e; return vl;
            default  : qWarning() << "read - unknown tree branch type:" << type;
        }
    }
    return 0;
}

ARootTreeRecord::ARootTreeRecord(TObject *tree, const QString &name) :
    ARootObjBase(tree, name, "") {}

ARootTreeRecord::~ARootTreeRecord()
{
    for (ABranchBuffer* bb : Branches) delete bb;
    Branches.clear();
    MapOfBranches.clear();
}

bool ARootTreeRecord::createTree(const QString &name, const QVector<QPair<QString, QString> > &branches)
{
    QMutexLocker locker(&Mutex);

    Branches.resize(branches.size());
    MapOfBranches.clear();
    if (Object) delete Object;

    TTree* t = new TTree(name.toLatin1().data(), "");
    Object = t;

    for (int ib = 0; ib < branches.size(); ib++)
    {        
        const QString& Bname = branches.at(ib).first;
        const QString& Btype = branches.at(ib).second;

        ABranchBuffer* bb = new ABranchBuffer(Bname, Btype, t);
        if (bb->isValid())
        {
            Branches[ib] = bb;
            MapOfBranches.insert( Bname, bb );
        }
        else
        {
            delete bb;
            return false;
        }
    }
    return true;
}

const QString ARootTreeRecord::loadTree(const QString &fileName, const QString treeNameInFile)
{
    TFile *f = TFile::Open(fileName.toLocal8Bit().data(), "READ");
    if (!f) return QString("Cannot open file ") + fileName;

    TTree *t = 0;
    if (treeNameInFile.isEmpty())
    {
        const int numKeys = f->GetListOfKeys()->GetEntries();
        for (int ikey = 0; ikey < numKeys; ikey++)
        {
            TKey *key = (TKey*)f->GetListOfKeys()->At(ikey);
            qDebug() << "Key->  name:" << key->GetName() << " class:" << key->GetClassName() <<" title:"<< key->GetTitle();

            const QString Type = key->GetClassName();
            if (Type != "TTree") continue;
            t = dynamic_cast<TTree*>(key->ReadObj());
            if (t) break;
        }
        if (!t)
        {
            delete f;
            return QString("No trees found in file ") + fileName;
        }
    }
    else
    {
        f->GetObject(treeNameInFile.toLocal8Bit().data(), t);
        if (!t)
        {
            delete f;
            return QString("Tree ") + treeNameInFile + " not found in file " + fileName;
        }
    }

    t->Print();

    QMutexLocker locker(&Mutex);

    Branches.clear();
    MapOfBranches.clear();
    if (Object) delete Object;
    Object = t;

    const int numBranches = t->GetNbranches();
    TObjArray* lb = t->GetListOfBranches();

    for (int ibranch = 0; ibranch < numBranches; ibranch++)
    {
        TBranch* branchPtr = (TBranch*)(lb->At(ibranch));
        QString branchName = branchPtr->GetName();
        QString branchType = branchPtr->GetClassName();
        if (branchType.isEmpty())
        {
            QString title = branchPtr->GetTitle();
            title.remove(branchName);
            title.remove("/");
            branchType = title;
        }
        // else    -> vector<T> is here
        qDebug() << branchName << branchType << branchPtr;

        ABranchBuffer* bb = new ABranchBuffer(branchName, branchType, branchPtr);
        if (bb->isValid())
        {
            Branches << bb;
            MapOfBranches.insert( branchName, bb );
        }
        else
        {
            delete bb;
            delete t;
            delete f;
            branchType.replace("<", "(");
            branchType.replace(">", ")");
            return QString("Unsupported branch type ") + branchType + " for tree in file " + fileName;
        }
    }
    return "";
}

int ARootTreeRecord::countBranches() const
{
    QMutexLocker locker(&Mutex);
    return Branches.size();
}

int ARootTreeRecord::countEntries() const
{
    QMutexLocker locker(&Mutex);

    return static_cast<TTree*>(Object)->GetEntries();
}

bool ARootTreeRecord::fillSingle(const QVariantList &vl)
{
    QMutexLocker locker(&Mutex);

    if (vl.size() != Branches.size()) return false;

    for (int ib = 0; ib < Branches.size(); ib++)
    {
        ABranchBuffer* bb = Branches[ib];
        bb->write(vl.at(ib));
    }
    static_cast<TTree*>(Object)->Fill();

    return true;
}

bool ARootTreeRecord::isBranchExist(const QString &branchName) const
{
    QMutexLocker locker(&Mutex);

    return MapOfBranches.value(branchName, 0);
}

const QVariantList ARootTreeRecord::getBranch(const QString &branchName)
{
    QVariantList res;

    ABranchBuffer* bb = MapOfBranches.value(branchName, 0);
    if (bb)
    {
        if (bb->branchPtr)
        {
            const int numEntries = bb->branchPtr->GetEntries();
            for (int i = 0; i < numEntries; i++)
            {
                Long64_t tentry = bb->treePtr->LoadTree(i);
                bb->branchPtr->GetEntry(tentry);
                res.push_back(bb->read());
            }
        }
    }
    return res;
}

const QVariant ARootTreeRecord::getBranch(const QString &branchName, int entry)
{
    ABranchBuffer* bb = MapOfBranches.value(branchName, 0);
    if (bb)
    {
        if (bb->branchPtr)
        {
            const int numEntries = bb->branchPtr->GetEntries();
            if (entry >= 0 && entry < numEntries)
            {
                const Long64_t tentry = bb->treePtr->LoadTree(entry);
                bb->branchPtr->GetEntry(tentry);
                return bb->read();
            }
        }
    }

    return QVariant();
}

const QVariantList ARootTreeRecord::getEntry(int entry)
{
    QVariantList res;

    for (int ibranch = 0; ibranch < Branches.size(); ibranch++)
    {
         ABranchBuffer* bb = Branches[ibranch];
         if (bb->branchPtr)
         {
             const int numEntries = bb->branchPtr->GetEntries();
             if (entry >= 0 && entry < numEntries)
             {
                 const Long64_t tentry = bb->treePtr->LoadTree(entry);
                 bb->branchPtr->GetEntry(tentry);
                 res.push_back( bb->read() );
             }
             else res.push_back( QVariant() );
         }
         else return QVariantList();
    }

    return res;
}

void ARootTreeRecord::save(const QString &FileName)
{
    TTree* t = static_cast<TTree*>(Object);
    t->SaveAs(FileName.toLatin1().data());
}
