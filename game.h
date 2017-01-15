#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define MAX_KEY 5
#define MAX_TARGETS 56
#define PI 3.14159265

_CRT_SECURE_NO_WARNINGS;

typedef enum
{
	MENU=20,PLAYING,PAUSE,TIPS,GAMEOVER,NEW_RAUND,RAUND_ONGOING
}stage_t;

typedef enum
{
	ESCAPE,MOUSE_L_DOWN,MOUSE_L_UP,MOUSE_R_DOWN,MOUSE_R_UP
}keys_t;

typedef enum
{
	SQUARE=50,LEFT_TR,RIGHT_TR,LEFT_FLIPPED_TR,RIGHT_FLIPPED_TR,PLUS,HORIZ_DECR,VERT_DECR,BOUNCER,COIN
}target_id_t;

typedef struct
{
	int figure_id;
	int alive;
	int lives;
	int col_num;
	int row_num;
	double dx;
	double dy;
	int speed;
	int direction;
	int hit;
	ALLEGRO_COLOR color;
}target_t;

typedef struct
{
	ALLEGRO_BITMAP *bitmap;
	float dx;
	float dy;
	float animate_from_x;
	float animate_to_x;
	float animate_from_y;
	float animate_to_y;
	float speed;
	int direction;
}animaton_t;

typedef struct ball_s
{
	int alive;
	int shot;
	int last_hit_target_id;
	double dx;
	double dy;
	double rad;
	double speed;
	int direction_x;
	int direction_y;
	double angle;
	int hit;

	struct ball_s * rest;
}ball_t;

typedef struct
{
	double dx;
	double dy;
	int speed;
	int direction;
	double angle;
	int coins;

	double animation_last_time;
	double animation_angle;

	int balls;
	int balls_left_unshot;
	ball_t * ball_h;
}player_t;

typedef struct 
{	
	int alive;
	double dx;
	double dy;
	double size;
	int speed;
	int direction_x;
	int direction_y;
	double angle;
	ALLEGRO_COLOR color;
}debris_t;

typedef struct 
{
	int debris_num;
	debris_t debris[70];

	double start_x;
	double start_y;
}boom_t;

typedef struct 
{
	int exp_num;
	boom_t * boom;
}explosion_t;

typedef struct 
{
	double dx;
	double dy;
	double width;
	double height;
	int speed;
	int direction_x;

	ALLEGRO_BITMAP * image;	
}background_t;

typedef struct 
{
	int alive;
	double dx;
	double dy;
	double transparency;
}plus_one_unit_t;

typedef struct 
{
	int size;
	plus_one_unit_t  plus_one_unit[10];

}plus_one_anim_t;

typedef struct 
{
	int alive;
	double start_time;
	double dx;
	double dy;
	char type;
}decr_animation_t;

void initDecrAnimation(decr_animation_t * decr_animation);
void startDecrAnimation(decr_animation_t * decr_animation,double dx,double dy,char type);
void updateDecrAnimation(decr_animation_t * decr_animation);
void drawDecrAnimation(decr_animation_t * decr_animation,double active_game_win_lb,double active_game_win_rb,double active_game_win_tb,double active_game_win_db);

void initPlusOneAnim(plus_one_anim_t * plus_one_anim);
void startPlusOneAnim(plus_one_anim_t * plus_one_anim,double dx,double dy);
void updatePlusOneAnim(plus_one_anim_t * plus_one_anim);
void drawPlusOneAmim(plus_one_anim_t * plus_one_anim,const ALLEGRO_FONT *font);

void initBackground(background_t * back,double dx,double dy,int speed,int direction_x,const char * filename);
void updatBackground(background_t * back,double from);
void drawBackground(background_t * back,double window_w,double window_h,double from,double to);

void initExplosion(explosion_t * explosion);
void startExplosion(explosion_t * explosion,double start_x, double start_y);
void updateExplosion(explosion_t * explosion);
void showExplosions(explosion_t * explosion);
void sortArr(boom_t boom[],int from,int to);

double CheckDistance(double x1, double y1, double x2, double y2);

void initPlayer(player_t * player,double active_game_win_lb,double active_game_win_w,double active_game_win_db);
void updatePlayer(player_t * player,double mouse_x,double mouse_y,double new_dx,int balls_shot);
void drawPlayer(player_t * player,int draw_arrow,ALLEGRO_BITMAP * guy_img, ALLEGRO_BITMAP * arrow_img,double padding,ALLEGRO_FONT * font,double ball_rad,int balls_shot);


void initBalls(player_t * player,double rad);
int ballsInTheAir(ball_t *balls);
void startBalls(player_t *player,int balls_shot);
void deleteBalls(ball_t	* ball_h);
void updateBalls(player_t * player,target_t * target,int size,int *flag,double * first_floor_hit,double cell_w,
double target_rad,double space,double active_game_win_lb,double active_game_win_rb,double active_game_win_db,
float active_game_win_tb,double new_dx,ALLEGRO_SAMPLE *hit_sound,ALLEGRO_SAMPLE * coin_sound,explosion_t * explosion,plus_one_anim_t * plus_one_anim,ALLEGRO_SAMPLE * plus_sound,ALLEGRO_SAMPLE * bounce_sound,
decr_animation_t * decr_animation,ALLEGRO_SAMPLE * laser_sound,double mouse_x,double mouse_y);
void drawBalls(ball_t *ball_h);


void initTargets(target_t *arr,int size,int raund,float cell_w,float active_game_win_lb,
	float active_game_win_tb);
int updateTargets(target_t *arr,int size,float active_game_win_db,float cell_w,float target_r);
void drawTargets(target_t *arr,int size,float cell_w,float target_r,const ALLEGRO_FONT *font1,const ALLEGRO_FONT *font2,const ALLEGRO_FONT *font3,
	ALLEGRO_BITMAP * plus_img,ALLEGRO_BITMAP * horiz_decr_img,ALLEGRO_BITMAP * vert_decr_img,ALLEGRO_BITMAP *bouncer_img,
	ALLEGRO_BITMAP * coin_img,float space);

void drawExtendedBackground(float window_lb,float window_rb,float image_w,
	float sx,float sy,float sw,float sh,float dx,float dy,int flag,ALLEGRO_BITMAP *bitmap);

void changeGameStage(int new_state,int * old_state);

int getMyCoins(const char * filename);
void saveMyCoins(const char * filename,int *coins);
int getMyTop(const char * filename);
void saveMyTop(const char * filename,int *top_score);


ALLEGRO_COLOR changeColor(ALLEGRO_COLOR old_color,int lives);

void force_balls_2_start_pos(ball_t * ball_h,double new_dx);

#endif
