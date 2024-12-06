/* Define & Macro */

/*
	macros
*/
#define forex(%0,%1) for(new %0 = 0; %0 < %1; %0++)

#define function%0(%1) forward %0(%1); public %0(%1)
#define Function:%0(%1) forward %0(%1); public %0(%1)
#define OGMI:%0(%1) forward ogmi_%0(%1); public ogmi_%0(%1)
#define OGME:%0(%1) forward ogme_%0(%1); public ogme_%0(%1)
#define FUNC::%0(%1) forward %0(%1); public %0(%1)

#define RGBAToInt(%0,%1,%2,%3)          ((16777216 * (%0)) + (65536 * (%1)) + (256 * (%2)) + (%3))

#define GetCharacterName(%0) 					PlayerData[%0][pName]
/* debug */
#define D:%0(%1) Debug_Msg%0(%1)
#define Debug_Msg1(%1); { if (DebugLevel >= 1) SendDebugMessage(%1); }
#define Debug_Msg2(%1); { if (DebugLevel >= 2) SendDebugMessage(%1); }
#define Debug_Msg3(%1); { if (DebugLevel >= 3) SendDebugMessage(%1); }
#define Debug_Msg4(%1); { if (DebugLevel >= 4) SendDebugMessage(%1); }
#define Debug_Msg5(%1); { if (DebugLevel >= 5) SendDebugMessage(%1); }

#define COLOR_YELLOW 			0xFFFF00FF
#define COLOR_SERVER 			0x00FFFFFF
#define COLOR_PURPLE 			0xD0AEEBFF
#define COLOR_CLIENT 			0xC6E2FFFF
#define COLOR_WHITE  			0xFFFFFFFF
#define COLOR_LIGHTRED    		0xFF6347FF

#define MAX_CHARS 3

#define DATABASE_ADDRESS "localhost" //Change this to your Database Address
#define DATABASE_USERNAME "root" // Change this to your database username
#define DATABASE_PASSWORD "" //Change this to your database password
#define DATABASE_NAME "basic"

/*
	Table sql
*/
#define SQL_TABLE_ACTOR 			"`assets_actor`"
#define SQL_TABLE_SERVER_DATA 		"`data_server`"
#define SQL_TABLE_TRASH				"`trash`"

#if !defined BCRYPT_HASH_LENGTH
	#define BCRYPT_HASH_LENGTH 250
#endif

#if !defined BCRYPT_COST
	#define BCRYPT_COST 12
#endif

#define SendServerMessage(%0,%1) \
	SendClientMessageEx(%0, 0x00FFFFFF, "SERVER:{FFFFFF} "%1)

#define SendSyntaxMessage(%0,%1) \
	SendClientMessageEx(%0, COLOR_GREY, "USAGE:{FFFFFF} "%1)
	
#define SendErrorMessage(%0,%1) \
	SendClientMessageEx(%0, COLOR_GREY, "ERROR: "%1)
	
#define SendCustomMessage(%0,%1,%2)     SendClientMessageEx(%0, COLOR_ARWIN, %1": "WHITE_E""%2)
#define PermissionError(%0) SendClientMessage(%0, COLOR_ARWIN, "ERROR: "WHITE_E"You don't have permission to use this command.")
#define DUTY_COMMAND "Mengunakan perintah ini harus on duty admin."
#define underline(%0) SendClientMessage(%0, -1, "_______________________________________________")
/*
	DEFINE MAX
*/
#define MAX_PLAYER_VEHICLE 			1000
#define MAX_GAS_PUMPS 				100
#define MAX_INVENTORY 				20
#define MAX_BUSINESS                100
#define MAX_DROPPED_ITEMS  			1000
#define MAX_RENTAL                  20
#define MAX_ECONOMY 				100
#define MAX_DYNAMIC_ACTOR			100
#define MAX_SEEDS 					500
#define MAX_PLANT 					1000
#define MAX_SETTING 				40
#define MAX_HOUSES 					500
#define MAX_HOUSE_STRUCTURES		300
#define MAX_TRASH  					500
/*
	Configuration
*/
#define EXPIRED_HOUSE_LICENSE		(20)
#define EXPIRED_BUSINESS_LICENSE	(20)

/* dealerahip */
#define MODEL_SELECTION_Motorcycle     5
#define MODEL_SELECTION_SUV     6
#define MODEL_SELECTION_PickupVehicles     7
#define MODEL_SELECTION_Lowriders     8
#define MODEL_SELECTION_Saloons     9
#define MODEL_SELECTION_Sport     10

#define MAX_DEALERSHIPVEHICLES 10
#define MAX_CARDEALERSHIPS 8
#define DIALOG_CDCHANGETYPE 1339
#define DIALOG_CDEDIT 1329
#define DIALOG_CDTILL 1327
#define DIALOG_CDEDITCARS 1326
#define DIALOG_CDEDITPARK 1322
#define DIALOG_CDNEWVEH 1320
#define DIALOG_CDRADIUS 1319
#define DIALOG_CDNAME 1318
#define DIALOG_CDPRICE 1317
#define DIALOG_CDBUY 1316
#define DIALOG_CDWITHDRAW 1315
#define DIALOG_CDDEPOSIT 1314
#define DIALOG_CDSELL 1313
#define COLOR_REALRED 0xFF0606FF

/*
	group
*/
#define MAX_GROUP_TYPE 2
#define INVALID_GROUP_TYPE 0

/*
	atm
*/
#define ATM_TABLE_SQL 					"`atms`"
#define MAX_ATMS 						12
#define MAX_ACCOUNT_ATM 				1000
#define INVALID_ITER_ID 				-1
#define ATM_OBJECT_ID 					19324

/* account */
#define GetPlayerSQLID(%0) 					PlayerData[%0][pID]

#define formatex(%0,%1) format(%0, sizeof(%0), %1)
#define PositionXYZ(%0) %0[0],%0[1],%0[2]
#define Array3X(%0) %0[0],%0[1],%0[2]

// atm
#define GetAtmData(%0,%1) AtmData[%0][%1]
#define GetAtmDataArray(%0,%1<%3>) AtmData[%0][%1][%3]

#define SetAtmData(%0,%1,%2) AtmData[%0][%1] = %2
#define AddAtmData(%0,%1,%2) AtmData[%0][%1] += %2
#define SetAtmDataArray(%0,%1<%3>,%2) AtmData[%0][%1][%3] = %2
#define AddAtmDataArray(%0,%1<%3>,%2) AtmData[%0][%1][%3] += %2

#define GetAccountAtmID(%0) 			AccountAtm_GetIDBySQLID(PlayerData[%0][pAccountATM])
/* econoky */
#define @economy%0\32; @ECONOMY@%0
#define @ECONOMY@get<%0> Economy_GetPriceByName(#%0)