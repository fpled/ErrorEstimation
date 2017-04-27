//
// C++ Implementation: main_pgd_cpp
//
// Description: Global/Goal-oriented error estimation methods for PGD
//
//
// Author: Pled Florent <pled@lmt.ens-cachan.fr>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "build/problem_space/all_in_one.h" // sert a forcer le logiciel scons a generer le repertoire build et ses codes sources .h et .cpp correspondant a la formulation
#include "build/problem_parameter/all_in_one.h" // sert a forcer le logiciel scons a generer le repertoire build et ses codes sources .h et .cpp correspondant a la formulation
#include "Mesh.h"
#include "Material_properties.h"
#include "Boundary_conditions.h"
#include "Display.h"
#include "CONNECTIVITY/Calcul_connectivity.h"
#include "VERIFICATION/Verification.h"
#include "DISCRETIZATION_ERROR/Calcul_discretization_error.h"
#include "Calcul_global_error_estimation_PGD.h"
#include "Calcul_goal_oriented_error_estimation.h"
#include "PGD/PGD.h"

#include "LMT/include/containers/gnuplot.h"
#include "LMT/include/containers/matlabplot.h"
#include "LMT/include/containers/matcholamd.h"
#include "LMT/include/containers/conjugate_gradient.h"
#include "LMT/include/containers/MatWithTinyBlocks.h"

#include "LMT/include/util/MKL_direct_solver.h"
#include "LMT/include/util/MKL_iterative_solver.h"
#include "LMT/include/util/MUMPS_solver.h"

using namespace LMT;
using namespace std;

int main( int argc, char **argv ) {
    TicToc t_total;
    t_total.start();
    static const unsigned dim = 2;
    static const bool wont_add_nz = true;
    typedef Mesh<Mesh_carac_space<double,dim> > TM;
    typedef Formulation<TM,FormulationElasticity,DefaultBehavior,double,wont_add_nz> TF;
    typedef TM::Pvec Pvec;
    typedef TM::TNode::T T;
    typedef Mesh<Mesh_carac_parameter<double,1> > TM_param;
    typedef Formulation<TM_param,FormulationParam,DefaultBehavior,double,wont_add_nz> TF_param;
    typedef TM_param::Pvec Pvec_param;
    typedef TM::TElemListPtr TElemListPtr;
    typedef Mat<T, Sym<>, SparseLine<> > TMatSymSparse;
    static const string structure = "plate_flexion"; // structure
    // 2D : plate_traction, plate_flexion, plate_hole, plate_crack, structure_crack, test_specimen, weight_sensor, circular_inclusions, circular_holes,
    //      square_n (n=32,64,128,256,512,1024,2048,4096), square_init_n (n=32,64,128,256,512,1024,2048,4096)
    // 3D : beam_traction, beam_flexion, beam_hole, plate_hole, plate_hole_full, hub_rotor_helico, reactor_head, door_seal, spot_weld, blade, pipe, SAP, spherical_inclusions, spherical_holes,
    //      test_specimen_n (n=5,10,15,20,25,Q1_5,Q3_5,Q3_10,Q3_15,Q3_20,Q3_25,Q4_5,Q6_5,Q8_5), hashin_op_n (op=filtered,truncated,willot2015; n=32,64,128,256,512)
    static const string mesh_size = "fine"; // taille du maillage : coarse, fine
    // 2D : plate_hole, plate_crack, structure_crack, test_specimen, weigth_sensor
    // 3D : plate_hole, spot_weld, reactor_head
    static const string loading = "Step-2"; // chargement
    // spot_weld (3D) : pull, shear, peeling
    // plate_crack (2D) : pull, shear
    // test_specimen_n (3D) : Step-1, ..., Step-9
    static const unsigned deg_p = 1; // degre de l'analyse elements finis : 1, 2, ...
    static const string boundary_condition_D = "penalty"; // methode de prise en compte des conditions aux limites de Dirichlet (en deplacement) pour le pb direct : lagrange, penalty
    
    /// Global discretization error
    /// ---------------------------
    static const bool want_global_discretization_error = 0; // calcul de l'erreur de discretisation globale du pb direct
    static const bool want_local_discretization_error = 0; // calcul de l'erreur de discretisation locale du pb direct
    static const bool want_solve_ref = 0; // calcul d'une solution de reference sur un maillage de reference (tres fin)
    static const unsigned refinement_level_ref = 2; // degre du h-refinement pour la construction du maillage de reference du pb direct :
    // 1 -> sous-decoupage en 4/8 elements en 2D/3D
    // 2 -> sous-decoupage en 16/64 elements en 2D/3D
    // 3 -> sous-decoupage en 64/512 elements en 2D/3D
    // 4 -> sous-decoupage en 256/4096 elements en 2D/3D
    // 5 -> sous-decoupage en 1024/32768 elements en 2D/3D
    // 6 -> sous-decoupage en 4096/32768 elements en 2D/3D
    // 7 -> sous-decoupage en 16384/262144 elements en 2D/3D
    // 8 -> sous-decoupage en 65536/2097152 elements en 2D/3D
    
    /// Global error estimation method
    /// ------------------------------
    static const bool want_global_estimation = 1; // calcul d'un estimateur d'erreur globale (au sens de la norme energetique)
    static const string method = "EET"; //methode de construction de champs admissibles pour le pb direct : EET, SPET, EESPT
    static const string method_adjoint = "EET"; // methode de construction de champs admissibles pour le pb adjoint : EET, SPET, EESPT
    static const unsigned cost_function = 0; // fonction-cout pour les methodes EET, EESPT :
    // 0 : norme matricielle sans coefficient de ponderation (matrice identite)
    // 1 : norme matricielle avec coeff de ponderation (en 1/mes(face)^2)
    // 2 : norme energetique
    static const T penalty_val_N = 1e6; // coefficient de penalisation pour la prise en compte des conditions aux limites de Neumann (en effort) (methode EESPT)
    static const string solver = "LDL"; // solveur pour la resolution des pbs locaux avec blocage auto du noyau : CholMod (sym, def, pos), LDL (sym) // types de solveur sans blocage auto du noyau (-> ne marche pas!) : CholFactorize (sym, def, pos), LUFactorize, Inv, UMFPACK
    static const string solver_minimisation = "UMFPACK"; // solveur pour la resolution des pbs de minimisation : LDL (sym), UMFPACK, LUFactorize, Inv
    
    /// Enhanced technique
    /// ------------------
    static const bool enhancement_with_geometric_criterium = 0; // amelioration de la construction des densites d'effort (methodes EET, EESPT) basee sur un critere geometrique
    static const string geometric_criterium = "radius_ratio"; // critere d'amelioration geometrique : radius_ratio, edge_ratio
    static const T val_geometric_criterium = 0.34; // valeur du critere d'amelioration geometrique
    // critere radius_ratio : rapport entre rayon du cercle/sphere inscrit(e) et rayon du cercle/sphere circonscrit(e) à un élément
    // critere edge_ratio : rapport entre longueur/aire minimale et longueur/aire maximale des bords/faces d'un élément
    static const bool enhancement_with_estimator_criterium = 0; // amelioration de la construction des densites d'effort (methodes EET, EESPT) basee sur un critere sur l'estimateur d'erreur
    static const T val_estimator_criterium = 0.8; // valeur du critere d'amelioration sur l'estimateur d'erreur : rapport entre la contribution elementaire au carre a l'erreur estimee et la contribution elementaire maximale au carre
    
    /// Goal-oriented error estimation method
    /// -------------------------------------
    static const bool want_local_estimation = 0; // calcul de l'erreur locale sur une quantite d'interet
    static const bool want_introduction_sigma_hat_m = 1; // introduction de sigma_hat_m pour le calcul de l'erreur sur une quantite d'interet locale
    static const bool want_local_refinement = 1; // raffinement local du mailage adjoint
    static const bool want_local_enrichment = 0; // enrichissement local avec fonctions handbook
    static const bool want_local_improvement = 0; // amelioration des bornes pour le calcul de l'erreur locale sur une quantite d'interet
    static const bool want_solve_eig_local_improvement = 0; // resolution du pb aux valeurs propres generalisees pour calculer la constante dans l'amelioration des bornes d'erreur locale
    static const bool use_mask_eig_local_improvement = 0; // utilisation d'un masque (image) pour definir le maillage du pb aux valeurs propres generalisees
    static const bool want_solve_local_ref = 0; // calcul de la quantite d'interet (quasi-)exacte sur un maillage de reference
    static const string interest_quantity = "mean_sigma"; // quantite d'interet : mean_sigma, mean_epsilon, pointwise_dep, pointwise_sigma, pointwise_epsilon, SIF (stress intensity factor)
    static const string direction_extractor = "xx"; // direction de l'operateur d'extraction
    // quantites d'interet mean_sigma, mean_epsilon, pointwise_sigma, pointwise_epsilon : xx, yy, xy, zz, xz, yz
    // quantite d'interet pointwise_dep : x, y, z
    // quantite d'interet SIF (stress intensity factor) : I, II, III
    
    /// Zone of interest
    /// ----------------
    static const Vec<unsigned> elem_list_interest_quantity( 4886 ); // liste des elements definissant la zone d'interet (quantite d'interet mean_sigma, mean_epsilon)
    static const string pointwise_interest_quantity = "node"; // definition de la quantite d'interet ponctuelle : node, pos
    static const unsigned node_interest_quantity( 661 ); // noeud definissant la zone d'interet (quantite d'interet pointwise_dep, pointwise_sigma, pointwise_epsilon)
    static const Pvec pos_interest_quantity( 49.5, 135.5 ); // position definissant la zone d'interet (quantite d'interet pointwise_dep, pointwise_sigma, pointwise_epsilon)
    static const Pvec pos_crack_tip( 109., 105. ); // position de la pointe de fissure (quantite d'interet SIF) : ( 3.5, 0. ) pour plate_crack et ( 109., 105. ) pour structure_crack
    static const T angle_crack = atan2( -17, -3 ); // angle de la fissure (en rad) (quantite d'interet SIF) : 0. pour plate_crack et atan2( -17, -3 ) pour structure_crack
    static const T radius_Ri = 6; // rayon du cercle interieur a la couronne omega entourant la pointe de fissure (quantite d'interet SIF) : 1.6 pour plate_crack et 6 pour structure_crack
    static const T radius_Re = 8; // rayon du cercle exterieur a la couronne omega entourant la pointe de fissure (quantite d'interet SIF) : 3.4 pour plate_crack et 8 pour structure_crack
    
    /// Local refinement for adjoint problem
    /// ------------------------------------
    static const T l_min_refinement = 1.0; // longueur minimale des cotes des elements du maillage adjoint
    static const T k_refinement = 1.0; // coefficient d'augmentation de la longueur maximale des cotes des elements en fonction de la distance au point, au cercle, ... autour duquel on souhaite raffiner le maillage
    static const bool spread_cut = true; // propagation du raffinement au reste du maillage (étendue de la coupe si l'arête coupée n'est pas la plus longue de l'élément)
    
    /// Local enrichment with handbook functions
    /// ----------------------------------------
    static const unsigned nb_layers_nodes_enrichment = 2; // nombre de couches/rangées de noeuds enrichis par la PUM (pb direct)
    
    /// Improved goal-oriented error estimation methods based on Steklov/Rayleigh constants
    /// -----------------------------------------------------------------------------------
    static const string local_improvement = "rayleigh"; // amelioration du calcul de l'erreur locale sur une quantite d'interet, basee sur la constante de Steklov ou le quotient de Rayleigh : steklov, rayleigh
    static const string shape = "circle"; // forme geometrique des domaines homothetiques
    static const T k_min = 2.5; // parametre k_min du domaine homothetique (amelioration steklov) : facteur multiplicatif devant le rayon du cercle/sphere (shape circle/sphere)
    static const T k_max = 7.; // parametre k_max du domaines homothetique (amelioration steklov) : facteur multiplicatif devant le rayon du cercle/sphere (shape circle/sphere)
    static const T k_opt = 4.4; // parametre k_opt du domaine homothetique (amelioration rayleigh) : facteur multiplicatif devant le rayon du cercle/sphere (shape circle/sphere)
    static const string integration_k = "trapeze"; // type d'integration sur le parametre k (amelioration steklov) : gauss, trapeze, IPP
    static const unsigned integration_nb_points = 1000; // nb d'intervalles pour l'integration type trapeze sur le parametre k (amelioration steklov)
    
    /// Proper Generalized Decomposition - PGD
    /// --------------------------------------
    static const bool want_normalization = 1; // normalisation
    static const unsigned max_mode = 2; // nb de modes max dans la decomposition
    static const unsigned max_iter = 10; // nb d'iterations max de l'algorithme de point fixe
    static const T tol_convergence_criterium_mode = 1e-4; // precision pour critere d'arret global (boucle sur les modes)
    static const T tol_convergence_criterium_iter = 1e-8; // precision pour critere d'arret local (processus iteratif)
    static const Vec<T,2> support_param( 1., 10. ); // support de l'espace des parametres
    static const unsigned nb_points_param = 100; // nb de points du maillage parametrique
    static const bool want_verif_PGD = 1; // verification de la decomposition PGD
    static const unsigned nb_vals_verif = 3; // nb de valeurs des parametres pris aleatoirement pour la verification de la decomposition PGD
    
    /// Verification equilibrium / solver
    /// ---------------------------------
    static const bool verif_eq = 1; // verification de l'equilibre global elements finis
    static const bool verif_compatibility_conditions = 1; // verification des conditions de compatibilite (equilibre elements finis) (methode EET)
    static const bool verif_eq_force_fluxes = 1; // verification de l'equilibre des densites d'effort (methodes EET, EESPT)
    static const T tol_compatibility_conditions = 1e-6; // tolerance pour la verification des conditions de compatibilite (equilibre elements finis) (methode EET)
    static const T tol_eq_force_fluxes = 1e-6; // tolerance pour la verification de l'equilibre des densites d'effort (methodes EET, EESPT)
    
    static const bool verif_solver = 1; // verification de la resolution des pbs locaux (methodes EET, SPET, EESPT)
    static const bool verif_solver_minimisation = 0; // verification de la resolution des pbs de minimisation (methodes EET, EESPT)
    static const bool verif_solver_enhancement = 0; // verification de la resolution des pbs locaux (amelioration des methodes EET, EESPT)
    static const bool verif_solver_minimisation_enhancement = 0; // verification de la resolution des pbs de minimisation (amelioration des methodes EET, EESPT)
    static const T tol_solver = 1e-6; // tolerance pour la verification de la resolution des pbs locaux (methodes EET, SPET, EESPT)
    static const T tol_solver_minimisation = 1e-6; // tolerance pour la verification de la resolution des pbs de minimisation (methodes EET, EESPT)
    static const T tol_solver_enhancement = 1e-6; // tolerance pour la verification de la resolution des pbs locaux (amelioration des methodes EET EESPT)
    static const T tol_solver_minimisation_enhancement = 1e-6; // tolerance pour la verification de la resolution des pbs de minimisation (amelioration des methodes EET, EESPT)
    
    /// Display outputs
    /// ---------------
    static const bool display_vtu = 0;
    static const bool display_vtu_adjoint = 0;
    static const bool display_vtu_lambda = 0;
    static const bool display_vtu_adjoint_lambda = 0;
    static const bool display_vtu_crown = 0;
    
    static const bool display_pvd_PGD_space = 0;
    static const bool display_pvd_PGD_param = 0;
    static const bool display_pvd_PGD_space_verif = 0;
    
    static const bool display_matlab = 0;
    
    /// -------------------------------------------- ///
    /// Construction de la solution PGD du pb direct ///
    /// -------------------------------------------- ///
    
    display_pb( dim, structure, deg_p  );
    
    /// Maillage en espace du pb direct
    /// -------------------------------
    TM m;
    set_mesh( m, structure, mesh_size, loading, deg_p, refinement_level_ref, want_global_discretization_error, want_local_discretization_error );
    string prefix = define_prefix( m, "direct", structure, loading, mesh_size, method, enhancement_with_geometric_criterium, enhancement_with_estimator_criterium, val_geometric_criterium, val_estimator_criterium, geometric_criterium, want_global_discretization_error, want_local_discretization_error, want_global_estimation, want_local_estimation, interest_quantity, direction_extractor, pointwise_interest_quantity, elem_list_interest_quantity, node_interest_quantity, pos_interest_quantity, pos_crack_tip, radius_Ri, radius_Re, want_local_improvement, local_improvement, shape, k_min, k_max, k_opt, want_local_enrichment, nb_layers_nodes_enrichment );
    
    /// Formulation en espace du pb direct
    /// ----------------------------------
    TF f( m );
    
    /// Proprietes materiaux du pb direct
    /// ---------------------------------
    set_material_properties( f, m, structure );
    
    /// Conditions aux limites du pb direct
    /// -----------------------------------
    set_constraints( f, m, boundary_condition_D, "direct", structure, loading );
    set_load_conditions( m, structure, loading, mesh_size );
    
    /// Partition des elements du maillage en espace du pb direct
    /// ---------------------------------------------------------
    Vec< Vec<unsigned> > elem_group; // vecteur contenant une liste de pointeurs sur les elements definissant les zones avec parametres inconnus
    partition_elem_list( m, structure, elem_group );
    
    /// Maillage en parametre du pb direct
    /// ----------------------------------
    Vec<TM_param> m_param;
    m_param.resize( elem_group.size()-1 );
    for (unsigned p=0;p<elem_group.size()-1;++p)
        set_mesh_param( m_param[p], support_param[0], support_param[1], nb_points_param );
    
    /// Formulation en parametre du pb direct
    /// -------------------------------------
    Vec<TF_param> f_param;
    f_param.resize( elem_group.size()-1 );
    for (unsigned p=0;p<elem_group.size()-1;++p)
        f_param[p].set_mesh( &m_param[p] );
    
    /// Defintion des fonctions a variables separees
    /// --------------------------------------------
    unsigned nb_modes; // nb de modes dans la decomposition
    Vec< Vec<T>, max_mode > dep_space;
    Vec< Vec< Vec<T>, max_mode > > dep_param;
    dep_param.resize( elem_group.size()-1 );
    Vec< Vec<T> > vals_param;
    vals_param.resize( elem_group.size()-1 );
    for (unsigned p=0;p<elem_group.size()-1;++p)
        vals_param[p] = generate( m_param[p].node_list, ExtractDMi<pos_DM>( 0 ) );
    
    Vec< DisplayParaview, max_mode > dp_space;
    Vec< Vec< DisplayParaview, max_mode > > dp_param;
    dp_param.resize( elem_group.size()-1 );
    Vec<string> lp_space("all");
//    lp_space.push_back( "dep" );
//    lp_space.push_back( "young_eff" );
    Vec<string> lp_param;
    lp_param.push_back( "dep" );
    
    /// Resolution du pb direct
    /// -----------------------
    TicToc t;
    t.start();
    
    /// Construction des opérateurs et du second membre en espace
    /// ---------------------------------------------------------
    f.allocate_matrices();
    f.shift();
    f.assemble( false, true );
    Vec<T> F_space = f.sollicitation;
    Vec<TMatSymSparse> K_space;
    K_space.resize(elem_group.size());
    for (unsigned g=0;g<elem_group.size();++g) {
        for (unsigned j=0;j<m.elem_list.size();++j) {
            if ( find( elem_group[g], _1 == j ) )
                m.elem_list[j]->set_field( "alpha", 1. );
            else
                m.elem_list[j]->set_field( "alpha", 0. );
        }
        f.assemble( true, false );
        K_space[g] = f.matrices(Number<0>());
    }
    
    /// Construction des opérateurs et seconds membres en parametre
    /// -----------------------------------------------------------
    Vec< Vec<T> > F_param;
    Vec< Vec<TMatSymSparse,2> > K_param;
    F_param.resize(elem_group.size()-1);
    K_param.resize(elem_group.size()-1);
    for (unsigned p=0;p<elem_group.size()-1;++p) {
        f_param[p].allocate_matrices();
        f_param[p].shift();
        f_param[p].assemble( false, true );
        F_param[p] = f_param[p].sollicitation;
        m_param[p].phi = 0;
        f_param[p].assemble( true, false );
        K_param[p][0] = f_param[p].matrices(Number<0>());
        m_param[p].phi = 1;
        f_param[p].assemble( true, false );
        K_param[p][1] = f_param[p].matrices(Number<0>());
    }
    
    /// Construction d'une solution elements finis en espace particuliere du pb direct
    /// ------------------------------------------------------------------------------
    for (unsigned i=0;i<m.elem_list.size();++i)
        m.elem_list[i]->set_field( "alpha", 1. );
    f.solve();
    Vec<T> dep_space_part = f.vectors[0];
    
    /// Verification de l'equilibre elements finis du pb direct
    /// -------------------------------------------------------
    if ( verif_eq )
        check_equilibrium( f, "direct" );
    
    /// Construction d'un champ de contrainte admissible particulier
    /// ------------------------------------------------------------
    
    Vec<bool> elem_flag_enh;
    Vec<bool> face_flag_enh;
    Vec<bool> elem_flag_bal;
    
    bool enhancement = 0;
    bool balancing = 0;
    
    Vec< Vec< Vec<T> > > force_fluxes_standard;
    Vec< Mat<T, Sym<> > > K_hat;
    Vec< Vec<T> > F_hat;
    Vec< Vec<T> > dep_space_part_hat;
    
    if ( want_global_estimation or want_local_estimation ) {
        
        display_method( "direct", "EET", cost_function, enhancement_with_geometric_criterium, enhancement_with_estimator_criterium, solver, solver_minimisation );
        
        cout << "Construction d'un champ de contrainte admissible particulier" << endl;
        cout << "------------------------------------------------------------" << endl << endl;
        
        construct_standard_force_fluxes_EET( m, f, "direct", cost_function, enhancement, face_flag_enh, solver_minimisation, force_fluxes_standard, want_local_enrichment, verif_solver_minimisation, tol_solver_minimisation, verif_compatibility_conditions, tol_compatibility_conditions );
        
        if ( verif_eq_force_fluxes )
            check_equilibrium_force_fluxes( m, f, "direct", force_fluxes_standard, tol_eq_force_fluxes, want_local_enrichment );
        
        construct_K_hat( m, f, K_hat );
        construct_F_hat( m, f, "direct", balancing, elem_flag_bal, elem_flag_enh, force_fluxes_standard, F_hat, want_local_enrichment );
        construct_dep_hat( m, f, solver, K_hat, F_hat, dep_space_part_hat, verif_solver, tol_solver );
    }
    
    T theta = 0.;
    Vec<T> theta_elem;
    Vec<T, max_mode > theta_2_mode;
    theta_2_mode.set( 0. );
    Vec< Vec< Vec<T> >, max_mode > dep_space_hat;
    
    unsigned n = 0;
    while ( true ) {
        
        cout << "Mode #" << n+1 << endl;
        cout << "-------" << endl << endl;
        
        /// Initialisation
        /// --------------
        unsigned k = 0;
        
        for (unsigned p=0;p<elem_group.size()-1;++p) {
            dep_param[ p ][ n ].resize( f_param[p].vectors[0].size() );
            dep_param[ p ][ n ].set( 1. );
        }
        dep_space[ n ].resize( f.vectors[0].size() );
        
        solve_space( m, f, n, F_space, F_param, K_param, elem_group, dep_param, dep_space );
        
        dp_space[ n ].add_mesh_iter( m, prefix + "_mode" + to_string(n+1) + "_space_iter", lp_space, k );
        for (unsigned p=0;p<elem_group.size()-1;++p)
            dp_param[ p ][ n ].add_mesh_iter( m_param[p], prefix + "_mode" + to_string(n+1) + "_param" + to_string(p+1) + "_iter", lp_param, k );
        
        /// Processus iteratif : Algorithme de point fixe
        /// ---------------------------------------------
        while ( true ) {
            k++;
            
            /// Construction et resolution des pbs en parametre
            /// -----------------------------------------------
            Vec< Vec<T> > dep_param_old;
            dep_param_old.resize( elem_group.size()-1 );
            for (unsigned p=0;p<elem_group.size()-1;++p) {
                dep_param_old[ p ] = dep_param[ p ][ n ];
                solve_param( m_param[p], f_param[p], p, n, F_space, F_param, K_space, K_param, elem_group, dep_space, dep_param, want_normalization );
            }
            
            /// Construction et resolution du pb en espace
            /// ------------------------------------------
            Vec<T> dep_space_old = dep_space[ n ];
            solve_space( m, f, n, F_space, F_param, K_param, elem_group, dep_param, dep_space );
            
//            cout << "Fonction en espace =" << endl;
//            cout << dep_space[ n ] << endl << endl;
//            for (unsigned p=0;p<elem_group.size()-1;++p) {
//                cout << "Fonction en parametre " << p << " =" << endl;
//                cout << dep_param[ p ][ n ] << endl << endl;
//            }
            
            dp_space[ n ].add_mesh_iter( m, prefix + "_mode" + to_string(n+1) + "_space_iter", lp_space, k );
            for (unsigned p=0;p<elem_group.size()-1;++p)
                dp_param[ p ][ n ].add_mesh_iter( m_param[p], prefix + "_mode" + to_string(n+1) + "_param" + to_string(p+1) + "_iter", lp_param, k );
            
            /// Stationnarite du produit mode en espace * modes en parametre dans le processus iteratif
            /// ---------------------------------------------------------------------------------------
            T stagnation_indicator = 0.;
            calc_stagnation_indicator( m, f, n, K_param, elem_group, dep_space, dep_param, dep_space_old, dep_param_old, stagnation_indicator );
            
            cout << "Iteration " << k << " : stagnation = " << stagnation_indicator << endl;
            if ( k >= max_iter ) { // if ( stagnation_indicator < tol_convergence_criterium_iter or k >= max_iter )
                cout << endl;
                break;
            }
        }
        
        /// Residu (au sens faible) associe a la solution PGD
        /// -------------------------------------------------
        T residual = 0.;
        T error_indicator = 0.;
        calc_error_indicator( m, f, n, F_space, F_param, K_param, elem_group, dep_space, dep_param, residual, error_indicator );
        
        cout << "Nb d'iterations = " << k << " : residu = " << residual << ", erreur = " << error_indicator << endl << endl;
        
        if ( want_global_estimation or want_local_estimation ) {
            
            /// Construction d'un champ de contrainte admissible a zero
            /// -------------------------------------------------------
            
            cout << "Construction d'un champ de contrainte admissible a zero" << endl;
            cout << "-------------------------------------------------------" << endl << endl;
            
            reset_load_conditions( m );
            
//            f.vectors[0] = - dep_space_part;
//            for (unsigned p=0;p<elem_group.size()-1;++p)
//                f.vectors[0] *= dot( F_param[p], dep_param[ p ][ n ] );
//            for (unsigned i=0;i<n+1;++i) {
//                T alpha = 1.;
//                for (unsigned p=0;p<elem_group.size()-1;++p) {
//                    if ( find( elem_group[p], _1 == elem.number ) )
//                        alpha *= dot( dep_param[ p ][ n ], K_param[p][1] * dep_param[ p ][ i ] );
//                    else
//                        alpha *= dot( dep_param[ p ][ n ], K_param[p][0] * dep_param[ p ][ i ] );
//                }
//                f.vectors[0] += alpha * dep_space[ i ];
//            }
            
            Vec< Vec< Vec<T> > > force_fluxes_standard_mode;
//            construct_standard_force_fluxes_EET( m, f, "direct", cost_function, enhancement, face_flag_enh, solver_minimisation, force_fluxes_standard_mode, want_local_enrichment, verif_solver_minimisation, tol_solver_minimisation, verif_compatibility_conditions, tol_compatibility_conditions );
            construct_standard_force_fluxes_EET_PGD( m, f, "direct", cost_function, enhancement, face_flag_enh, solver_minimisation, force_fluxes_standard_mode, dep_space, dep_param, dep_space_part, F_param, K_param, elem_group, n, want_local_enrichment, verif_solver_minimisation, tol_solver_minimisation, verif_compatibility_conditions, tol_compatibility_conditions );
            
            if ( verif_eq_force_fluxes )
                check_equilibrium_force_fluxes( m, f, "direct", force_fluxes_standard_mode, tol_eq_force_fluxes, want_local_enrichment );
            
            construct_F_hat( m, f, "direct", balancing, elem_flag_bal, elem_flag_enh, force_fluxes_standard_mode, F_hat, want_local_enrichment );
            construct_dep_hat( m, f, solver, K_hat, F_hat, dep_space_hat[ n ], verif_solver, tol_solver );
            
            /// Calcul d'un estimateur d'erreur globale
            /// ---------------------------------------
            
            theta = 0.;
            calcul_error_estimate_prolongation_condition_PGD( m, f, "direct", "EET", theta, theta_elem, dep_space, dep_param, dep_space_hat, dep_space_part_hat, elem_group, n, want_global_discretization_error, want_local_discretization_error );
            
            theta_2_mode[ n ] = pow( theta, 2 );
        }
        
        if ( n >= max_mode-1 ) { // if ( error_indicator < tol_convergence_criterium_mode or n >= max_mode-1 ) {
            nb_modes = n+1;
            cout << "Convergence de l'algorithme : nb de modes = " << nb_modes << ", residu = " << residual << ", erreur = " << error_indicator << endl << endl;
            break;
        }
        n++;
    }
    
    MatlabPlot mp(display_matlab);
    mp.cd_cwd();
    for (unsigned n=0;n<nb_modes;++n) {
        if ( display_pvd_PGD_space )
            dp_space[ n ].exec( prefix + "_mode" + to_string(n+1) + "_space" );
        else
            dp_space[ n ].make_pvd_file( prefix + "_mode" + to_string(n+1) + "_space" );
        for (unsigned p=0;p<elem_group.size()-1;++p) {
            if ( display_pvd_PGD_param )
                dp_param[ p ][ n ].exec( prefix + "_mode" + to_string(n+1) + "_param" + to_string(p+1) );
            else
                dp_param[ p ][ n ].make_pvd_file( prefix + "_mode" + to_string(n+1) + "_param" + to_string(p+1) );
            string output = "'" + prefix + "_mode" + to_string(n+1) + "_param" + to_string(p+1);
            string xlabel = "'$p_" + to_string(p+1) + "$'";
            string ylabel = "'$\\gamma_{" + to_string(p+1) + "," + to_string(n+1) + "}$'";
//            string params = "notitle w l lt " + to_string(p+1) + " lw 1";
//            save_plot( vals_param[p], dep_param[ p ][ n ], (output + ".tex'").c_str(), xlabel.c_str(), ylabel.c_str(), params.c_str() );
            string params = ",'LineStyle','-','Color',getfacecolor(" + to_string(p+4) + "),'LineWidth',1";
            mp.save_plot( vals_param[p], dep_param[ p ][ n ], (output + ".fig'").c_str(), xlabel.c_str(), ylabel.c_str(), params.c_str() );
            mp.save_plot( vals_param[p], dep_param[ p ][ n ], (output + ".epsc2'").c_str(), xlabel.c_str(), ylabel.c_str(), params.c_str() );
            mp.save_plot( vals_param[p], dep_param[ p ][ n ], (output + ".tex'").c_str(), xlabel.c_str(), ylabel.c_str(), params.c_str() );
        }
    }
    string output = "'" + prefix + "_estimates";
    string xlabel = "'$m$'";
    string ylabel = "'$E^2_{\\mathrm{CRE}}$'";
    string legend = "'$E^2_{\\mathrm{CRE}}$'";
    string params = ",'LineStyle','-','Color',getfacecolor(4),'LineWidth',1";
    mp.figure();
    mp.semilogy( range(1,nb_modes+1), theta_2_mode, params.c_str() );
    mp.grid_on();
    mp.box_on();
    mp.set_fontsize(16);
    mp.set_xlabel_interpreter(xlabel.c_str(),"'latex'");
//    mp.set_ylabel_interpreter(ylabel.c_str(),"'latex'");
    mp.set_legend_interpreter(legend.c_str(),"'latex'");
    mp.save_output((output + ".fig'").c_str());
    mp.save_output((output + ".epsc2'").c_str());
    mp.save_output((output + ".tex'").c_str());
    mp.close();
//    mp.save_semilogy( range(1,nb_modes+1), theta_2_mode, (output + ".fig'").c_str(), xlabel.c_str(), ylabel.c_str(), params.c_str() );
//    mp.save_semilogy( range(1,nb_modes+1), theta_2_mode, (output + ".epsc2'").c_str(), xlabel.c_str(), ylabel.c_str(), params.c_str() );
//    mp.save_semilogy( range(1,nb_modes+1), theta_2_mode, (output + ".tex'").c_str(), xlabel.c_str(), ylabel.c_str(), params.c_str() );
    
    if ( want_verif_PGD )
        check_PGD( m_param, m, f, "direct", structure, loading, mesh_size, elem_group, nb_vals_verif, vals_param, nb_modes, dep_space, dep_param, prefix, display_pvd_PGD_space_verif );
    
    t.stop();
    cout << "temps de calcul de la resolution du pb direct = " << t.res << endl << endl;
    
    /// Calcul de la norme du champ de deplacement approche du pb direct
    /// ----------------------------------------------------------------
//    calcul_norm_dep( m, f, "direct" );
    
    /// ---------------------- ///
    /// Sauvegarde / Affichage ///
    /// ---------------------- ///
    
    if ( display_vtu )
        display( m, prefix );
    else
        save( m, prefix );
    
    t_total.stop();
    cout << "temps de calcul total = " << t_total.res << endl << endl;
    
}
