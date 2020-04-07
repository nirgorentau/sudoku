#include <stdio.h>
#include <lp_solver.h>
#include <board.h>
#include <display_board.h>
#include <game.h>

int main() {
    LinkedList* lst = NULL;
    Board* board = NULL;
    solve(&board, "load_file.txt", &lst);
    display_board(board);
    printf("%d\n", lst->curr->move_count);
    if(set(board, 9, 9, 4, lst))
    {
        puts("set error");
    }
    set(board, 9, 9, 5, lst);
    printf("%d\n", lst->curr->move_count);
    display_board(board);
    if(undo(board, lst) == 0)
    {
        puts("undo succesful");
    }
    display_board(board);
    redo(board, lst);
    display_board(board);
    undo(board, lst);
    display_board(board);
    undo(board, lst);
    display_board(board);
    redo(board, lst);
    display_board(board);
    redo(board, lst);
    display_board(board);
    reset(board, lst);
    display_board(board);
    redo(board, lst);
    display_board(board);
    validate(board);
    hint(board, 1, 1);
    autofill(board, lst);
    display_board(board);
    save(board, "save_file.txt");
    edit(&board, "save_file.txt", &lst);
    display_board(board);
    generate(board, 5, 10, lst);
    display_board(board);
    exit_program(board, lst);
    return 0;
}