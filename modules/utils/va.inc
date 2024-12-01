function SendClientMessageEx(playerid, colour, const text[], va_args<>)
{
    new str[145];
    va_format(str, sizeof(str), text, va_start<3>);
    return SendClientMessage(playerid, colour, str);
}

function WriteLog(logFile[], logText[], va_args<>)
{
    new
		sz_string[256],
		sz_logEntry[256],
		File: logfile,
		i_dateTime[2][3];

	gettime(i_dateTime[0][0], i_dateTime[0][1], i_dateTime[0][2]);
	getdate(i_dateTime[1][0], i_dateTime[1][1], i_dateTime[1][2]);
	va_format(sz_string, sizeof(sz_string), logText, va_start<2>);
	format(sz_logEntry, sizeof(sz_logEntry), "[%i/%i/%i - %i:%02i:%02i] %s\r\n", i_dateTime[1][2], i_dateTime[1][1], i_dateTime[1][0], i_dateTime[0][0], i_dateTime[0][1], i_dateTime[0][2], sz_string);
	if(logfile) f_close(logfile);
	if(!file_exists(logFile)) file_create(logFile), logfile = f_open(logFile, "w");
	else logfile = f_open(logFile, "a");
	if(logfile)
	{
		f_write(logfile, sz_logEntry);
		f_close(logfile);
	}
}

function SendDebugMessage(const text[], va_args<>)
{
	new str[146];
	va_format(str, sizeof(str), text, va_start<1>);
	printf("[DEBUG] %s",str);
	WriteLog(DIRECTORY_DEBUG, str);
	foreach(new admin : Player) if(PlayerData[admin][pAdmin] == DEVELOPER)
	{
		SendClientMessageEx(admin, COLOR_CLIENT, "DEBUG: %s", str);
	}
	return 1;
}

function SendNearbyMessage(playerid, Float:radius, color, const str[], {Float,_}:...)
{
	static
	    args,
	    start,
	    end,
	    string[144]
	;
	#emit LOAD.S.pri 8
	#emit STOR.pri args

	if (args > 16)
	{
		#emit ADDR.pri str
		#emit STOR.pri start

	    for (end = start + (args - 16); end > start; end -= 4)
		{
	        #emit LREF.pri end
	        #emit PUSH.pri
		}
		#emit PUSH.S str
		#emit PUSH.C 144
		#emit PUSH.C string

		#emit LOAD.S.pri 8
		#emit CONST.alt 4
		#emit SUB
		#emit PUSH.pri

		#emit SYSREQ.C format
		#emit LCTRL 5
		#emit SCTRL 4

        foreach (new i : Player)
		{
			if (IsPlayerNearPlayer(i, playerid, radius) && PlayerData[i][pSpawned])
			{
  				SendClientMessage(i, color, string);
			}
		}
		return 1;
	}
	foreach (new i : Player)
	{
		if (IsPlayerNearPlayer(i, playerid, radius) && PlayerData[i][pSpawned])
		{
			SendClientMessage(i, color, str);
		}
	}
	return 1;
}

SendAdminMessage(color, const str[], {Float,_}:...)
{
    static
        args,
        start,
        end,
        string[512]
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
            if(PlayerData[i][pAdmin] >= 1 /*&& !pData[i][pDisableAdmin]*/) {
				SendClientMessageEx(i, color, ""TOMATO_E"%s", string);
            }
        }
        return 1;
    }
    foreach (new i : Player)
    {
        if(PlayerData[i][pAdmin] >= 1 /*&& !pData[i][pDisableAdmin]*/) {
			SendClientMessageEx(i, color, ""TOMATO_E"%s", string);
        }
    }
    return 1;
}