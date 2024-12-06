#include <UtilsScripts/Als>
#define INVALID_GROUP_ID            (0)
#define GROUP_ERROR                 (MAX_GROUPS+1)
#define MAX_GROUPS                  100
#define MAX_TEAM_NAME               32

enum static e_TeamData
{
	bool:teamExists,
	teamName[MAX_TEAM_NAME],
	teamLeader[MAX_PLAYER_NAME],
	teamMember,
    teamRequestID
}
static Cx_teamData[MAX_GROUPS][e_TeamData];
static Cx_PlayerGroupRequest[MAX_PLAYERS];

stock Group_GetLeader(i) return Cx_teamData[i][teamLeader];
stock Group_GetCountMember(i)
{
    static count;
    foreach(new playerid : Player) if(GetPVarInt(playerid, "cx_groupid") == i)
    {
        count++;
    }
    return count;
}
stock GetPlayerGroup(playerid) return GetPVarInt(playerid, "cx_groupid");
stock Group_GetPlayerRequest(playerid) return Cx_PlayerGroupRequest[playerid];

stock Group_Create(playerid, teamname[])
{
    for(new i = 1; i != MAX_GROUPS; i++) if(!strcmp(Cx_teamData[i][teamName], teamname, false))
    {
        return GROUP_ERROR;
    }
    for(new i = 1; i != MAX_GROUPS; i++) if(!Cx_teamData[i][teamExists])
    {
    	format(Cx_teamData[i][teamName], MAX_TEAM_NAME, "%s", teamname);
    	Cx_teamData[i][teamMember] = 1;
        Cx_teamData[i][teamLeader] = playerid;
    	Cx_teamData[i][teamExists] = true;
    	// stats player
    	SetPVarInt(playerid, "cx_groupid", i);
        Cx_PlayerGroupRequest[playerid] = INVALID_PLAYER_ID;
        if(funcidx("OnGroupCreated") != 1)
        {
            CallLocalFunction("OnGroupCreated", "isi", i, teamname, playerid);
        }
    	return i;
	}
	return 0;
}

stock Group_Delete(groupid)
{
    if(Group_Exists(groupid) == -1)
        return 0;

    format(Cx_teamData[groupid][teamName], MAX_TEAM_NAME, "group_none");
    Cx_teamData[groupid][teamLeader] = INVALID_PLAYER_ID;
    Cx_teamData[groupid][teamMember] = 0;
    Cx_teamData[groupid][teamExists] = false;
    // stats player
    foreach(new playerid : Player) if(GetPlayerGroup(playerid) == groupid)
    {
        SetPVarInt(playerid, "cx_groupid", 0);
        Cx_PlayerGroupRequest[playerid] = INVALID_PLAYER_ID;
    }
    if(funcidx("OnGroupDeleted") != 1)
    {
        CallLocalFunction("OnGroupDeleted", "i", groupid);
    }
	return 1;
}

stock Group_Exists(teamid){
    if(teamid < 1 && teamid > MAX_GROUPS) 
        return printf("[CoreX/CX_Group]: Invalid Group ID, Maximum Group ID %i\ncxfunc Group_Exists(%i)", MAX_GROUPS, teamid);
    
    if(Cx_teamData[teamid][teamExists] == true)
    {
        
        return teamid;
    }
    else if(teamid == 0 && Cx_teamData[teamid][teamExists] == false)
    {
        return 0;
    }
    else printf("[CoreX/CX_Group]: Group ID %i does not exist on the server\ncxfunc Group_Exists(%i)", teamid, teamid);
    return -1;
}

stock Group_SetLeader(groupid, leaderid)
{
    if(Group_Exists(groupid) == -1) return 0;
    Cx_teamData[groupid][teamLeader] = leaderid;
    return 1;
}

stock Group_SetPlayerRequest(playerid, requestid){
    Cx_PlayerGroupRequest[playerid] = requestid;
    return 1;
}

stock Group_ResetPlayerRequest(playerid)
{
    Cx_PlayerGroupRequest[playerid] = INVALID_PLAYER_ID;
    return 1;
}

stock Group_GetName(i, name[], length = sizeof(name))
{
    format(name, length, "%s", Cx_teamData[i][teamName]);
    return 1;
}

stock SetPlayerGroup(playerid, teamid)
{
    if(teamid < 0 && teamid > MAX_GROUPS) return printf("[CoreX/CX_Group]: Invalid Group ID, Maximum Group ID %i\ncxfunc SetPlayerGroup(%i,%i)", MAX_GROUPS, playerid, teamid);
    if(Group_Exists(teamid) == -1) return printf("[CoreX/CX_Group]: Group ID %i does not exist on the server\ncxfunc SetPlayerGroup(%i,%i)", teamid, playerid, teamid);
    SetPVarInt(playerid, "cx_groupid", teamid);
    return 1;
}

FindTeamByName(const name[])
{
    for (new i = 1; i < MAX_GROUPS; i++)
    {
        if (strfind(Cx_teamData[i][teamName], name, false) == 0)
        {
            return i;
        }
    }
    return -1;
}