#include <magsusp3d.h>
#include <Qt3DExtras/QMetalRoughMaterial>

MagSusp3D::MagSusp3D(Qt3DCore::QEntity* rootEntity)
{
    // Anchor
    m_anchorCubeMesh  = new Qt3DExtras::QCuboidMesh();
    m_anchorCubeTransform = new Qt3DCore::QTransform();
    m_anchorCubeTransform->setScale(1.0f);
    auto anchorCube = new Qt3DCore::QEntity(rootEntity);
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
    auto leftCube = new Qt3DCore::QEntity(rootEntity);
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
    auto rightCube = new Qt3DCore::QEntity(rootEntity);
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
    auto leftIsol = new Qt3DCore::QEntity(rootEntity);
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
    auto rightIsol = new Qt3DCore::QEntity(rootEntity);
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
    auto coil1 = new Qt3DCore::QEntity(rootEntity);
    coil1->addComponent(m_coilCubeMesh1);
    coil1->addComponent(coilMaterial);
    m_coilTransform1 = new Qt3DCore::QTransform();
    m_coilTransform1->setScale(1.0f);
    coil1->addComponent(m_coilTransform1);

    m_coilCubeMesh2  = new Qt3DExtras::QCuboidMesh();
    auto coil2 = new Qt3DCore::QEntity(rootEntity);
    coil2->addComponent(m_coilCubeMesh2);
    coil2->addComponent(coilMaterial);
    m_coilTransform2 = new Qt3DCore::QTransform();
    m_coilTransform2->setScale(1.0f);
    coil2->addComponent(m_coilTransform2);

    m_coilCylMesh1 = new Qt3DExtras::QCylinderMesh();
    m_coilCylMesh1->setRings(10);
    m_coilCylMesh1->setSlices(30);
    m_coilCylTransform1 = new Qt3DCore::QTransform();
    m_coilCylTransform1->setScale(1.0f);
    m_coilCylTransform1->setRotationZ(90);
    auto coilCyl1 = new Qt3DCore::QEntity(rootEntity);
    coilCyl1->addComponent(m_coilCylMesh1);
    coilCyl1->addComponent(coilMaterial);
    coilCyl1->addComponent(m_coilCylTransform1);

    m_coilCylMesh2 = new Qt3DExtras::QCylinderMesh();
    m_coilCylMesh2->setRings(10);
    m_coilCylMesh2->setSlices(30);
    m_coilCylTransform2 = new Qt3DCore::QTransform();
    m_coilCylTransform2->setScale(1.0f);
    m_coilCylTransform2->setRotationZ(90);
    auto coilCyl2 = new Qt3DCore::QEntity(rootEntity);
    coilCyl2->addComponent(m_coilCylMesh2);
    coilCyl2->addComponent(coilMaterial);
    coilCyl2->addComponent(m_coilCylTransform2);

    m_coilCylMesh3 = new Qt3DExtras::QCylinderMesh();
    m_coilCylMesh3->setRings(10);
    m_coilCylMesh3->setSlices(30);
    m_coilCylTransform3 = new Qt3DCore::QTransform();
    m_coilCylTransform3->setScale(1.0f);
    m_coilCylTransform3->setRotationZ(90);
    auto coilCyl3 = new Qt3DCore::QEntity(rootEntity);
    coilCyl3->addComponent(m_coilCylMesh3);
    coilCyl3->addComponent(coilMaterial);
    coilCyl3->addComponent(m_coilCylTransform3);

    m_coilCylMesh4 = new Qt3DExtras::QCylinderMesh();
    m_coilCylMesh4->setRings(10);
    m_coilCylMesh4->setSlices(30);
    m_coilCylTransform4 = new Qt3DCore::QTransform();
    m_coilCylTransform4->setScale(1.0f);
    m_coilCylTransform4->setRotationZ(90);
    auto coilCyl4 = new Qt3DCore::QEntity(rootEntity);
    coilCyl4->addComponent(m_coilCylMesh4);
    coilCyl4->addComponent(coilMaterial);
    coilCyl4->addComponent(m_coilCylTransform4);

    // set sizes
    const tsizes sz = {a_m, b_m, l_m, l_h, b_h, b_x, a_gap};

    updateSizes(sz);
}

void MagSusp3D::updateSizes(const tsizes& sz)
{
    a_m = sz.a.has_value() ? sz.a.value() : a_m;
    b_m = sz.b.has_value() ? sz.b.value() : b_m;
    l_m = sz.h.has_value() ? sz.h.value() : l_m;
    l_h = sz.l.has_value() ? sz.l.value() : l_h;
    b_h = sz.c.has_value() ? sz.c.value() : b_h;
    b_x = sz.b_x.has_value() ? sz.b_x.value() : b_x;
    a_gap = sz.gap.has_value() ? sz.gap.value() : a_gap;
    del_m = sz.delm.has_value() ? sz.delm.value() : del_m;
    h_p = sz.h_p.has_value() ? sz.h_p.value() : h_p;

    // anchor
    m_anchorCubeMesh->setXExtent(l_h);
    m_anchorCubeMesh->setYExtent(b_x);
    m_anchorCubeMesh->setZExtent(a_m);
    m_anchorCubeTransform->setTranslation(QVector3D(0, l_m/2 + a_gap + b_x/2, 0));

    // left cube
    m_leftCubeMesh->setXExtent(b_m);
    m_leftCubeMesh->setYExtent(l_m);
    m_leftCubeMesh->setZExtent(a_m);
    m_leftCubeTransform->setTranslation(QVector3D(b_m/2 - l_h/2, 0, 0));

    // right cube
    m_rightCubeMesh->setXExtent(b_m);
    m_rightCubeMesh->setYExtent(l_m);
    m_rightCubeMesh->setZExtent(a_m);
    m_rightCubeTransform->setTranslation(QVector3D(l_h/2 - b_m/2, 0, 0));

    // left isol
    m_leftIsolMesh->setXExtent(del_m);
    auto isol_h = b_h + 2*del_m + 2*h_p + 0.1*h_p;
    m_leftIsolMesh->setYExtent(isol_h);
    auto isol_w = a_m + 2*del_m + 2*h_p + 0.1*h_p;
    m_leftIsolMesh->setZExtent(isol_w);
    m_leftIsolTransform->setTranslation({b_m - l_h/2 + del_m/2, -l_m/2 + b_h/2, 0});

    // right isol
    m_rightIsolMesh->setXExtent(del_m);
    m_rightIsolMesh->setYExtent(isol_h);
    m_rightIsolMesh->setZExtent(isol_w);
    m_rightIsolTransform->setTranslation({-b_m + l_h/2 - del_m/2, -l_m/2 + b_h/2, 0});

    // coil crossed cubes
    auto crWidth = l_h - 2*b_m - 2*del_m;
    m_coilCubeMesh1->setXExtent(crWidth);
    m_coilCubeMesh1->setYExtent(b_h + 2*del_m + 2*h_p - 2*rd);
    m_coilCubeMesh1->setZExtent(a_m + 2*del_m + 2*h_p);
    m_coilTransform1->setTranslation({0, -l_m/2 + b_h/2, 0});

    m_coilCubeMesh2->setXExtent(crWidth);
    m_coilCubeMesh2->setYExtent(b_h + 2*del_m + 2*h_p);
    m_coilCubeMesh2->setZExtent(a_m + 2*del_m + 2*h_p - 2*rd);
    m_coilTransform2->setTranslation({0, -l_m/2 + b_h/2, 0});

    auto cylLen = l_h - 2*b_m - 2*del_m;
    m_coilCylMesh1->setLength(cylLen);
    m_coilCylMesh1->setRadius(rd);
    m_coilCylTransform1->setTranslation({0,
            -l_m/2 + b_h + del_m + h_p - rd,
            a_m/2 + del_m + h_p - rd});

    m_coilCylMesh2->setLength(cylLen);
    m_coilCylMesh2->setRadius(rd);
    m_coilCylTransform2->setTranslation({0,
            -l_m/2 - del_m - h_p + rd,
            a_m/2 + del_m + h_p - rd});

    m_coilCylMesh3->setLength(cylLen);
    m_coilCylMesh3->setRadius(rd);
    m_coilCylTransform3->setTranslation({0,
            -l_m/2 - del_m - h_p + rd,
            -a_m/2 - del_m - h_p + rd});

    m_coilCylMesh4->setLength(cylLen);
    m_coilCylMesh4->setRadius(rd);
    m_coilCylTransform4->setTranslation({0,
            -l_m/2 + b_h + del_m + h_p - rd,
            -a_m/2 - del_m - h_p + rd});
}
