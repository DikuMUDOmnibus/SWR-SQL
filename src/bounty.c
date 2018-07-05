/***************************************************************************
*                           STAR WARS REALITY 1.0                          *
*--------------------------------------------------------------------------*
* Star Wars Reality Code Additions and changes from the Smaug Code         *
* copyright (c) 1997 by Sean Cooper                                        *
* -------------------------------------------------------------------------*
* Starwars and Starwars Names copyright(c) Lucas Film Ltd.                 *
*--------------------------------------------------------------------------*
* SMAUG 1.0 (C) 1994, 1995, 1996 by Derek Snider                           *
* SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus,                    *
* Scryn, Rennard, Swordbearer, Gorog, Grishnakh and Tricops                *
* ------------------------------------------------------------------------ *
* Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        *
* Chastain, Michael Quan, and Mitchell Tse.                                *
* Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          *
* Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     *
* ------------------------------------------------------------------------ *
* All SWR:SQL additions/modifications added by Rojan                       *
* ------------------------------------------------------------------------ *
*		            Bounty Hunter Module    			   *   
*                    (  and area capturing as well  )                      * 
****************************************************************************/

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
/* #include <stdlib.h> */
#include <time.h>
#include "mud.h"


BOUNTY_DATA *first_bounty;
BOUNTY_DATA *last_bounty;
BOUNTY_DATA *first_disintegration;
BOUNTY_DATA *last_disintegration;


void disintegration args( ( CHAR_DATA * ch, CHAR_DATA * victim, long amount ) );
void nodisintegration args( ( CHAR_DATA * ch, CHAR_DATA * victim, long amount ) );
int xp_compute( CHAR_DATA * ch, CHAR_DATA * victim );

void save_disintegration( BOUNTY_DATA *tbounty )
{
    char selectquery[MAX_STRING_LENGTH*2];
    char updatequery[MAX_STRING_LENGTH*2];
MYSQL_RES   *qresult;
sprintf(selectquery, "SELECT * FROM bounties where id='%d'", tbounty->id);
qresult = query_db(selectquery);

if (!sql_data_returned(qresult))
sprintf(updatequery, "INSERT INTO bounties SET bounty='%s', price='%ld'", tbounty->target, tbounty->amount);
else
sprintf(updatequery, "UPDATE bounties SET price='%ld' where id='%d'", tbounty->amount, tbounty->id);

query_db(updatequery);

}


bool is_disintegration( CHAR_DATA * victim )
{
   BOUNTY_DATA *bounty;

   for( bounty = first_disintegration; bounty; bounty = bounty->next )
      if( !str_cmp( victim->name, bounty->target ) )
         return TRUE;
   return FALSE;
}

BOUNTY_DATA *get_disintegration( char *target )
{
   BOUNTY_DATA *bounty;

   for( bounty = first_disintegration; bounty; bounty = bounty->next )
      if( !str_cmp( target, bounty->target ) )
         return bounty;
   return NULL;
}

void load_bounties(  )
{
    char selectquery[MAX_STRING_LENGTH*2];
       BOUNTY_DATA *bounty;
MYSQL_RES   *qresult;
MYSQL_ROW bountydata;
sprintf(selectquery, "SELECT * FROM bounties");
qresult = query_db(selectquery);
   first_disintegration = NULL;
   last_disintegration = NULL;

   log_string( "Loading disintegrations..." );

   if (!sql_data_returned(qresult))
      return;

while( (bountydata = mysql_fetch_row(qresult)) )
{
          CREATE( bounty, BOUNTY_DATA, 1 );
      LINK( bounty, first_disintegration, last_disintegration, next, prev );
bounty->target = bountydata[2];
bounty->amount = atoi(bountydata[1]);
bounty->id = atoi(bountydata[0]);
}
   log_string( " Done bounties " );
   return;
}

void do_bounties( CHAR_DATA * ch, char *argument )
{
   BOUNTY_DATA *bounty;
   int count = 0;

   set_char_color( AT_WHITE, ch );
   send_to_char( "\n\rBounty                      Amount\n\r", ch );
   for( bounty = first_disintegration; bounty; bounty = bounty->next )
   {
      set_char_color( AT_RED, ch );
      ch_printf( ch, "%-26s %-14ld\n\r", bounty->target, bounty->amount );
      count++;
   }

   if( !count )
   {
      set_char_color( AT_GREY, ch );
      send_to_char( "There are no bounties set at this time.\n\r", ch );
      return;
   }
}

void disintegration( CHAR_DATA * ch, CHAR_DATA * victim, long amount )
{
   BOUNTY_DATA *bounty;
   bool found;
   char buf[MAX_STRING_LENGTH];
   
   found = FALSE;

   for( bounty = first_disintegration; bounty; bounty = bounty->next )
   {
      if( !str_cmp( bounty->target, victim->name ) )
      {
         found = TRUE;
         break;
      }
   }

   if( !found )
   {
      CREATE( bounty, BOUNTY_DATA, 1 );
      LINK( bounty, first_disintegration, last_disintegration, next, prev );

      bounty->target = STRALLOC( victim->name );
      bounty->amount = 0;
   }

   bounty->amount = bounty->amount + amount;
   save_disintegration( bounty );

   sprintf( buf, "%s has added %ld credits to the bounty on %s.", ch->name, amount, victim->name );
   echo_to_all( AT_RED, buf, 0 );

}

void do_addbounty( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_STRING_LENGTH];
   long int amount;
   CHAR_DATA *victim;

   if( !argument || argument[0] == '\0' )
   {
      do_bounties( ch, argument );
      return;
   }

   argument = one_argument( argument, arg );

   if( argument[0] == '\0' )
   {
      send_to_char( "Usage: Addbounty <target> <amount>\n\r", ch );
      return;
   }

   if( ch->pcdata && ch->pcdata->clan && !str_cmp( ch->pcdata->clan->name, "the hunters guild" ) )
   {
      send_to_char( "Your job is to collect bounties not post them.", ch );
      return;
   }

   if( !ch->in_room || ch->in_room->vnum != 6604 )
   {
      send_to_char( "You will have to go to the Hunters Guild on Tatooine to add a new bounty.", ch );
      return;
   }

   if( argument[0] == '\0' )
      amount = 0;
   else
      amount = atoi( argument );

   if( amount < 5000 )
   {
      send_to_char( "A bounty should be at least 5000 credits.\n\r", ch );
      return;
   }

   if( !( victim = get_char_world( ch, arg ) ) )
   {
      send_to_char( "They don't appear to be here .. wait til they log in.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "You can only set bounties on other players .. not mobs!\n\r", ch );
      return;
   }

   if( amount <= 0 )
   {
      send_to_char( "Nice try! How about 1 or more credits instead...\n\r", ch );
      return;
   }

   if( ch->gold < amount )
   {
      send_to_char( "You don't have that many credits!\n\r", ch );
      return;
   }

   ch->gold = ch->gold - amount;

   disintegration( ch, victim, amount );
}

void remove_disintegration( BOUNTY_DATA * bounty )
{
    char deletequery[MAX_STRING_LENGTH*2];
       if( !bounty )
   {
      bug( "bounty remove: null bounty", 0 );
      return;
   }
sprintf(deletequery, "DELETE FROM bounties where id='%d'", bounty->id);
query_db(deletequery);

   UNLINK( bounty, first_disintegration, last_disintegration, next, prev );
   STRFREE( bounty->target );
   DISPOSE( bounty );
}

void claim_disintigration( CHAR_DATA * ch, CHAR_DATA * victim )
{
   BOUNTY_DATA *bounty;
   long int gexp;
   char buf[MAX_STRING_LENGTH];

   if( IS_NPC( victim ) )
      return;

   bounty = get_disintegration( victim->name );

   if( ch == victim )
   {
      if( bounty != NULL )
         remove_disintegration( bounty );
      return;
   }

   if( bounty && ( !ch->pcdata || !ch->pcdata->clan || str_cmp( ch->pcdata->clan->name, "the hunters guild" ) ) )
   {
      remove_disintegration( bounty );
      bounty = NULL;
   }

   if( bounty == NULL )
   {
      if( IS_SET( victim->act, PLR_KILLER ) && !IS_NPC( ch ) )
      {
         gexp =
            URANGE( 1, xp_compute( ch, victim ),
                    ( exp_level( ch->skill_level[HUNTING_ABILITY] + 1 ) - exp_level( ch->skill_level[HUNTING_ABILITY] ) ) );
         gain_exp( ch, gexp, HUNTING_ABILITY );
         set_char_color( AT_BLOOD, ch );
         ch_printf( ch, "You receive %ld hunting experience for executing a wanted killer.\n\r", gexp );
      }
      else if( !IS_NPC( ch ) )
      {
         SET_BIT( ch->act, PLR_KILLER );
         ch_printf( ch, "You are now wanted for the murder of %s.\n\r", victim->name );
      }
      sprintf( buf, "%s is Dead!", victim->name );
      echo_to_all( AT_RED, buf, 0 );
      return;

   }

   ch->gold += bounty->amount;

   gexp =
      URANGE( 1, bounty->amount + xp_compute( ch, victim ),
              ( exp_level( ch->skill_level[HUNTING_ABILITY] + 1 ) - exp_level( ch->skill_level[HUNTING_ABILITY] ) ) );
   gain_exp( ch, gexp, HUNTING_ABILITY );

   set_char_color( AT_BLOOD, ch );
   ch_printf( ch, "You receive %ld experience and %ld credits,\n\r from the bounty on %s\n\r", gexp, bounty->amount,
              bounty->target );

   sprintf( buf, "%s has claimed the disintegration bounty on %s!", ch->name, victim->name );
   echo_to_all( AT_RED, buf, 0 );
   sprintf( buf, "%s is Dead!", victim->name );
   echo_to_all( AT_RED, buf, 0 );

   if( !IS_SET( victim->act, PLR_KILLER ) )
      SET_BIT( ch->act, PLR_KILLER );
   remove_disintegration( bounty );
}
