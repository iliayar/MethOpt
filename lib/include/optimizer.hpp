#pragma once

class Optimizer {
public:
    virtual bool forward() = 0;
    virtual double get_min() = 0;
};

class GoldenRation {
public:
    GoldenRation(double, double, double);
    bool forward();
    double get_min();
private:
    double m_xr;
    double m_xl;
    double m_eps;
};
