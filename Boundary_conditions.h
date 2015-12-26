//
// C++ Interface: Boundary_conditions
//
// Description: creation des conditions aux limites
//
//
// Author: Pled Florent <pled@lmt.ens-cachan.fr>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef Boundary_conditions_h
#define Boundary_conditions_h

#include "INTEREST_QUANTITY/Interest_quantity.h"
#include "GEOMETRY/Calcul_geometry.h"

using namespace LMT;
using namespace std;

//struct Set_Field_f_surf {
//    template<class TE, class TM> void operator()( TE &elem, TM &m ) const {
//        m.update_elem_children();
//        typedef typename TE::T T;
//        Mat<T,Sym<TE::dim > > pre_sig = elem.get_field( "pre_sigma", StructForType<Mat<T,Sym<TE::dim > > >() );
//        for (unsigned k=0;k<NbChildrenElement<typename TE::NE,1>::res;++k) {
//            if ( m.sub_mesh(Number<1>()).get_parents_of_EA( m.get_children_of(elem,Number<1>())[k] ).size() == 1 ) {
//                Vec<T,TE::dim> force_surf = pre_sig * m.sub_mesh(Number<1>()).elem_list[ m.get_children_of( elem, Number<1>() )[k]->number ]->sample_normal_virtual() * -1.;
//                m.sub_mesh(Number<1>()).elem_list[ m.get_children_of( elem, Number<1>() )[k]->number ]->set_field( "f_surf", force_surf );
////                cout << "face : "  << m.get_children_of( elem, Number<1>() )[k]->number << endl;
////                cout << force_surf << endl;
//            }
//        }
//    }
//};

/// Creation des conditions aux limites
/// -----------------------------------
template<class TF, class TM>
void set_boundary_conditions( TF &f, TM &m, const string &boundary_condition_D, const string &pb, const string &structure, const string &loading, const string &mesh_size ) {

    static const unsigned dim = TM::dim;
    typedef typename TM::TNode::T T;

    if ( m.node_list.size() ) {
        T penalty_val;
        if ( boundary_condition_D == "lagrange" )
            penalty_val = 0;
        else if ( boundary_condition_D == "penalty" )
            penalty_val = 1e8;

        /// Dimension 2
        /// -----------
        if ( dim == 2 ) {
            /// blocage des noeuds situes en x = 0 dans toutes les directions
            /// -------------------------------------------------------------
//            for (unsigned i=0;i<m.node_list.size();++i) {
//                if ( m.node_list[i].pos[0] < 1e-6 ) {
//                    for (unsigned d=0;d<dim;++d) {
//                        f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val ); // la fonction add_constraint() sert a fixer une contrainte a un noeud
//                    }
//                }
//            }
            /// blocage des noeuds situes en y = 0 dans toutes les directions
            /// -------------------------------------------------------------
//            for (unsigned i=0;i<m.node_list.size();++i) {
//                if ( m.node_list[i].pos[1] < 1e-6 ) {
//                    for (unsigned d=0;d<dim;++d) {
//                        f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
//                    }
//                }
//            }
            /// blocage du noeud (0.5, 0.5) dans toutes les directions
            /// blocage du noeud (0.5, 1) dans la direction y
            /// ------------------------------------------------------
//            for (unsigned i=0;i<m.node_list.size();++i) {
//                if ( 0.5 - 1e-6 < m.node_list[i].pos[0] and m.node_list[i].pos[0] < 0.5 + 1e-6 and 0.5 - 1e-6 < m.node_list[i].pos[1] and m.node_list[i].pos[1] < 0.5 + 1e-6 ) {
//                    for (unsigned d=0;d<dim;++d) {
//                        f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
//                    }
//                }
//            }
//            for (unsigned i=0;i<m.node_list.size();++i) {
//                if ( 0.5 - 1e-6 < m.node_list[i].pos[0] and m.node_list[i].pos[0] < 0.5 + 1e-6 and m.node_list[i].pos[1] > 1. - 1e-6 ) {
//                    f.add_constraint( "node["+to_string(i)+"].dep[0]", penalty_val );
//                }
//            }
            /// blocage du noeud (0.5, 0) dans toutes les directions
            /// blocage du noeud (0.5, 1) dans la direction y
            /// ----------------------------------------------------
//            for (unsigned i=0;i<m.node_list.size();++i) {
//                if ( 0.5 - 1e-6 < m.node_list[i].pos[0] and m.node_list[i].pos[0] < 0.5 + 1e-6 and m.node_list[i].pos[1] < 1e-6 ) {
//                    for (unsigned d=0;d<dim;++d) {
//                        f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
//                    }
//                }
//            }
//            for (unsigned i=0;i<m.node_list.size();++i) {
//                if ( 0.5 - 1e-6 < m.node_list[i].pos[0] and m.node_list[i].pos[0] < 0.5 + 1e-6 and m.node_list[i].pos[1] > 1. - 1e-6 ) {
//                    f.add_constraint( "node["+to_string(i)+"].dep[0]", penalty_val );
//                }
//            }
            /// Plaque rectangulaire 2D en traction
            /// Plaque rectangulaire 2D en flexion
            /// blocage des noeuds situes en x = 0 dans toutes les directions
            /// -------------------------------------------------------------
            if ( structure == "plate_traction" or structure == "plate_flexion" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( m.node_list[i].pos[0] < 1e-6 ) {
                        for (unsigned d=0;d<dim;++d) {
                            f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
                        }
                    }
                }
            }
            /// Quart de plaque rectangulaire trouee 2D
            /// blocage des noeuds situes en x = 0 dans la direction x
            /// blocage des noeuds situes en y = 0 dans la direction y
            /// ------------------------------------------------------
            else if ( structure == "plate_hole" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( m.node_list[i].pos[0] < 1e-6 ) {
                        f.add_constraint( "node["+to_string(i)+"].dep[0]", penalty_val );
                    }
                    if ( m.node_list[i].pos[1] < 1e-6 ) {
                        f.add_constraint( "node["+to_string(i)+"].dep[1]", penalty_val );
                    }
                }
            }
            /// Plaque fissuree 2D
            /// loading pull : blocage du noeud (7, 0) dans toutes les directions
            ///                blocage du noeud voisin au noeud (7, 0) situe en y=0 dans la direction y
            /// loading shear : blocage des noeuds situes en y = -8 dans toutes les directions
            /// ---------------------------------------------------------------------------------------
            else if ( structure == "plate_crack" ) {
                m.update_node_neighbours();
                if ( loading == "pull" ) {
                    for (unsigned i=0;i<m.node_list.size();++i) {
                        if ( -1e-6 < m.node_list[i].pos[1] and m.node_list[i].pos[1] < 1e-6 and m.node_list[i].pos[0] > 7. - 1e-6 ) {
                            for (unsigned d=0;d<dim;++d) {
                                f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
                            }
                            for (unsigned j=0;j<m.get_node_neighbours( i ).size();++j) {
                                if ( m.get_node_neighbours( i )[ j ]->pos[0] < m.node_list[i].pos[0] and -1e-6 < m.get_node_neighbours( i )[ j ]->pos[1] and m.get_node_neighbours( i )[ j ]->pos[1] < 1e-6 ) {
                                    f.add_constraint( "node["+to_string(m.get_node_neighbours( i )[ j ]->number)+"].dep[1]", penalty_val );
                                }
                            }
                        }
                    }
                }
                else if ( loading == "shear" ) {
                    for (unsigned i=0;i<m.node_list.size();++i) {
                        if ( m.node_list[i].pos[1] < -8. + 1e-6 ) {
                            for (unsigned d=0;d<dim;++d) {
                                f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
                            }
                        }
                    }
                }
            }
            /// Structure fissuree 2D
            /// blocage des noeuds du trou bas ( pour Quad, noeuds 105 a 121 ; pour Quad_8 et Quad_9, noeuds 105 a 121 et 330, 328, 326, 477, 474, 204, 201, 366, 364, 357, 354, 347, 344, 341, 338, 335, 331  ) dans toutes les directions
            /// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            else if ( structure == "structure_crack" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( pow(m.node_list[i].pos[0] - 66.5, 2) + pow(m.node_list[i].pos[1] - 69., 2) < pow(37.8 + 1e-6, 2) ) { // ( x - 66.5 )^2 + ( y - 69 )^2 = 37.8^2
                        for (unsigned d=0;d<dim;++d) {
                            f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
                        }
                    }
                }
            }
            /// Eprouvette 2D
            /// deplacement impose aux noeuds situes en x = 0 dans la direction y : u_d = -y
            /// deplacement impose aux noeuds situes en x = 3 dans la direction y : u_d = +y
            // blocage des noeuds situes sur les deux arcs de cercle
            /// blocage du noeud (1.5, 0.25) dans la direction x
            /// ----------------------------------------------------------------------------
            else if ( structure == "test_specimen" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( pb == "direct" ) {
                        if ( m.node_list[i].pos[0] < 1e-6 ) {
                            f.add_constraint( "node["+to_string(i)+"].dep[1] + 1", penalty_val );
                        }
                        if ( m.node_list[i].pos[0] > 3. - 1e-6 ) {
                            f.add_constraint( "node["+to_string(i)+"].dep[1] - 1", penalty_val );
                        }
                    }
                    else if (pb == "adjoint" ) {
                        if ( m.node_list[i].pos[0] < 1e-6 ) {
                            f.add_constraint( "node["+to_string(i)+"].dep[1]", penalty_val );
                        }
                        if ( m.node_list[i].pos[0] > 3. - 1e-6 ) {
                            f.add_constraint( "node["+to_string(i)+"].dep[1]", penalty_val );
                        }
                    }
//                    if ( pow(m.node_list[i].pos[0] - 1.5, 2) + pow(m.node_list[i].pos[1] + 0.375, 2) < pow(0.625 + 1e-6, 2) ) { // ( x - 1.5 )^2 + ( y + 0.375 )^2 = 0.625^2
//                        for (unsigned d=0;d<dim;++d) {
//                            f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
//                        }
//                    }
//                    if ( pow(m.node_list[i].pos[0] - 1.5, 2) + pow(m.node_list[i].pos[1] - 1.375, 2) < pow(0.625 + 1e-6, 2) ) { // ( x - 1.5 )^2 + ( y - 1.375 )^2 = 0.625^2
//                        for (unsigned d=0;d<dim;++d) {
//                            f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
//                        }
//                    }
                    if ( 1.5 - 1e-6 < m.node_list[i].pos[0] and m.node_list[i].pos[0] < 1.5 + 1e-6 and 0.25 - 1e-6 < m.node_list[i].pos[1] and m.node_list[i].pos[1] < 0.25 + 1e-6 ) {
                        f.add_constraint( "node["+to_string(i)+"].dep[0]", penalty_val );
                    }
                }
            }
            /// Capteur d'effort 2D
            /// blocage des noeuds situes en y = -1 dans toutes les directions
            /// --------------------------------------------------------------
            else if ( structure == "weight_sensor" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( m.node_list[i].pos[1] < -1. + 1e-6 ) {
                        for (unsigned d=0;d<dim;++d) {
                            f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
                        }
                    }
                }
            }
            /// Inclusions/Trous circulaires 2D
            /// blocage des noeuds situes en x = 0 dans toutes les directions
            /// -------------------------------------------------------------
            else if ( structure == "circular_inclusions" or structure == "circular_holes" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( m.node_list[i].pos[0] < 1e-6 ) {
                        for (unsigned d=0;d<dim;++d) {
                            f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
                        }
                    }
                }
            }
            /// Carre 2D
            /// condition de periodicite aux noeuds situes en x = 0 et x = 1
            /// condition de periodicite aux noeuds situes en y = 0 et y = 1
            /// blocage du noeud (0.25, 0.25) dans toutes les directions
            // blocage de la moyenne du deplacement dans toutes les directions
            /// application du champ de deplacement u a tous les noeuds
            /// ---------------------------------------------------------------
            else if ( structure.find("square") != string::npos ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( m.node_list[i].pos[0] < 1e-6 ) {
                        for (unsigned j=0;j<m.node_list.size();++j) {
                            if ( m.node_list[j].pos[0] > 1 - 1e-6 and abs(m.node_list[j].pos[1] - m.node_list[i].pos[1]) < 1e-6 ) {
                                for (unsigned d=0;d<dim;++d) {
                                    f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"] - node["+to_string(j)+"].dep["+to_string(d)+"]", penalty_val );
                                }
                            }
                        }
                    }
                    else if ( m.node_list[i].pos[1] < 1e-6 ) {
                        for (unsigned j=0;j<m.node_list.size();++j) {
                            if ( m.node_list[j].pos[1] > 1 - 1e-6 and abs(m.node_list[j].pos[0] - m.node_list[i].pos[0]) < 1e-6 ) {
                                for (unsigned d=0;d<dim;++d) {
                                    f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"] - node["+to_string(j)+"].dep["+to_string(d)+"]", penalty_val );
                                }
                            }
                        }
                    }
//                    else if ( ( 0.25 - 1e-6 < m.node_list[i].pos[0] and m.node_list[i].pos[0] < 0.25 + 1e-6 and ( ( 0.25 - 1e-6 < m.node_list[i].pos[1] and m.node_list[i].pos[1] < 0.25 + 1e-6 ) or ( 0.75 - 1e-6 < m.node_list[i].pos[1] and m.node_list[i].pos[1] < 0.75 + 1e-6 ) ) ) or ( 0.75 - 1e-6 < m.node_list[i].pos[0] and m.node_list[i].pos[0] < 0.75 + 1e-6 and ( ( 0.25 - 1e-6 < m.node_list[i].pos[1] and m.node_list[i].pos[1] < 0.25 + 1e-6 ) or ( 0.75 - 1e-6 < m.node_list[i].pos[1] and m.node_list[i].pos[1] < 0.75 + 1e-6 ) ) ) ) {
                    else if ( 0.25 - 1e-6 < m.node_list[i].pos[0] and m.node_list[i].pos[0] < 0.25 + 1e-6 and 0.25 - 1e-6 < m.node_list[i].pos[1] and m.node_list[i].pos[1] < 0.25 + 1e-6 ) {
                        for (unsigned d=0;d<dim;++d) {
                            f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
                        }
                    }
                }

//                for (unsigned d=0;d<dim;++d) {
//                    string txt_mean_constraint = "";
//                    for (unsigned i=0;i<m.node_list.size();++i) {
//                        if ( ( m.node_list[i].pos[0] < 1e-6 and m.node_list[i].pos[1] < 1e-6 ) or ( m.node_list[i].pos[0] > 1 - 1e-6 and m.node_list[i].pos[1] < 1e-6 ) or ( m.node_list[i].pos[0] < 1e-6 and m.node_list[i].pos[1] > 1 - 1e-6 ) or ( m.node_list[i].pos[0] > 1 - 1e-6 and m.node_list[i].pos[1] > 1 - 1e-6 ) )
//                            txt_mean_constraint += "node["+to_string(i)+"].dep["+to_string(d)+"] * 1/4 + ";
//                        else if ( m.node_list[i].pos[0] < 1e-6 or m.node_list[i].pos[0] > 1 - 1e-6 or m.node_list[i].pos[1] < 1e-6 or m.node_list[i].pos[1] > 1 - 1e-6 )
//                            txt_mean_constraint += "node["+to_string(i)+"].dep["+to_string(d)+"] * 1/2 + ";
//                        else
//                            txt_mean_constraint += "node["+to_string(i)+"].dep["+to_string(d)+"] + ";
//                    }
//                    txt_mean_constraint.resize( txt_mean_constraint.size()-3 );
//                    f.add_constraint( txt_mean_constraint, penalty_val );
//                }

                size_t offset = structure.rfind( "_" )+1;
                string str = structure.substr( offset );
                istringstream buffer(str);
                int N;
                buffer >> N;
                Hdf hdf("DATA_HDF5/square-" + str + "x" + str + ".hdf5");

                Vec<int> s;
                hdf.read_size( "/u", s );

                Tens3<T> u;
                u.resize( s );
                hdf.read_data( "/u", u.ptr(), s, s );

                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( m.node_list[i].pos[0] < 1. - 1e-6 and m.node_list[i].pos[1] < 1. - 1e-6 ) {
                        for (unsigned d=0;d<dim;++d)
                            m.node_list[i].dep[ d ] = u( d, int(m.node_list[i].pos[1]*N), int(m.node_list[i].pos[0]*N) );
                    }
                    else if ( m.node_list[i].pos[0] > 1. - 1e-6 and m.node_list[i].pos[1] < 1. - 1e-6 ) {
                        for (unsigned d=0;d<dim;++d)
                            m.node_list[i].dep[ d ] = u( d, int(m.node_list[i].pos[1]*N), 0 );
                    }
                    else if ( m.node_list[i].pos[1] > 1. - 1e-6 and m.node_list[i].pos[0] < 1. - 1e-6 ) {
                        for (unsigned d=0;d<dim;++d)
                            m.node_list[i].dep[ d ] = u( d, 0, int(m.node_list[i].pos[0]*N) );
                    }
                    else {
                        for (unsigned d=0;d<dim;++d)
                            m.node_list[i].dep[ d ] = u( d, 0, 0 );
                    }
                }
            }

            m.update_skin();
            if ( pb == "direct" ) {
                /// effort surfacique applique sur les bords des elements situes en x = 0 : F_d = -x
                /// --------------------------------------------------------------------------------
//                for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
//                    if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] < 1e-6 ) {
//                        m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( -1., 0. ) );
//                    }
//                }
                /// Plaque rectangulaire 2D en traction
                /// effort surfacique applique sur les bords des elements situes en x = 2 : F_d = +x
                /// --------------------------------------------------------------------------------
                if ( structure == "plate_traction" ) {
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                        if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] > 2. - 1e-6 ) {
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 1., 0. ) );
                        }
                    }
                }
                /// Plaque rectangulaire 2D en flexion
                /// effort surfacique applique sur les bords des elements situes en y = 1 : F_d = -y
                /// --------------------------------------------------------------------------------
                else if ( structure == "plate_flexion" ) {
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                        if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] > 1. - 1e-6 ) {
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 0., -1. ) );
                        }
                    }
                }
                /// Quart de plaque rectangulaire trouee 2D
                /// effort surfacique applique sur les bords des elements situes en x = 1 : F_d = +x
                /// --------------------------------------------------------------------------------
                else if ( structure == "plate_hole" ) {
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                        if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] > 1. - 1e-6 ) {
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 1., 0. ) );
                        }
                    }
                }
                /// Plaque fissuree 2D
                /// loading pull : effort surfacique applique sur les bords des elements situes en y = -8 : F_d = -y
                ///                effort surfacique applique sur les bords des elements situes en y = 8 : F_d = +y
                /// loading shear : effort surfacique applique sur les bords des elements situes en y = 8 : F_d = +x
                /// ------------------------------------------------------------------------------------------------
                else if ( structure == "plate_crack" ) {
                    if ( loading == "pull" ) {
                        for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                            if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] < -8. + 1e-6 ) {
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 0., -1. ) );
                            }
                            if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] > 8. - 1e-6 ) {
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 0., 1. ) );
                            }
                        }
                    }
                    else if ( loading == "shear" ) {
                        for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                            if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] > 8. - 1e-6 ) {
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 1., 0. ) );
                            }
                        }
                    }
                }
                /// Structure fissuree 2D
                /// effort surfacique applique sur les bords du trou haut ( pour maillage grossier : faces 125, 127, 129, 242, 240, 238, 244, 247 ) : F_d = -n
                /// effort surfacique applique sur les bords de la surface haute ( pour maillage grossier : faces 284, 281, 277 ) : F_d = +n
                /// ------------------------------------------------------------------------------------------------------------------------------------------
                else if ( structure == "structure_crack" ) {
//                    Vec<unsigned> faces_trou_haut( 125, 127, 129, 242, 240, 238, 244, 247 );
//                    Vec<unsigned> faces_surface_haute( 284, 281, 277 );
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
//                        if ( find( faces_trou_haut, _1 == i ) ) {
                        if ( pow(center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] - 112., 2) + pow(center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] - 142., 2) < pow(20. + 1e-6, 2) ) { // ( x - 112 )^2 + ( y - 142 )^2 = 20^2
                            Vec<T,dim> force_surf = m.sub_mesh(Number<1>()).elem_list[i]->sample_normal_virtual() * -1.;
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", force_surf );
                        }
//                        if ( find( faces_surface_haute, _1 == i ) ) {
                        if ( 12./33. * center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] + 6438./33. - 1e-6 < center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] ) { // a * x + b = y
                            Vec<T,dim> force_surf = m.sub_mesh(Number<1>()).elem_list[i]->sample_normal_virtual() * 1.;
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", force_surf );
                        }
                    }
                }
                /// Eprouvette 2D
                /// effort surfacique applique sur les bords des elements situes en x = 0 : F_d = -x
                /// effort surfacique applique sur les bords des elements situes en x = 3 : F_d = +x
                /// --------------------------------------------------------------------------------
                else if ( structure == "test_specimen" ) {
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                        if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] < 1e-6 ) {
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( -1., 0. ) );
                        }
                        if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] > 3. - 1e-6 ) {
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 1., 0. ) );
                        }
                    }
                }
                /// Capteur d'effort 2D
                /// effort surfacique applique sur les bords des elements situes en y = 15 : F_d = -y
                /// ---------------------------------------------------------------------------------
                else if ( structure == "weight_sensor" ) {
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                        if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] > 15. - 1e-1 ) {
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 0., -1. ) );
                        }
                    }
                }
                /// Inclusions/Trous circulaires 2D
                /// effort surfacique applique sur les bords des elements situes en x = 1 : F_d = +x
                /// --------------------------------------------------------------------------------
                else if ( structure == "circular_inclusions" or structure == "circular_holes" ) {
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                        if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] > 1. - 1e-6 ) {
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 1., 0. ) );
                        }
                    }
                }
                /// Carre 2D
                /// pre-deformation et pre-contrainte appliquees sur tous les elements
                /// ------------------------------------------------------------------
                else if ( structure.find("square") != string::npos ) {
                    if ( structure.find("init") != string::npos ) {
                        Vec<T,unsigned(dim*(dim+1)/2) >  pre_eps;
                        pre_eps.set( 0, 0. );
                        pre_eps.set( 1, -1./2 );
                        pre_eps.set( 2, 0. );
                        for (unsigned n=0;n<m.elem_list.size();++n)
                            m.elem_list[n]->set_field( "pre_epsilon", pre_eps );
                    }
                    else {
                        size_t offset = structure.rfind( "_" )+1;
                        const string str = structure.substr( offset );
                        istringstream buffer(str);
                        int N;
                        buffer >> N;
                        Hdf hdf("DATA_HDF5/square-" + str + "x" + str + ".hdf5");

                        Vec<int> s;
                        hdf.read_size( "/tau", s );

                        Tens3<T> tau;
                        tau.resize( s );
                        hdf.read_data( "/tau", tau.ptr(), s, s );

                        Vec<T,unsigned(dim*(dim+1)/2) > pre_sig;
                        for (unsigned n=0;n<m.elem_list.size();++n) {
                            int i = int(center( *m.elem_list[n] )[0]*N-1./2);
                            int j = int(center( *m.elem_list[n] )[1]*N-1./2);
                            pre_sig[ 0 ] = -tau( 0, j, i );
                            pre_sig[ 1 ] = -tau( 2, j, i )/sqrt(2.);
                            pre_sig[ 2 ] = -tau( 1, j, i );
                            m.elem_list[n]->set_field( "pre_sigma", pre_sig );
                        }

//                        for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
//                            if ( m.sub_mesh(Number<1>()).get_parents_of_EA( m.sub_mesh(Number<1>()).elem_list[i] ).size() == 1 ) {
//                                unsigned n = m.sub_mesh(Number<1>()).get_parents_of_EA( m.sub_mesh(Number<1>()).elem_list[i] )[0]->number;
//                                Mat<T,Sym<dim> > pre_sig = m.elem_list[n]->get_field( "pre_sigma", StructForType<Mat<T,Sym<dim> > >() );
//                                Vec<T,dim> force_surf = pre_sig * m.sub_mesh(Number<1>()).elem_list[i]->sample_normal_virtual() * -1.;
//                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", force_surf );
//                            }
//                        }
//                        apply( m.elem_list, Set_Field_f_surf(), m );
                    }
                }
            }
        }
        /// Dimension 3
        /// -----------
        else if ( dim == 3 ) {
            /// blocage des noeuds situes en x = 0 dans toutes les directions
            /// -------------------------------------------------------------
//            for (unsigned i=0;i<m.node_list.size();++i) {
//                if ( m.node_list[i].pos[0] < 1e-6 ) {
//                    for (unsigned d=0;d<dim;++d) {
//                        f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val ); // la fonction add_constraint() sert a fixer une contrainte a un noeud
//                    }
//                }
//            }
            /// blocage des noeuds situes en x = 0 dans la direction x
            /// blocage du noeud (0, 0, 0) dans les directions y et z
            /// blocage du noeud (0, 0, 1) dans la direction y
            ///-------------------------------------------------------
//            for (unsigned i=0;i<m.node_list.size();++i) {
//                if ( m.node_list[i].pos[0] < 1e-6 ) {
//                    f.add_constraint( "node["+to_string(i)+"].dep[0]", penalty_val );
//                }
//            }
//            for (unsigned i=0;i<m.node_list.size();++i) {
//                if ( m.node_list[i].pos[0] < 1e-6 and m.node_list[i].pos[1] < 1e-6 and m.node_list[i].pos[2] < 1e-6 ) {
//                    f.add_constraint( "node["+to_string(i)+"].dep[1]", penalty_val );
//                    f.add_constraint( "node["+to_string(i)+"].dep[2]", penalty_val );
//                }
//            }
//            for (unsigned i=0;i<m.node_list.size();++i) {
//                if ( m.node_list[i].pos[0] < 1e-6 and m.node_list[i].pos[1] < 1e-6 and m.node_list[i].pos[2] > 1. - 1e-6 ) {
//                    f.add_constraint( "node["+to_string(i)+"].dep[1]", penalty_val );
//                }
//            }
            /// Barre rectangulaire en traction 3D
            /// blocage des noeuds situes en z = 0 dans la direction z
            /// blocage des noeuds situes en z = 0 et y = 0 dans la direction y
            /// blocage du noeud (0, 0, 0) dans la direction x
            ///----------------------------------------------------------------
            if ( structure == "beam_traction" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( m.node_list[i].pos[2] < 1e-6 ) {
                        f.add_constraint( "node["+to_string(i)+"].dep[2]", penalty_val );
                        if ( m.node_list[i].pos[1] < 1e-6 ) {
                            f.add_constraint( "node["+to_string(i)+"].dep[1]", penalty_val );
                            if ( m.node_list[i].pos[0] < 1e-6 ) {
                                f.add_constraint( "node["+to_string(i)+"].dep[0]", penalty_val );
                            }
                        }
                    }
                }
            }
            /// Barre rectangulaire en flexion 3D
            /// Plaque rectangulaire trouee complete 3D
            /// blocage des noeuds situes en x = 0 dans toutes les directions
            /// -------------------------------------------------------------
            else if ( structure == "beam_flexion" or structure == "plate_hole_full" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( m.node_list[i].pos[0] < 1e-6 ) {
                        for (unsigned d=0;d<dim;++d) {
                            f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
                        }
                    }
                }
            }
            /// Barre rectangulaire trouee 3D
            /// blocage des noeuds situes en y = 0 dans la direction y
            /// blocage du noeud (0.4, 0, 0) dans toutes les directions
            /// blocage du noeud (0, 0, 0) dans la direction z
            ///-----------------------------------------------------------
            else if ( structure == "beam_hole" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( m.node_list[i].pos[1] < 1e-6 ) {
                        f.add_constraint( "node["+to_string(i)+"].dep[1]", penalty_val );
                        if ( m.node_list[i].pos[2] < 1e-6 ) {
                            if ( 0.4 - 1e-6 < m.node_list[i].pos[0] and m.node_list[i].pos[0] < 0.4 + 1e-6 ) {
                                f.add_constraint( "node["+to_string(i)+"].dep[0]", penalty_val );
                                f.add_constraint( "node["+to_string(i)+"].dep[2]", penalty_val );
                            }
                            else if ( m.node_list[i].pos[0] < 1e-6 )
                                f.add_constraint( "node["+to_string(i)+"].dep[2]", penalty_val );
                        }
                    }
                }
            }
            /// Quart de plaque rectangulaire trouee 3D
            /// blocage des noeuds situes en x = 0 dans la direction x
            /// blocage des noeuds situes en y = 0 dans la direction y
            /// blocage des noeuds situes en z = 0 dans la direction z
            ///-------------------------------------------------------
            else if ( structure == "plate_hole" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( m.node_list[i].pos[0] < 1e-6 ) {
                        f.add_constraint( "node["+to_string(i)+"].dep[0]", penalty_val );
                    }
                    if ( m.node_list[i].pos[1] < 1e-6 ) {
                        f.add_constraint( "node["+to_string(i)+"].dep[1]", penalty_val );
                    }
                    if ( m.node_list[i].pos[2] < 1e-6 ) {
                        f.add_constraint( "node["+to_string(i)+"].dep[2]", penalty_val );
                    }
                }
            }
            /// Moyeu-rotor 3D de l'helicoptere NH90
            // blocage des noeuds situes en x = -123 et z < 81.1 et z > -81.1 dans toutes les directions
            /// blocage des noeuds situes en x < -124 et z = 70.1 et z = -70.1 dans toutes les directions
            /// -----------------------------------------------------------------------------------------
            else if ( structure == "hub_rotor_helico" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
//                    if ( m.node_list[i].pos[0] > -123.1 and m.node_list[i].pos[0] < -122.9 and m.node_list[i].pos[2] > -81.1 and m.node_list[i].pos[2] < 81.1 ) {
//                        for (unsigned d=0;d<dim;++d) {
//                            f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
//                        }
//                    }
                    if ( m.node_list[i].pos[2] > -70.1 and m.node_list[i].pos[2] < 70.1 and m.node_list[i].pos[0] < -124 ) {
                        for (unsigned d=0;d<dim;++d) {
                            f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
                        }
                    }
                }
            }
            /// Joint de porte de vehicule auto 3D
            /// blocage des noeuds situes en x = 0.433 dans toutes les directions
            /// blocage des noeuds situes en z = 0 et z = 0.05 dans la direction z
            /// ------------------------------------------------------------------
            else if ( structure == "door_seal" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( m.node_list[i].pos[0] < 0.433 + 1e-6 ) {
                        for (unsigned d=0;d<dim;++d) {
                            f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
                        }
                    }
                    if ( m.node_list[i].pos[2] < 1e-6 or m.node_list[i].pos[2] > 0.05 - 1e-6 ) {
                        f.add_constraint( "node["+to_string(i)+"].dep[2]", penalty_val );
                    }
                }
            }
            /// Point de soudure 3D
            /// loading peeling : blocage des noeuds situes en x = 9 dans toutes les directions
            /// loading pull or shear : blocage des noeuds situes en y = -6.3 dans toutes les directions
            /// ----------------------------------------------------------------------------------------
            else if ( structure == "spot_weld" ) {
                if ( loading == "pull" or loading == "shear" ) {
                    for (unsigned i=0;i<m.node_list.size();++i) {
                        if ( m.node_list[i].pos[1] < -6.3 + 1e-6 ) {
                            for (unsigned d=0;d<dim;++d) {
                                f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
                            }
                        }
                    }
                }
                else if ( loading == "peeling" ) {
                    for (unsigned i=0;i<m.node_list.size();++i) {
                        if ( m.node_list[i].pos[0] > 9. - 1e-6 ) {
                            for (unsigned d=0;d<dim;++d) {
                                f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
                            }
                        }
                    }
                }
            }
            /// Quart de tete de reacteur nucleaire 3D
            /// blocage des noeuds situes en x = 0 dans la direction x
            /// blocage des noeuds situes en z = 0 dans la direction z
            /// blocage des noeuds situes en y = 301 dans la direction y
            ///---------------------------------------------------------
            else if ( structure == "reactor_head" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( m.node_list[i].pos[0] < 1e-6 ) {
                        f.add_constraint( "node["+to_string(i)+"].dep[0]", penalty_val );
                    }
                    if ( m.node_list[i].pos[2] < 1e-6 ) {
                        f.add_constraint( "node["+to_string(i)+"].dep[2]", penalty_val );
                    }
                    if ( m.node_list[i].pos[1] < 301. + 1e-6 ) {
                        f.add_constraint( "node["+to_string(i)+"].dep[1]", penalty_val );
                    }
                }
            }
            /// Ailette 3D
            /// blocage des noeuds situes en ( z = 0 ou z = 50 ) et non ( x > 16 et y < 16 ) dans toutes les directions
            /// blocage des noeuds situes en ( z = 0 ou z = 50 ) et ( x > 16 et y < 16 ) dans les directions x et y
            /// deplacement impose aux noeuds situes en ( z = 0 ou z = 50 ) et ( x > 16 et y < 16 ) dans la direction z : u_d = 0.2 z
            /// ---------------------------------------------------------------------------------------------------------------------
            else if ( structure == "blade" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( ( m.node_list[i].pos[2] == 0 or m.node_list[i].pos[2] == 50 ) and ( not ( m.node_list[i].pos[0] > 16 and m.node_list[i].pos[1] < 16 ) ) ) {
                        for (unsigned d=0;d<dim;++d) {
                            f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
                        }
                    }
                    if ( ( m.node_list[i].pos[2] == 0 or m.node_list[i].pos[2] == 50 ) and ( m.node_list[i].pos[0] > 16 and m.node_list[i].pos[1] < 16 ) ) {
                        for (unsigned d=0;d<2;++d) {
                            f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
                        }
                        f.add_constraint( "node["+to_string(i)+"].dep[2] - 0.2", penalty_val );
                    }
                }
            }
            /// Quart de conduite 3D
            /// blocage des noeuds situes en x = 0 dans la direction x
            /// blocage des noeuds situes en y = 0.528 dans la direction y
            /// blocage des noeuds situes en z = 0.229 dans la direction z
            ///-----------------------------------------------------------
            else if ( structure == "pipe" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( m.node_list[i].pos[0] > -1e-6 ) {
                        f.add_constraint( "node["+to_string(i)+"].dep[0]", penalty_val );
                    }
                    if ( m.node_list[i].pos[1] > 0.528 - 1e-6 ) {
                        f.add_constraint( "node["+to_string(i)+"].dep[1]", penalty_val );
                    }
                    if ( m.node_list[i].pos[2] > 0.229 - 1e-6 ) {
                        f.add_constraint( "node["+to_string(i)+"].dep[2]", penalty_val );
                    }
                }
            }
            /// SAP 3D
            /// blocage des noeuds situes a l'exterieur du cylindre C_1 d'axe (Ox) et de rayon 0.04675 : y^2 + z^2 = 0.04674^2 et au-dessus dun plan P_1 : x = 0.0675
            ///------------------------------------------------------------------------------------------------------------------------------------------------------
            else if ( structure == "SAP" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( pow( m.node_list[i].pos[1], 2 ) + pow( m.node_list[i].pos[2], 2 ) > pow( 0.04675, 2 ) - 1e-6 and m.node_list[i].pos[0] > 0.0675 - 1e-6 ) {
                        for (unsigned d=0;d<dim;++d) {
                            f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
                        }
                    }
                }
            }
            /// Inclusions/Trous spheriques 3D
            /// blocage des noeuds situes en x = 0 dans toutes les directions
            /// -------------------------------------------------------------
            else if ( structure == "spherical_inclusions" or structure == "spherical_holes" ) {
                for (unsigned i=0;i<m.node_list.size();++i) {
                    if ( m.node_list[i].pos[0] < 1e-6 ) {
                        for (unsigned d=0;d<dim;++d) {
                            f.add_constraint( "node["+to_string(i)+"].dep["+to_string(d)+"]", penalty_val );
                        }
                    }
                }
            }
            m.update_skin();
            if ( pb == "direct" ) {
                /// effort surfacique applique sur les bords des elements situes en x = 0 : F_d = -x
                /// --------------------------------------------------------------------------------
//                for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
//                    if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] < 1e-6 ) {
//                        m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( -1., 0., 0. ) );
//                    }
//                }
                /// effort surfacique applique sur les bords des elements situes en x = 1 : F_d = +x
                /// --------------------------------------------------------------------------------
//                for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
//                    if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] > 1. - 1e-6 ) {
//                        m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 1., 0., 0. ) );
//                    }
//                }
                /// effort surfacique applique sur les bords des elements situes en z = 0 : F_d = +z
                /// --------------------------------------------------------------------------------
//                for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
//                    if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[2] < 1e-6 ) {
//                        m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 0., 0., 1. ) );
//                    }
//                }
                /// Barre rectangulaire 3D en traction
                /// effort surfacique applique sur les bords des elements situes en z = 1 : F_d = -z
                /// --------------------------------------------------------------------------------
                if ( structure == "beam_traction" ) {
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                        if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[2] > 1. - 1e-6 ) {
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 0., 0., -1 ) );
                        }
                    }
                }
                /// Barre rectangulaire 3D en flexion
                /// effort surfacique applique sur les bords des elements situes en z = 1 : F_d = -z
                /// --------------------------------------------------------------------------------
                else if ( structure == "beam_flexion" ) {
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                        if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[2] > 1. - 1e-6 ) {
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 0., 0., -1. ) );
                        }
                    }
                }
                /// Barre rectangulaire trouee 3D
                /// effort surfacique applique sur les bords des elements situes en y = 0.7 : F_d = +y
                /// ----------------------------------------------------------------------------------
                else if ( structure == "beam_hole" ) {
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                        if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] > 0.7 - 1e-6 ) {
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 0., 1., 0. ) );
                        }
                    }
                }
                /// Quart de plaque rectangulaire trouee 3D
                /// effort surfacique applique sur les bords des elements situes en x = 10 : F_d = +x
                /// ---------------------------------------------------------------------------------
                else if ( structure == "plate_hole" ) {
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                        if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] > 10. - 1e-6 ) {
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 1., 0., 0. ) );
                        }
                    }
                }
                /// Plaque rectangulaire trouee complete 3D
                /// effort surfacique applique sur les bords des elements situes en x = 20 : F_d = +x
                /// ---------------------------------------------------------------------------------
                else if ( structure == "plate_hole_full" ) {
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                        if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] > 20. - 1e-6 ) {
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 1., 0., 0. ) );
                        }
                    }
                }
                /// Moyeu-rotor 3D de l'helicoptere NH90
                /// effort surfacique applique sur les bords des elements situes a l'exterieur du plan P : 0.993771220888593 * (x - 505.668689385943) - 0.00145804725003041 * (y + 1.04229616690589) + 0.111429953925374 * (z + 6.713826134760) = 0 : F_d = +n
                /// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                else if ( structure == "hub_rotor_helico" ) {
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                        if ( 0.993771220888593 * (center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] - 505.668689385943) - 0.00145804725003041 * (center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] + 1.04229616690589) + 0.111429953925374 * (center( *m.sub_mesh(Number<1>()).elem_list[i] )[2] + 6.713826134760)  > 0. ) {
                            Vec<T,dim> force_surf = m.sub_mesh(Number<1>()).elem_list[i]->sample_normal_virtual() * 1.;
//                             Vec<T,dim> force_surf( 1., 0., 0. );
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", force_surf );
                        }
                    }
                }
                /// Quart de tete de reacteur nucleaire 3D
                /// effort surfacique applique sur toute la surface interne : bords des elements situes sur la sphere de centre (0, 332.75061, 0) et de rayon 51.5 : x^2 + (y - 332.75061)^2 + z^2 = 51.5^2 : F_d = -n
                ///                                                           bords des elements situes sous le plan P_1 : y = 332.75061 et sur le cylindre C_1 d'axe (Oy) et de rayon 51.5 : x^2 + z^2 = 51.5^2 : F_d = -n
                ///                                                           bords des elements situes sous le plan P_2 : y = 310.75 et sur le cylindre C_2 d'axe (Oy) et de rayon 54.5 : x^2 + z^2 = 54.5^2 : F_d = -n
                ///                                                           bords des elements situes sur le plan P_2 : y = 310.75 et compris entre les cylindres C_1 et C_2 : F_d = -n
                ///                                                           bords des elements situes sur le cylindre C_3 d'axe (Ay) et de rayon 5.125 : (x - 12)^2 + (z - 36)^2 = 5.125^2 : F_d = -n
                ///                                                           bords des elements situes sur le cylindre C_4 d'axe (By) et de rayon 5.125 : (x - 36)^2 + (z - 12)^2 = 5.125^2 : F_d = -n
                /// effort surfacique applique sur les bords des elements situes en y = 306.75 et a l'exterieur du cylindre C_5 d'axe (Oy) et de rayon 68 : x^2 + z^2 = 68^2 : F_d = +y = -n
                /// effort surfacique applique sur les bords des elements situes en y = 329.75 et a l'exterieur du cylindre C_5 : F_d = -y = -n
                /// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                else if ( structure == "reactor_head" ) {
                    if ( mesh_size == "coarse" ) {
                        for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                            if ( pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0], 2 ) + pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] - 332.75061, 2 ) + pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[2], 2 ) < pow( 51.5, 2 ) + 1e-6 ) {
                                Vec<T,dim> force_surf = m.sub_mesh(Number<1>()).elem_list[i]->sample_normal_virtual() * -1.;
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", force_surf );
                            }
                            if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] < 332.75061 + 1e-6 and pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0], 2 ) + pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[2], 2 ) < pow( 51.5, 2 ) + 1e-6 ) {
                                Vec<T,dim> force_surf = m.sub_mesh(Number<1>()).elem_list[i]->sample_normal_virtual() * -1.;
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", force_surf );
                            }
                        }
                    }
                    else if ( mesh_size == "fine" ) {
                        for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                            if ( pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0], 2 ) + pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] - 332.75061, 2 ) + pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[2], 2 ) < pow( 51.5, 2 ) + 1e-6 ) {
                                Vec<T,dim> force_surf = m.sub_mesh(Number<1>()).elem_list[i]->sample_normal_virtual() * -1.;
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", force_surf );
                            }
                            if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] < 332.75061 + 1e-6 and pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0], 2 ) + pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[2], 2 ) < pow( 51.5, 2 ) + 1e-6 ) {
                                Vec<T,dim> force_surf = m.sub_mesh(Number<1>()).elem_list[i]->sample_normal_virtual() * -1.;
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", force_surf );
                            }
                            if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] < 310.75 + 1e-6 and pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0], 2 ) + pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[2], 2 ) < pow( 54.5, 2 ) + 1e-6 ) {
                                Vec<T,dim> force_surf = m.sub_mesh(Number<1>()).elem_list[i]->sample_normal_virtual() * -1.;
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", force_surf );
                            }
                            if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] > 310.75 - 1e-6 and center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] < 310.75 + 1e-6 and pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0], 2 ) + pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[2], 2 ) < pow( 54.5, 2 ) + 1e-6 ) {
                                Vec<T,dim> force_surf = m.sub_mesh(Number<1>()).elem_list[i]->sample_normal_virtual() * -1.;
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", force_surf );
                            }
                            if ( pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] - 12., 2 ) + pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[2] - 36., 2 ) < pow( 5.125, 2 ) + 1e-6 ) {
                                Vec<T,dim> force_surf = m.sub_mesh(Number<1>()).elem_list[i]->sample_normal_virtual() * -1.;
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", force_surf );
                            }
                            if ( pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] - 36., 2 ) + pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[2] - 12., 2 ) < pow( 5.125, 2 ) + 1e-6 ) {
                                Vec<T,dim> force_surf = m.sub_mesh(Number<1>()).elem_list[i]->sample_normal_virtual() * -1.;
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", force_surf );
                            }
                            if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] > 306.75 - 1e-6 and center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] < 306.75 + 1e-6 and pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0], 2 ) + pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[2], 2 ) > pow( 68., 2 ) - 1e-6 ) {
                                Vec<T,dim> force_surf = m.sub_mesh(Number<1>()).elem_list[i]->sample_normal_virtual() * -1.;
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", force_surf );
                            }
                            if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] > 329.75 - 1e-6 and center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] < 329.75 + 1e-6 and pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0], 2 ) + pow( center( *m.sub_mesh(Number<1>()).elem_list[i] )[2], 2 ) > pow( 68., 2 ) - 1e-6 ) {
                                Vec<T,dim> force_surf = m.sub_mesh(Number<1>()).elem_list[i]->sample_normal_virtual() * -1.;
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", force_surf );
                            }
                        }
                    }
                }
                /// Joint de porte 3D de vehicule auto
                /// effort surfacique applique sur les bords des elements situes a l'exterieur du plan P : 0.999275522826568 * x + 0.038058238000309 * y - 0.469486267231102 = 0 : F_d = -x
                /// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------
                else if ( structure == "door_seal" ) {
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                        if ( 0.999275522826568 * center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] + 0.038058238000309 * center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] - 0.469486267231102 > 0. ) {
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( -1., 0., 0. ) );
                        }
                    }
                }
                /// Point de soudure 3D
                /// loading pull : effort surfacique applique sur les bords des elements situes en y = 3.5 : F_d = +y
                /// loading peeling : effort surfacique applique sur les bords des elements situes en y = 3.5 : F_d = +y
                ///                   effort surfacique applique sur les bords des elements situes en y = -6.3 : F_d = -y
                /// loading shear : effort surfacique applique sur les bords des elements situes en y = 3.5 F_d = +z
                /// -----------------------------------------------------------------------------------------------------
                else if ( structure == "spot_weld" ) {
                    if ( loading == "pull" ) {
                        for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                            if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] > 3.5 - 1e-6 ) {
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 0., 1., 0. ) );
                            }
                        }
                    }
                    else if ( loading == "peeling" ) {
                        for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                            if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] > 3.5 - 1e-6 ) {
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 0., 1., 0. ) );
                            }
                            if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] < -6.3 + 1e-6 ) {
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 0., -1., 0. ) );
                            }
                        }
                    }
                    else if ( loading == "shear" ) {
                        for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                            if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[1] > 3.5 - 1e-6 ) {
                                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 0., 0., 1. ) );
                            }
                        }
                    }
                }
                /// Quart de conduite 3D
                /// effort surfacique applique sur les bords des elements situes en z = 0 : F_d = -8.281e6 z
                /// ----------------------------------------------------------------------------------------
                else if ( structure == "pipe" ) {
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                        if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[2] < 1e-6 ) {
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 0., 0., -8.281e6 ) );
                        }
                    }
                }
                /// SAP 3D
                /// effort surfacique applique sur les bords des elements situes en x = 0.1 : F_d = +1e6 x
                /// --------------------------------------------------------------------------------------
                else if ( structure == "SAP" ) {
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                        if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] > 0.1 - 1e-6 ) {
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 1e6, 0., 0. ) );
                        }
                    }
                }
                /// Inclusions/Trous spheriques 3D
                /// effort surfacique applique sur les bords des elements situes en x = 1 : F_d = +x
                /// --------------------------------------------------------------------------------
                else if ( structure == "spherical_inclusions" or structure == "spherical_holes" ) {
                    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
                        if ( center( *m.sub_mesh(Number<1>()).elem_list[i] )[0] > 1. - 1e-6 ) {
                            m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", Vec<T,dim>( 1., 0., 0. ) );
                        }
                    }
                }
            }
        }
    }
}

/// Creation des conditions de chargement du pb adjoint a partir de l'extracteur
/// ----------------------------------------------------------------------------
template<class TM, class TF, class Pvec>
void set_load_conditions( TM &m_adjoint, const TF &f_adjoint, const TM &m, const TM &m_crown, const Vec<unsigned> &elem_list_interest_quantity, const unsigned &node_interest_quantity, const Pvec &pos_interest_quantity, const string &interest_quantity, const string &direction_extractor, const string &pointwise_interest_quantity, const bool want_local_enrichment = false ) {
    
    if ( want_local_enrichment and interest_quantity.find("pointwise") != string::npos ) {
        if ( pointwise_interest_quantity == "node" )
            m_adjoint.pos_handbook = m.node_list[ node_interest_quantity ].pos;
        else if ( pointwise_interest_quantity == "pos" )
            m_adjoint.pos_handbook = pos_interest_quantity;
    }
    
    if ( interest_quantity == "mean_sigma" ) {
        if ( want_local_enrichment == 0 )
            apply( m_adjoint.elem_list, Construct_Extractor_Mean_Sigma(), m, elem_list_interest_quantity );
        else {
            cerr << "Arret brutal, car l'enrichissement local pour la quantite d'interet " << interest_quantity << " dans la direction " << direction_extractor << " n'est pas implementee..." << endl << endl;
            throw "Anguille sous coquille...";
        }
    }
    else if ( interest_quantity == "mean_epsilon" ) {
        if ( want_local_enrichment == 0 )
            apply( m_adjoint.elem_list, Construct_Extractor_Mean_Epsilon(), m, elem_list_interest_quantity );
        else {
            cerr << "Arret brutal, car l'enrichissement local pour la quantite d'interet " << interest_quantity << " dans la direction " << direction_extractor << " n'est pas implementee..." << endl << endl;
            throw "Anguille sous coquille...";
        }
    }
    else if ( interest_quantity.find("pointwise") != string::npos ) {
        if ( want_local_enrichment )
            apply( m_adjoint.elem_list, Calcul_Dep_Handbook_In_Infinite_Domain(), m_adjoint, f_adjoint );
        else {
            if ( interest_quantity == "pointwise_dep" and pointwise_interest_quantity == "node" )
                apply( m_adjoint.node_list, Construct_Extractor_Pointwise_Dep(), m, node_interest_quantity );
            else {
                cerr << "Arret brutal, car l'application d'un chargement ponctuel sans enrichissement local pour la quantite d'interet " << interest_quantity << " dans la direction " << direction_extractor << " n'est pas implementee..." << endl << endl;
                throw "Anguille sous coquille...";
            }
        }
    }
    else if ( interest_quantity == "SIF" or interest_quantity == "stress_intensity_factor" ) {
        if ( want_local_enrichment == 0 )
            apply( m_adjoint.elem_list, Construct_Extractor_SIF(), m_crown );
        else {
            cerr << "Arret brutal, car l'enrichissement local pour la quantite d'interet " << interest_quantity << " dans la direction " << direction_extractor << " n'est pas implementee..." << endl << endl;
            throw "Anguille sous coquille...";
        }
    }
}

/// Verification des contraintes cinematiques
/// -----------------------------------------
template<class TF>
void check_constraints( const TF &f ) {
    cout << "Verification des contraintes cinematiques" << endl << endl;

    cout << "nb de constraintes : " << f.nb_constraints() << endl;
    for (unsigned nc=0;nc<f.constraints.size();++nc) { // f.nb_constraints() = f.constraints.size()
        for (unsigned j=0;j<f.constraints[nc].coeffs.size();++j) {
            if ( f.constraints[nc].coeffs[j].type_var == -1 )
                cout << f.constraints[nc].coeffs[j].val << " * unk[ noeud " << f.constraints[nc].coeffs[j].num << ", direction " << f.constraints[nc].coeffs[j].num_in_vec << " ]";
            else if ( f.constraints[nc].coeffs[j].type_var == -2 )
                cout << f.constraints[nc].coeffs[j].val << " * unk[ global, direction " << f.constraints[nc].coeffs[j].num_in_vec << " ]";
            cout << (j<f.constraints[nc].coeffs.size()-1 ? " + " : "" );
        }
        cout << " == " << f.constraints[nc].res << endl;
    }
    cout << endl;
}

/// Creation des conditions de chargement nul
/// -----------------------------------------
template<class TF, class TM>
void reset_load_conditions( TF &f, TM &m, const bool debug_geometry = false ) {
    
    static const unsigned dim = TM::dim;
    typedef typename TM::TNode::T T;
    
    Vec< Vec<unsigned> > face_type;
    construct_face_type( m, f, face_type, debug_geometry );
    
    m.update_skin();
    
    for (unsigned i=0;i<m.sub_mesh(Number<1>()).elem_list.size();++i) {
        for (unsigned d=0;d<dim;++d) {
            if ( face_type[ m.sub_mesh(Number<1>()).elem_list[i]->number ][ d ] == 2 ) {
                Vec<T,dim> f_surf = m.sub_mesh(Number<1>()).elem_list[i]->get_field( "f_surf", StructForType<Vec<T,dim> >() );
                f_surf[ d ] = 0.;
                m.sub_mesh(Number<1>()).elem_list[i]->set_field( "f_surf", f_surf );
            }
        }
    }
    
    for (unsigned i=0;i<m.node_list.size();++i) {
        for (unsigned d=0;d<dim;++d) {
            m.node_list[i].f_nodal[d] = 0.;
            m.node_list[i].pre_f_nodal[d] = 0.;
        }
    }
    
    Vec<T,dim> f_vol, pre_f_vol;
    f_vol.set( 0. );
    pre_f_vol.set( 0. );
    Vec<T,unsigned(dim*(dim+1)/2) > pre_sig, pre_eps;
    pre_sig.set( 0. );
    pre_eps.set( 0. );
    
    set_field_alternativeontype( m, Number< AreSameType< typename ExtractDM<f_vol_DM>::ReturnType<TM>::T, void >::res >(), f_vol, f_vol_DM() );
    set_field_alternativeontype( m, Number< AreSameType< typename ExtractDM<pre_f_vol_DM>::ReturnType<TM>::T, void >::res >(), pre_f_vol, pre_f_vol_DM() );
    set_field_alternativeontype( m, Number< AreSameType< typename ExtractDM<pre_sigma_DM>::ReturnType<TM>::T, void >::res >(), pre_sig, pre_sigma_DM() );
    set_field_alternativeontype( m, Number< AreSameType< typename ExtractDM<pre_epsilon_DM>::ReturnType<TM>::T, void >::res >(), pre_eps, pre_epsilon_DM() );
}

#endif // Boundary_conditions_h
