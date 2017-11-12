#include "field.h"

double Field::peak_power() const {
    double power = 0;
    for (unsigned long i = 0; i < size(); ++i)
        if (power < norm(at(i))) power = norm(at(i));

    return power;
}

double Field::average_power() const {
    double power = 0;
    for (unsigned long i = 0; i < size(); ++i)
        power += norm(at(i));

    return power / size();
}

Field Field::chomp(const unsigned long& at_begin,
                   const unsigned long& at_end) const {
    Field chomped(size() - at_begin - at_end);
    for (unsigned long i = 0; i < chomped.size(); ++i) {
        chomped[i] = at(i + at_begin);
    }

    return chomped;
}

Field Field::operator+(const Field& summands) const {
    Field copy(*this);
    if (size() == summands.size())
        for (unsigned long i = 0; i < copy.size(); ++i)
            copy[i] += summands[i];
    else
        throw std::logic_error("fields size mismatch");

    return copy;
}

Field& Field::operator+=(const Field& summands) {
    if (size() == summands.size())
        for (unsigned long i = 0; i < size(); ++i)
            at(i) += summands[i];
    else
        throw std::logic_error("fields size mismatch");
    return *this;
}

Field Field::operator-(const Field& subtrahends) const {
    Field copy(*this);
    if (size() == subtrahends.size())
        for (unsigned long i = 0; i < copy.size(); ++i)
            copy[i] -= subtrahends[i];
    else
        throw std::logic_error("fields size mismatch");
    return copy;
}

Field& Field::operator-=(const Field& subtrahends) {
    if (size() == subtrahends.size())
        for (unsigned long i = 0; i < size(); ++i)
            at(i) -= subtrahends[i];
    else
        throw std::logic_error("fields size mismatch");
    return *this;
}

Field Field::operator*(const Complex& multiplier) const {
    Field copy(*this);
    for (unsigned long i = 0; i < copy.size(); ++i)
        copy[i] *= multiplier;
    return copy;
}

Field Field::operator*(const Field& multipliers) const {
    Field copy(*this);
    if (size() == multipliers.size())
        for (unsigned long i = 0; i < copy.size(); ++i)
            copy[i] *= multipliers[i];
    else
        throw std::logic_error("fields size mismatch");

    return copy;
}

Field& Field::operator*=(const Complex& multiplier) {
    for (unsigned long i = 0; i < size(); ++i)
        at(i) *= multiplier;
    return *this;
}

Field& Field::operator*=(const Field& multipliers) {
    if (size() == multipliers.size())
        for (unsigned long i = 0; i < size(); ++i)
            at(i) *= multipliers[i];
    else
        throw std::logic_error("fields size mismatch");

    return *this;
}

Field Field::operator/(const Complex& divider) const {
    Field copy(*this);
    for (unsigned long i = 0; i < copy.size(); ++i)
        copy[i] /= divider;

    return copy;
}

// Field Field::operator/(const Field& dividers) const {
//     Field copy(*this);
//     if (size() == dividers.size())
//         for (unsigned long i = 0; i < copy.size(); ++i)
//             copy[i] /= dividers[i];
//     else
//         throw std::logic_error("fields size mismatch");

//     return copy;
// }

Field& Field::operator/=(const Complex& divider) {
    for (unsigned long i = 0; i < size(); ++i)
        at(i) /= divider;
    return *this;
}

void Field::setSamplingRate(const double& rate) {
    sampling_rate = rate;
    unsigned long samples = size();
    omega.assign(samples, 2.0 * M_PI * rate / samples);

    for (unsigned long i = 0; i <= samples / 2; ++i)
        omega[i] *= double(i);

    for (unsigned long i = samples / 2 + 1; i < samples; ++i) {
        omega[i] *= double(i) - double(samples);
    }
}

double Field::getSamplingRate() const { return sampling_rate; }

double Field::dt() const { return 1.0 / sampling_rate; }

double Field::df() const { return sampling_rate / size(); }

double Field::dw() const { return 2.0 * M_PI * sampling_rate / size(); }

double Field::f(const unsigned long& i) const { return omega[i] / (2 * M_PI); }

double Field::w(const unsigned long& i) const { return omega[i]; }

RealVector Field::temporal_power() const {
    RealVector power(size(), 0);
    for (unsigned long i = 0; i < size(); ++i)
        power[i] = norm(at(i));

    return power;
}

RealVector Field::spectral_power() const {
    RealVector power(size(), 0);
    Field copy = *this;
    copy.fft_inplace();
    for (unsigned long i = 0; i < size(); ++i)
        power[i] = norm(copy[i]);

    return power;
}

Field Field::fft() const {
    Field transformed = *this;
    fftw_plan complex_inplace =
        fftw_plan_dft_1d(int(size()),
                         reinterpret_cast<fftw_complex*>(transformed.data()),
                         reinterpret_cast<fftw_complex*>(transformed.data()),
                         FFTW_FORWARD,
                         FFTW_ESTIMATE);
    fftw_execute(complex_inplace);
    fftw_destroy_plan(complex_inplace);

    transformed *= 1.0 / double(size());
    return transformed;
}

Field Field::ifft() const {
    Field transformed = *this;
    fftw_plan complex_inplace =
        fftw_plan_dft_1d(int(size()),
                         reinterpret_cast<fftw_complex*>(transformed.data()),
                         reinterpret_cast<fftw_complex*>(transformed.data()),
                         FFTW_BACKWARD,
                         FFTW_ESTIMATE);
    fftw_execute(complex_inplace);
    fftw_destroy_plan(complex_inplace);
    return transformed;
}

Field& Field::fft_inplace() {
    fftw_plan complex_inplace =
        fftw_plan_dft_1d(int(size()),
                         reinterpret_cast<fftw_complex*>(data()),
                         reinterpret_cast<fftw_complex*>(data()),
                         FFTW_FORWARD,
                         FFTW_ESTIMATE);
    fftw_execute(complex_inplace);
    fftw_destroy_plan(complex_inplace);

    for (unsigned long i = 0; i < size(); ++i)
        at(i) /= size();
    return *this;
}

Field& Field::ifft_inplace() {
    fftw_plan complex_inplace =
        fftw_plan_dft_1d(int(size()),
                         reinterpret_cast<fftw_complex*>(data()),
                         reinterpret_cast<fftw_complex*>(data()),
                         FFTW_BACKWARD,
                         FFTW_ESTIMATE);
    fftw_execute(complex_inplace);
    fftw_destroy_plan(complex_inplace);
    return *this;
}

Field& Field::fft_shift() {
    Complex buffer;
    unsigned long half_size = size() / 2;
    for (unsigned long i = 0; i < half_size; ++i) {
        buffer = at(i);
        at(i) = at(i + half_size);
        at(i + half_size) = buffer;
    }

    return *this;
}

Field Polarizations::x() { return (right + left) / sqrt(2); }

Field Polarizations::y() { return (right - left) / Complex(0, sqrt(2)); }

Polarizations Polarizations::operator*(const Complex& multiplier) const {
    return Polarizations{right * multiplier, left * multiplier};
}

Polarizations Polarizations::operator*(const Field& multipliers) const {
    return Polarizations{right * multipliers, left * multipliers};
}

Polarizations& Polarizations::operator*=(const Complex& multiplier) {
    right *= multiplier;
    left *= multiplier;

    return *this;
}

Polarizations& Polarizations::operator*=(const Field& multipliers) {
    right *= multipliers;
    left *= multipliers;

    return *this;
}

Field convolution(const Field& x, const Field& y) {
    Field z(x.size() + y.size() - 1);
    for (unsigned long i = 0; i < x.size(); ++i)
        for (unsigned long j = 0; j < y.size(); ++j)
            z[i + j] += x[i] * y[j];

    return z;
}