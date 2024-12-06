function IsPlayerADuty(playerid, level)
{
	if(PlayerData[playerid][pAdmin] >= level && PlayerData[playerid][pAduty]) return 1;
	return 0;
}

stock Admin_GetRank(playerid, rank[], len = sizeof(rank))
{
    switch(PlayerData[playerid][pAdmin])
    {
        case 1: format(rank, 32, "Staff");
        case 2: format(rank, 32, "Supervisor");
        case 3: format(rank, len, "Operations Lead");
        case 4: format(rank, len, "Owner");
        case 5: format(rank, len, "Developer");
        default: format(rank, len, "None");
    }
    return 1;
}