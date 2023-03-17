#include "typedefs/search.h"
#include "typedefs/attack.h"
#include "typedefs/board.h"
#include "typedefs/evaluate.h"
#include "typedefs/io.h"
#include "typedefs/misc.h"
#include "typedefs/move.h"
#include "typedefs/movegen.h"
#include "typedefs/pv.h"
#include "typedefs/utils.h"
#include <assert.h>
#include <stdbool.h>

// Called every 4k nodes or so
static void checkUp()
{
    // Check if time up or interrupt from GUI
}

// Mutates the move list by swapping current move with the one with the highest score
// Traverses the list from the given moveNum
// TODO: wouldn't it be better to simply sort the list right after we generate it ?
static void orderNextMove(int moveNum, POTENTIAL_MOVE_LIST *list)
{
    int index = moveNum;
    int bestScore = 0;

    for (int i = moveNum; i < list->count; i++) {
        if (list->moves[i].score > bestScore) {
            index = i;
            bestScore = list->moves[i].score;
        }
    }

    POTENTIAL_MOVE temp = list->moves[moveNum];
    list->moves[moveNum] = list->moves[index];
    list->moves[index] = temp;
}

// Is there a repetition of the current board position in the last moves,
// excluding the captures and pawn movements because those cannot be repeated,
// that's why we start off from i = pos->historyPly - pos->fiftyMove
static bool isRepetition(const BOARD *pos)
{
    for (int i = pos->historyPly - pos->fiftyMove; i < pos->historyPly - 1; i++) {
        if (pos->history[i].posKey == pos->posKey) return true;
    }
    return false;
}

static void clearForSearch(BOARD *pos, SEARCH_INFO *info)
{
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < BRD_SQ_NUM; j++) {
            pos->searchHistory[i][j] = 0;
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < BRD_SQ_NUM; j++) {
            pos->searchKillers[i][j] = 0;
        }
    }

    clearPvTable(pos->pvTable);
    pos->ply = 0;

    info->starttime = getTimeMs();
    info->nodes = 0;
    info->stopped = false;
    info->failHigh = 0;
    info->failHighFirst = 0;
}

// Limits the horizon effect: allows optimizing evaluation of captures,
// by for instance evaluate the postion only when all the captures have been made.
static int quiescence(int alpha, int beta, BOARD *pos, SEARCH_INFO *info)
{
    return 0;
}

// alpha: the lower bound for the maximizer (white)
// beta: the lower bound for the minimizer (black)
// In other words, alpha is the best possible score for the maximizer in the current search
// and beta is the opposite, the best possible score for the minimizer.
// alpha cut-off: when beta < alpha in the current search, meaning that black would be better
// beta cut-off is when the maximizer can out-play the minimizer's beta, i.e. score > beta
static int alphaBeta(int alpha, int beta, int depth, BOARD *pos, SEARCH_INFO *info, bool DoNull)
{
    ASSERT(checkBoard(pos));

    if (depth == 0) {
        info->nodes++;
        return evaluatePosition(pos);
    } /* main base case */

    if (isRepetition(pos) || pos->fiftyMove >= 100) return 0; /* draw case */
    if (pos->ply > MAX_DEPTH) return evaluatePosition(pos);   /* will probably never happen ? */

    POTENTIAL_MOVE_LIST list;
    generateAllMoves(pos, &list);

    int legal = 0; /* used to know if there are any legal moves in the position */
    int prevAlpha = alpha;
    int bestMove = NOMOVE;
    int score = -INFINITY;
    int pvMove = probePvMove(pos);

    // if we have a move here, it means that we are currently in the mainline
    // i.e. the line that is considered the best in previous depths search
    if (pvMove != NOMOVE) {
        for (int i = 0; i < list.count; i++) {
            if (list.moves[i].move == pvMove) {
                list.moves[i].score = 2000000;
                break;
            }
        }
    }

    for (int i = 0; i < list.count; i++) {
        orderNextMove(i, &list);
        int move = list.moves[i].move;
        if (!makeMove(pos, move)) continue;
        legal++;
        // as the side to play changes, alpha and beta are swapped
        score = -alphaBeta(-beta, -alpha, depth - 1, pos, info, true);
        takeMove(pos);
        if (score > alpha) { /* improvement on alpha */
            /* beta cut-off, not acceptable for the minimizer */
            if (score >= beta) {
                if (legal == 1) info->failHighFirst++;
                info->failHigh++;

                // beta cut-off is not a capture
                // for further orderding beta cut-offs that are not captures
                if (!(move & MOVE_CAPT_FLAG)) {
                    pos->searchKillers[1][pos->ply] = pos->searchKillers[0][pos->ply]; /* unshifting */
                    pos->searchKillers[0][pos->ply] = move;
                }

                return beta;
            }
            alpha = score;
            bestMove = move;
            // for further ordering of alpha improvements that are not captures
            if (!(move & MOVE_CAPT_FLAG)) {
                pos->searchHistory[pos->pieces[FROM_SQ(move)]][TO_SQ(move)] += depth;
            }
        }
    }

    if (!legal) {
        if (isSquareAttacked(pos->kingSq[pos->side], pos->side ^ 1, pos)) return -MATE + pos->ply; /* MAT */
        return 0;                                                                                  /* PAT */
    }

    if (prevAlpha != alpha) storePvMove(pos, bestMove);

    return alpha;
}

// The core of the engine
void searchPosition(BOARD *pos, SEARCH_INFO *info)
{
    int bestMove = NOMOVE;
    int bestScore = -INFINITY;
    int pvMoves = 0;

    clearForSearch(pos, info);

    for (int depth = 1; depth <= info->depth; depth++) {
        // check out of time ?
        bestScore = alphaBeta(-INFINITY, INFINITY, depth, pos, info, true);
        pvMoves = updatePvLine(depth, pos);
        bestMove = pos->pvArray[0];

        printf("depth: %d, move: %s, score: %d, nodes: %ld, ", depth, printMove(bestMove, pos), bestScore, info->nodes);
        printf("pv: ");
        for (int i = 0; i < pvMoves; i++) {
            int move = pos->pvArray[i];
            printf("%s ", printMove(move, pos));
        }
        putc('\n', stdout);
        printf("Ordering: %.2f\n", info->failHighFirst / info->failHigh);
    }
}