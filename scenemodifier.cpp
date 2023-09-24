#include "scenemodifier.h"
#include <unistd.h>

#include <QtCore/QDebug>
#include <Qt3DExtras/QMetalRoughMaterial>
#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>
#include "calc_thread.h"

SceneModifier::SceneModifier(Qt3DCore::QEntity* rootEntity, QWidget* parent)
    : m_rootEntity(rootEntity)
    , m_Parent(parent)
{
    m_susp = new MagSusp3D(rootEntity);
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

