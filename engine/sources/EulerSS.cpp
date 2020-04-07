#include "EulerSS.h"


AbstrEulerSS::AbstrEulerSS(double dt) :
    AbstrConstStepSS(dt) {}

void AbstrEulerSS::reset_stepper() {}

double AbstrEulerSS::do_ode_step() {
    namespace pl = std::placeholders;
    auto rhs_system = std::bind(&AbstrSS::rhs, std::ref(*this), pl::_1 , pl::_2 , pl::_3);
    stepper.do_step(rhs_system, pos, time, try_dt);
    double step_done_with_dt = try_dt;
    time += step_done_with_dt;  // do_step takes time by value
    try_dt = dt;
    return step_done_with_dt;
}

EulerGillSS::EulerGillSS(double h_0, Parameters* p, unsigned int seed, double dt) :
        AbstrSS(h_0, p, seed),
        AbstrEulerSS(dt),
        AbstrGillSS() {}

EulerProbSS::EulerProbSS(double h_0, Parameters* p, unsigned int seed, double dt) :
        AbstrSS(h_0, p, seed),
        AbstrEulerSS(dt),
        AbstrProbSS() {}