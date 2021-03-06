#include "Parameters.h"

#include "helpers.h"



void Parameters::LigandType::add_bond_type(AbstractBondType *bond_type) {
    bonds_types.push_back(bond_type);
}

void Parameters::LigandType::update_max_surf_dist() {
    double max_sd_for_bond_type;
    for (const auto &bond_type : bonds_types) {
        max_sd_for_bond_type = helpers::bisection(bond_type->eq_bond_len, 1.0,
            [&](double sd) -> double {
                return bond_type->binding_rate(sd, p->temp) - MIN_RATE;
            }, MIN_RATE / 10);
        max_surf_dist = std::max(max_surf_dist, max_sd_for_bond_type);
    }
}

Parameters::Parameters(double r_cell, double visc, double temp, double dens_diff, double rep_0, double rep_scale) :

        r_cell(r_cell),
        // conversion to kg/(μm s)
        visc(visc * 1e-7),
        temp(temp),
        // conversion to kg/μm^3
        dens_diff(dens_diff * 1e-15),
        rep_0(rep_0),
        rep_scale(rep_scale) {}


void Parameters::add_ligands(LigandType *lig_type, size_t n_of_lig) {
    lig_type->p = this;
    lig_type->update_max_surf_dist();
    lig_types_and_nrs.emplace_back(lig_type, n_of_lig);
    max_surf_dist = std::max(max_surf_dist, lig_type->max_surf_dist);
}
