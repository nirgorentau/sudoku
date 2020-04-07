#include "lp_solver.h"
#include "deterministic_solver.h"
#include "gurobi_c.h"
#include <stdlib.h>

/* The variable that represents the value <val> for cell <x,y> is in cell (N^2*x +N*y + z-1)
 in the variable array for gurobi */
int calc_index(int N, int x, int y, int val) {
    return N*N*x + N*y + val - 1;
}

/* TODO: Code cleanup */
int integer_linear_solve(Board* b, Board** res) {
    GRBenv* env = NULL;
    GRBmodel* model = NULL;
    int err;
    int N = get_N(b);
    char* var_types = (char*) malloc(sizeof(int)*N*N*N);
    double* constraints = (double*) malloc(sizeof(double)*N); /* We wouldn't need more than N variables in each constraint */
    int* const_ind = (int*) malloc(sizeof(int)*N);
    double* sol = (double*) malloc(sizeof(double)*N*N*N);
    int solveable;
    double* in_use = (double*) malloc(sizeof(double)*N*N*N);
    int* in_use_ind;
    int num_in_use = 0;
    int i, j, k, c;
    int not_in_block; /* Or row, or column */
    Cell* tmp;
    for (i=0; i < N*N*N; i++) {
	var_types[i] = GRB_BINARY;
	in_use[i] = 0.0;
    }
    for (i = 0; i < N; i++) {
	const_ind[i] = 0;
	constraints[i] = 0.0;
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 1; k <= N; k++) {
                if(cell_at(b, i, j)->value == 0 && is_valid(b, i, j, k)) {
                    in_use[calc_index(N, i, j, k)] = 1.0;
                    num_in_use++;
                }
            }
        }
    }
    in_use_ind = (int*) malloc(num_in_use*sizeof(int));
    if (in_use_ind == NULL) {
        printf("Error allocating in_use index array\n");
        return -1;
    }
    c = 0;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 1; k <= N; k++) {
                if(in_use[calc_index(N, i, j, k)] == 1.0) {
                    in_use_ind[c] = calc_index(N, i, j, k);
                    c++;
                }
            }
        }
    }

    /* Gurobi init */
    err = GRBloadenv(&env, "sol_log.log");
    if (err) {
        printf("Error code %d in GRBloadenv(): %s\n", err, GRBgeterrormsg(env));
        return -1;
    }

    err = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (err) {
        printf("Error code %d in GRBsetintparam(): %s\n", err, GRBgeterrormsg(env));
        return -1;
    }

    err = GRBnewmodel(env, &model, "int_sol", 0, NULL, NULL, NULL, NULL, NULL);
    if (err) {
        printf("Error code %d in GRBnewmodel(): %s\n", err, GRBgeterrormsg(env));
        return -1;
    }


    err = GRBaddvars(model, N*N*N, 0, NULL, NULL, NULL, in_use, NULL, NULL, var_types, NULL);
    if (err) {
        printf("Error code %d in GRBaddvars(): %s\n", err, GRBgeterrormsg(env));
        return -1;
    }

    err = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
    if (err) {
        printf("Error code %d in GRBsetintattr(): %s\n", err, GRBgeterrormsg(env));
        return -1;
    }

    err = GRBupdatemodel(model);
    if (err) {
        printf("Error code %d in GRBupdatemodel(): %s\n", err, GRBgeterrormsg(env));
        return -1;
    }


    /* Single value per cell */
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            c = 0;
            for (k = 1; k <=N; k++) {
                if (in_use[calc_index(N, i, j, k)] == 1.0) {
                    constraints[c] = 1.0;
                    const_ind[c] = calc_index(N, i, j, k);
                    c++;
                }
            }
            /* TODO: Autofill at start */
            if (c > 0) {
                err = GRBaddconstr(model, c, const_ind, constraints, GRB_EQUAL, 1.0, NULL);
                if (err) {
                    printf("Error code %d in GRBaddconstr() for cell (%d, %d): %s\n", err, i, j, GRBgeterrormsg(env));
                    return -1;
                }
                /* TODO: give more meaningful constraint names */
            }
        }
    }

    /* Single value per row */
    for (i = 0; i < N; i++) {
        for (k = 1; k <= N; k++) {
            c = 0;
            not_in_block = 1;
            for (j = 0; j < N && (not_in_block); j++) {
                if (cell_at(b, i, j)->value == k) {
                    not_in_block = 0;
                }
            }
            if (not_in_block) {
                for (j = 0; j < N; j++) {
                    if(in_use[calc_index(N, i, j, k)] == 1.0) {
                        constraints[c] = 1.0;
                        const_ind[c] = calc_index(N, i, j, k);
                        c++;
                    }
                }
                err = GRBaddconstr(model, c, const_ind, constraints, GRB_EQUAL, 1.0, NULL);
                if (err) {
                    printf("Error code %d in GRBaddconstr() for row %d, value %d: %s\n", err, i, k, GRBgeterrormsg(env));
                    return -1;
                }
            }
        }
    }
    
    /* Single value per column */
    for (j = 0; j < N; j++) {
        for (k = 1; k <= N; k++) {
            c = 0;
            not_in_block = 1;
            for (i = 0; i < N && (not_in_block); i++) {
                if (cell_at(b, i, j)->value == k) {
                    not_in_block = 0;
                }
            }
            if (not_in_block) {
                for (i = 0; i < N; i++) {
                    if(in_use[calc_index(N, i, j, k)] == 1.0) {
                        constraints[c] = 1.0;
                        const_ind[c] = calc_index(N, i, j, k);
                        c++;
                    }
                }
                err = GRBaddconstr(model, c, const_ind, constraints, GRB_EQUAL, 1.0, NULL);
                if (err) {
                    printf("Error code %d in GRBaddconstr() for column %d, value %d: %s\n", err, j, k, GRBgeterrormsg(env));
                    return -1;
                }
            }
        }
    }

    /* Single value per block */
    for (i = 0; i < N; i++) {
        for (k = 1; k <= N; k++) {
            c = 0;
            not_in_block = 1;
            for (j = 0; j < N && (not_in_block); j++) {
                if (cell_at_block(b, i, j)->value == k) {
                    not_in_block = 0;
                }
            }
            if (not_in_block) {
                for (j = 0; j < N; j++) {
                    if(in_use[calc_index(N, (j / b->n) + b->m*(i/b->m), (j % b->n) + b->n*(i % b->m), k)] == 1.0) {
                        constraints[c] = 1.0;
                        const_ind[c] = calc_index(N, (j / b->n) + b->m*(i/b->m), (j % b->n) + b->n*(i % b->m), k);
                        c++;
                    }
                }
                err = GRBaddconstr(model, c, const_ind, constraints, GRB_EQUAL, 1.0, NULL);
                if (err) {
                    printf("Error code %d in GRBaddconstr() for block %d, value %d: %s\n", err, i, k, GRBgeterrormsg(env));
                    return -1;
                }
            }
        }
    }
    err = GRBoptimize(model);
    if (err) {
        printf("Error code %d in GRBOptimize(): %s\n", err, GRBgeterrormsg(env));
        return -1;
    }

    /* For testing - review that model is correct */
    err = GRBwrite(model, "int_sol.lp");
    if (err) {
        printf("Error code %d in GRBwrite(): %s\n", err, GRBgeterrormsg(env));
        return -1;
    }

    err = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &solveable);
    if (err) {
        printf("Error code %d in GRBgetintattr(): %s\n", err, GRBgeterrormsg(env));
        return -1;
    }
    free(in_use);
    free(const_ind);
    free(constraints);
    free(var_types);
    if (solveable == GRB_OPTIMAL) {
        err = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N*N*N, sol);
        if (err) {
            printf("Error code %d in GRBgetdblattrarray(): %s\n", err, GRBgeterrormsg(env));
            return -1;
        }
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                tmp = cell_at(b, i, j);
                if (tmp->value != 0) {
                    cell_at(*res, i, j)->value = tmp->value;
                } else {
                    for (k = 1; k <= N; k++) {
                        if (sol[calc_index(N, i, j, k)] == 1) {
                            cell_at(*res, i, j)->value = k;
                        }
                    }
                }
            }
        }
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return 0;
    } else {
        /* Unsolveable */
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return 1;
    }
}

int is_solvable(Board* b) {
    Board* bin = new_board(b->m, b->n);
    if(integer_linear_solve(b, &bin)) {
	    return 0;
    }
    return 1;
}