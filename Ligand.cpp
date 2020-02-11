#include "Ligand.h"

#include "helpers.h"
#include "Parameters.h"

#include <cmath>


// Public methods

Ligand::Ligand(double lig_x, double lig_y, LigandParameters* lig_p_) {
    auto r_alpha_pair = helpers::parametrize_ligand(lig_x, lig_y);
    r_cir = r_alpha_pair.first;
    alpha_inc = r_alpha_pair.second;

    lig_p = lig_p_;
}

bool Ligand::prepare_binding(double h, double alpha_0, double dt, Parameters *p, generator_t generator) {
    if (bond_state != 0)
        return false;

    BondParameters* bond_p = lig_p->bonds_p[0];
    // TODO: more binding rates and bond states (different receptors)
    double deviation = std::abs(surface_dist(h, alpha_0, p) - bond_p->lambda_);
    // Here we assume, that binding happens at rate corresponding to slip bond. It may be wrong.
    double binding_rate = helpers::bell_binding_rate(deviation, bond_p->k01s, bond_p->sigma, bond_p->x1s);
    double binding_probability = 1.0 - exp(-binding_rate * dt);
    if (helpers::draw_from_uniform_dist(generator) < binding_probability) {
        prepared_bond_state = 1;
        return true;
    } else
        return false;

}

bool Ligand::prepare_rupture(double h, double alpha_0, double dt, Parameters *p, generator_t generator) {
    if (bond_state == 0)
        return false;

    BondParameters* bond_p = lig_p->bonds_p[bond_state - 1];
    double bond_f = bond_force(h, alpha_0);
    double rupture_rate;

    if (lig_p->lig_type == psgl) {
        if (bond_state == PSGL_ESEL_STATE)
            // PSGL + E-selectin slip bond
            rupture_rate = helpers::esel_rupture_rate(bond_f, bond_p->k01s, bond_p->x1s);
        else if (bond_state == PSGL_PSEL_STATE)
            // PSGL + P-selectin catch-slip bond
            rupture_rate = helpers::psel_rupture_rate(bond_f, bond_p->k01s, bond_p->k01c,
                    bond_p->x1s, bond_p->x1c);
        else abort();

    } else if (lig_p->lig_type == integrin) {
        rupture_rate = helpers::integrin_rupture_rate(bond_f, bond_p->k01s, bond_p->k01c,
                bond_p->x1s, bond_p->x1c);
    }
    else abort();

    double rupture_probability = 1.0 - exp(-rupture_rate * dt);
    return helpers::draw_from_uniform_dist(generator) < rupture_probability;
}

void Ligand::bond(double alpha_0) {
    bond_state = prepared_bond_state;
    prepared_bond_state = 0;
    bd_rec_x = x_pos(alpha_0);
}

void Ligand::rupture() {
    // TODO: implement
}

void Ligand::move_bd_rec(double x_dist) {
    // TODO: implement
}

forces_t Ligand::bond_forces(double h, double alpha_0, Parameters *p) {
    // TODO: implement
    return {};
}

// Private methods

double Ligand::x_pos(double alpha_0) {
    return r_cir * sin(alpha_0 + alpha_inc);
}

double Ligand::y_pos(double alpha_0) {
    return r_cir * cos(alpha_0 + alpha_inc);
}

double Ligand::surface_dist(double h, double alpha_0, Parameters *p) {
    return h + R_C + y_pos(alpha_0);
}





