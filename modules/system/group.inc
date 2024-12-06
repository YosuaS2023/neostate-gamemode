//new Float:groupVehicleSpawn[MAX_GROUP_TYPE][4];

forward OnGroupCreated(teamid, teamname[], leaderid);
forward OnGroupDeleted(teamid, teamname[]);

public OnGroupCreated(teamid, teamname[], leaderid)
{
    printf("Group %s[ID: %i] created successfully with leader: %i playerid", teamname, teamid, leaderid);
    return 1;
}

public OnGroupDeleted(teamid, teamname[])
{
    printf("[Group/CoreX]: Deleting group id %i", teamid);
    return 1;
}

SendGroupMessage(groupid, color, const str[], {Float,_}:...)
{
    static
        args,
        start,
        end,
        string[144]
    ;
    #emit LOAD.S.pri 8
    #emit STOR.pri args

    if(args > 8)
    {
        #emit ADDR.pri str
        #emit STOR.pri start

        for (end = start + (args - 8); end > start; end -= 4)
        {
            #emit LREF.pri end
            #emit PUSH.pri
        }
        #emit PUSH.S str
        #emit PUSH.C 144
        #emit PUSH.C string

        #emit LOAD.S.pri 8
        #emit ADD.C 4
        #emit PUSH.pri

        #emit SYSREQ.C format
        #emit LCTRL 5
        #emit SCTRL 4

        foreach (new i : Player)
        {
            if(GetPlayerGroup(i) == groupid) {
				SendClientMessageEx(i, color, "[GROUP]: %s", string);
            }
        }
        return 1;
    }
    foreach (new i : Player)
    {
        if(GetPlayerGroup(i) == groupid) {
			SendClientMessageEx(i, color, "[GROUP]: %s", string);
        }
    }
    return 1;
}

CMD:creategroup(playerid)
{
	Dialog_Show(playerid, teamcreate, DIALOG_STYLE_LIST, "Create Group", ""YELLOW"Create a group"WHITE"\nSearch for groups (by group name)\n"RED"Quit Group"WHITE"\nKick Member", "Click", "Cancel");
	return true;
}

Dialog:teamcreate(playerid, response, listitem, inputtext[])
{
    if(response)
    {
        if(!strcmp(inputtext, "Create a group", false))
        {
            if(GetPlayerGroup(playerid) != 0) return SendErrorMessage(playerid, "You've signed in with another group.");
            Dialog_Show(playerid, creategroup, DIALOG_STYLE_INPUT, "Create a group","Please enter the group name you want to make.\nExample: CoreX, EAGLE", "Accept", "Cancel");
        }
        else if(!strcmp(inputtext, "Search for groups (by group name)", false))
        {
            if(GetPlayerGroup(playerid) != 0) return SendErrorMessage(playerid, "You've signed in with another group.");
            Dialog_Show(playerid, findgroup, DIALOG_STYLE_INPUT, "Search for groups", "Enter the name of the group you want to join", "Accept", "Cancel");
        }
        else if(!strcmp(inputtext, "Quit Group", false))
        {
            if(GetPlayerGroup(playerid) == 0) return SendErrorMessage(playerid, "You've signed in with another group."); 
            Dialog_Show(playerid, quitgroup, DIALOG_STYLE_MSGBOX, "Quit Group ", "Apakah anda yakin keluar dari group?", "Accept", "Cancel");
        }
        else if(!strcmp(inputtext, "Kick Member", false))
        {
            if(GetPlayerGroup(playerid) == 0) return SendErrorMessage(playerid, "You are not in the group.");
            if(Group_GetLeader(GetPlayerGroup(playerid)) != playerid) return SendErrorMessage(playerid, "You can't kick out members because you're not the group leader");
            new mstring[300];
            format(mstring, 300, "");
            foreach(new memberid : Player)
            {
                if(GetPlayerGroup(playerid) == GetPlayerGroup(memberid) && memberid != playerid)
                {
                    format(mstring, sizeof(mstring), "%s%s\n", mstring, GetName(memberid));
                }
            }
            Dialog_Show(playerid, kickmember_group, DIALOG_STYLE_LIST, "Kick Member", mstring, "Accept", "Cancel");
        }
    }
    return 1;
}
Dialog:creategroup(playerid, response, listitem, inputtext[])
{
	if(!response) 
        return SendErrorMessage(playerid, "You canceled the creation of the group.");

	new ret = Group_Create(playerid, inputtext);
	if(!ret)  return SendCustomMessage(playerid, "GROUP", "You cannot create a team because it is full.");
    else if(ret == GROUP_ERROR) return SendErrorMessage(playerid, "Group name" YELLOW"%s"WHITE" already available!");
    else SendCustomMessage(playerid, "GROUP", "You have successfully created a group, "YELLOW"%s [Group ID: %i]", inputtext, ret);
	return 1;
}

Dialog:quitgroup(playerid, response, listitem, inputtext[])
{
    if(response)
    {
        static groupid;
        groupid = GetPlayerGroup(playerid);
        if (Group_GetLeader(groupid) == playerid)
        {
            if(Group_GetCountMember(groupid) <= 1)
            {
                Group_Delete(groupid);
                Group_ResetData(groupid);
                SetPlayerGroup(playerid, INVALID_GROUP_ID);
                SendServerMessage(playerid, "You have left the group, and the group has been deleted.");
                return 1;
            }
            foreach(new memberid : Player) if (groupid == GetPlayerGroup(memberid) && memberid != playerid)
            {
                Group_SetLeader(groupid, memberid);
                SetPlayerGroup(playerid, INVALID_GROUP_ID);
                SendServerMessage(playerid, "You have left the group");
                SendGroupMessage(groupid, COLOR_YELLOW, "The group leader is now %s(id:%i)", GetName(memberid), memberid);
                SendGroupMessage(groupid, COLOR_LIGHTRED, "Member %s(id:%i) has left the group.", GetName(playerid), playerid);
                return 1;
            }
        }
        else if(Group_GetLeader(groupid) != playerid)
        {
            SetPlayerGroup(playerid, INVALID_GROUP_ID);
            SendGroupMessage(groupid, COLOR_LIGHTRED, "Member %s(id:%i) has left the group.", GetName(playerid), playerid);
            return 1;
        }
    }
    return 1;
}

Dialog:findgroup(playerid, response, listitem, inputtext[])
{
	if(!response) return 1;
	new teamid = FindTeamByName(inputtext);
	if(teamid == 0) return SendCustomMessage(playerid, "GROUP", "The team you are looking for (%s) is not on the server.", inputtext);
	
    Group_SetPlayerRequest(Group_GetLeader(teamid), playerid);
	SendCustomMessage(Group_GetLeader(teamid), "GROUP", "%s Request group entry, command "YELLOW"'/accept group'"WHITE" To accept.", GetName(playerid));
    return 1;
}

Dialog:kickmember_group(playerid, response, listitem, inputtext[])
{
    if(response)
    {
        foreach(new memberid : Player)
        {
            if(!strcmp(inputtext, GetName(memberid), false))
            {
                new group_name[32];
                Group_GetName(GetPlayerGroup(playerid), group_name, 32);
                SetPlayerGroup(memberid, 0);
                SendCustomMessage(memberid, "GROUP", "You have been kicked out of the %s group", group_name);
            }
        }
    }
    return 1;
}

hook OnPlayerDisconnect(playerid, reasons)
{
    new groupid = GetPlayerGroup(playerid);
    if (Group_GetLeader(groupid) == playerid)
    {
        if(Group_GetCountMember(groupid) <= 1)
        {
            Group_Delete(groupid);
            Group_ResetData(groupid);
            SetPlayerGroup(playerid, INVALID_GROUP_ID);
            return ALS_CONTINUE;
        }
        foreach(new memberid : Player) if (groupid == GetPlayerGroup(memberid) && memberid != playerid)
        {
            Group_SetLeader(groupid, memberid);
            SetPlayerGroup(playerid, INVALID_GROUP_ID);
            SendGroupMessage(groupid, COLOR_YELLOW, "The group leader is now %s(id:%i)", GetName(memberid), memberid);
            SendGroupMessage(groupid, COLOR_LIGHTRED, "Leader %s(id:%i) has left the group (reason: disconnect).", GetName(playerid), playerid);
            return ALS_CONTINUE;
        }
    }
    else if(Group_GetLeader(groupid) != playerid)
    {
        SetPlayerGroup(playerid, INVALID_GROUP_ID);
        SendGroupMessage(groupid, COLOR_LIGHTRED, "Member %s(id:%i) has left the group (reason: disconnect).", GetName(playerid), playerid);
        return ALS_CONTINUE;
    }
    return ALS_CONTINUE;
}