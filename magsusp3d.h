#ifndef MAGSUSP3D_H
#define MAGSUSP3D_H

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

#include <consts.hpp>

class MagSusp3D : public QObject
{
    Q_OBJECT

public:
    explicit MagSusp3D(Qt3DCore::QEntity *rootEntity);
    ~MagSusp3D() = default;

public slots:
    void getRes()
    {}

private:
    void updateSizes(const tsizes& sz);

private:
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
    Qt3DCore::QTransform* m_coilTransform1 = nullptr;

    Qt3DExtras::QCuboidMesh* m_coilCubeMesh2 = nullptr;
    Qt3DCore::QTransform* m_coilTransform2 = nullptr;

    Qt3DExtras::QCylinderMesh* m_coilCylMesh1 = nullptr;
    Qt3DCore::QTransform*  m_coilCylTransform1 = nullptr;

    Qt3DExtras::QCylinderMesh* m_coilCylMesh2 = nullptr;
    Qt3DCore::QTransform*  m_coilCylTransform2 = nullptr;

    Qt3DExtras::QCylinderMesh* m_coilCylMesh3 = nullptr;
    Qt3DCore::QTransform*  m_coilCylTransform3 = nullptr;

    Qt3DExtras::QCylinderMesh* m_coilCylMesh4 = nullptr;
    Qt3DCore::QTransform*  m_coilCylTransform4 = nullptr;

private:
    float a_m = 1.0f;
    float b_m = 1.0f;
    float l_m = 2.2f;
    float l_h = 3.3f;
    float b_h = 1.0f;
    float b_x = 0.6f;
    float l_x = l_h;
    float a_gap = 0.1f;
    float del_m = 0.05f;
    float h_p = 0.5f;

    // roundness of coil
    float rd = 0.3f;
};

#endif // MAGSUSP3D_H
