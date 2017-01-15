#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "game.h"


int main(void)
{
	/*---------------------------------*/
	/*ALLEGRO VARIABLES ---------------*/
	/*---------------------------------*/
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_DISPLAY_MODE disp_data;
	ALLEGRO_EVENT_QUEUE *event_queue=NULL;
	ALLEGRO_EVENT event;
	ALLEGRO_TIMER *timer=NULL;
	ALLEGRO_BITMAP * game_window;
	ALLEGRO_FONT *font18=NULL;
	ALLEGRO_FONT *font65=NULL;
	ALLEGRO_BITMAP *menu_image1;
	ALLEGRO_BITMAP *menu_image2;
	ALLEGRO_BITMAP * bbtan_text;
	ALLEGRO_BITMAP *button1;
	ALLEGRO_BITMAP * button2;
	ALLEGRO_BITMAP * small_filler;
	ALLEGRO_BITMAP * big_filler;
	ALLEGRO_BITMAP * top_layer;
	ALLEGRO_BITMAP * pause_image;
	ALLEGRO_BITMAP * question_image;
	ALLEGRO_MOUSE_STATE mouse_state;
	ALLEGRO_FONT *font35;
	ALLEGRO_BITMAP * plus_img;
	ALLEGRO_BITMAP * horiz_decr_img;
	ALLEGRO_BITMAP * vert_decr_img;
	ALLEGRO_BITMAP *bouncer_img;
	ALLEGRO_BITMAP * coin_img;
	ALLEGRO_BITMAP * guy_in_game_img;
	ALLEGRO_BITMAP * arrow_img;
	ALLEGRO_FONT *font23;
	ALLEGRO_FONT *font27;
	ALLEGRO_SAMPLE *hit_sound = NULL;
	ALLEGRO_SAMPLE *start_sound = NULL;
	ALLEGRO_SAMPLE *coin_sound = NULL;
	ALLEGRO_SAMPLE *plus_sound = NULL;
	ALLEGRO_SAMPLE *bounce_sound = NULL;
	ALLEGRO_SAMPLE * laser_sound=NULL;
	ALLEGRO_SAMPLE * lose_sound=NULL;

	/*---------------------------------*/

	/*---------------------------------*/
	/*GAME VARIABLES-------------------*/
	/*---------------------------------*/
	target_t target[MAX_TARGETS];/*ARRAY OF TARGETS*/
	player_t player;

	explosion_t explosion = {0,NULL};
	plus_one_anim_t plus_one_anim;
	background_t back;
	decr_animation_t decr_animation;

	int i;
	int draw_arr;
	int balls_in_the_air;
	int balls_shot;
	int space;
	double ball_rad;
	int flag;
	double first_floor_hit;

	float target_w;
	float target_r;
	float padding;
	float flag_w;

	int raund_num;
	int top_score;

	int last_time;
	int min_left;
	int sec_left;
	int r,g,b;

	int key[MAX_KEY]={0,0,0,0,0};
	int game_stage;
	int game_substage;
	int finished=0;
	int render;
	int game_time;
	int frames=0;
	int game_FPS;

	animaton_t menu_ball;
	animaton_t guy_image;
	animaton_t what_happ_text;

	double mouse_x;
	double mouse_y;

	/*CURRENT DISPLAY WIDTH AND HEIGHT-*/
	float display_w;
	float display_h;
	/*GAME WINDOW*/
	float game_win_w;
	float game_win_lb; /*left border*/
	float game_win_rb; /*right border*/
	float active_game_win_w;
	float active_game_win_h;
	float active_game_win_lb;
	float active_game_win_rb;
	float active_game_win_tb;
	float active_game_win_db;

	/*---------------------------------*/

	if(!al_init())
	{
		fprintf(stderr,"%s\n","al_init() faild,exiting from main with -1 status");
		return -1;
	}

	 /*ADDON INSTALL*/
   	al_install_keyboard();
   	al_install_mouse();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();



	al_get_display_mode(al_get_num_display_modes()-1,&disp_data);
	

	/*SET CURRENT DISPLAY WIDTH AND HEIGHT VARIABLES*/
	display_w=disp_data.width;
	display_h=disp_data.height;

	padding=display_h/100*9;/*TOP PADDING <<<<*/
	active_game_win_h=display_h-padding*3;
	target_w=active_game_win_h/9; /*TARGET WIDTH*/
	active_game_win_w=target_w*7;
	game_win_w=active_game_win_w + 40 *2;
	game_win_lb=(display_w-game_win_w)/2;
	game_win_rb=display_w-game_win_lb;
	active_game_win_lb=game_win_lb+40;
	active_game_win_rb=game_win_rb-40;
	active_game_win_tb=padding;
	active_game_win_db=display_h-padding*2;
	active_game_win_h=active_game_win_db- active_game_win_tb;
	active_game_win_w=active_game_win_rb-active_game_win_lb;\

	target_r=26; /*TARGET RADIOUS*/
	space=7;
	ball_rad=14;

	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
   	if(!(display = al_create_display(display_w, display_h)))
   	{
   		fprintf(stderr, "%s\n","al_create_display failed,exiting from main with -1 status");
   		return -1;
   	}

   	/*TIME AND EVENT_QUEUE INIT */
   	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);


	/*FICING THE PATH ISSUE (COULDN'T FIND AND OPEN PNG FILES)*/
	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(path, "resources");
	al_change_directory(al_path_cstr(path, '/'));  // change the working directory
	al_destroy_path(path);

	{
		ALLEGRO_BITMAP* bmp1 = al_load_bitmap("foo.png");
		if (bmp1 == NULL)
		{
			printf("Yes\n");
		}
	}

	/*FONT INIT*/
	font18 = al_load_font("Chunkfive.otf", 18, 0);
	font65 =al_load_font("Chunkfive.otf", 65, 0);
	font35=al_load_font("Chunkfive.otf", 35, 0);
	font23=al_load_font("Chunkfive.otf", 23, 0);
	font27=al_load_font("Chunkfive.otf", 27, 0);

	/*SOUNDS INIT*/
	al_reserve_samples(100);
	hit_sound = al_load_sample("ball_hit.wav");
	start_sound=al_load_sample("start.wav");
	coin_sound=al_load_sample("coin_sound.wav");
	plus_sound=al_load_sample("plus_sound.wav");
	bounce_sound=al_load_sample("bounce_sound.wav");
	laser_sound=al_load_sample("laser_sound.wav");
	lose_sound=al_load_sample("lose_sound.wav");

	/*EVENTS SOURCE REFISTER*/
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue,al_get_mouse_event_source());
	al_register_event_source(event_queue,al_get_display_event_source(display));


	/*CREATING BITMAPS*/
	game_window=al_create_bitmap(game_win_w,display_h);
  	al_set_target_bitmap(game_window);
  	al_clear_to_color(al_map_rgb(0,0,0));
  	al_set_target_bitmap(al_get_backbuffer(display));

	menu_image2 = al_load_bitmap("menu_image2.png");
	al_convert_mask_to_alpha(menu_image2, al_map_rgb(0, 0, 0));

  	menu_image1=al_load_bitmap("menu_image1.png");
  	al_convert_mask_to_alpha(menu_image1,al_map_rgb(0,0,0));

  	guy_image.bitmap=al_load_bitmap("Guy whit mustache.png");
  	al_convert_mask_to_alpha(guy_image.bitmap,al_map_rgb(35, 24, 21));

  	bbtan_text=al_load_bitmap("bbtan_image.png");
  	al_convert_mask_to_alpha(bbtan_text,al_map_rgb(0, 0, 0));

  	button1=al_load_bitmap("play_button.png");
  	al_convert_mask_to_alpha(button1,al_map_rgb(0, 0, 0));

  	button2=al_load_bitmap("change_ball_button.png");
  	al_convert_mask_to_alpha(button2,al_map_rgb(0, 0, 0));

  	what_happ_text.bitmap=al_load_bitmap("what happents after 30 min.png");
  	al_convert_mask_to_alpha(what_happ_text.bitmap,al_map_rgb(0,0,0));

  	coin_img=al_load_bitmap("coin_img.png");
  	al_convert_mask_to_alpha(coin_img,al_map_rgb(0,0,0));

  	pause_image=al_load_bitmap("pause_image.png");
  	al_convert_mask_to_alpha(pause_image,al_map_rgb(26,26,26));

  	question_image=al_load_bitmap("question_mark.jpg");
  	al_convert_mask_to_alpha(question_image,al_map_rgb(0,0,0));

  	top_layer=al_create_bitmap(active_game_win_w,display_h/100*9);
  	al_set_target_bitmap(top_layer);
  	al_clear_to_color(al_map_rgba_f(0,0,0,0.6f));
  	al_set_target_bitmap(al_get_backbuffer(display));

  	small_filler=al_create_bitmap(active_game_win_lb-game_win_lb,display_h);
  	al_set_target_bitmap(small_filler);
  	al_clear_to_color(al_map_rgb(0,0,0));
  	al_set_target_bitmap(al_get_backbuffer(display));

  	big_filler=al_create_bitmap(game_win_lb,display_h);
	al_set_target_bitmap(big_filler);
  	al_clear_to_color(al_map_rgb(0,0,0));
  	al_set_target_bitmap(al_get_backbuffer(display));

  	plus_img=al_load_bitmap("plus_img.png");
  	al_convert_mask_to_alpha(plus_img,al_map_rgb(0,0,0));

	horiz_decr_img=al_load_bitmap("horiz_decr.png");
	al_convert_mask_to_alpha(horiz_decr_img,al_map_rgb(0,0,0));

	vert_decr_img=al_load_bitmap("vert_decr.png");
	al_convert_mask_to_alpha(vert_decr_img,al_map_rgb(0,0,0));

	bouncer_img=al_load_bitmap("bouncer_img.png");
	al_convert_mask_to_alpha(bouncer_img,al_map_rgb(0,0,0));

	guy_in_game_img=al_load_bitmap("guy_in_game_img.png");
	al_convert_mask_to_alpha(guy_in_game_img,al_map_rgb(255,0,0));

	arrow_img=al_load_bitmap("arrow_img.png");
	al_convert_mask_to_alpha(arrow_img,al_map_rgb(0,0,0));

	initPlusOneAnim(&plus_one_anim);

  	/*MENU GUY ANIMATION INIT*/
  	guy_image.dx=(display_w/2)-(al_get_bitmap_width(bbtan_text)/2);
  	guy_image.dy=display_h/100*12;
  	guy_image.speed=0.5;
  	guy_image.direction = 1;
  	guy_image.animate_from_x=display_w/2-al_get_bitmap_width(bbtan_text)/2;
  	guy_image.animate_to_x=(display_w/2)-(al_get_bitmap_width(bbtan_text)/2)+(active_game_win_w/100*10);

  	/*MENU BALL INIT*/
  	menu_ball.dx=active_game_win_lb+(active_game_win_w/100*53)+(al_get_bitmap_width(button1)/100*80);
  	menu_ball.dy=display_h/100*62;
  	menu_ball.speed=3;
  	menu_ball.direction=-1;
  	menu_ball.animate_from_y=display_h/100*62;
  	menu_ball.animate_to_y=display_h/100*62-40;


  	/*WHAT HAPP TEXT INIT*/
  	what_happ_text.dx=active_game_win_lb;
  	what_happ_text.dy=display_h-padding+padding/2-al_get_bitmap_height(what_happ_text.bitmap)/2;
  	what_happ_text.speed=2;
  	what_happ_text.direction=1;
  	what_happ_text.animate_from_x=active_game_win_lb-al_get_bitmap_width(what_happ_text.bitmap);
  	what_happ_text.animate_to_x=active_game_win_rb;

  	initBackground(&back,active_game_win_lb,padding,1,-1,"starBG.png");
    initDecrAnimation(& decr_animation);

	al_start_timer(timer);
	game_time=al_current_time();
	game_stage=MENU;
	srand(time(NULL));
	player.coins=getMyCoins("coins_dat");
	top_score=getMyTop("top_score_dat");

	for(i=0;i<MAX_TARGETS;++i)
	{
		target[i].alive=0;
	}

	al_play_sample(start_sound,1,0,1,ALLEGRO_PLAYMODE_ONCE, 0);

	/*GAME LOOP*/
	while(!finished)
	{
		al_wait_for_event(event_queue, &event);

		/*------------------*/
		/* GAME EVENTS    	*/
		/*------------------*/
		if(event.type==ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(event.keyboard.keycode)
			{
				case ALLEGRO_KEY_ESCAPE:
				{
					key[ESCAPE]=1;
				}
				break;

				case ALLEGRO_KEY_SPACE:
				{

				}
				break;

				default:break;
			}
		}
		else if(event.type==ALLEGRO_EVENT_MOUSE_AXES)
		{
			mouse_x=event.mouse.x;
			mouse_y=event.mouse.y;
		}
		else if (event.type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if(event.mouse.button &1)
			{
				key[MOUSE_L_DOWN]=1;
				key[MOUSE_L_UP]=0;

			}
			else if(event.mouse.button & 2)
			{
				key[MOUSE_R_DOWN]=1;
				key[MOUSE_R_UP]=0;
			}
		}
		else if(event.type ==ALLEGRO_EVENT_MOUSE_BUTTON_UP )
		{
			if(event.mouse.button &1)
			{
				key[MOUSE_L_UP]=1;
				key[MOUSE_L_DOWN]=0;
			}
			else if(event.mouse.button & 2)
			{
				key[MOUSE_R_UP]=1;
				key[MOUSE_R_DOWN]=0;
			}
		}
		else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			finished=1;
		}

		/*-----------------*/
		/*  GAME UPDATE    */
		/*-----------------*/
        if(event.type == ALLEGRO_EVENT_TIMER)
		{
			render = 1;

			/*UPDATE FPS-----------------------*/
			frames++;
			if(al_current_time() - game_time >= 1)
			{
				game_time = al_current_time();
				game_FPS = frames;
				frames = 0;
			}
			/*---------------------------------*/

			updatBackground(&back,active_game_win_lb);

			/*UPDATE ACCORDING TO THE CURRENT GAME STAGE*/
			switch(game_stage)
			{
				case MENU:
				{
					if(guy_image.dx >= guy_image.animate_to_x)
					{
						guy_image.direction=-1;
					}
					else if(guy_image.dx <= guy_image.animate_from_x)
					{
						guy_image.direction=1;
					}

					guy_image.dx=guy_image.dx+guy_image.speed*guy_image.direction;

					if(menu_ball.dy >= menu_ball.animate_from_y)
					{
						menu_ball.direction=-1;
					}
					else if(menu_ball.dy <= menu_ball.animate_to_y)
					{
						menu_ball.direction=1;
					}

					menu_ball.dy=menu_ball.dy+menu_ball.speed*menu_ball.direction;

					if(what_happ_text.dx >=what_happ_text.animate_to_x)
					{
						what_happ_text.direction=-1;
					}
					else if(what_happ_text.dx <=what_happ_text.animate_from_x)
					{
						what_happ_text.direction=1;
					}

					what_happ_text.dx=what_happ_text.dx+what_happ_text.speed*what_happ_text.direction;

					if(key[ESCAPE])
					{
						finished=1;
						key[ESCAPE]=0;
					}

					if( mouse_x >=active_game_win_lb+(active_game_win_w/100*53) && mouse_x <= active_game_win_lb+(active_game_win_w/100*53) + al_get_bitmap_width(button1) &&
						mouse_y >= display_h/100*62 && mouse_y <= display_h/100*62+al_get_bitmap_height(button1))
					{
						al_get_mouse_state(& mouse_state);
						if (mouse_state.buttons & 1)
						{
						    changeGameStage(PLAYING,&game_stage);
						    changeGameStage(NEW_RAUND,&game_substage);
						    last_time=al_current_time();
							min_left=30;
							sec_left=0;
							flag_w=target_w;
							raund_num=1;

							for(i=0;i<MAX_TARGETS;i++)
							{
								target[i].alive=0;
							}

							initPlayer(&player, active_game_win_lb, active_game_win_w, active_game_win_db);

							key[MOUSE_L_UP]=0;

							first_floor_hit=active_game_win_lb+active_game_win_w/2;

							balls_shot=-1; /* initialisation*/

							initExplosion(&explosion);
						}
					}
				}
				break;

				case PLAYING:
				{
					updateDecrAnimation(& decr_animation);

					updatePlusOneAnim(&plus_one_anim);

					updateExplosion(&explosion);

					draw_arr=0;

					updatePlayer( &player, mouse_x, mouse_y,first_floor_hit,balls_shot);

					startBalls(&player,balls_shot);

					updateBalls(&player,target,MAX_TARGETS,&flag,&first_floor_hit,target_w,
								target_r, space, active_game_win_lb, active_game_win_rb, active_game_win_db,active_game_win_tb,first_floor_hit,hit_sound,coin_sound,
								&explosion,&plus_one_anim,plus_sound,bounce_sound,&decr_animation,laser_sound,mouse_x,mouse_y);


					if(key[ESCAPE])
					{
						changeGameStage(MENU,&game_stage);
						key[ESCAPE]=0;
						if(player.balls > top_score)
						{
							top_score=player.balls;
						}
						al_play_sample(start_sound,1,0,1,ALLEGRO_PLAYMODE_ONCE, 0);
					}

					/*GAME TIMER*/
					if(al_current_time()-last_time >= 1)
					{
						if(sec_left==0)
						{
							sec_left=60;
							--min_left;
						}
						--sec_left;
						last_time=al_current_time();
						r=rand()%255+1;
						g=rand()%255+1;
						b=rand()%255+1;
					}

					switch(game_substage)
					{
						case NEW_RAUND:
						{

							if(flag_w==target_w)
							{
								initTargets(target,MAX_TARGETS,raund_num,target_w,active_game_win_lb, active_game_win_tb);
							}

							flag_w-=2;
							if(updateTargets(target,MAX_TARGETS,active_game_win_db,target_w,target_r))
							{
								al_play_sample(lose_sound,1,0,1,ALLEGRO_PLAYMODE_ONCE, 0);

								changeGameStage(GAMEOVER,&game_stage);
								if(player.balls > top_score)
								{
									top_score=player.balls;
								}
							}

							if(flag_w <= 0)
							{
								changeGameStage(RAUND_ONGOING,&game_substage);
								++raund_num;
								balls_shot=0;
								key[MOUSE_L_UP]=0;
								initBalls(&player,ball_rad);
								force_balls_2_start_pos(player.ball_h,first_floor_hit);
								flag=0;
								player.balls_left_unshot=player.balls;
							}
						}break;

						case RAUND_ONGOING:
						{

							balls_in_the_air=ballsInTheAir(player.ball_h);

							if(key[MOUSE_L_DOWN] && !balls_shot && !balls_in_the_air)
							{
								draw_arr=1;
							}
							else if(key[MOUSE_L_UP] && !balls_shot && !balls_in_the_air)
							{
								draw_arr=0;
								startBalls(&player,balls_shot);
								balls_shot=1;
								balls_in_the_air=1;
								key[MOUSE_L_UP]=0;
								player.animation_last_time=al_current_time();
							}

							if(!balls_in_the_air && balls_shot==1)
							{
								changeGameStage(NEW_RAUND,&game_substage);
								flag_w=target_w;
							}

						}break;

						default:break;
					}

				}
				break;

				case PAUSE:
				{
					
				}
				break;

				case TIPS:
				{
					
				}
				break;

				case GAMEOVER:
				{

					if( mouse_x >=active_game_win_lb+(active_game_win_w/2) && mouse_x <= active_game_win_lb+(active_game_win_w/2) + al_get_bitmap_width(button1) &&
					mouse_y >= active_game_win_tb+active_game_win_h/2 && mouse_y <= active_game_win_tb+active_game_win_h/2+al_get_bitmap_height(button1))
					{
						al_get_mouse_state(& mouse_state);
						if (mouse_state.buttons & 1)
						{
						    changeGameStage(PLAYING,&game_stage);
						    changeGameStage(NEW_RAUND,&game_substage);
						    last_time=al_current_time();
							min_left=30;
							sec_left=0;
							flag_w=target_w;
							raund_num=1;

							for(i=0;i<MAX_TARGETS;i++)
							{
								target[i].alive=0;
							}

							initPlayer(&player, active_game_win_lb, active_game_win_w, active_game_win_db);

							key[MOUSE_L_UP]=0;

							first_floor_hit=active_game_win_lb+active_game_win_w/2;

							balls_shot=-1; /* initialisation*/

							initExplosion(&explosion);
						}
					}

					if(key[ESCAPE])
					{
						finished=1;
						key[ESCAPE]=0;
					}
				}
				break;

				default:break;
			}
		}

		/*===========*/
		/*  RENDER   */
		/*===========*/
		if(render && al_is_event_queue_empty(event_queue))
		{
			render = 0;

			al_draw_bitmap(game_window,game_win_lb,0,0);

			/*drawBackground(&back,active_game_win_w,active_game_win_h,active_game_win_lb,active_game_win_rb);*/

			/*BEGIN PROJECT RENDER*/
			switch(game_stage)
			{
				case MENU:
				{
					drawExtendedBackground(active_game_win_lb,active_game_win_rb,al_get_bitmap_width(menu_image1),0,0,al_get_bitmap_width(menu_image1),padding,
						active_game_win_lb,0,0,menu_image1);

					drawExtendedBackground(active_game_win_lb,active_game_win_rb,al_get_bitmap_width(menu_image1),0,0,al_get_bitmap_width(menu_image1),padding,
						active_game_win_lb,display_h-padding*2,0,menu_image2);

					al_draw_bitmap(guy_image.bitmap,guy_image.dx,guy_image.dy,0);

					al_draw_bitmap(bbtan_text,display_w/2-al_get_bitmap_width(bbtan_text)/2,guy_image.dy+al_get_bitmap_height(guy_image.bitmap),0);

					al_draw_bitmap(button2,active_game_win_lb+(active_game_win_w/100*18),display_h/100*62,0);

					al_draw_bitmap(button1,active_game_win_lb+(active_game_win_w/100*55),display_h/100*62,0);

					al_draw_filled_circle(menu_ball.dx- ball_rad,menu_ball.dy- ball_rad,ball_rad,al_map_rgb(255,255,255));

					al_draw_rectangle(active_game_win_lb,display_h-padding,active_game_win_rb,display_h,al_map_rgb(131, 210, 245),7);

					al_draw_bitmap(what_happ_text.bitmap,what_happ_text.dx,what_happ_text.dy,0);

					al_draw_bitmap(small_filler,game_win_lb-2,0,0);
					al_draw_bitmap(small_filler,active_game_win_rb+2,0,0);
					al_draw_bitmap(big_filler,0,0,0);
					al_draw_bitmap(big_filler,game_win_rb,0,0);

					al_draw_line(active_game_win_lb,display_h-padding*2,active_game_win_rb,display_h-padding*2,al_map_rgb(255,255,255),5);
					al_draw_line(active_game_win_lb,padding,active_game_win_rb,padding,al_map_rgb(255,255,255),5);
					al_draw_line(game_win_lb,0,game_win_lb,display_h,al_map_rgb(255,255,255),1);
					al_draw_line(game_win_rb,0,game_win_rb,display_h,al_map_rgb(255,255,255),1);

					al_draw_textf(font35, al_map_rgb(255, 255, 255), active_game_win_lb+active_game_win_w/2, active_game_win_tb+active_game_win_h-al_get_font_line_height(font35),
					 ALLEGRO_ALIGN_CENTRE , "PRESS ESCAPE TO EXIT...");


					al_draw_textf(font18, al_map_rgb(255, 0, 255), 5, 5, 0, "FPS: %i", game_FPS);

				}
				break;

				case PLAYING:
				{
					drawExtendedBackground(active_game_win_lb,active_game_win_rb,al_get_bitmap_width(menu_image1),0,0,al_get_bitmap_width(menu_image1),padding,
						active_game_win_lb,0,0,menu_image1);

					al_draw_bitmap(top_layer,active_game_win_lb,0,0);

					drawExtendedBackground(active_game_win_lb,active_game_win_rb,al_get_bitmap_width(menu_image1),0,0,al_get_bitmap_width(menu_image1),padding,
						active_game_win_lb,display_h-padding*2,0,menu_image2);

					al_draw_bitmap(top_layer,active_game_win_lb,display_h-padding*2,0);

					al_draw_bitmap(coin_img,active_game_win_lb+30,(display_h-padding*2)+padding/2-al_get_bitmap_height(coin_img)/2,0);

					al_draw_textf(font35,al_map_rgb(255,255,255),active_game_win_lb+30+al_get_bitmap_width(coin_img)+20,(display_h-padding*2)+padding/2-al_get_font_ascent(font35)/2,
						0,"%d",player.coins);

					al_draw_textf(font65,al_map_rgb(255,255,255),display_w/2,padding/2-al_get_font_ascent(font65)/2,ALLEGRO_ALIGN_CENTRE ,"%d",raund_num);

					al_draw_textf(font35,al_map_rgb(255,255,255),active_game_win_lb+active_game_win_w/100*95,(padding/100*10),
						ALLEGRO_ALIGN_CENTRE ,"TOP");

					al_draw_textf(font35,al_map_rgb(255,255,255),active_game_win_lb+active_game_win_w/100*95,(padding/100*50),
						ALLEGRO_ALIGN_CENTRE ,"%d",top_score);

					al_draw_bitmap(pause_image,active_game_win_lb+active_game_win_w/100*2,padding/2-al_get_bitmap_height(pause_image)/2,0);
					al_draw_bitmap(question_image,active_game_win_lb+active_game_win_w/100*5+al_get_bitmap_width(pause_image),padding/2-al_get_bitmap_height(question_image)/2,0);

					al_draw_line(active_game_win_lb,display_h-padding*2,active_game_win_rb,display_h-padding*2,al_map_rgb(255,255,255),5);
					al_draw_line(active_game_win_lb,padding,active_game_win_rb,padding,al_map_rgb(255,255,255),5);

					al_draw_rectangle(active_game_win_lb,display_h-padding,active_game_win_rb,display_h,al_map_rgb(131, 210, 245),7);

					if(min_left>=0 && sec_left>=0)
					{
						al_draw_textf(font65, al_map_rgb(r,g,b) , display_w/2,display_h-padding/2-al_get_font_ascent(font65)/2,ALLEGRO_ALIGN_CENTRE , "%02d:%02d", min_left,sec_left);
					}
					else
					{
						al_draw_textf(font65, al_map_rgb(r,g,b) , display_w/2,display_h-padding/2-al_get_font_ascent(font65)/2,ALLEGRO_ALIGN_CENTRE , "So now what ? ");
					}

					drawTargets(target,MAX_TARGETS,target_w,target_r,font35,font27,font23,plus_img,horiz_decr_img,vert_decr_img,bouncer_img,coin_img,space);

					drawPlayer( &player, draw_arr,guy_in_game_img, arrow_img,padding,font23,ball_rad,balls_shot);

					drawBalls(player.ball_h);

					showExplosions(&explosion);

					drawPlusOneAmim(&plus_one_anim,font27);

					drawDecrAnimation(& decr_animation, active_game_win_lb, active_game_win_rb, active_game_win_tb, active_game_win_db);

					al_draw_bitmap(big_filler,0,0,0);
					al_draw_bitmap(big_filler,game_win_rb,0,0);

					al_draw_line(game_win_lb,0,game_win_lb,display_h,al_map_rgb(255,255,255),1);

					al_draw_textf(font18, al_map_rgb(255, 0, 255), 5, 5, 0, "FPS: %i", game_FPS);	/*display FPS on screen*/
				}
				break;

				case PAUSE:
				{

				}
				break;

				case TIPS:
				{

				}
				break;

				case GAMEOVER:
				{
					drawExtendedBackground(active_game_win_lb,active_game_win_rb,al_get_bitmap_width(menu_image1),0,0,al_get_bitmap_width(menu_image1),padding,
						active_game_win_lb,0,0,menu_image1);

					al_draw_bitmap(top_layer,active_game_win_lb,0,0);

					drawExtendedBackground(active_game_win_lb,active_game_win_rb,al_get_bitmap_width(menu_image1),0,0,al_get_bitmap_width(menu_image1),padding,
						active_game_win_lb,display_h-padding*2,0,menu_image2);

					al_draw_bitmap(top_layer,active_game_win_lb,display_h-padding*2,0);

					al_draw_bitmap(coin_img,active_game_win_lb+30,(display_h-padding*2)+padding/2-al_get_bitmap_height(coin_img)/2,0);

					al_draw_textf(font35,al_map_rgb(255,255,255),active_game_win_lb+30+al_get_bitmap_width(coin_img)+20,(display_h-padding*2)+padding/2-al_get_font_ascent(font35)/2,
						0,"%d",player.coins);

					al_draw_textf(font65,al_map_rgb(255,255,255),display_w/2,padding/2-al_get_font_ascent(font65)/2,ALLEGRO_ALIGN_CENTRE ,"%d",raund_num);

					al_draw_textf(font35,al_map_rgb(255,255,255),active_game_win_lb+active_game_win_w/100*95,(padding/100*10),
						ALLEGRO_ALIGN_CENTRE ,"TOP");

					al_draw_textf(font35,al_map_rgb(255,255,255),active_game_win_lb+active_game_win_w/100*95,(padding/100*50),
						ALLEGRO_ALIGN_CENTRE ,"%d",top_score);

					al_draw_bitmap(pause_image,active_game_win_lb+active_game_win_w/100*2,padding/2-al_get_bitmap_height(pause_image)/2,0);
					al_draw_bitmap(question_image,active_game_win_lb+active_game_win_w/100*5+al_get_bitmap_width(pause_image),padding/2-al_get_bitmap_height(question_image)/2,0);

					al_draw_line(active_game_win_lb,display_h-padding*2,active_game_win_rb,display_h-padding*2,al_map_rgb(255,255,255),5);
					al_draw_line(active_game_win_lb,padding,active_game_win_rb,padding,al_map_rgb(255,255,255),5);

					al_draw_rectangle(active_game_win_lb,display_h-padding,active_game_win_rb,display_h,al_map_rgb(131, 210, 245),7);

					if(min_left>=0 && sec_left>=0)
					{
						al_draw_textf(font65, al_map_rgb(r,g,b) , display_w/2,display_h-padding/2-al_get_font_ascent(font65)/2,ALLEGRO_ALIGN_CENTRE , "%02d:%02d", min_left,sec_left);
					}
					else
					{
						al_draw_textf(font65, al_map_rgb(r,g,b) , display_w/2,display_h-padding/2-al_get_font_ascent(font65)/2,ALLEGRO_ALIGN_CENTRE , "So now what ? ");
					}

					drawTargets(target,MAX_TARGETS,target_w,target_r,font35,font27,font23,plus_img,horiz_decr_img,vert_decr_img,bouncer_img,coin_img,space);

					drawPlayer( &player, draw_arr,guy_in_game_img, arrow_img,padding,font23,ball_rad,balls_shot);

					drawBalls(player.ball_h);

					showExplosions(&explosion);

					drawPlusOneAmim(&plus_one_anim,font27);

					drawDecrAnimation(& decr_animation, active_game_win_lb, active_game_win_rb, active_game_win_tb, active_game_win_db);

					al_draw_bitmap(big_filler,0,0,0);
					al_draw_bitmap(big_filler,game_win_rb,0,0);

					al_draw_line(game_win_lb,0,game_win_lb,display_h,al_map_rgb(255,255,255),1);

					al_draw_textf(font18, al_map_rgb(255, 0, 255), 5, 5, 0, "FPS: %i", game_FPS);	/*display FPS on screen*/

					al_draw_filled_rectangle(game_win_lb, 0, game_win_rb, display_h,al_map_rgba_f(0,0,0,0.9f));
					al_draw_textf(font65, al_map_rgb(255, 255, 255), active_game_win_lb+active_game_win_w/2, active_game_win_tb+active_game_win_h/3, ALLEGRO_ALIGN_CENTRE , "CONTINUE?");
					al_draw_bitmap(button1,active_game_win_lb+(active_game_win_w/2)-al_get_bitmap_width(button1)/2,active_game_win_tb+active_game_win_h/2,0);
					al_draw_textf(font35, al_map_rgb(255, 255, 255), active_game_win_lb+active_game_win_w/2, active_game_win_tb+active_game_win_h-al_get_font_line_height(font35),
					 ALLEGRO_ALIGN_CENTRE , "PRESS ESCAPE TO EXIT...");

					al_draw_bitmap(big_filler,0,0,0);
					al_draw_bitmap(big_filler,game_win_rb,0,0);
					al_draw_line(game_win_lb,0,game_win_lb,display_h,al_map_rgb(255,255,255),1);
					al_draw_line(game_win_rb,0,game_win_rb,display_h,al_map_rgb(255,255,255),1);

				}
				break;
			}

			/*FLIP BUFFERS*/
			al_flip_display();
			al_clear_to_color(al_map_rgb(255,255,255));
		}
	}

	saveMyCoins("coins_dat",&player.coins);
	saveMyTop("top_score_dat",& top_score);


	/*DESTROY OBJECTS*/
	al_destroy_sample(hit_sound);
	al_destroy_sample(start_sound);
	al_destroy_sample(coin_sound);
	al_destroy_sample(plus_sound);
	al_destroy_sample(bounce_sound);
	al_destroy_sample(laser_sound);
	al_destroy_sample(lose_sound);
	al_destroy_font(font18);
	al_destroy_font(font65);
	al_destroy_font(font35);
	al_destroy_font(font23);
	al_destroy_font(font27);
   	al_destroy_display(display);
   	al_destroy_event_queue(event_queue);
   	al_destroy_timer(timer);
   	al_destroy_bitmap(game_window);
   	al_destroy_bitmap(menu_image1);
   	al_destroy_bitmap(menu_image2);
   	al_destroy_bitmap(guy_image.bitmap);
   	al_destroy_bitmap(bbtan_text);
   	al_destroy_bitmap(button1);
   	al_destroy_bitmap(button2);
   	al_destroy_bitmap(what_happ_text.bitmap);
   	al_destroy_bitmap(small_filler);
   	al_destroy_bitmap(big_filler);
   	al_destroy_bitmap(top_layer);
   	al_destroy_bitmap(coin_img);
   	al_destroy_bitmap(pause_image);
   	al_destroy_bitmap(question_image);
	al_destroy_bitmap(plus_img);
	al_destroy_bitmap(horiz_decr_img);
	al_destroy_bitmap(vert_decr_img);
	al_destroy_bitmap(bouncer_img);
	al_destroy_bitmap(guy_in_game_img);
	al_destroy_bitmap(arrow_img);
	al_destroy_bitmap(back.image);
	deleteBalls(player.ball_h);


	return 1;
}
