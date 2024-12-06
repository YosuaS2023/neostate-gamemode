/*

	____      _ _                   _ 
	|  _ \ ___| (_) __ _ _______  __| |
	| |_) / _ \ | |/ _` |_  / _ \/ _` |
	|  _ <  __/ | | (_| |/ /  __/ (_| |
	|_| \_\___|_|_|\__,_/___\___|\__,_|
                                    
   	• Credits: - Kalcor [SAMP]
			   - Y_Less [sscanf]
			   - pBlueG [MySQL]
			   - ZeeX [crashdetect/compiler]
			   - Yashas [Command]
			   - SyS [samp-bcrypt]
			   - Suzy [Main developer gamemode]
			   - TsumuX [Developer gamemode]
			   - YME
			   
	Note: ini adalah projek gamemode serta mengunakan UtilsScript sebagai
	fitur tambahan untuk membuat kode didalam gamemode, yg dimana UtilsScript dibuat
	di waktu bersamaan

	By Suzy & TsumuX

	ChangeLogs[0.1]
	- TsumuX
	[UPDATED-SISTEM&PLAYER] System account

	- Suzy
	[ADDED-PROPERTY] Dealership system
	[ADDED-PROPERTY] Pump dynamic system
	[ADDED-SYSTEM&SERVER] Economy dynamic
	[ADDED-ASSETS] Dynamic atm with system account atm
	[ADDED-ASSETS] Dynamic plants
	[ADDED-JOB] Job farmer wheat ( beta )
	[ADDED-PROPERTY] Dynamic house property
	[ADDED-PROPERTY] Structure house (/hbmenu)
	[ADDED-JOB] Sweeper (salary per meter x money_per_meter)
	[ADDED-SYSTEM] Dynamic setting (can add/change the settings used)
	[ADDED-SYSTEM-SERVER] Dynamic GPS (can add/delete libraries GPS and GPS locations)
	[ADDED-SYSTEM-PLAYER] Salary
	[ADDED-SERVER] Dynamic Vehicle (kendaraan job ataupun kendaraan global akan mengunakan ini)
	[ADDED-ASSETS] Dynamic Actor
*/
#pragma dynamic 123456789
new DebugLevel = 0;

#define DIRECTORY_DEBUG "log_debug.txt"
#define PP_SYNTAX

/* Includes */
#include <a_samp>
#include <a_mysql>
#include <a_zones>
#include <filemanager>
#include <easyDialog>
#include <dialog>
#include <selection>
#include <strlib>
#include <dini>
#include <foreach>
#include <UtilsScripts\hashstring>
#include <UtilsScripts\VaFunction>
#include <UtilsScripts\Timer>
#include <UtilsScripts\Mysql>
#include <UtilsScripts\LoadingBar>
#include <UtilsScripts\Function>
#include <UtilsScripts\bit>

#include <CoreX\CX_Group.inc>
#include <samp_bcrypt>
#include <izcmd>
#include <EVF2>
#include <progress2>
#include <sscanf2>
#include <streamer>
#include <PreviewModelDialog2>

/* utils */
#include "../modules/utils/colours.inc"
#include "../modules/utils/define.inc"
#include "../modules/utils/variabels.inc"
#include "../modules/utils/enumerated.inc"
#include "../modules/utils/struct-sql.inc"
#include "../modules/utils/va.inc"
#include "../modules/utils/functions.inc"
#include "../modules/utils/task.inc"
#include "../modules/utils/callback.inc"
#include "../modules/utils/mysql.inc"
/* server */
#include "../modules/server/setting.inc"
#include "../modules/server/economy.inc"
#include "../modules/server/vehicle.inc"
/* system */
#include "../modules/system/gps.inc"
#include "../modules/system/salary.inc"
#include "../modules/system/admin.inc"
#include "../modules/system/texture.inc"
#include "../modules/system/group.inc"
/* player */
#include "../modules/player/accounts.inc"
#include "../modules/player/visual.inc"
#include "../modules/player/items.inc"
#include "../modules/player/vehicle.inc"
/* property */
#include "../modules/property/house.inc"
#include "../modules/property/house-structure.inc"
#include "../modules/property/business.inc"
#include "../modules/property/rental.inc"
#include "../modules/property/dealership.inc"
#include "../modules/property/fuel_station.inc"
/* command */
#include "../modules/command/general.inc"
#include "../modules/command/admins.inc"
/* assets */
#include "../modules/utils/object.inc"
#include "../modules/assets/atm.inc"
#include "../modules/assets/plant.inc"
#include "../modules/assets/actor.inc"
#include "../modules/assets/trash.inc"
/* job 
#include "../modules/job/general.inc"
#include "../modules/job/sweeper.inc"
#include "../modules/job/mower.inc"*/
#include "../modules/job/cleaning/core.inc"
#include "../modules/job/farmer.inc"
/* feature */
#include "../modules/feature/cityhall.inc"

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	switch(HashStr(VehCore[vehicleid][vehType]))
	{
		case HS<sweeper>: if(PlayerData[playerid][SweeperVehicle] == INVALID_VEHICLE_ID && PlayerData[playerid][pSidejob] == SIDEJOB_SWEEPER) PlayerData[playerid][SweeperVehicle] = vehicleid;
		case HS<mower>: if(PlayerData[playerid][MowerVehicle] == INVALID_VEHICLE_ID && PlayerData[playerid][pSidejob] == SIDEJOB_MOWER) PlayerData[playerid][MowerVehicle] = vehicleid;
	}
	return 1;
}
public OnPlayerExitVehicle(playerid, vehicleid)
{
	PlayerData[playerid][pLastVehicle] = vehicleid;
	return 1;
}