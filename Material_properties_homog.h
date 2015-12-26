//
// C++ Interface: Material_properties_homog
//
// Description: creation des proprietes materiaux
//
//
// Author: Pled Florent <pled@lmt.ens-cachan.fr>, (C) 2015
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef Material_properties_homog_h
#define Material_properties_homog_h

using namespace LMT;
using namespace std;

/// Number<0> : champ global
/// Number<1> : champ elementaire
/// -----------------------------
template<class TM, class TF, unsigned n1, unsigned n2> void calc_material_coefficients_init_alternativeontype( TM &m, TF &f, const Number<n1> &, const Number<n2> & );

template<class TM, class TF> void calc_material_coefficients_init_alternativeontype( TM &m, TF &f, Number<0>, Number<0> ) {
    static const unsigned dim = TM::dim;
    calc_material_coefficients_init( m, f, Number<dim>() );
}

struct Calcul_Elem_Material_Coefficients_Init {
    template<class TE, class TM, class TF> void operator()( TE &elem, const TM &m, const TF &f ) const {
        calc_elem_material_coefficients_init( elem, m, f );
    }
};

template<class TM, class TF> void calc_material_coefficients_init_alternativeontype( TM &m, TF &f, Number<1>, Number<0> ) {
    apply( m.elem_list, Calcul_Elem_Material_Coefficients_Init(), m, f );
}

template<class TM, class TF> void calc_material_coefficients_init_alternativeontype( TM &m, TF &f, Number<0>, Number<1> ) {
    apply( m.elem_list, Calcul_Elem_Material_Coefficients_Init(), m, f );
}

template<class TM, class TF> void calc_material_coefficients_init_alternativeontype( TM &m, TF &f, Number<1>, Number<1> ) {
    apply( m.elem_list, Calcul_Elem_Material_Coefficients_Init(), m, f );
}

/// Creation des proprietes materiaux initiales
/// -------------------------------------------
template<class TF, class TM>
void set_material_properties_init( TF &f, TM &m, const string &structure ) {

    typedef typename TM::TNode::T T;

    if ( m.node_list.size() ) {
        T young = 1;
        T poisson = 0.3;
        T density = 1;
        /// Carre 2D
        /// --------
        if ( structure.find("square") != string::npos ) {
            for (unsigned n=0;n<m.elem_list.size();++n) {
                if ( center( *m.elem_list[n] )[0] < 0.5 and center( *m.elem_list[n] )[1] < 0.5 ) { // x < 0.5 and y < 0.5
                    T mu = 100.;
                    poisson = 0.2;
                    young = 2*(1+poisson)*mu;
                    m.elem_list[n]->set_field( "young_init", young );
                    m.elem_list[n]->set_field( "poisson_init", poisson );
                }
                else {
                    T mu = 1.;
                    poisson = 0.3;
                    young = 2*(1+poisson)*mu;
                    m.elem_list[n]->set_field( "young_init", young );
                    m.elem_list[n]->set_field( "poisson_init", poisson );
                }
            }

            calc_material_coefficients_init_alternativeontype( m, f, Number< AreSameType< typename ExtractDM<young_init_DM>::ReturnType<TM>::T, void >::res >(), Number< AreSameType< typename ExtractDM<poisson_init_DM>::ReturnType<TM>::T, void >::res >() );

            set_field_alternativeontype( m, Number< AreSameType< typename ExtractDM<density_DM>::ReturnType<TM>::T, void >::res >(), density, density_DM() );
        }
    }
}

#endif // Material_properties_homog_h
