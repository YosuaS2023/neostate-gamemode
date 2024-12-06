#if defined _inc_async_dialogs
	#undef _inc_async_dialogs
#endif 

#if defined async_dialogs_included
	#endinput
#endif
#define async_dialogs_included

#include <PawnPlus>

#if !defined PAWN_PLUS_DIALOG_ID
	const PAWN_PLUS_DIALOG_ID = 0x502B; // "P+" in ascii hex
#endif

#if defined PPDIALOGS_NO_COMPAT_NATIVE_CALL
	native ShowPlayerDialogStr(playerid, dialogid, style, ConstAmxString:caption, ConstAmxString:info, ConstAmxString:button1, ConstAmxString:button2) = ShowPlayerDialog;
#endif

enum dialog_data
{
	Response,
	Listitem,
	Inputtext[128 + 1],
}

static Task:PlayerDialogTasks[MAX_PLAYERS] = {Task:0, ...};

static CallbackHandler:AsyncDialogResponseHandler;

forward _pp@on_init@PPAsyncDialogs();
public _pp@on_init@PPAsyncDialogs()
{
	// I wish I didn't had to do this, but rewriting the public function header is cool!!1
	wait_ticks(1);
	AsyncDialogResponseHandler = pawn_register_callback("OnDialogResponse", "AsyncDialogsHandler");
}

forward _pp@on_exit@PPAsyncDialogs();
public _pp@on_exit@PPAsyncDialogs()
{
	pawn_unregister_callback(AsyncDialogResponseHandler);
}

#pragma deprecated Use ShowPlayerAsyncDialog instead
stock AwaitAsyncDialog(playerid, dialog_response[dialog_data], style, const caption[], const info[], const button1[], const button2[])
{
	#if defined PPDIALOGS_NO_COMPAT_NATIVE_CALL
		ShowPlayerDialogStr(playerid, PAWN_PLUS_DIALOG_ID, style, caption, info, button1, button2);
	#else
		pawn_call_native("ShowPlayerDialog", "dddssss", playerid, PAWN_PLUS_DIALOG_ID, style, caption, info, button1, button2);
	#endif

	if(PlayerDialogTasks[playerid])
	{
		task_delete(PlayerDialogTasks[playerid]);
	}
	new const Task:dialog_task = PlayerDialogTasks[playerid] = task_new();
	task_await(dialog_task);

	task_get_result_arr(dialog_task, dialog_response);
}

#pragma deprecated Use ShowPlayerAsyncDialogStr instead
stock AwaitAsyncDialogStr(playerid, dialog_response[dialog_data], style, ConstStringTag:caption, ConstStringTag:info, ConstStringTag:button1, ConstStringTag:button2)
{
	#if defined PPDIALOGS_NO_COMPAT_NATIVE_CALL
		ShowPlayerDialogStr(playerid, PAWN_PLUS_DIALOG_ID, style, caption, info, button1, button2);
	#else
		pawn_call_native("ShowPlayerDialog", "dddSSSS", playerid, PAWN_PLUS_DIALOG_ID, style, _:caption, _:info, _:button1, _:button2);
	#endif

	if(PlayerDialogTasks[playerid])
	{
		task_delete(PlayerDialogTasks[playerid]);
	}
	new const Task:dialog_task = PlayerDialogTasks[playerid] = task_new();
	task_await(dialog_task);

	task_get_result_arr(dialog_task, dialog_response);
}

stock Task:ShowPlayerAsyncDialog(playerid, style, const caption[], const info[], const button1[], const button2[])
{
	#if defined PPDIALOGS_NO_COMPAT_NATIVE_CALL
		ShowPlayerDialogStr(playerid, PAWN_PLUS_DIALOG_ID, style, caption, info, button1, button2);
	#else
		pawn_call_native("ShowPlayerDialog", "dddssss", playerid, PAWN_PLUS_DIALOG_ID, style, caption, info, button1, button2);
	#endif

	if(PlayerDialogTasks[playerid])
	{
		task_delete(PlayerDialogTasks[playerid]);
	}
	return PlayerDialogTasks[playerid] = task_new();
}

stock Task:ShowPlayerAsyncDialogStr(playerid, style, ConstStringTag:caption, ConstStringTag:info, ConstStringTag:button1, ConstStringTag:button2)
{
	#if defined PPDIALOGS_NO_COMPAT_NATIVE_CALL
		ShowPlayerDialogStr(playerid, PAWN_PLUS_DIALOG_ID, style, caption, info, button1, button2);
	#else
		pawn_call_native("ShowPlayerDialog", "dddSSSS", playerid, PAWN_PLUS_DIALOG_ID, style, _:caption, _:info, _:button1, _:button2);
	#endif

	if(PlayerDialogTasks[playerid])
	{
		task_delete(PlayerDialogTasks[playerid]);
	}
	return PlayerDialogTasks[playerid] = task_new();
}

forward AsyncDialogsHandler(playerid, dialogid, response, listitem, const inputtext[]);
public AsyncDialogsHandler(playerid, dialogid, response, listitem, const inputtext[])
{
	if(dialogid == PAWN_PLUS_DIALOG_ID)
	{
		new dialog_response[dialog_data];
		dialog_response[Response] = response;
		dialog_response[Listitem] = listitem;
		strcat(dialog_response[Inputtext], inputtext, 129);

		new const Task:task = PlayerDialogTasks[playerid];
		PlayerDialogTasks[playerid] = Task:0;
		task_set_result_arr(task, dialog_response);

		return 1;
	}
	return 0;
}