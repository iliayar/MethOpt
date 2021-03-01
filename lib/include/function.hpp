#pragma once

class Function {
public:
    virtual double operator()(double) = 0;
};

class Parabola : public Function {
public:
    Parabola();
    double operator()(double);
};
