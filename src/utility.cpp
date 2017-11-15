#include "utility.h"

double sinc(const double& x) {
    if (x == 0) return 1;
    return std::sin(M_PI * x) / M_PI / x;
}

double db_to_linear(const double& db_value) {
    return std::pow(10, db_value / 10);
}

double dbm_to_watts(const double& dbm_power) {
    return 1e-3 * std::pow(10, dbm_power / 10);
}

double db_to_natural(const double& db_value) {
    return db_value * std::log(10) / 10;
}

double disp_to_beta2(const double& dispersion, const double& wavelength) {
    return -wavelength * wavelength * dispersion /
           (2 * math_pi * light_speed::mps);
}

Complex i_exp(const double& x) { return Complex(std::cos(x), std::sin(x)); }

Field sqrt(const Field& field) {
    Field copy(field);
    for (unsigned long i = 0; i < field.size(); ++i)
        copy[i] = sqrt(field[i].real());
    return copy;
}

Field sech_pulse(const int& nodes_quantity, const double& width) {
    Field pulse(nodes_quantity, 0);
    double argument;
    for (int i = 0; i < nodes_quantity; i++) {
        argument = double(i) / (nodes_quantity - 1.0);
        argument = 20 * argument - 20 / 2.0;
        pulse[i] = 1.0 / std::cosh(argument / width);
        pulse[i] /= width;
    }

    pulse.setSamplingRate(nodes_quantity / 4.0);

    return pulse;
}

Field gaussian(const int& nodes_quantity,
    const double& peak_power,
    const double& fwhm,
    const double& grid_step) {
    Field pulse(nodes_quantity, sqrt(peak_power));
    double arg;
    for (int i = 0; i < nodes_quantity; ++i) {
        arg = grid_step * double((i - nodes_quantity / 2));
        pulse[i] *= exp(-arg * arg / fwhm / fwhm * 2.0 * log(2.0));
    }
    pulse.setTimeStep(grid_step);
    return pulse;
}

Field lorentzian(const int& nodes_quantity,
    const double& peak_power,
    const double& fwhm,
    const double& grid_step) {
    Field pulse(nodes_quantity, sqrt(peak_power));
    double arg;
    for (int i = 0; i < nodes_quantity; ++i) {
        arg = grid_step * double((i - nodes_quantity / 2));
        pulse[i] *= 1.0 / (1.0 + 4.0 * arg * arg / fwhm / fwhm);
    }
    pulse.setTimeStep(grid_step);
    return pulse;
}