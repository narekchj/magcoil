#ifndef SCENEMODIFIER_H
#define SCENEMODIFIER_H

#include <iostream>
#include <optional>
#include <QtCore/QObject>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>

typedef struct tsizes
{
    std::optional<float> a = {};
    std::optional<float> b = {};
    std::optional<float> h = {};
    std::optional<float> l = {};
    std::optional<float> c = {};
    std::optional<float> b_x = {};
    std::optional<float> gap = {};
    std::optional<float> h_p = {};
} tsizes;

class SceneModifier : public QObject
{
    Q_OBJECT

    public:

public:
    explicit SceneModifier(Qt3DCore::QEntity *rootEntity);
    ~SceneModifier() = default;

public slots:
    void getRes()
    {
        std::cout << "Got results\n";
    }
    void aValChanged(int val)
    {
        auto yval = float(val)/100.0f;
        const tsizes sz = {yval, {}, {}, {}, {}, {}, {}};

        updateSizes(sz);
    }

    void bValChanged(int val)
    {
        auto yval = float(val)/100.0f;
        const tsizes sz = {{}, yval, {}, {}, {}, {}, {}};

        updateSizes(sz);
    }


    void hValChanged(int val)
    {
        auto yval = float(val)/100.0f;
        const tsizes sz = {{}, {}, yval, {}, {}, {}, {}};

        updateSizes(sz);
    }

    void lValChanged(int val)
    {
        auto yval = float(val)/100.0f;
        const tsizes sz = {{}, {}, {}, yval, {}, {}, {}};

        updateSizes(sz);
    }

    void cValChanged(int val)
    {
        auto yval = float(val)/100.0f;
        const tsizes sz = {{}, {}, {}, {}, yval, {}, {}};

        updateSizes(sz);
    }

    void anchorValChanged(int val)
    {
        auto yval = float(val)/100.0f;
        const tsizes sz = {{}, {}, {}, {}, {}, yval, {}};
        updateSizes(sz);
    }

    void gapValChanged(int val)
    {
        auto yval = float(val)/100.0f;
        const tsizes sz = {{}, {}, {}, {}, {}, {}, yval};

        updateSizes(sz);
    }

    void startCalculation();

    void updateSizes(const tsizes& sz)
    {
        a = sz.a.has_value() ? sz.a.value() : a;
        b = sz.b.has_value() ? sz.b.value() : b;
        h = sz.h.has_value() ? sz.h.value() : h;
        l = sz.l.has_value() ? sz.l.value() : l;
        c = sz.c.has_value() ? sz.c.value() : c;
        b_x = sz.b_x.has_value() ? sz.b_x.value() : b_x;
        gap = sz.gap.has_value() ? sz.gap.value() : gap;
        h_isol = sz.h_p.has_value() ? sz.h_p.value() : h_isol;
        h_isol1 = h_isol - h_isol*0.01f;

        // anchor
        m_anchorCubeMesh->setXExtent(l);
        m_anchorCubeMesh->setYExtent(b_x);
        m_anchorCubeMesh->setZExtent(a);
        m_anchorCubeTransform->setTranslation(QVector3D(0, h - c/2 + gap + b_x/2, 0));

        // left cube
        m_leftCubeMesh->setXExtent(b);
        m_leftCubeMesh->setYExtent(h);
        m_leftCubeMesh->setZExtent(a);
        m_leftCubeTransform->setTranslation(QVector3D(b/2 - l/2, h/2 - c/2, 0));

        // right cube
        m_rightCubeMesh->setXExtent(b);
        m_rightCubeMesh->setYExtent(h);
        m_rightCubeMesh->setZExtent(a);
        m_rightCubeTransform->setTranslation(QVector3D(l/2 - b/2, h/2 - c/2, 0));

        // left isol
        m_leftIsolMesh->setXExtent(d_isol);
        m_leftIsolMesh->setYExtent(c + 2*h_isol);
        m_leftIsolMesh->setZExtent(a + 2*h_isol);
        m_leftIsolTransform->setTranslation(QVector3D(b - l/2 + d_isol/2, 0, 0));

        // right isol
        m_rightIsolMesh->setXExtent(d_isol);
        m_rightIsolMesh->setYExtent(c + 2*h_isol);
        m_rightIsolMesh->setZExtent(a + 2*h_isol);
        m_rightIsolTransform->setTranslation(QVector3D(-b + l/2 - d_isol/2, 0, 0));

        m_coilCubeMesh1->setXExtent(l- 2*b - 2*d_isol);
        m_coilCubeMesh1->setYExtent(c + 2*h_isol1 - 2*rd);
        m_coilCubeMesh1->setZExtent(a + 2*h_isol1);

        m_coilCubeMesh2->setXExtent(l- 2*b - 2*d_isol);
        m_coilCubeMesh2->setYExtent(c + 2*h_isol1);
        m_coilCubeMesh2->setZExtent(a + 2*h_isol1 - 2*rd);

        m_coilCylMesh1->setLength(l- 2*b - 2*d_isol);
        m_coilCylMesh1->setRadius(rd);
        m_coilCylTransform1->setTranslation(QVector3D(0, c/2 + h_isol1 - rd, a/2 + h_isol1 - rd));

        m_coilCylMesh2->setLength(l- 2*b - 2*d_isol);
        m_coilCylMesh2->setRadius(rd);
        m_coilCylTransform2->setTranslation(QVector3D(0, c/2 + h_isol1 - rd, -a/2 - h_isol1 + rd));

        m_coilCylMesh3->setLength(l- 2*b - 2*d_isol);
        m_coilCylMesh3->setRadius(rd);
        m_coilCylTransform3->setTranslation(QVector3D(0, -c/2 - h_isol1 + rd, -a/2 - h_isol1 + rd));

        m_coilCylMesh4->setLength(l- 2*b - 2*d_isol);
        m_coilCylMesh4->setRadius(rd);
        m_coilCylTransform4->setTranslation(QVector3D(0, -c/2 - h_isol1 + rd, a/2 + h_isol1 - rd));
    }

private:
    Qt3DCore::QEntity *m_rootEntity = nullptr;

    Qt3DExtras::QCuboidMesh* m_anchorCubeMesh = nullptr;
    Qt3DCore::QTransform* m_anchorCubeTransform = nullptr;

    Qt3DExtras::QCuboidMesh* m_leftCubeMesh = nullptr;
    Qt3DCore::QTransform* m_leftCubeTransform = nullptr;
    
    Qt3DExtras::QCuboidMesh* m_rightCubeMesh = nullptr;
    Qt3DCore::QTransform* m_rightCubeTransform = nullptr;

    Qt3DExtras::QCuboidMesh* m_leftIsolMesh = nullptr;
    Qt3DCore::QTransform* m_leftIsolTransform = nullptr;

    Qt3DExtras::QCuboidMesh* m_rightIsolMesh = nullptr;
    Qt3DCore::QTransform* m_rightIsolTransform = nullptr;

    Qt3DExtras::QCuboidMesh* m_coilCubeMesh1 = nullptr;
    Qt3DExtras::QCuboidMesh* m_coilCubeMesh2 = nullptr;

    Qt3DExtras::QCylinderMesh* m_coilCylMesh1 = nullptr;
    Qt3DCore::QTransform*  m_coilCylTransform1 = nullptr;

    Qt3DExtras::QCylinderMesh* m_coilCylMesh2 = nullptr;
    Qt3DCore::QTransform*  m_coilCylTransform2 = nullptr;

    Qt3DExtras::QCylinderMesh* m_coilCylMesh3 = nullptr;
    Qt3DCore::QTransform*  m_coilCylTransform3 = nullptr;

    Qt3DExtras::QCylinderMesh* m_coilCylMesh4 = nullptr;
    Qt3DCore::QTransform*  m_coilCylTransform4 = nullptr;


private:
    float a = 1.0f;
    float b = 1.0f;
    float h = 1.6f;
    float l = 5.3f;
    float c = 1.0f;
    float b_x = 0.6f;
    float gap = 0.2f;

    float h_isol = 0.3f;
    float h_isol1 = h_isol - h_isol*0.01f;
    float d_isol = 0.1f;
    float rd = 0.5f;
};

#endif // SCENEMODIFIER_H
