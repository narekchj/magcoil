#ifndef SCENEMODIFIER_H
#define SCENEMODIFIER_H

#include <iostream>
#include <optional>
#include <QtCore/QObject>
#include <QLineEdit>

#include "magsusp3d.h"
#include "consts.hpp"
#include "ratio_mag_model.hpp"

class QVBoxLayout;
class QHBoxLayout;

class QLabel;
class QLayout;
class QDoubleSpinBox;
class QTextEdit;
class SceneModifier;


struct CBlock 
{
    QLabel* label;
    QLayout* layout;
    QDoubleSpinBox* spinBox;
    using Action = void (SceneModifier::*)(double);
    Action act;
};

class SceneModifier : public QObject
{
    Q_OBJECT

public:
    explicit SceneModifier(Qt3DCore::QEntity *rootEntity, QWidget* parent);
    ~SceneModifier() = default;

public:
    QVBoxLayout* getControlLayout() const 
    {
        return m_vLay;
    }

public slots:
    void setAm(double in);
    void setBm(double in);
    void setLm(double in);
    void setLh(double in);
    void setBh(double in);
    void setBx(double in);
    void setHp(double in);
    void setAGap(double in);
    void setDelm(double in);

    void setKe(double in);
    void setKm(double in);
    void setKmm(double in);
    void setKx(double in);
    void setKh(double in);
    void setKdel(double in);
    void setKp(double in);
    void setBAir(double in);

    void startCalculation();
    void showFinalResult();
    void reInitSusp();

private:
    void updateSizes();
    void updateK();

private:
    CBlock Am;
    CBlock Bm;
    CBlock Lm;
    CBlock Lh;
    CBlock Bh;
    CBlock Bx;
    CBlock Hp;
    CBlock AGap;
    CBlock Delm;

    CBlock Ke;
    CBlock Km;
    CBlock Kmm;
    CBlock Kx;
    CBlock Kh;
    CBlock Kdel;
    CBlock Kp;
    CBlock BAir;


private:
    Qt3DCore::QEntity *m_rootEntity = nullptr;
    QWidget* m_Parent = nullptr;
    MagSusp3D* m_susp = nullptr;
    QVBoxLayout* m_vLay = nullptr;
    QHBoxLayout* m_hLay = nullptr;
    QTextEdit* mResultsDialog = nullptr;

private:
    using rm_t = ratio_model<ratio_susp_data>;
    rm_t rm;
    std::unique_ptr<ratio_susp_data> sp_data;
};

#endif // SCENEMODIFIER_H
