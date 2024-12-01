ptask Mowler[1000](playerid)
{
    new actorid = Actor_Nearest(playerid, 3.5);
    if(Actor_Type(actorid, "mowler-job"))
    {
        new Float:anglePlayer;
        GetPlayerFacingAngle(playerid, anglePlayer);

        new Float:diff = abs(anglePlayer - ActorData[actorid][actorPos][3]);
        if(diff > 180.0) diff = 360.0 - diff;

        if(diff <= 45.0)
        {
            SendDebugMessage("anda menghadap nldengan actor %.1f", diff);
        }
    }
}