#include "scenemodifier.h"
#include <unistd.h>

#include <QtCore/QDebug>
#include <Qt3DExtras/QMetalRoughMaterial>
#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>
#include "calc_thread.h"

SceneModifier::SceneModifier(Qt3DCore::QEntity *rootEntity)
    : m_rootEntity(rootEntity)
{
    qRegisterMetaType<tsizes>("tsizes");

    // Anchor
    m_anchorCubeMesh  = new Qt3DExtras::QCuboidMesh();
    m_anchorCubeTransform = new Qt3DCore::QTransform();
    m_anchorCubeTransform->setScale(1.0f);
    auto anchorCube = new Qt3DCore::QEntity(m_rootEntity);
    anchorCube->addComponent(m_anchorCubeMesh);
    auto anchorMaterial = new Qt3DExtras::QMetalRoughMaterial();
    anchorMaterial->setBaseColor(QColor(QRgb(0x2F4F4F)));
    anchorMaterial->setMetalness(0.4f);
    anchorCube->addComponent(anchorMaterial);
    anchorCube->addComponent(m_anchorCubeTransform);

    //
    // Left cube.
    //
    m_leftCubeMesh  = new Qt3DExtras::QCuboidMesh();
    m_leftCubeTransform = new Qt3DCore::QTransform();
    m_leftCubeTransform->setScale(1.0f);
    auto leftCube = new Qt3DCore::QEntity(m_rootEntity);
    leftCube->addComponent(m_leftCubeMesh);
    auto leftMaterial = new Qt3DExtras::QMetalRoughMaterial();
    leftMaterial->setBaseColor(QColor(QRgb(0x2F4F4F)));
    leftMaterial->setMetalness(0.4f);
    leftCube->addComponent(leftMaterial);
    leftCube->addComponent(m_leftCubeTransform);

    //
    // Right cube.
    //
    m_rightCubeMesh  = new Qt3DExtras::QCuboidMesh();
    m_rightCubeTransform = new Qt3DCore::QTransform();
    m_rightCubeTransform->setScale(1.0f);
    auto rightCube = new Qt3DCore::QEntity(m_rootEntity);
    rightCube->addComponent(m_rightCubeMesh);
    auto rightMaterial = new Qt3DExtras::QMetalRoughMaterial();
    rightMaterial->setBaseColor(QColor(QRgb(0x2F4F4F)));
    rightMaterial->setMetalness(0.4f);
    rightCube->addComponent(rightMaterial);
    rightCube->addComponent(m_rightCubeTransform);

    //
    // Left isol.
    //
    m_leftIsolMesh  = new Qt3DExtras::QCuboidMesh();
    m_leftIsolTransform = new Qt3DCore::QTransform();
    m_leftIsolTransform->setScale(1.0f);
    auto leftIsol = new Qt3DCore::QEntity(m_rootEntity);
    leftIsol->addComponent(m_leftIsolMesh);
    auto leftIsolMaterial = new Qt3DExtras::QMetalRoughMaterial();
    leftIsolMaterial->setBaseColor(QColor(QRgb(0x00FFFF)));
    leftIsolMaterial->setMetalness(0.1f);
    leftIsol->addComponent(leftIsolMaterial);
    leftIsol->addComponent(m_leftIsolTransform);

    //
    // Right isol.
    //
    m_rightIsolMesh  = new Qt3DExtras::QCuboidMesh();
    m_rightIsolTransform = new Qt3DCore::QTransform();
    m_rightIsolTransform->setScale(1.0f);
    auto rightIsol = new Qt3DCore::QEntity(m_rootEntity);
    rightIsol->addComponent(m_rightIsolMesh);
    auto rightIsolMaterial = new Qt3DExtras::QMetalRoughMaterial();
    rightIsolMaterial->setBaseColor(QColor(QRgb(0x00FFFF)));
    rightIsolMaterial->setMetalness(0.1f);
    rightIsol->addComponent(rightIsolMaterial);
    rightIsol->addComponent(m_rightIsolTransform);

    //
    // Coil
    //
    auto coilMaterial = new Qt3DExtras::QMetalRoughMaterial();
    coilMaterial->setBaseColor(QColor(QRgb(0xFE5E00)));
    coilMaterial->setMetalness(0.2f);

    m_coilCubeMesh1 = new Qt3DExtras::QCuboidMesh();
    auto coil1 = new Qt3DCore::QEntity(m_rootEntity);
    coil1->addComponent(m_coilCubeMesh1);
    coil1->addComponent(coilMaterial);
    auto coilTransform1 = new Qt3DCore::QTransform();
    coilTransform1->setScale(1.0f);
    coil1->addComponent(coilTransform1);

    m_coilCubeMesh2  = new Qt3DExtras::QCuboidMesh();
    auto coil2 = new Qt3DCore::QEntity(m_rootEntity);
    coil2->addComponent(m_coilCubeMesh2);
    coil2->addComponent(coilMaterial);
    auto coilTransform2 = new Qt3DCore::QTransform();
    coilTransform2->setScale(1.0f);
    coil2->addComponent(coilTransform2);

    m_coilCylMesh1 = new Qt3DExtras::QCylinderMesh();
    m_coilCylMesh1->setRings(10);
    m_coilCylMesh1->setSlices(30);
    m_coilCylTransform1 = new Qt3DCore::QTransform();
    m_coilCylTransform1->setScale(1.0f);
    m_coilCylTransform1->setRotationZ(90);
    auto coilCyl1 = new Qt3DCore::QEntity(m_rootEntity);
    coilCyl1->addComponent(m_coilCylMesh1);
    coilCyl1->addComponent(coilMaterial);
    coilCyl1->addComponent(m_coilCylTransform1);

    m_coilCylMesh2 = new Qt3DExtras::QCylinderMesh();
    m_coilCylMesh2->setRings(10);
    m_coilCylMesh2->setSlices(30);
    m_coilCylTransform2 = new Qt3DCore::QTransform();
    m_coilCylTransform2->setScale(1.0f);
    m_coilCylTransform2->setRotationZ(90);
    auto coilCyl2 = new Qt3DCore::QEntity(m_rootEntity);
    coilCyl2->addComponent(m_coilCylMesh2);
    coilCyl2->addComponent(coilMaterial);
    coilCyl2->addComponent(m_coilCylTransform2);

    m_coilCylMesh3 = new Qt3DExtras::QCylinderMesh();
    m_coilCylMesh3->setRings(10);
    m_coilCylMesh3->setSlices(30);
    m_coilCylTransform3 = new Qt3DCore::QTransform();
    m_coilCylTransform3->setScale(1.0f);
    m_coilCylTransform3->setRotationZ(90);
    auto coilCyl3 = new Qt3DCore::QEntity(m_rootEntity);
    coilCyl3->addComponent(m_coilCylMesh3);
    coilCyl3->addComponent(coilMaterial);
    coilCyl3->addComponent(m_coilCylTransform3);

    m_coilCylMesh4 = new Qt3DExtras::QCylinderMesh();
    m_coilCylMesh4->setRings(10);
    m_coilCylMesh4->setSlices(30);
    m_coilCylTransform4 = new Qt3DCore::QTransform();
    m_coilCylTransform4->setScale(1.0f);
    m_coilCylTransform4->setRotationZ(90);
    auto coilCyl4 = new Qt3DCore::QEntity(m_rootEntity);
    coilCyl4->addComponent(m_coilCylMesh4);
    coilCyl4->addComponent(coilMaterial);
    coilCyl4->addComponent(m_coilCylTransform4);

    // set sizes
    const tsizes sz = {a, b, h, l, c, b_x, gap};

    updateSizes(sz);
}

void SceneModifier::showFinalResult(const tsizes& sz)
{
    QDialog finalRes;
    QVBoxLayout dlLayout;
    finalRes.setLayout(&dlLayout);

    QTextEdit txEdit;
    txEdit.setTextInteractionFlags(Qt::NoTextInteraction);

    dlLayout.addWidget(&txEdit);

    finalRes.show();
    finalRes.exec();
}

void SceneModifier::startCalculation()
{
   CalcThread* tr = new CalcThread(this);
   connect(tr, &CalcThread::resultReady, this, &SceneModifier::updateSizes);
   connect(tr, &CalcThread::finalResult, this, &SceneModifier::showFinalResult);
   connect(tr, &CalcThread::finished, tr, &QObject::deleteLater);

   tr->start();
}

