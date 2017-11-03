#ifndef HWP_QWP_H_
#define HWP_QWP_H_

#include "../core/module.h"

// HWP + QWP    -   half / quarter wave plates

// E_+ = E_1 e^(i psi) cos (xi - pi/4)
// E_- = E_1 e^(i psi) cos (xi + pi/4)

class HWP_QWP : public Module {
    double psi, xi;

  public:
    HWP_QWP();
    HWP_QWP(const double& in_psi, const double& in_xi);

    void execute(Field* signal);
    void execute(Polarizations* signal);
};

#endif  // HWP_QWP_H_