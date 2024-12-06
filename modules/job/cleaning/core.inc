GetSidejobType(playerid)
{
    new actorid = Actor_Nearest(playerid, 2.5);
    if(actorid == -1) return 0;
    switch(HashStr(ActorData[actorid][actorType]))
    {
        case HS<cleaning>: return GROUP_JOB_CLEANING;
    }
    return 0;
}

Group_ResetData(groupid)
{
    foreach(new i : Player) if(GetPlayerGroup(i) == groupid)
    {
        PlayerData[i][pGroupJob] = GROUP_JOB_NONE;
    }
    
    VS_Respawn(VS_GetID(GroupData[groupid][GROUP_VEHICLEID]));
    GroupData[groupid][GROUP_VEHICLEID] = INVALID_VEHICLE_ID;
}

Cleaning_ResetData(groupid)
{
    foreach(new playerid : Player) if(GetPlayerGroup(playerid) == groupid)
    {
        PlayerData[playerid][pGroupJob] = GROUP_JOB_NONE;
        DeletePVar(playerid, "cleaning_trash");
    }
}

Cleaning_SetupData(groupid)
{
    foreach(new playerid : Player) if(GetPlayerGroup(playerid) == groupid)
    {
        SetPVarInt(playerid, "cleaning_trash", 0);
        PlayerData[playerid][pGroupJob] = GROUP_JOB_CLEANING;
    }
}

CMD:group(playerid, params[])
{
    new
        option[32],
        arg[128],
        groupid = GetPlayerGroup(playerid),
        leaderid = Group_GetLeader(groupid),
        count_member
    ;
    if(!GetSidejobType(playerid)) return 1;
    if(!GetPlayerGroup(playerid)) return SendErrorMessage(playerid, "anda harus membentuk sebuah " YELLOW"group"WHITE" terlebih dahulu untuk memulai!");
    if(sscanf(params, "s[32]S()[123]", option, arg)) return SendSyntaxMessage(playerid, "/group [start]");

    switch(HashStr(option))
    {
        case HS<start>:
        {
            new Float:lX, Float:lY, Float:lZ;
            if(playerid != leaderid) return SendErrorMessage(playerid, "anda bukan ketua group!");
            GetPlayerPos(playerid, lX, lY, lZ);
            foreach(new memberid : Player) if(GetPlayerGroup(playerid) == GetPlayerGroup(memberid))
            {
                if(IsPlayerInRangeOfPoint(memberid, 10.0, lX, lY, lZ)) count_member++;
            }
            
            if(count_member != Group_GetCountMember(groupid)) return SendErrorMessage(playerid, "Anggota group belum berada di dekat anda!");
            SendGroupMessage(groupid, COLOR_YELLOW, WHITE"tugas group telah dimulai!, silahkan ketua group memilih kendaraan yang akan digunakan");

            Cleaning_ResetData(groupid);
            Cleaning_SetupData(groupid);
        }
    }
    return 1;
}