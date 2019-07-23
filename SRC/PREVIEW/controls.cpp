#include "ZTM_DEF.h"

extern int DisplayTEST;
bool NoClip=0;
sKeyState m_keys[256], m_mouse[5];

sKeyState GetKey(int nKeyID){ return m_keys[nKeyID]; }


short   m_keyNewState[256]    = { 0 };
short   m_keyOldState[256]    = { 0 };
POINT p                     = { 0 };
vector3 m_pos                 = { 0 };

void resetKeyPresses(player_t * player)
{
    if (player->PLAYER_ID >= 4) return;

    for (int i = 0; i < 256; i++)
    {
        m_keyNewState[i] = GetAsyncKeyState(i);
		m_keys[i].bPressed = false;	m_keys[i].bReleased = false;

		if (m_keyNewState[i] != m_keyOldState[i])
		{
            if (m_keyNewState[i] & 0x8000)
			{
                m_keys[i].bPressed = !m_keys[i].bHeld;
				m_keys[i].bHeld = true;
            }
			else
			{
                m_keys[i].bReleased = true;
				m_keys[i].bHeld = false;
            }
        }
        m_keyOldState[i] = m_keyNewState[i];
    }

}


#define SPEED_INCREASE (3.5f*ZT_FRAMERATE)
#define FRICTION        (1.85f)
#define SPEED_VALUE     (100.0f)
#define TURN_INCREASE   (1.5f * ZT_FRAMERATE)
#define LOOK_INCREASE   (1.5f * ZT_FRAMERATE)
/**Movement**/
void move_front(player_t * currentPlayer){   if ((currentPlayer->MOVEMENT_SPEED+=SPEED_INCREASE)>SPEED_VALUE)       currentPlayer->MOVEMENT_SPEED=SPEED_VALUE;}
void move_back(player_t * currentPlayer){    if ((currentPlayer->MOVEMENT_SPEED-=SPEED_INCREASE)<-SPEED_VALUE)        currentPlayer->MOVEMENT_SPEED=-SPEED_VALUE;}
void move_left(player_t * currentPlayer){    if ((currentPlayer->LATERAL_SPEED -= SPEED_INCREASE) < -SPEED_VALUE)    currentPlayer->LATERAL_SPEED = -SPEED_VALUE;}
void move_right(player_t * currentPlayer){   if ((currentPlayer->LATERAL_SPEED += SPEED_INCREASE) > SPEED_VALUE)     currentPlayer->LATERAL_SPEED = SPEED_VALUE;}

void move_friction(player_t * currentPlayer)
{
    /*if (fabs(currentPlayer->MOVEMENT_SPEED)<=FRICTION) currentPlayer->MOVEMENT_SPEED=0;
    else    currentPlayer->MOVEMENT_SPEED *= FRICTION;*/
    if (currentPlayer->MOVEMENT_SPEED > SPEED_INCREASE) currentPlayer->MOVEMENT_SPEED-=FRICTION*ZT_FRAMERATE;
    else if (currentPlayer->MOVEMENT_SPEED < -SPEED_INCREASE) currentPlayer->MOVEMENT_SPEED+=FRICTION*ZT_FRAMERATE;
    else currentPlayer->MOVEMENT_SPEED = 0;
}


/**For the Lookup/down values : DEGtoANG(45) = 45/360 * 65536.0**/
void look_up(player_t * currentPlayer)   {    currentPlayer->ROTATION[X]-=LOOK_INCREASE;  }  //if (currentPlayer->ROTATION[Z] > 45.0)  currentPlayer->ROTATION[Z] = 45.0;}
void look_down(player_t * currentPlayer) {    currentPlayer->ROTATION[X] += LOOK_INCREASE; }   //if (currentPlayer->ROTATION[Z] < -45.0) currentPlayer->ROTATION[Z] = -45.0;}

/**Turn left/right - SATURN ONLY, DIGITAL PAD ONLY**/
void turn_right(player_t * currentPlayer){    currentPlayer->ROTATION[Z] -= TURN_INCREASE;}
void turn_left(player_t * currentPlayer){    currentPlayer->ROTATION[Z] += TURN_INCREASE;}

/**Fire current player's weapon**/
void fire_weapon(player_t * currentPlayer)
{ if (currentPlayer->IS_FIRING==0)
    {
        currentPlayer->IS_FIRING=32;
    }
}//Should be set depending on the weapons' value. Kind of like a timer to allow you to fire again, the firerate...
/**While using digital control pad***/
/*void digital_control(player_t * currentPlayer)
{
    //PerDigital *dig_pre=(PerDigital*)Smpc_Peripheral;

    if (KEY_DOWN(currentPlayer->PLAYER_ID, JO_KEY_START))        {RESET(currentPlayer); return;}
    //Look up/down
    if (KEY_DOWN(currentPlayer->PLAYER_ID, JO_KEY_X))
        {if (currentPlayer->TIMERS.KEY_TIMER > 1) {currentPlayer->TIMERS.KEY_TIMER=-1; currentPlayer->TIMERS.RESET_VIEW=true;}}
    else if (KEY_PRESS(currentPlayer->PLAYER_ID, JO_KEY_X))  //If you keep X pressed, you can look around
    {
        if (currentPlayer->TIMERS.KEY_TIMER > 60) {currentPlayer->TIMERS.KEY_TIMER= 60;} //Can be improved. The idea is just to reset the view if you press the button long enough, but press X again quickly enough (it avoids resetting the view when you just want to ajust it. So you kind of double press X...). Maybe it's not even necessary?
        else currentPlayer->TIMERS.KEY_TIMER+=ZT_SET_FRAMERATE;
        if      (KEY_PRESS(currentPlayer->PLAYER_ID, JO_KEY_UP))         look_up(currentPlayer);
        else if (KEY_PRESS(currentPlayer->PLAYER_ID, JO_KEY_DOWN))       look_down(currentPlayer);
        move_friction(currentPlayer);
    }
    //Move forward/backward
    else
    {
        currentPlayer->TIMERS.KEY_TIMER-=ZT_SET_FRAMERATE;  if (currentPlayer->TIMERS.KEY_TIMER<-1) currentPlayer->TIMERS.KEY_TIMER=-1;
        if (KEY_PRESS(currentPlayer->PLAYER_ID, JO_KEY_UP)) move_front(currentPlayer);
        else if (KEY_PRESS(currentPlayer->PLAYER_ID, JO_KEY_DOWN)) move_back(currentPlayer);
        else move_friction(currentPlayer);
    }


    //Turn left/right
    if (KEY_PRESS(currentPlayer->PLAYER_ID, JO_KEY_RIGHT)) turn_right(currentPlayer);
    else if (KEY_PRESS(currentPlayer->PLAYER_ID, JO_KEY_LEFT)) turn_left(currentPlayer);



    //Switch weapons
    if (KEY_DOWN(currentPlayer->PLAYER_ID, JO_KEY_Y))            switch_weapons(currentPlayer, 0);
    else if (KEY_DOWN(currentPlayer->PLAYER_ID, JO_KEY_Z))       switch_weapons(currentPlayer, 1);


    //L/R shoulder buttons
    if (KEY_PRESS(currentPlayer->PLAYER_ID, JO_KEY_L) && KEY_PRESS(currentPlayer->PLAYER_ID, JO_KEY_R))    {}            //Could be used for a modifier for another action (like melee attack) or for a debug menu. It also avoids straffing one direction when you press both L and R
    else if (KEY_PRESS(currentPlayer->PLAYER_ID, JO_KEY_L))     move_left(currentPlayer);
    else if (KEY_PRESS(currentPlayer->PLAYER_ID, JO_KEY_R))     move_right(currentPlayer);
    else if (currentPlayer->LATERAL_SPEED > SPEED_INCREASE) currentPlayer->LATERAL_SPEED -=SPEED_INCREASE;
    else if (currentPlayer->LATERAL_SPEED < -SPEED_INCREASE) currentPlayer->LATERAL_SPEED += SPEED_INCREASE;
    else currentPlayer->LATERAL_SPEED = 0;

    //Jump
    if (KEY_DOWN(currentPlayer->PLAYER_ID, JO_KEY_B)) jump(currentPlayer);

    //Fire weapon
    if (KEY_PRESS(currentPlayer->PLAYER_ID, JO_KEY_A)) fire_weapon(currentPlayer);

    if (currentPlayer->TIMERS.RESET_VIEW==true)
    {
        if (currentPlayer->ROTATION[Z] >= 910) currentPlayer->ROTATION[Z] -= 910;
        else if (currentPlayer->ROTATION[Z] <= -910) currentPlayer->ROTATION[Z] += 910;
        else  {currentPlayer->ROTATION[Z]=0;currentPlayer->TIMERS.RESET_VIEW=false;}
    }
}*/

#define ANGtoFLOAT(d) ((float)((d) * 16384.0f ))
extern tr4_level_load level;
void resetPlayer(player_t * player)
{
    player->POSITION.x=player->POSITION.y=player->POSITION.z=
    player->ROTATION[Y]=player->ROTATION[X]=player->ROTATION[Z]=0;
    for (unsigned int i=0; i<level.items_count; i++)
    {
        if (level.items[i].object_id == 0)
        {
            player->POSITION.x = level.items[i].pos.x *scaleF;
            player->POSITION.y = level.items[i].pos.y *scaleF - 512*scaleF ;
            player->POSITION.z = level.items[i].pos.z *scaleF;
            player->ROTATION[Z] = ((float)level.items[i].rotation)/32768.0*360.0;
            player->roomId = level.items[i].room;

        }
    }

//player->ROTATION[Y]=0.5f;


    player->IS_FIRING=0;
    player->LATERAL_SPEED=0;
    player->MOVEMENT_SPEED=0;

}
void debugKeys()
{
    if (m_keys['X'].bHeld || m_keys['V'].bPressed)
    {if (++DisplayTEST > 1) DisplayTEST=0;}
    if (m_keys['B'].bPressed)
    {if (--DisplayTEST < 0) DisplayTEST=1;}

    if (m_keys['R'].bPressed)
        resetPlayer(player);

    if (m_keys['F'].bPressed)
    {
        ZT_FRAMERATE++;
        if (ZT_FRAMERATE>4) ZT_FRAMERATE=1;
    }


    if (m_keys['C'].bPressed)
        NoClip^=1;

    extern bool SaturnTest;
    if (m_keys['T'].bPressed)
        SaturnTest^=1;


}

void mouse_controls(player_t * player)
{
    return; ///Disabled for now
    ShowCursor(false);
    if (GetCursorPos(&p))
    {
        p.x = p.x - 640 * 0.5f;
        p.y = p.y - 480 * 0.5f;
    }
    player->ROTATION[Z] -= p.x * 0.0625f;
    player->ROTATION[X] += p.y * 0.0625f;
    SetCursorPos(640*0.5f,480*0.5f);
}


int keyboard_controls(player_t * player)
{
    if (m_keys[KB_ESC].bHeld || m_keys['O'].bHeld  ) return -1;
    /**REMOVED FOR DEBUGGING**/

    if (m_keys['D'].bHeld)
        move_right(player);
    else if (m_keys['A'].bHeld)
        move_left(player);
    else if (player->LATERAL_SPEED > SPEED_INCREASE) player->LATERAL_SPEED-=SPEED_INCREASE;
    else if (player->LATERAL_SPEED < -SPEED_INCREASE) player->LATERAL_SPEED+=SPEED_INCREASE;
    else player->LATERAL_SPEED=0;

    if (fabs(player->LATERAL_SPEED)< FRICTION) player->LATERAL_SPEED=0;

    if (m_keys['W'].bHeld)    {
        move_front(player);
    }
    else if (m_keys['S'].bHeld){
        move_back(player);
    }
    else move_friction(player);

    if (m_keys[VK_LEFT].bHeld)    {
        turn_left(player);
    }
    else if (m_keys[VK_RIGHT].bHeld){
        turn_right(player);
    }

    if (m_keys[VK_UP].bHeld) look_up(player);
    else if (m_keys[VK_DOWN].bHeld) look_down(player);

/**For debugging, no collision**/
    if (m_keys['Q'].bHeld || m_keys[VK_SPACE].bHeld)    {
        player->POSITION.y-=64*ZT_FRAMERATE;//player->SPEED[Y]-=1.5f*ZT_FRAMERATE;
    }
    else if (m_keys['Z'].bHeld){
        player->POSITION.y+=64*ZT_FRAMERATE;
    }




    if (fabs(player->SPEED[Y]) < 1.0f/32.0f) player->SPEED[Y]=0;

    debugKeys();

    return 0;
}



bool collisionCheck(player_t * player);
/**Update player's data**/
void update(player_t * currentPlayer)
{
    currentPlayer->ROTATION[Y] = currentPlayer->ROTATION[Z];

    currentPlayer->SPEED[Z] = AslMulFX(std::cos(currentPlayer->ROTATION[Z]*conv), (currentPlayer->MOVEMENT_SPEED)) +
        AslMulFX(std::sin(currentPlayer->ROTATION[Z]*conv), (currentPlayer->LATERAL_SPEED));

    currentPlayer->SPEED[X] = -AslMulFX(std::sin(currentPlayer->ROTATION[Z]*conv), (currentPlayer->MOVEMENT_SPEED)) +
        AslMulFX(std::cos(currentPlayer->ROTATION[Z]*conv), (currentPlayer->LATERAL_SPEED));

    if (player->SPEED[Y] > 16.0f) player->SPEED[Y] = 16.0f;
    player->NEXT_POSITION.x = player->POSITION.x + player->SPEED[X] * ZT_FRAMERATE;
    player->NEXT_POSITION.y = player->POSITION.y + player->SPEED[Y] * ZT_FRAMERATE;
    player->NEXT_POSITION.z = player->POSITION.z + player->SPEED[Z] * ZT_FRAMERATE;

    if (!NoClip)
        collisionCheck(player);
    //Disabled for debug
    //player->STATUS &= ~CAN_JUMP;
}
/**Temporary functions...**/
int myControls(player_t * player)
{


   /* if (is_input_available(currentPlayer->PLAYER_ID) == false) return;

    if(Smpc_Peripheral[currentPlayer->PLAYER_ID].id == 0x16)
        {analog_control(currentPlayer);}
	else
        {digital_control(currentPlayer);}

    update(currentPlayer);*/


    if (!NoClip) {player->SPEED[Y] += 3.0f * ZT_FRAMERATE;}
    else player->SPEED[Y]=0;

    mouse_controls(player);
    resetKeyPresses(player);
    if (keyboard_controls(player)==-1) return -1;

    update(player);

    cout << "Player Position : " << (int)player->POSITION.x << ", " << (int)player->POSITION.y << ", " << (int)player->POSITION.z << "\n";
    return 0;
}
