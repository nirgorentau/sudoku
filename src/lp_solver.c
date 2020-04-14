#include "lp_solver.h"
#include "deterministic_solver.h"
#include "gurobi_c.h"
#include <stdlib.h>
#include <string.h>

#define NAME_MAX_LEN 128
#define VAR_NAME 0
#define ROW_CONST 1
#define COL_CONST 2
#define BLOCK_CONST 3
#define CELL_CONST 4


Scores_matrix* new_scores_matrix(int N)
{
    int i, j;
    Scores_matrix* scores_matrix = malloc(sizeof(Scores_matrix));
    if(scores_matrix == NULL)
    {
        printf("Memory allocation failed\n");
        exit(-1);
    }
    scores_matrix->matrix = malloc(sizeof(double*) * N);
    if(scores_matrix->matrix == NULL)
    {
        printf("Memory allocation failed\n");
        exit(-1);
    }
    for (i = 0; i < N; i++)
    {
        scores_matrix->matrix[i] = malloc(sizeof(double) * N);
        if(scores_matrix->matrix[i] == NULL)
        {
            printf("Memory allocation failed\n");
            exit(-1);
        }   
        for (j = 0; j < N; j++)
        {
            scores_matrix->matrix[i][j] = 0.0;
        }
    }
  return scores_matrix;
}

void free_scores_matrix(Scores_matrix* scores_matrix)
{
    int i, N;
    if(scores_matrix == NULL) return;
    N = get_scores_matrix_N(scores_matrix);
    for (i = 0; i < N; i++)
    {
        free(scores_matrix->matrix[i]);
    }
    free(scores_matrix->matrix);
    free(scores_matrix);
}

int get_scores_matrix_N(Scores_matrix* scores_matrix)
{
    return scores_matrix->N;
}

double score_at(Scores_matrix* scores_matrix, int i, int j)
{
    return scores_matrix->matrix[i][j];
}

/* The variable that represents the value <val> for cell <x,y> is in cell (N^2*x +N*y + z-1)
 in the variable array for gurobi */
int calc_index(int N, int x, int y, int val) {
    return N*N*x + N*y + val - 1;
}

/* converts n to a string. n must be up to 4 digits */
char* int_to_str(int n) {
    char* ret = (char*) malloc(sizeof(char)*5);
    int i;
    for (i = 1; i <= 4; i++) {
        ret[4-i] = '0' + (n % 10);
        n = n/10;
    }
    ret[4] = '\0';
    return ret;
}

/* Generates a string name accortding to the given type:
   VAR_NAME: a name for a variable
   ROW_CONST: row constraint name
   COL_CONST: column constraint name
   BLOCK_CONST: block constraint name
   uses the given values accordingly:
   i: the y value / block number
   j: the x value
   k: cell's content */
char* format_name(int type, int i, int j, int k) {
    char* ret = (char*) malloc(sizeof(char)*NAME_MAX_LEN);
    ret[0] = '\0';
    if (type == VAR_NAME) {
        strcat(ret, "Cell(");
        strcat(ret, int_to_str(i));
        strcat(ret, ",");
        strcat(ret, int_to_str(j));
        strcat(ret, "):");
        strcat(ret, int_to_str(k));
        strcat(ret, "\0");
    } else if (type == CELL_CONST) {
        strcat(ret, "VCell(");
        strcat(ret, int_to_str(i));
        strcat(ret, ",");
        strcat(ret, int_to_str(j));
        strcat(ret, ")");
        strcat(ret, "\0");
    } else if (type == ROW_CONST) {
        strcat(ret, "Row(");
        strcat(ret, int_to_str(i));
        strcat(ret, "):");
        strcat(ret, int_to_str(j+1));
        strcat(ret, "\0");
    } else if (type == COL_CONST) {
        strcat(ret, "Col(");
        strcat(ret, int_to_str(i));
        strcat(ret, "):");
        strcat(ret, int_to_str(j+1));
        strcat(ret, "\0");
    } else if (type == BLOCK_CONST) {
        strcat(ret, "Blk(");
        strcat(ret, int_to_str(i));
        strcat(ret, "):");
        strcat(ret, int_to_str(j+1));
        strcat(ret, "\0");
    }
    return ret;
}

/*
generates all constraints according to <type> and adds them to <model>.
returns:
0 == success
-1 == error
*/
int set_constraints(int type, GRBenv* env, GRBmodel* model, Board* b, double* in_use, int N) {
    int i, j, k;
    int count;
    int err = 0;
    double not_in_block;
    double* constraints = (double*) malloc(sizeof(double)*N); /* We wouldn't need more than N variables in each constraint */
    int* const_ind = (int*) malloc(sizeof(int)*N);
    if ((type != CELL_CONST) && (type != ROW_CONST) && (type != COL_CONST) && (type != BLOCK_CONST)) {
        printf("Error at set_constraints(): Unidentified type\n");
        free(const_ind);
        free(constraints);
        return -1;
    }
    for (i = 0; i < N; i++) {
        const_ind[i] = 0;
        constraints[i] = 0.0;
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            count = 0;
            not_in_block = 1.0;
            for (k = 0; k < N; k++) {
                if (type == CELL_CONST) {
                    /* i: row, j: column, k: value */
                    constraints[count] = in_use[calc_index(N, i, j, k+1)];
                    const_ind[count] = calc_index(N, i, j, k+1);
                    count++;
                } else if (type == ROW_CONST) {
                    /* i: row, j: value, k: column */
                    if (cell_at(b, i, k)->value == j+1) {
                        not_in_block = 0.0;
                    }
                    constraints[count] = in_use[calc_index(N, i, k, j+1)];
                    const_ind[count] = calc_index(N, i, k, j+1);
                    count++;
                } else if (type == COL_CONST) {
                    /* i: column, j: value, k: row */
                    if (cell_at(b, k, i)->value == j+1) {
                        not_in_block = 0.0;
                    }
                    constraints[count] = in_use[calc_index(N, k, i, j+1)] == 1.0;
                    const_ind[count] = calc_index(N, k, i, j+1);
                    count++;
                } else {
                    /* i: Block number, j: value, k: cell in block */
                    if (cell_at_block(b, i, k)->value == j+1) {
                        not_in_block = 0.0;
                    }
                    constraints[count] = in_use[calc_index(N, cell_at_block(b, i, k)->i, cell_at_block(b, i, k)->j, j+1)];
                    const_ind[count] = calc_index(N, cell_at_block(b, i, k)->i, cell_at_block(b, i, k)->j, j+1);
                    count++;
                }
            }
            if ((type != CELL_CONST) || (cell_at(b, i, j)->value == 0)) {
                /* We don't add a constraint for an already-filled cell */
                err = GRBaddconstr(model, count, const_ind, constraints, GRB_EQUAL, not_in_block, format_name(type, i, j, 0));
            } else {
                err = 0;
            }
            if (err) {
                if (type == CELL_CONST) {
                    printf("Error code %d in GRBaddconstr() for cell (%d, %d): %s\n", err, i, j, GRBgeterrormsg(env));
                } else if (type == ROW_CONST) {
                    printf("Error code %d in GRBaddconstr() for row %d, value %d: %s\n", err, i, j, GRBgeterrormsg(env));
                } else if (type == COL_CONST) {
                    printf("Error code %d in GRBaddconstr() for column %d, value %d: %s\n", err, i, j, GRBgeterrormsg(env));
                } else {
                    printf("Error code %d in GRBaddconstr() for block %d, value %d: %s\n", err, i, j, GRBgeterrormsg(env));
                } 
                free(const_ind);
                free(constraints);
                return -1;
            }
        }
    }
    /* Finished successfully */
    return 0;
}

/* TODO: Code cleanup */
int integer_linear_solve(Board* b, Board* res) {
    GRBenv* env = NULL;
    GRBmodel* model = NULL;
    int err;
    int N = get_N(b);
    char* var_types = (char*) malloc(sizeof(int)*N*N*N);
    double* sol = (double*) malloc(sizeof(double)*N*N*N);
    int solveable;
    double* in_use = (double*) malloc(sizeof(double)*N*N*N);
    int num_in_use = 0;
    int i, j, k;
    Cell* tmp;
    char** var_names = (char**) malloc(sizeof(char*)*N*N*N);
    for (i=0; i < N*N*N; i++) {
        var_types[i] = GRB_BINARY;
        in_use[i] = 0.0;
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 1; k <= N; k++) {
                if(cell_at(b, i, j)->value == 0 && is_valid(b, i, j, k)) {
                    in_use[calc_index(N, i, j, k)] = 1.0;
                    num_in_use++;
                }
                var_names[calc_index(N, i, j, k)] = format_name(VAR_NAME, i, j, k);
            }
        }
    }

    /* Gurobi init */
    err = GRBloadenv(&env, "sol_log.log");
    if (err) {
        printf("Error code %d in GRBloadenv(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    err = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (err) {
        printf("Error code %d in GRBsetintparam(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    err = GRBnewmodel(env, &model, "int_sol", 0, NULL, NULL, NULL, NULL, NULL);
    if (err) {
        printf("Error code %d in GRBnewmodel(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    err = GRBaddvars(model, N*N*N, 0, NULL, NULL, NULL, in_use, NULL, NULL, var_types, var_names);
    if (err) {
        printf("Error code %d in GRBaddvars(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    err = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
    if (err) {
        printf("Error code %d in GRBsetintattr(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    err = GRBupdatemodel(model);
    if (err) {
        printf("Error code %d in GRBupdatemodel(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    /* Single value per cell */
    if (set_constraints(CELL_CONST, env, model, b, in_use, N)) {
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    /* Single value per row */
    if (set_constraints(ROW_CONST, env, model, b, in_use, N) == -1) {
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }
    
    /* Single value per column */
    if (set_constraints(COL_CONST, env, model, b, in_use, N) == -1) {
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    /* Single value per block */
    if (set_constraints(BLOCK_CONST, env, model, b, in_use, N) == -1) {
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    err = GRBoptimize(model);
    if (err) {
        printf("Error code %d in GRBOptimize(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    /* For testing - review that model is correct */
    err = GRBwrite(model, "int_sol.lp");
    if (err) {
        printf("Error code %d in GRBwrite(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    err = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &solveable);
    if (err) {
        printf("Error code %d in GRBgetintattr(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }
    free(in_use);
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
                    cell_at(res, i, j)->value = tmp->value;
                } else {
                    for (k = 1; k <= N; k++) {
                        if (sol[calc_index(N, i, j, k)] == 1) {
                            cell_at(res, i, j)->value = k;
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
    if(integer_linear_solve(b, bin)) {
	    return 0;
    }
    free_board(bin);
    return 1;
}

int linear_solve(Board* board, Scores_matrix** scores_matrices, int N) {
    GRBenv* env = NULL;
    GRBmodel* model = NULL;
    int err;
    char* var_types = (char*) malloc(sizeof(int)*N*N*N);
    double* sol = (double*) malloc(sizeof(double)*N*N*N);
    double* in_use = (double*) malloc(sizeof(double)*N*N*N);
    int num_in_use = 0;
    int i, j, k;
    char** var_names = (char**) malloc(sizeof(char*)*N*N*N);
    double* ub = (double*) malloc(sizeof(double)*N*N*N);
    for (i=0; i < N*N*N; i++) {
        var_types[i] = GRB_CONTINUOUS;
        in_use[i] = 0.0;
        ub[i] = 1.0;
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 1; k <= N; k++) {
                if(cell_at(board, i, j)->value == 0 && is_valid(board, i, j, k)) {
                    in_use[calc_index(N, i, j, k)] = 1.0;
                    num_in_use++;
                }
                var_names[calc_index(N, i, j, k)] = format_name(VAR_NAME, i, j, k);
            }
        }
    }

    /* Gurobi init */
    err = GRBloadenv(&env, "sol_log.log");
    if (err) {
        printf("Error code %d in GRBloadenv(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    err = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (err) {
        printf("Error code %d in GRBsetintparam(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    err = GRBnewmodel(env, &model, "int_sol", 0, NULL, NULL, NULL, NULL, NULL);
    if (err) {
        printf("Error code %d in GRBnewmodel(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    err = GRBaddvars(model, N*N*N, 0, NULL, NULL, NULL, in_use, NULL, ub, var_types, var_names);
    if (err) {
        printf("Error code %d in GRBaddvars(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    err = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
    if (err) {
        printf("Error code %d in GRBsetintattr(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    err = GRBupdatemodel(model);
    if (err) {
        printf("Error code %d in GRBupdatemodel(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    /* Single value per cell */
    if (set_constraints(CELL_CONST, env, model, board, in_use, N)) {
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    /* Single value per row */
    if (set_constraints(ROW_CONST, env, model, board, in_use, N) == -1) {
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }
    
    /* Single value per column */
    if (set_constraints(COL_CONST, env, model, board, in_use, N) == -1) {
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    /* Single value per block */
    if (set_constraints(BLOCK_CONST, env, model, board, in_use, N) == -1) {
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    err = GRBoptimize(model);
    if (err) {
        printf("Error code %d in GRBOptimize(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }

    /* For testing - review that model is correct */
    err = GRBwrite(model, "int_sol.lp");
    if (err) {
        printf("Error code %d in GRBwrite(): %s\n", err, GRBgeterrormsg(env));
        free(in_use);
        free(var_types);
        free(sol);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return -1;
    }
    free(in_use);
    free(var_types);
    err = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N*N*N, sol);
    if (err) {
        printf("Error code %d in GRBgetdblattrarray(): %s\n", err, GRBgeterrormsg(env));
        return -1;
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k=0; k < N; k++) {
                scores_matrices[k]->matrix[i][j] = sol[calc_index(N, i, j, k+1)];
            }
        }
    }
    free(sol);
    GRBfreemodel(model);
    GRBfreeenv(env);
    return 0;
}