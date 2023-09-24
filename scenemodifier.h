#ifndef SCENEMODIFIER_H
#define SCENEMODIFIER_H

#include <iostream>
#include <optional>
#include <QtCore/QObject>
#include <QLineEdit>

#include "magsusp3d.h"
#include "consts.hpp"

class SceneModifier : public QObject
{
    Q_OBJECT

    public:

public:
    explicit SceneModifier(Qt3DCore::QEntity *rootEntity, QWidget* parent);
    ~SceneModifier() = default;

public slots:
    void getRes()
    {
        std::cout << "Got results\n";
    }
    void aValChanged()
    {
   //     bool ok = false;
   //     float val = a_mLineEdit->text().toFloat(&ok);
   //     if (!ok) return;

   //     const tsizes sz = {val * 10.0f, {}, {}, {}, {}, {}, {}};

   //     m_susp->updateSizes(sz);
    }

    void bValChanged()
    {
   //     bool ok = false;
   //     float val = b_mLineEdit->text().toFloat(&ok);
   //     if (!ok) return;

   //     const tsizes sz = {{}, val * 10.0f, {}, {}, {}, {}, {}};
    }


    void hValChanged()
    {
   //     bool ok = false;
   //     float val = h_mLineEdit->text().toFloat(&ok);
   //     if (!ok) return;

   //     const tsizes sz = {{}, {}, val * 10.0f, {}, {}, {}, {}};
    }

//    void lValChanged(int val)
//    {
//    //    auto yval = float(val)/100.0f;
//    //    const tsizes sz = {{}, {}, {}, yval, {}, {}, {}};
//    }
//
//    void cValChanged(int val)
//    {
//        auto yval = float(val)/100.0f;
//        const tsizes sz = {{}, {}, {}, {}, yval, {}, {}};
//    }
//
//    void anchorValChanged(int val)
//    {
//        auto yval = float(val)/100.0f;
//        const tsizes sz = {{}, {}, {}, {}, {}, yval, {}};
//    }
//
//    void gapValChanged(int val)
//    {
//        auto yval = float(val)/100.0f;
//        const tsizes sz = {{}, {}, {}, {}, {}, {}, yval};
//    }
//
//    void setLineEditAm(QLineEdit* _in)
//    {
//        a_mLineEdit = _in;
//    }
//
//    void setLineEditBm(QLineEdit* _in)
//    {
//        b_mLineEdit = _in;
//    }
//
//    void setLineEditHm(QLineEdit* _in)
//    {
//        h_mLineEdit = _in;
//    }

    void startCalculation();

    void showFinalResult(const tsizes& sz);

private:
    Qt3DCore::QEntity *m_rootEntity = nullptr;
    QWidget* m_Parent = nullptr;
    MagSusp3D* m_susp = nullptr;
};

#endif // SCENEMODIFIER_H
