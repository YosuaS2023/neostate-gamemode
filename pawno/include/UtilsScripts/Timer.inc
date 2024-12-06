#include <PawnPlus>

#define TASK__%0[%1](%2) forward gtask@%0(%2); public gtask@%0(%2){ return SetTimer(#%0,%1,true); } forward %0(%2); public %0(%2)
#define PTASK__%0[%1](%2) forward ptask@%0(%2); public ptask@%0(%2){ return SetTimerEx(#%0,%1,true, "i", %2); } forward %0(%2); public %0(%2)

#define ptask@%0\32; ptask@
#define gtask@%0\32; gtask@

#define ptask%0[%1](%2) PTASK__%0[%1](%2)
#define task%0[%1](%2) TASK__%0[%1](%2)

#if !defined MAX_FUNCTION_NAME
#define MAX_FUNCTION_NAME 31
#endif

#define MAX_PLAYER_TASK 100

new us_player_task[MAX_PLAYERS][MAX_PLAYER_TASK];

stock us_task_free_id(playerid)
{
	for(new i ; i < MAX_PLAYER_TASK; i++)
	{
		if(us_player_task[playerid][i] == 0) return i;
	}
	return -1;
}

stock us_task_refresh(playerid)
{
	new count_kill;
	for(new i ; i  < MAX_PLAYER_TASK; i++)
	{
		if(us_player_task[playerid][i] != 0) KillTimer(us_player_task[playerid][i]), count_kill++;
		us_player_task[playerid][i] = 0;
	}
	printf("*** [UtilsScripts] (timer) kill and refresh as many player timer %i", count_kill);
	return 1;
}

forward _pp@on_init@Timers();
public _pp@on_init@Timers()
{
	new totalPublics = amx_num_publics();
	for (new i = 0; i < totalPublics; i++) 
	{
		new publicName[MAX_FUNCTION_NAME];
		amx_public_name(i, publicName);

		if (strfind(publicName, "gtask@") != 0){
			continue;
		}
		new taskid = CallLocalFunction(publicName, "");
		printf("*** [Playef-Timer] Menjalankan timer %s(task id : %i)", publicName[4], taskid);
	}
	
	printf("*** [UtilsScripts] (timer) successfully loaded timer.inc");
	return 1;
}
public OnPlayerConnect(playerid)
{
	us_task_refresh(playerid);
	
	new totalPublics = amx_num_publics();
	for (new i = 0; i < totalPublics; i++) 
	{
		new publicName[MAX_FUNCTION_NAME];
		amx_public_name(i, publicName);

		if (strfind(publicName, "ptask@") != 0){
			continue;
		}
		new taskid = CallLocalFunction(publicName, "i", playerid),
		         tid = us_task_free_id(playerid)
		;
		us_player_task[playerid][tid] = taskid;
		printf("*** [Playef-Timer] Menjalankan timer %s(task id : %i)", publicName[4], taskid);
	}

	#if defined XTimer_Player
	return XTimer_Player(playerid);
	#else
	return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect XTimer_Player
forward XTimer_Player(playerid);

public OnPlayerDisconnect(playerid, reason)
{
	us_task_refresh(playerid);
	#if defined XTimer_Disconnect
	return XTimer_Disconnect(playerid,reason);
	#else
	return 1;
	#endif
}

#if defined _ALS_OnPlayerDisconnect
#undef OnPlayerDisconnect
#else
#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect XTimer_Disconnect
forward XTimer_Disconnect(playerid,reason);
