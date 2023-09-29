#include "scenemodifier.h"
#include <unistd.h>

#include <QtCore/QDebug>
#include <Qt3DExtras/QMetalRoughMaterial>
#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include "calc_thread.h"

namespace {

    using Slot = void (SceneModifier::*)(double);
    QLayout* createCtrlButton(const QString& label,
            float initVal,
            SceneModifier* sc,
            Slot fun,
            float step = 0.005f,
            float max = 1.0f,
            float min = 0.0f)
    {
        QHBoxLayout* lay1 = new QHBoxLayout;

        QLabel* lab1 = new QLabel(label);
        QDoubleSpinBox* box1 = new QDoubleSpinBox;
        box1->setDecimals(4);
        box1->setValue(initVal);
        box1->setMinimum(min);
        box1->setMaximum(max);
        box1->setSingleStep(step);
        lay1->addSpacing(50);
        lay1->addWidget(lab1);
        lay1->addWidget(box1);
        lay1->addSpacing(50);

        using ft = void (QDoubleSpinBox::*)(double);

        QObject::connect(box1,
                static_cast<ft>(&QDoubleSpinBox::valueChanged),
                sc,
                fun);

        return lay1;
    }

}

SceneModifier::SceneModifier(Qt3DCore::QEntity* rootEntity, QWidget* parent)
    : m_rootEntity(rootEntity)
    , m_Parent(parent)
{
    m_susp = new MagSusp3D(rootEntity);

    m_vLay = new QVBoxLayout(parent);
    m_vLay->setAlignment(Qt::AlignTop);


    auto* lay = createCtrlButton("aմ (մ)", m_susp->getAm(), this, &SceneModifier::setAm);
    m_vLay->addLayout(lay);

    lay = createCtrlButton("bմ (մ)", m_susp->getBm(), this, &SceneModifier::setBm);
    m_vLay->addLayout(lay);

    lay = createCtrlButton("lմ (մ)", m_susp->getLm(), this, &SceneModifier::setLm);
    m_vLay->addLayout(lay);

    lay = createCtrlButton("lհ (մ)", m_susp->getLh(), this, &SceneModifier::setLh);
    m_vLay->addLayout(lay);

    lay = createCtrlButton("bհ (մ)", m_susp->getBh(), this, &SceneModifier::setBh);
    m_vLay->addLayout(lay);

    lay = createCtrlButton("bխ (մ)", m_susp->getBx(), this, &SceneModifier::setBx);
    m_vLay->addLayout(lay);

    lay = createCtrlButton("δ (մ)", m_susp->getAGap(), this, &SceneModifier::setAGap);
    m_vLay->addLayout(lay);

   lay = createCtrlButton("Δմ (մ)", 0.005, this, &SceneModifier::setDelm, 0.0005f);
   m_vLay->addLayout(lay);
}

void SceneModifier::setAm(double in)
{
    const tsizes sz = {in * 10.0f, {}, {}, {}, {}, {}, {}};
    m_susp->updateSizes(sz);
}

void SceneModifier::setBm(double in)
{
    const tsizes sz = {{}, in * 10.0f, {}, {}, {}, {}, {}};
    m_susp->updateSizes(sz);
}

void SceneModifier::setLm(double in)
{
    const tsizes sz = {{}, {}, in * 10.0f, {}, {}, {}, {}};
    m_susp->updateSizes(sz);
}

void SceneModifier::setLh(double in)
{
    const tsizes sz = {{}, {}, {}, in * 10.0f, {}, {}, {}};
    m_susp->updateSizes(sz);
}

void SceneModifier::setBh(double in)
{
    const tsizes sz = {{}, {}, {}, {}, in * 10.0f, {}, {}};
    m_susp->updateSizes(sz);
}

void SceneModifier::setBx(double in)
{
    const tsizes sz = {{}, {}, {}, {}, {}, in * 10.0f, {}};
    m_susp->updateSizes(sz);
}

void SceneModifier::setAGap(double in)
{
    const tsizes sz = {{}, {}, {}, {}, {}, {}, in * 10.0f};
    m_susp->updateSizes(sz);
}

void SceneModifier::setDelm(double in)
{
    const tsizes sz = {{}, {}, {}, {}, {}, {}, {}, in * 10.0f};
    m_susp->updateSizes(sz);
}


void SceneModifier::showFinalResult(const tsizes& sz)
{
    QDialog finalRes;
    QVBoxLayout dlLayout;
    finalRes.setLayout(&dlLayout);

    QTextEdit txEdit;
    txEdit.setTextInteractionFlags(Qt::NoTextInteraction);

    QString str;
    str += "aմ = " + QString::number(sz.a.value()/10) + " մ\n"; 
    str += "bմ = " + QString::number(sz.b.value()/10) + " մ\n"; 
    str += "lմ = " + QString::number(sz.h.value()/10) + " մ\n"; 
    str += "bհ = " + QString::number(sz.c.value()/10) + " մ\n"; 
    str += "lհ = " + QString::number(sz.l.value()/10) + " մ\n"; 
    str += "aխ = " + QString::number(sz.a.value()/10) + " մ\n"; 
    str += "bխ = " + QString::number(sz.b_x.value()/10) + " մ\n"; 
    str += "δ = " + QString::number(sz.gap.value()/10) + " մ\n"; 
    str += "P = " + QString::number(sz.P.value()) + " Վտ\n"; 
    str += "θփ = " + QString::number(sz.T.value()) + " °C\n"; 

    txEdit.setText(str);
    dlLayout.addWidget(&txEdit);

    finalRes.show();
    finalRes.exec();
}

void SceneModifier::startCalculation()
{
   CalcThread* tr = new CalcThread(this);
   //connect(tr, &CalcThread::resultReady, this, &SceneModifier::updateSizes);
   connect(tr, &CalcThread::finalResult, this, &SceneModifier::showFinalResult);
   connect(tr, &CalcThread::finished, tr, &QObject::deleteLater);

   tr->start();
}

