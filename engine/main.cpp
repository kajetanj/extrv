#include "Parameters.h"
#include "AbstractBondType.h"
#include "SimulationState.h"

#include <iostream>

int main() {
    for (int i = 100; i < 200; i++) {
        Parameters p = Parameters(
                4.5,
                0.01,
                310,
                0.05,
                5074.616349947093,
                1146.409200818891
        );

        Parameters::LigandType psgl = Parameters::LigandType();

        SlipBondType psgl_plus_esel_bond = SlipBondType(
                27,
                100,
                0.06,
                750,
                0.18,
                2.6
        );
        psgl.add_bond_type(&psgl_plus_esel_bond);
        p.add_ligands(&psgl, 20000);
        unsigned int seed = i;
//        unsigned int seed = 751134721;


        std::cout << "seed " << seed << " started." << std::endl;
        auto s = SimulationState(0.03, &p, seed, 1e-2);
        size_t max_steps_falling = 2e6;
        size_t max_steps_rolling = 6e6;

        s.simulate(1, max_steps_falling);
        std::cout << s.diag.n_bonds_created << " bonds created during falling." << std::endl;
        s.shear_rate = 0.9;
        double max_time = 11.0;
        s.simulate(max_time, max_steps_rolling);
        std::cout << s.diag.n_bonds_created << " bonds created during falling and rolling." << std::endl;
        std::cout << "Done." << std::endl << std::endl;
    }
}