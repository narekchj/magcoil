#include "scenemodifier.h"
#include <unistd.h>

#include <QtCore/QDebug>
#include <Qt3DExtras/QMetalRoughMaterial>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include "calc_thread.h"

namespace {

    using Slot = void (SceneModifier::*)(double);
    struct COpts {
        QString label;
        float initVal;
        SceneModifier* sc;
        Slot fun;
        float step = 0.005f;
        float min = 0.0f;
        float max = 1.0f;
        bool enab = true;
    };

    CBlock createCtrlButton(COpts opts)
    {
        QHBoxLayout* lay1 = new QHBoxLayout;

        QLabel* lab1 = new QLabel(opts.label);
        QDoubleSpinBox* box1 = new QDoubleSpinBox;
        box1->setEnabled(opts.enab);
        box1->setDecimals(4);
        box1->setValue(opts.initVal);
        box1->setMinimum(opts.min);
        box1->setMaximum(opts.max);
        box1->setSingleStep(opts.step);
        lay1->addSpacing(50);
        lay1->addWidget(lab1);
        lay1->addWidget(box1);
        lay1->addSpacing(100);

        using ft = void (QDoubleSpinBox::*)(double);

        QObject::connect(box1,
                static_cast<ft>(&QDoubleSpinBox::valueChanged),
                opts.sc,
                opts.fun);

        return {lab1, lay1, box1, opts.fun};
    }


    using Slot1 = void (SceneModifier::*)();
    QLayout* createButton(const QString& Name, SceneModifier* sc, Slot1 fun)
    {
        QHBoxLayout* lay1 = new QHBoxLayout;
        QPushButton* but1 = new QPushButton(Name);
        lay1->addWidget(but1);

        QObject::connect(but1,
                &QPushButton::clicked,
                sc,
                fun);

        return lay1;
    }

}

SceneModifier::SceneModifier(Qt3DCore::QEntity* rootEntity, QWidget* parent)
    : m_rootEntity(rootEntity)
    , m_Parent(parent)
    , rm({200000, 20})
    , sp_data(std::make_unique<ratio_susp_data>())
{
    m_susp = new MagSusp3D(rootEntity);


    QVBoxLayout* laySize = new QVBoxLayout();
    laySize->setAlignment(Qt::AlignTop);
    Am = createCtrlButton({"aմ (մ)", m_susp->getAm(), this, &SceneModifier::setAm});
    laySize->addLayout(Am.layout);
    Am.spinBox->setEnabled(false);

    Bm = createCtrlButton({"bմ (մ)", m_susp->getBm(), this, &SceneModifier::setBm});
    laySize->addLayout(Bm.layout);
    Bm.spinBox->setEnabled(false);

    Lm = createCtrlButton({"lմ (մ)", m_susp->getLm(), this, &SceneModifier::setLm});
    laySize->addLayout(Lm.layout);
    Lm.spinBox->setEnabled(false);

    Lh = createCtrlButton({"lհ (մ)", m_susp->getLh(), this, &SceneModifier::setLh});
    laySize->addLayout(Lh.layout);
    Lh.spinBox->setEnabled(false);

    Bh = createCtrlButton({"bհ (մ)", m_susp->getBh(), this, &SceneModifier::setBh});
    laySize->addLayout(Bh.layout);
    Bh.spinBox->setEnabled(false);

    Bx = createCtrlButton({"bխ (մ)", m_susp->getBx(), this, &SceneModifier::setBx});
    laySize->addLayout(Bx.layout);
    Bx.spinBox->setEnabled(false);

    Hp = createCtrlButton({"hփ (մ)", m_susp->getHp(), this, &SceneModifier::setHp});
    laySize->addLayout(Hp.layout);
    Hp.spinBox->setEnabled(false);

    AGap = createCtrlButton({"δ (մ)", m_susp->getAGap(), this, &SceneModifier::setAGap});
    laySize->addLayout(AGap.layout);
    AGap.spinBox->setEnabled(false);

    Delm = createCtrlButton({"Δմ (մ)", 0.005, this, &SceneModifier::setDelm, 0.0005f});
    laySize->addLayout(Delm.layout);
    Delm.spinBox->setEnabled(false);


    rm.init_suspension(*sp_data);

    QVBoxLayout* layK = new QVBoxLayout();
    layK->setAlignment(Qt::AlignTop);
    QString keStr = QString("ke [%1 - %2]")
        .arg(QString::number(range_k_e.first), QString::number(range_k_e.second));
    Ke = createCtrlButton({keStr, sp_data->cpack.k_e.value(), 
            this, &SceneModifier::setKe,
            0.001,
            range_k_e.first,
            range_k_e.second});
    layK->addLayout(Ke.layout);

    QString kmStr = QString("km [%1 - %2]")
        .arg(QString::number(range_k_m.first), QString::number(range_k_m.second));
    Km = createCtrlButton({kmStr, sp_data->cpack.k_m.value(), 
            this, &SceneModifier::setKm,
            0.001,
            range_k_m.first,
            range_k_m.second});
    layK->addLayout(Km.layout);

    QString kmmStr = QString("kmm [%1 - %2]")
        .arg(QString::number(range_k_mm.first), QString::number(range_k_mm.second));
    Kmm = createCtrlButton({kmmStr, sp_data->cpack.k_mm.value(), 
            this, &SceneModifier::setKmm,
            0.001,
            range_k_mm.first,
            range_k_mm.second});
    layK->addLayout(Kmm.layout);

    QString kxStr = QString("kx [%1 - %2]")
        .arg(QString::number(range_k_x.first), QString::number(range_k_x.second));
    Kx = createCtrlButton({kxStr, sp_data->cpack.k_x.value(), 
            this, &SceneModifier::setKx,
            0.001,
            range_k_x.first,
            range_k_x.second});
    layK->addLayout(Kx.layout);

    QString khStr = QString("kh [%1 - %2]")
        .arg(QString::number(range_k_h.first), QString::number(range_k_h.second));
    Kh = createCtrlButton({khStr, sp_data->cpack.k_h.value(), 
            this, &SceneModifier::setKh,
            0.001,
            range_k_h.first,
            range_k_h.second});
    layK->addLayout(Kh.layout);

    QString kdelStr = QString("kd [%1 - %2]")
        .arg(QString::number(range_k_delta.first),
                QString::number(range_k_delta.second));
    Kdel = createCtrlButton({kdelStr, sp_data->cpack.k_delta.value(), 
            this, &SceneModifier::setKdel,
            0.0005,
            range_k_delta.first,
            range_k_delta.second});
    layK->addLayout(Kdel.layout);

    QString kpStr = QString("kp [%1 - %2]")
        .arg(QString::number(range_k_p.first),
                QString::number(range_k_p.second));
    Kp = createCtrlButton({kpStr, sp_data->cpack.k_p.value(), 
            this, &SceneModifier::setKp,
            0.001,
            range_k_p.first,
            range_k_p.second});
    layK->addLayout(Kp.layout);

    QString BAirStr = QString("B [%1 - %2]")
        .arg(QString::number(range_B_air.first),
                QString::number(range_B_air.second));
    BAir = createCtrlButton({BAirStr, sp_data->cpack.B_air.value(), 
            this, &SceneModifier::setBAir,
            0.001,
            range_B_air.first,
            range_B_air.second});
    layK->addLayout(BAir.layout);

    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->addLayout(laySize);
    hLay->addLayout(layK);

    m_vLay = new QVBoxLayout();
    m_vLay->addLayout(hLay);

    m_vLay->addSpacing(100);
    m_vLay->addLayout(createButton("Generate random K set and Calculate", this, &SceneModifier::reInitSusp));
    m_vLay->addSpacing(10);
    m_vLay->addLayout(createButton("Calculate", this, &SceneModifier::startCalculation));

    m_vLay->addSpacing(100);
    QLabel* resLabel = new QLabel("Results");
    m_vLay->addWidget(resLabel);
    m_vLay->setAlignment(resLabel, Qt::AlignHCenter);
    mResultsDialog = new QTextEdit;
    mResultsDialog->setFixedSize(QSize(300,300));
    mResultsDialog->setTextInteractionFlags(Qt::NoTextInteraction);
    m_vLay->addWidget(mResultsDialog);
    m_vLay->setAlignment(mResultsDialog, Qt::AlignHCenter);
}

void SceneModifier::setAm(double in)
{
    float val = 10.0f * in;
    const tsizes sz = {val};
    m_susp->updateSizes(sz);

    auto& sp = rm.get_susp();
    sp.set_a_m(in);

}

void SceneModifier::setBm(double in)
{
    float val = 10.0f * in;
    const tsizes sz = {{}, val};
    m_susp->updateSizes(sz);

    auto& sp = rm.get_susp();
    sp.set_b_m(in);
}

void SceneModifier::setLm(double in)
{
    float val = 10.0f * in;
    const tsizes sz = {{}, {}, val};
    m_susp->updateSizes(sz);

    auto& sp = rm.get_susp();
    sp.set_l_m(in);
}

void SceneModifier::setLh(double in)
{
    float val = 10.0f * in;
    const tsizes sz = {{}, {}, {}, val};
    m_susp->updateSizes(sz);

    auto& sp = rm.get_susp();
    sp.set_l_h(in);
}

void SceneModifier::setBh(double in)
{
    float val = 10.0f * in;
    const tsizes sz = {{}, {}, {}, {}, val};
    m_susp->updateSizes(sz);

    auto& sp = rm.get_susp();
    sp.set_b_h(in);
}

void SceneModifier::setBx(double in)
{
    float val = 10.0f * in;
    const tsizes sz = {{}, {}, {}, {}, {}, val};
    m_susp->updateSizes(sz);

    auto& sp = rm.get_susp();
    sp.set_b_x(in);
}

void SceneModifier::setAGap(double in)
{
    float val = 10.0f * in;
    const tsizes sz = {{}, {}, {}, {}, {}, {}, val};
    m_susp->updateSizes(sz);

    auto& sp = rm.get_susp();
    sp.set_air_gap(in);
}

void SceneModifier::setDelm(double in)
{
    float val = 10.0f * in;
    const tsizes sz = {{}, {}, {}, {}, {}, {}, {}, val};
    m_susp->updateSizes(sz);

    auto& sp = rm.get_susp();
    sp.set_Delta_m(in);
}

void SceneModifier::setHp(double in)
{
    float val = 10.0f * in;
    const tsizes sz = {{}, {}, {}, {}, {}, {}, {}, {}, val};
    m_susp->updateSizes(sz);

    auto& sp = rm.get_susp();
    sp.set_h_p(in);
}

void SceneModifier::setKe(double in)
{
    sp_data->cpack.k_e = in;
    rm.init_suspension(*sp_data);
    updateSizes();
}

void SceneModifier::setKm(double in)
{
    sp_data->cpack.k_m = in;
    rm.init_suspension(*sp_data);
    updateSizes();
}

void SceneModifier::setKmm(double in)
{
    sp_data->cpack.k_mm = in;
    rm.init_suspension(*sp_data);
    updateSizes();
}

void SceneModifier::setKx(double in)
{
    sp_data->cpack.k_x = in;
    rm.init_suspension(*sp_data);
    updateSizes();
}

void SceneModifier::setKh(double in)
{
    sp_data->cpack.k_h = in;
    rm.init_suspension(*sp_data);
    updateSizes();
}

void SceneModifier::setKdel(double in)
{
    sp_data->cpack.k_delta = in;
    rm.init_suspension(*sp_data);
    updateSizes();
}

void SceneModifier::setKp(double in)
{
    sp_data->cpack.k_p = in;
    rm.init_suspension(*sp_data);
    updateSizes();
}

void SceneModifier::setBAir(double in)
{
    sp_data->cpack.B_air = in;
    rm.init_suspension(*sp_data);
    updateSizes();
}

void SceneModifier::reInitSusp()
{
    sp_data->resetInput();
    rm.init_suspension(*sp_data);
    updateSizes();
    updateK();

    startCalculation();
}

void SceneModifier::updateSizes()
{
    auto& sp = rm.get_susp();
    Am.spinBox->setValue(sp.get_a_m());
    Bm.spinBox->setValue(sp.get_b_m());
    Lm.spinBox->setValue(sp.get_l_m());
    Lh.spinBox->setValue(sp.get_l_h());
    Bh.spinBox->setValue(sp.get_b_h());
    Bx.spinBox->setValue(sp.get_b_x());
    Hp.spinBox->setValue(sp.get_h_p());
    AGap.spinBox->setValue(sp.get_air_gap());
    Delm.spinBox->setValue(sp.get_Delta_m());
}

void SceneModifier::updateK()
{
    Ke.spinBox->setValue(sp_data->cpack.k_e.value());
    Km.spinBox->setValue(sp_data->cpack.k_m.value());
    Kmm.spinBox->setValue(sp_data->cpack.k_mm.value());
    Kx.spinBox->setValue(sp_data->cpack.k_x.value());
    Kh.spinBox->setValue(sp_data->cpack.k_h.value());
    Kdel.spinBox->setValue(sp_data->cpack.k_delta.value());
    Kp.spinBox->setValue(sp_data->cpack.k_p.value());
    BAir.spinBox->setValue(sp_data->cpack.B_air.value());
}

void SceneModifier::showFinalResult()
{
    mResultsDialog->clear();
    mResultsDialog->setAlignment(Qt::AlignCenter);
    QString str1 = {"F = " + QString::number(sp_data->dir_out.F_mshu, 'f') + " Ն\n"}; 
    str1 += "Pէ = " + QString::number(sp_data->dir_in.P, 'f') + " Ն\n"; 
    str1 += "P = " + QString::number(sp_data->coil_out.P, 'f') + " Վտ\n"; 
    mResultsDialog->insertPlainText(str1);
   
    auto T = sp_data->coil_out.T;
    QString str2 = {"θփ = " + QString::number(T, 'f') + " °C\n"}; 
    mResultsDialog->setTextColor( (T >= 130 && T <=160) ? Qt::darkGreen : Qt::red);
    mResultsDialog->insertPlainText(str2);

    QString str3 = {"W = " + QString::number(std::ceil(sp_data->coil_out.W), 'f',1) +  "\n"}; 
    str3 += "Cost = " + QString::number(std::ceil(sp_data->other.Price), 'f',1) +  "$\n"; 
    mResultsDialog->setTextColor(Qt::black);
    mResultsDialog->insertPlainText(str3);
}

void SceneModifier::startCalculation()
{
    rm.calculate_direct(*sp_data);
    rm.calculate_reverse(*sp_data);
    rm.calculate_coil(*sp_data);
    calculate_price(*sp_data, rm.get_susp());

    //rm.calculate_reverse(*sp_data);
    //rm.calculate_coil(*sp_data);
    showFinalResult();
}

