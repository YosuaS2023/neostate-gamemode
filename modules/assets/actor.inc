#include <UtilsScripts\Als.inc>

enum e_actor_data
{
	actorID,
	actorVirtualWorld,
	actorInterior,
	Float:actorPos[4],
    Float:actorInteraction[3],
	actorName[MAX_PLAYER_NAME+1],
	
    STREAMER_TAG_ACTOR:actorObject,
	actorModel,
	Text3D:actorText,
    actorCP,
	actorStatus,
	actorType[24],
	actorAnimLib[128],
	actorAnimName[128],
	actorCash,
	actorNotification,
	actorCooldown
};
new ActorData[MAX_DYNAMIC_ACTOR][e_actor_data];
new Iterator:AssetsActor<MAX_DYNAMIC_ACTOR>;

/*
    function
*/
Actor_IsExists(index)
{
	if(Iter_Contains(AssetsActor, index))
		return 1;
	
	return 0;
}

function Actor_Create(playerid, model, name[])
{
	static
		index;
	new 
		string[255],
		Float:x,
		Float:y,
		Float:z,
		Float:a,
		vw,
		int
	;
	if((index = Iter_Free(AssetsActor)) != cellmin)
	{
		Iter_Add(AssetsActor, index);

		GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, a);
		vw = GetPlayerVirtualWorld(playerid);
		int = GetPlayerInterior(playerid);

		ActorData[index][actorPos][0] = x;
		ActorData[index][actorPos][1] = y;
		ActorData[index][actorPos][2] = z;
		ActorData[index][actorPos][3] = a;
		ActorData[index][actorVirtualWorld] = vw;
		ActorData[index][actorInterior] = int;
		ActorData[index][actorModel] = model;
		ActorData[index][actorStatus] = 0;
		format(ActorData[index][actorType], 24, "None");
		ActorData[index][actorCash] = 15000;
		ActorData[index][actorNotification] = 0;
		format(ActorData[index][actorAnimLib], 128, "");
		format(ActorData[index][actorAnimName], 128, "");

		format(ActorData[index][actorName], MAX_PLAYER_NAME+1, name);

		ActorData[index][actorObject] = CreateDynamicActor(ActorData[index][actorModel], ActorData[index][actorPos][0], ActorData[index][actorPos][1], ActorData[index][actorPos][2], ActorData[index][actorPos][3], true, 100, ActorData[index][actorVirtualWorld], ActorData[index][actorInterior]);
		format(string, sizeof(string), "%s (%d)", ActorData[index][actorName], index);
		ActorData[index][actorText] = CreateDynamic3DTextLabel(string, COLOR_WHITE, ActorData[index][actorPos][0], ActorData[index][actorPos][1], ActorData[index][actorPos][2]+1, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, ActorData[index][actorVirtualWorld], ActorData[index][actorInterior]);
		
		Streamer_SetIntData(STREAMER_TYPE_ACTOR, ActorData[index][actorObject], E_STREAMER_EXTRA_ID, index);

		mysql_tquery(sqlcon, sprintf("INSERT INTO "SQL_TABLE_ACTOR"(`actorInterior`,`actorVirtualWorld`) VALUES('%d', '%d');", ActorData[index][actorInterior], ActorData[index][actorVirtualWorld]), "OnActorCreated", "d", index);
		return index;
	}
	return -1;
}

function Actor_SetType(index, type[])
{
	if(Actor_IsExists(index))
	{
		format(ActorData[index][actorType], 24, "%s", type);
		Actor_Save(index);
		return 1;
	}
	return 0;
}

function Actor_Type(actorid, type[])
{
	if(!strcmp(ActorData[actorid][actorType], type, true)) return 1;
	return 0;
}

function Actor_SetAnim(index, animlib[], animname[])
{
	if(Actor_IsExists(index))
	{
		strcat(ActorData[index][actorAnimLib], animlib);
		strcat(ActorData[index][actorAnimName], animname);
		ApplyDynamicActorAnimation(ActorData[index][actorObject], animlib, animname, 4.1, 1, 0, 0, 1, 0);
//		printf("%d %s %s %s %s", index, animlib, animname, ActorData[index][actorAnimLib], ActorData[index][actorAnimName]);
		Actor_Save(index);
		return 1;
	}
	return 0;
}
Actor_SetCash(index, amount, bool:reduce = false)
{
	if(Actor_IsExists(index))
	{
		if(reduce)
		{
			ActorData[index][actorCash] -= amount;
			if(ActorData[index][actorCash] <= 0)
				ActorData[index][actorCash] = 0;
		}
		else
		{
			ActorData[index][actorCash] = amount;
		}
		Actor_Save(index);
		return 1;
	}
	return 0;
}

Actor_Delete(index)
{
	if(Actor_IsExists(index))
	{
		Iter_Remove(AssetsActor, index);
		mysql_tquery(sqlcon, sprintf("DELETE FROM "SQL_TABLE_ACTOR" WHERE `actorID`='%d';", ActorData[index][actorID]));

		if(IsValidDynamicActor(ActorData[index][actorObject]))
			DestroyDynamicActor(ActorData[index][actorObject]);

		if(IsValidDynamic3DTextLabel(ActorData[index][actorText]))
			DestroyDynamic3DTextLabel(ActorData[index][actorText]);

		new tmp_ActorData[e_actor_data];
		ActorData[index] = tmp_ActorData;

		ActorData[index][actorText] = Text3D:INVALID_STREAMER_ID;
		ActorData[index][actorObject] = INVALID_STREAMER_ID;
		return 1;
	}
	return 0;
}

stock Actor_Sync(index)
{
	if(Actor_IsExists(index))
	{	
		new string[255];
		if(IsValidDynamicActor(ActorData[index][actorObject]))
		{
			Streamer_SetFloatData(STREAMER_TYPE_ACTOR, ActorData[index][actorObject], E_STREAMER_X, ActorData[index][actorPos][0]);
			Streamer_SetFloatData(STREAMER_TYPE_ACTOR, ActorData[index][actorObject], E_STREAMER_Y, ActorData[index][actorPos][1]);
			Streamer_SetFloatData(STREAMER_TYPE_ACTOR, ActorData[index][actorObject], E_STREAMER_Z, ActorData[index][actorPos][2]);
			Streamer_SetFloatData(STREAMER_TYPE_ACTOR, ActorData[index][actorObject], E_STREAMER_R_Z, ActorData[index][actorPos][3]);

			Streamer_SetIntData(STREAMER_TYPE_ACTOR, ActorData[index][actorObject], E_STREAMER_INTERIOR_ID, ActorData[index][actorInterior]);
			Streamer_SetIntData(STREAMER_TYPE_ACTOR, ActorData[index][actorObject], E_STREAMER_WORLD_ID, ActorData[index][actorVirtualWorld]);
		}
		else 
		{
			if(strcmp(ActorData[index][actorAnimLib], "", true) && strcmp(ActorData[index][actorAnimName], "", true))
			{
				ActorData[index][actorObject] = CreateDynamicActor(ActorData[index][actorModel], ActorData[index][actorPos][0], ActorData[index][actorPos][1], ActorData[index][actorPos][2], ActorData[index][actorPos][3], true, 100, ActorData[index][actorVirtualWorld], ActorData[index][actorInterior]);
			}
			else
			{
				ActorData[index][actorObject] = CreateDynamicActor(ActorData[index][actorModel], ActorData[index][actorPos][0], ActorData[index][actorPos][1], ActorData[index][actorPos][2], ActorData[index][actorPos][3], true, 100, ActorData[index][actorVirtualWorld], ActorData[index][actorInterior]);
				ApplyDynamicActorAnimation(ActorData[index][actorObject], ActorData[index][actorAnimLib], ActorData[index][actorAnimName], 4.1, 1, 0, 0, 1, 0);
			}
			Streamer_SetIntData(STREAMER_TYPE_ACTOR, ActorData[index][actorObject], E_STREAMER_EXTRA_ID, index);
		}
		if(IsValidDynamic3DTextLabel(ActorData[index][actorText]))
		{
			Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, ActorData[index][actorText], E_STREAMER_X, ActorData[index][actorPos][0]);
			Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, ActorData[index][actorText], E_STREAMER_Y, ActorData[index][actorPos][1]);
			Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, ActorData[index][actorText], E_STREAMER_Z, ActorData[index][actorPos][2]+1);

			Streamer_SetIntData(STREAMER_TYPE_3D_TEXT_LABEL, ActorData[index][actorText], E_STREAMER_INTERIOR_ID, ActorData[index][actorInterior]);
			Streamer_SetIntData(STREAMER_TYPE_3D_TEXT_LABEL, ActorData[index][actorText], E_STREAMER_WORLD_ID, ActorData[index][actorVirtualWorld]);
		}
		else 
		{
			format(string, sizeof(string), "%s (%d)", ActorData[index][actorName], index);
			ActorData[index][actorText] = CreateDynamic3DTextLabel(string, COLOR_WHITE, ActorData[index][actorPos][0], ActorData[index][actorPos][1], ActorData[index][actorPos][2]+1, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, ActorData[index][actorVirtualWorld], ActorData[index][actorInterior]);
		}
	}
	return 1;
}

Actor_Save(index)
{
	if(Actor_IsExists(index))
	{
		//printf("Saving %d %s %s", index, ActorData[index][actorAnimLib], ActorData[index][actorAnimName]);
		mysql_tquery(sqlcon, sprintf("UPDATE "SQL_TABLE_ACTOR" SET `actorName` ='%s', `actorAnimLib` ='%s', `actorAnimName`='%s', `actorX`='%f', `actorY`='%f', `actorZ`='%f', `actorA`='%f', `actorInterior`='%d', `actorVirtualWorld`='%d', `actorModel`='%d', `actorType`='%s', `actorCash`='%d' WHERE `actorID`= '%d';", 
			ActorData[index][actorName],
			ActorData[index][actorAnimLib],
			ActorData[index][actorAnimName],
			ActorData[index][actorPos][0],
			ActorData[index][actorPos][1],
			ActorData[index][actorPos][2],
			ActorData[index][actorPos][3],
			ActorData[index][actorInterior],
			ActorData[index][actorVirtualWorld],
			ActorData[index][actorModel],
			ActorData[index][actorType],
			ActorData[index][actorCash],
			ActorData[index][actorID]
		));
		return 1;
	}
	return 0;
}

Actor_Nearest(playerid, Float:range = 6.0)
{
	foreach(new i : AssetsActor) if(IsPlayerInRangeOfPoint(playerid, range, ActorData[i][actorPos][0], ActorData[i][actorPos][1], ActorData[i][actorPos][2]))
	{
		if(GetPlayerInterior(playerid) == ActorData[i][actorInterior] && GetPlayerVirtualWorld(playerid) == ActorData[i][actorVirtualWorld])
			return i;
	}
	return -1;
}

SSCANF:ActorMenu(string[]) 
{
 	if(!strcmp(string,"create",true)) return 1;
 	else if(!strcmp(string,"delete",true)) return 2;
 	else if(!strcmp(string,"type",true)) return 3;
 	else if(!strcmp(string,"cash",true)) return 4;
 	else if(!strcmp(string,"anim",true)) return 5;
 	else if(!strcmp(string,"point",true)) return 6;
 	return 0;
}

CMD:resetactor(playerid, params[])
{
    if(!IsPlayerADuty(playerid, 3)) return PermissionError(playerid);

	foreach(new i : AssetsActor)
	{
		if(ActorData[i][actorCooldown])
		{
			ActorData[i][actorCooldown] = 0;
		}
	}
	SendAdminMessage(X11_TOMATO_1, "AdmWarn: %s has using reset actor robbery cooldown.", ReturnName(playerid));
	return 1;
}

CMD:nearactor(playerid)
{
	new actid;
	if((actid = Actor_Nearest(playerid)) != -1)
		return SendServerMessage(playerid, "Nearest actor id %i", actid);
	
	return 1;
}

CMD:actormenu(playerid, params[])
{
    if(!IsPlayerADuty(playerid, 3)) return PermissionError(playerid);
	
	static index, action, nextParams[128];

	if(sscanf(params, "k<ActorMenu>S()[128]", action, nextParams))
	{
		SendSyntaxMessage(playerid, "/actormenu [entity]");
		SendCustomMessage(playerid, "OPTION", "[create/delete/type/cash/anim/point]");
		return 1;
	}

	switch(action)
	{
		case 1: // Create
		{
			new name[128], model;
			if(sscanf(nextParams, "ds[128]", model, name))
				return SendSyntaxMessage(playerid, "/actormenu create [model] [name]");

			if((index = Actor_Create(playerid, model, name)) != -1) SendServerMessage(playerid, "successfully creating actor "YELLOW"id: %d.", index);
			else SendErrorMessage(playerid, "The number of actors has reached the limit limit ("#MAX_DYNAMIC_ACTOR" actor)");
		}
		case 2: // Delete
		{
			if(sscanf(nextParams, "d", index))
				return SendSyntaxMessage(playerid, "/actormenu delete [actorid]");

			if(Actor_Delete(index)) SendServerMessage(playerid, "You have deleted the actor "YELLOW"id: %d.", index);
			else SendErrorMessage(playerid, "The actor id that you input is not registered!");
		}
		case 3:
		{
			new type[24];
			if(sscanf(nextParams, "ds[24]", index, type))
				return SendSyntaxMessage(playerid, "/actormenu type [actorid] [name type]");
			
			if(Actor_SetType(index, type)) SendServerMessage(playerid, "You set actor id %d type to %s", index, type);
			else SendErrorMessage(playerid, "The actor id that you input is not registered!");
		}
		case 4:
		{
			new amount;
			if(sscanf(nextParams, "dd", index, amount))
				return SendSyntaxMessage(playerid, "/actormenu cash [actorid] [amount]");
			
			if(Actor_SetCash(index, amount)) SendServerMessage(playerid, "You set actor id %d cash to $%d", index, amount);
			else SendErrorMessage(playerid, "The actor id that you input is not registered!");
		}
		case 5:
		{
			new animlib[128], animname[128];
			if(sscanf(nextParams, "ds[128]s[128]", index, animlib, animname))
				return SendSyntaxMessage(playerid, "/actormenu anim [actorid] [animlib] [animname]");
			
			if(isnull(animname) || isnull(animlib))
        		return SendSyntaxMessage(playerid, "/actormenu anim [actorid] [animlib] [animname]");

			if(Actor_SetAnim(index, animlib, animname)) SendServerMessage(playerid, "You set actor id %d animation", index);
			else SendErrorMessage(playerid, "The actor id that you input is not registered! ");
		}
		default: 
		{
			SendSyntaxMessage(playerid, "/actormenu [entity]");
			SendCustomMessage(playerid, "OPTION", "[create/delete/type/cash/anim/point]");
		}
	}
	return 1;
}

forward OnActorCreated(index);
public OnActorCreated(index)
{
	ActorData[index][actorID] = cache_insert_id();
	//Actor_Sync(index);
	Actor_Save(index);
	return 1;
}

forward Actor_Load();
public Actor_Load()
{
	if(cache_num_rows())
	{
		for(new i = 0; i != cache_num_rows(); i++)
		{
			Iter_Add(AssetsActor, i);

			cache_get_field_content(i, "actorName", ActorData[i][actorName], 128);
			cache_get_field_content(i, "actorAnimLib", ActorData[i][actorAnimLib], 128);
			cache_get_field_content(i, "actorAnimName", ActorData[i][actorAnimName], 128);
			cache_get_field_content(i, "actorType", ActorData[i][actorType], 24);

			ActorData[i][actorID] = cache_get_field_int(i, "actorID");
			ActorData[i][actorInterior] = cache_get_field_int(i, "actorInterior");
			ActorData[i][actorVirtualWorld] = cache_get_field_int(i, "actorVirtualWorld");
            ActorData[i][actorPos][0] = cache_get_field_float(i, "actorX");
            ActorData[i][actorPos][1] = cache_get_field_float(i, "actorY");
            ActorData[i][actorPos][2] = cache_get_field_float(i, "actorZ");
            ActorData[i][actorPos][3] = cache_get_field_float(i, "actorA");
			ActorData[i][actorModel] = cache_get_field_int(i, "actorModel");
			ActorData[i][actorCash] = cache_get_field_int(i, "actorCash");
			ActorData[i][actorStatus] = 0;

			Actor_Sync(i);
		}
		printf("*** [Actor] Loaded %d Actor's", cache_num_rows());
	}
	return 1;
}

/*
    Callback
*/
OGMI:ActorInit()
{
    mysql_tquery(sqlcon, "SELECT * FROM " SQL_TABLE_ACTOR, "Actor_Load");
    return 1;
}

OGME:ActorExit()
{
    foreach(new actorid : AssetsActor)
    {
        Actor_Save(actorid);
    }
    return 1;
}