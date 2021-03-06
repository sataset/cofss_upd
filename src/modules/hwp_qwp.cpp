#include "hwp_qwp.h"
#include "../utility.h"

HWP_QWP::HWP_QWP() {
    name = "hwp+qwp";
    psi_ = 0;
    chi_ = 0;
}

HWP_QWP::HWP_QWP(const double& psi, const double& xi) {
    name = "hwp+qwp";
    psi_ = psi;
    chi_ = xi;
}

void HWP_QWP::execute(Field* signal) {}

void HWP_QWP::execute(Polarizations* signal) {
    // Field E_1 = signal->x();
    // temporary dog-nail
    Field E_1 = signal->right;
    signal->right = E_1 * i_exp(psi_) * std::cos(chi_ - math_pi / 4.0);
    signal->left = E_1 * i_exp(-psi_) * std::cos(chi_ + math_pi / 4.0);

    // Executor::instance()->enqueue(next, signal);
}

void HWP_QWP::change_psi(double psi) { psi_ = psi; }

void HWP_QWP::change_chi(double chi) { chi_ = chi; }