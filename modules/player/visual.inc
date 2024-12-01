function CreatePlayerHUD(playerid)
{
	/* Energy */
	ENERGYTD[playerid][0] = CreatePlayerTextDraw(playerid, 571.000000, 134.000000, "_");
	PlayerTextDrawFont(playerid, ENERGYTD[playerid][0], 1);
	PlayerTextDrawLetterSize(playerid, ENERGYTD[playerid][0], 0.595833, 4.250002);
	PlayerTextDrawTextSize(playerid, ENERGYTD[playerid][0], 298.500000, 75.000000);
	PlayerTextDrawSetOutline(playerid, ENERGYTD[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, ENERGYTD[playerid][0], 0);
	PlayerTextDrawAlignment(playerid, ENERGYTD[playerid][0], 2);
	PlayerTextDrawColor(playerid, ENERGYTD[playerid][0], -1);
	PlayerTextDrawBackgroundColor(playerid, ENERGYTD[playerid][0], 255);
	PlayerTextDrawBoxColor(playerid, ENERGYTD[playerid][0], 135);
	PlayerTextDrawUseBox(playerid, ENERGYTD[playerid][0], 1);
	PlayerTextDrawSetProportional(playerid, ENERGYTD[playerid][0], 1);
	PlayerTextDrawSetSelectable(playerid, ENERGYTD[playerid][0], 0);

	ENERGYTD[playerid][1] = CreatePlayerTextDraw(playerid, 547.000000, 136.000000, "ENERGY");
	PlayerTextDrawFont(playerid, ENERGYTD[playerid][1], 1);
	PlayerTextDrawLetterSize(playerid, ENERGYTD[playerid][1], 0.412499, 1.549999);
	PlayerTextDrawTextSize(playerid, ENERGYTD[playerid][1], 400.000000, 17.000000);
	PlayerTextDrawSetOutline(playerid, ENERGYTD[playerid][1], 0);
	PlayerTextDrawSetShadow(playerid, ENERGYTD[playerid][1], 0);
	PlayerTextDrawAlignment(playerid, ENERGYTD[playerid][1], 1);
	PlayerTextDrawColor(playerid, ENERGYTD[playerid][1], -168436481);
	PlayerTextDrawBackgroundColor(playerid, ENERGYTD[playerid][1], 255);
	PlayerTextDrawBoxColor(playerid, ENERGYTD[playerid][1], 50);
	PlayerTextDrawUseBox(playerid, ENERGYTD[playerid][1], 0);
	PlayerTextDrawSetProportional(playerid, ENERGYTD[playerid][1], 1);
	PlayerTextDrawSetSelectable(playerid, ENERGYTD[playerid][1], 0);
	
	/* Speedometer */
	SPEEDOTD[playerid][0] = CreatePlayerTextDraw(playerid, 572.000000, 372.000000, "_");
	PlayerTextDrawFont(playerid, SPEEDOTD[playerid][0], 1);
	PlayerTextDrawLetterSize(playerid, SPEEDOTD[playerid][0], 0.600000, 8.300003);
	PlayerTextDrawTextSize(playerid, SPEEDOTD[playerid][0], 298.500000, 135.000000);
	PlayerTextDrawSetOutline(playerid, SPEEDOTD[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, SPEEDOTD[playerid][0], 0);
	PlayerTextDrawAlignment(playerid, SPEEDOTD[playerid][0], 2);
	PlayerTextDrawColor(playerid, SPEEDOTD[playerid][0], -1);
	PlayerTextDrawBackgroundColor(playerid, SPEEDOTD[playerid][0], 255);
	PlayerTextDrawBoxColor(playerid, SPEEDOTD[playerid][0], 135);
	PlayerTextDrawUseBox(playerid, SPEEDOTD[playerid][0], 1);
	PlayerTextDrawSetProportional(playerid, SPEEDOTD[playerid][0], 1);
	PlayerTextDrawSetSelectable(playerid, SPEEDOTD[playerid][0], 0);

	SPEEDOTD[playerid][1] = CreatePlayerTextDraw(playerid, 519.000000, 412.000000, "FUEL:");
	PlayerTextDrawFont(playerid, SPEEDOTD[playerid][1], 2);
	PlayerTextDrawLetterSize(playerid, SPEEDOTD[playerid][1], 0.287500, 1.350000);
	PlayerTextDrawTextSize(playerid, SPEEDOTD[playerid][1], 400.000000, 17.000000);
	PlayerTextDrawSetOutline(playerid, SPEEDOTD[playerid][1], 0);
	PlayerTextDrawSetShadow(playerid, SPEEDOTD[playerid][1], 0);
	PlayerTextDrawAlignment(playerid, SPEEDOTD[playerid][1], 1);
	PlayerTextDrawColor(playerid, SPEEDOTD[playerid][1], -1061109505);
	PlayerTextDrawBackgroundColor(playerid, SPEEDOTD[playerid][1], 255);
	PlayerTextDrawBoxColor(playerid, SPEEDOTD[playerid][1], 50);
	PlayerTextDrawUseBox(playerid, SPEEDOTD[playerid][1], 0);
	PlayerTextDrawSetProportional(playerid, SPEEDOTD[playerid][1], 1);
	PlayerTextDrawSetSelectable(playerid, SPEEDOTD[playerid][1], 0);

	SPEEDOTD[playerid][2] = CreatePlayerTextDraw(playerid, 519.000000, 396.000000, "HEALTH:");
	PlayerTextDrawFont(playerid, SPEEDOTD[playerid][2], 2);
	PlayerTextDrawLetterSize(playerid, SPEEDOTD[playerid][2], 0.287500, 1.350000);
	PlayerTextDrawTextSize(playerid, SPEEDOTD[playerid][2], 400.000000, 17.000000);
	PlayerTextDrawSetOutline(playerid, SPEEDOTD[playerid][2], 0);
	PlayerTextDrawSetShadow(playerid, SPEEDOTD[playerid][2], 0);
	PlayerTextDrawAlignment(playerid, SPEEDOTD[playerid][2], 1);
	PlayerTextDrawColor(playerid, SPEEDOTD[playerid][2], -1061109505);
	PlayerTextDrawBackgroundColor(playerid, SPEEDOTD[playerid][2], 255);
	PlayerTextDrawBoxColor(playerid, SPEEDOTD[playerid][2], 50);
	PlayerTextDrawUseBox(playerid, SPEEDOTD[playerid][2], 0);
	PlayerTextDrawSetProportional(playerid, SPEEDOTD[playerid][2], 1);
	PlayerTextDrawSetSelectable(playerid, SPEEDOTD[playerid][2], 0);

	HEALTHTD[playerid] = CreatePlayerTextDraw(playerid, 572.000000, 396.000000, "--");
	PlayerTextDrawFont(playerid, HEALTHTD[playerid], 2);
	PlayerTextDrawLetterSize(playerid, HEALTHTD[playerid], 0.287500, 1.350000);
	PlayerTextDrawTextSize(playerid, HEALTHTD[playerid], 400.000000, 17.000000);
	PlayerTextDrawSetOutline(playerid, HEALTHTD[playerid], 0);
	PlayerTextDrawSetShadow(playerid, HEALTHTD[playerid], 0);
	PlayerTextDrawAlignment(playerid, HEALTHTD[playerid], 1);
	PlayerTextDrawColor(playerid, HEALTHTD[playerid], -1061109505);
	PlayerTextDrawBackgroundColor(playerid, HEALTHTD[playerid], 255);
	PlayerTextDrawBoxColor(playerid, HEALTHTD[playerid], 50);
	PlayerTextDrawUseBox(playerid, HEALTHTD[playerid], 0);
	PlayerTextDrawSetProportional(playerid, HEALTHTD[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, HEALTHTD[playerid], 0);

	SPEEDOTD[playerid][3] = CreatePlayerTextDraw(playerid, 519.000000, 380.000000, "SPEED:");
	PlayerTextDrawFont(playerid, SPEEDOTD[playerid][3], 2);
	PlayerTextDrawLetterSize(playerid, SPEEDOTD[playerid][3], 0.287500, 1.350000);
	PlayerTextDrawTextSize(playerid, SPEEDOTD[playerid][3], 400.000000, 17.000000);
	PlayerTextDrawSetOutline(playerid, SPEEDOTD[playerid][3], 0);
	PlayerTextDrawSetShadow(playerid, SPEEDOTD[playerid][3], 0);
	PlayerTextDrawAlignment(playerid, SPEEDOTD[playerid][3], 1);
	PlayerTextDrawColor(playerid, SPEEDOTD[playerid][3], -1061109505);
	PlayerTextDrawBackgroundColor(playerid, SPEEDOTD[playerid][3], 255);
	PlayerTextDrawBoxColor(playerid, SPEEDOTD[playerid][3], 50);
	PlayerTextDrawUseBox(playerid, SPEEDOTD[playerid][3], 0);
	PlayerTextDrawSetProportional(playerid, SPEEDOTD[playerid][3], 1);
	PlayerTextDrawSetSelectable(playerid, SPEEDOTD[playerid][3], 0);

	KMHTD[playerid] = CreatePlayerTextDraw(playerid, 572.000000, 379.000000, "--");
	PlayerTextDrawFont(playerid, KMHTD[playerid], 2);
	PlayerTextDrawLetterSize(playerid, KMHTD[playerid], 0.287500, 1.350000);
	PlayerTextDrawTextSize(playerid, KMHTD[playerid], 400.000000, 17.000000);
	PlayerTextDrawSetOutline(playerid, KMHTD[playerid], 0);
	PlayerTextDrawSetShadow(playerid, KMHTD[playerid], 0);
	PlayerTextDrawAlignment(playerid, KMHTD[playerid], 1);
	PlayerTextDrawColor(playerid, KMHTD[playerid], -1061109505);
	PlayerTextDrawBackgroundColor(playerid, KMHTD[playerid], 255);
	PlayerTextDrawBoxColor(playerid, KMHTD[playerid], 50);
	PlayerTextDrawUseBox(playerid, KMHTD[playerid], 0);
	PlayerTextDrawSetProportional(playerid, KMHTD[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, KMHTD[playerid], 0);

	VEHNAMETD[playerid] = CreatePlayerTextDraw(playerid, 519.000000, 362.000000, "--");
	PlayerTextDrawFont(playerid, VEHNAMETD[playerid], 0);
	PlayerTextDrawLetterSize(playerid, VEHNAMETD[playerid], 0.408333, 1.500000);
	PlayerTextDrawTextSize(playerid, VEHNAMETD[playerid], 400.000000, 17.000000);
	PlayerTextDrawSetOutline(playerid, VEHNAMETD[playerid], 1);
	PlayerTextDrawSetShadow(playerid, VEHNAMETD[playerid], 0);
	PlayerTextDrawAlignment(playerid, VEHNAMETD[playerid], 1);
	PlayerTextDrawColor(playerid, VEHNAMETD[playerid], -1061109505);
	PlayerTextDrawBackgroundColor(playerid, VEHNAMETD[playerid], 255);
	PlayerTextDrawBoxColor(playerid, VEHNAMETD[playerid], 50);
	PlayerTextDrawUseBox(playerid, VEHNAMETD[playerid], 0);
	PlayerTextDrawSetProportional(playerid, VEHNAMETD[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, VEHNAMETD[playerid], 0);
}

function CreateGlobalTextDraw()
{

}