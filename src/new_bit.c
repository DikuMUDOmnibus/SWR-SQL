// VERS 1.1
///* new.bit.c -- infinite bitmask system                      *///
// This is Runter's system for infinite bits. It was made for use
// in Feltain (feltain.org 7777). I, Runter, require no credit, no
// help file documentation of your use, and no email confirmation.
// The only thing I do ask is if you find a bug or have a major change
// that makes the system more efficient or more intelligent than 
// let me know about it. Just remember what goes around comes around.
// We all benifit when we work together.
//
// To successfully use this you will use the variable BITMASK
// "BITMASK act;"
// instead of int in your struct declarations for your bitmasks.
// On this new variable type just pass the variables address to
// the new functions-- set_bit, remove_bit, is_set.
// example: if(is_set(&ch->act, ACT_NPC))
//
// This system passes normal numbers as the bits so you'll need
// to redeclare stuff as you are passing it. 
//
// if(is_set(&ch->act, 400)) // is the 400th bit set?
//
// A good way to change your entire system over would be to redo
// your definitions of A B C D E... aa bb cc and such from defines
// to one enum struct.
// 
// Another important note is that you need to use the new loading
// and saving functions provided when saving your bitmasks. It
// saves them to a single line and loads them from a single line.
//
// Technical note: This system will only use memory required for the
// size of your bitmask. It allocates memory as needed only. This does
// use malloc() and free(). Some servers have memory management systems
// in with their own functions. Feel free to change it how you see fit.
// If you need any more help installing the system you can find me at
// feltain.org 7777 or aim me at jeffreybasurto.

// VERS 2
// In version 2 it still maintains the above functionality with fixes
// but also lets you serialize your bitmask to an array of values
// as well as free and initilize it easily. 
// 
// I use this system as a high level solution to linked lists and have
// used it for my char_list, obj_list, and other places. It is 
// efficient enough to do this and actually saves you managing linked 
// lists and next pointers of ANY kind. It can be adapted for room lists
// for objects and players or global lists. This is an abstract data
// type and you can NOT use this as a queue. Ie, the order you set it
// in will not be the order it comes out serialized. It will sort the
// serialization in no particular way like a linked list does. 
//
// This system also allows you to use qsort out of the box after calling
// serialize_bitmask(). 
//
// After getting your array after calling serialize_bitmask() you will have
// to loop through it to get what is in the list and it will be NUL
// terminated like a string. It can be of any size so don't assume, please.
//
// Report any bugs or additions you have made please!! Any praise is 
// appreciated as well. ^_- I spent a lot of intellectual time trying
// to think of a good implementation of this.
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mud.h"
/* This struct stuff goes in your global.h -- usually
 * merc.h or mud.h
 *************************************************

 typedef struct bmlist BMlist;
 struct bmlist {
     BMlist *next;
     long set;
     long tar_mask;
 };

 typedef struct bitmask {
     long bits; // number of bits set in total.
     long masks; // number of masks in all.
     BMlist *int_list;
 } BITMASK;

 *************************************************/
// returns false if no removal. True if so.
int REMOVE_BIT(BITMASK *mask, short bit) {
    BMlist *pBlist, *last = 0;


    if (!is_set(mask, bit)) // nothing doing. It isn't set.
        return FALSE;
    --bit;
    // loop through each bitmask we have allocated
    for(pBlist = mask->int_list;pBlist;pBlist = pBlist->next) {
        if (pBlist->set == bit / 32) {// Is this the correct set?
            pBlist->tar_mask &= ~(1 << (bit % 32)); // remove it.
            --mask->bits;
            if (pBlist->tar_mask == 0) {
                if (last)
                    last->next = pBlist->next;
                else
                    mask->int_list = pBlist->next;
                free(pBlist);
                --mask->masks;
            }
            return TRUE;
        }
        last = pBlist;
    }

    return FALSE; // a bug happened. Wee. Somehow.
}

// returns true if a bit was set. Returns 2 if had to allocate. 
// False if already set.
int SET_BIT(BITMASK *mask, short bit) {
    BMlist *pBlist;

    --bit;

    // loop through each bitmask we have already allocated.
    for(pBlist = mask->int_list;pBlist;pBlist = pBlist->next) {
        if (pBlist->set == bit / 32) { // Is this the correct set?
            if (pBlist->tar_mask & (1 << (bit % 32)))
                return FALSE; // Already set.

            pBlist->tar_mask |= 1 << (bit % 32); // Set our bit inside the mask.
            ++mask->bits;
            return 1; // return true with no allocation
        }
    }

    // Not found but not set. We have to allocate and add to the list.
    pBlist = malloc(sizeof(BMlist));
    ++mask->masks;
    pBlist->next = mask->int_list;
    mask->int_list = pBlist;
    pBlist->tar_mask = 0;
    pBlist->set = bit / 32;
    pBlist->tar_mask |= 1 << (bit % 32); // set our bit on new mask
    ++mask->bits;
    return 2; // return true with allocation
}

bool IS_SET(BITMASK *mask, short bit) {
    BMlist *pBlist;

    --bit;

    for(pBlist = mask->int_list;pBlist;pBlist = pBlist->next)
        if (pBlist->set == bit / 32) {
            if (pBlist->tar_mask & 1 << (bit % 32))
                return TRUE;
            else
                return FALSE;
        }

    return FALSE;
}

// Let's serialize this and return all bits that are set in a dynamic list.
// This is useful when you just want to know what to traverse in a bitmask.
// Good for high level solutions to lists of pointers! This is a high level
// solution that *does not* maintain an ordered queue for specific BFS or
// DFS algorithms. This is for algorithms that do not matter.
int *serialize_bitmask(BITMASK *mask) {
    BMlist *pBlist;
    int *ilist = malloc(sizeof(int) * mask->bits + 1), i = 0, z;
    ilist[mask->bits] = 0; // zero terminates it. 0th bit CAN NOT BE SET.

    for(pBlist = mask->int_list;pBlist;pBlist = pBlist->next) {
        for(z = 0;z < 32;++z) {
            if (i > mask->bits) {
                // more found than we have allocated. Maybe residual.
                // May add a check before setting ilist to make sure it
                // doesn't over run but this should be fine. Just don't
                // bug it here for sure. It may not be an error at this
                // point. If you do decide to error check this better
                // this break should be replaced to let it go through.
                break;
            }

            if (pBlist->tar_mask & 1 << z)
                ilist[i++] = pBlist->set * 32 + z + 1;
        }
    }
    if (i < mask->bits + 1) {
        // error. We have less recorded bits than we allocated.
    }
    // This must be freed somewhere with free(ilist) or it will leak.
    // Using a static buffer here would be a poor idea. Using temporary
    // memory allocation would be a good one. Too bad there isn't an
    // ANSI thing for that in C.
    return ilist;

}

// Frees your bitmask. Safe to call dry. Returns 2 if
// it frees anything. 1 if it doesn't.
void free_bitmask(BITMASK *pBmask) {
    BMlist *pBMlist, *next;
    int found = 1;
    for(pBMlist = pBmask->int_list;pBMlist;pBMlist = next) {
        next = pBMlist->next;
        free_mem(pBMlist);
        found = 2;
    }
    return found;
}


// initialze a bitmask.
// bm = init_bitmask(NULL);
// init_bitmask(&bm);
// Both of above work.
BITMASK init_bitmask(BITMASK *bm) {
    static BITMASK bmzero;
    if (bm == 0)
        return bmzero;

    *bm = bmzero;
    return bmzero;
}

// #masks #bits #mask #vector #mask #vector ...
void load_bitmask(BITMASK *pBmask, FILE *fp) {
    int i;
    BMlist *pBMlist;

    pBmask->masks = fread_number(fp);
    pBmask->bits = fread_number(fp);

    for(i = 0;i < pBmask->masks;++i) {
        pBMlist = malloc(sizeof(BMlist));

        pBMlist->set = fread_number(fp);
        pBMlist->next = pBmask->int_list;
        pBmask->int_list = pBMlist;
        pBMlist->tar_mask = fread_number(fp);
    }
}

void save_bitmask(BITMASK *pBmask, FILE *fp) {
    BMlist *pBMlist;


    fprintf(fp, "%d %d", pBmask->masks, pBmask->bits);

    for(pBMlist = pBmask->int_list;pBMlist;pBMlist = pBMlist->next)
        fprintf(fp, " %d %ld", pBMlist->set, pBMlist->tar_mask);

    fprintf(fp, "\n");
}



