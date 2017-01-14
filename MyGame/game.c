#include "game.h"

void initDecrAnimation(decr_animation_t * decr_animation)
{
	decr_animation->alive=0;
}

void startDecrAnimation(decr_animation_t * decr_animation,double dx,double dy,char type)
{
	decr_animation->alive=1;
	decr_animation->start_time=al_current_time();
	decr_animation->dx=dx;
	decr_animation->dy=dy;
	decr_animation->type=type;
}

void updateDecrAnimation(decr_animation_t * decr_animation)
{
	if(decr_animation->alive)
	{
		if(al_current_time()-decr_animation->start_time >= 0.015)
		{
			decr_animation->alive=0;
		}
	}
}

void drawDecrAnimation(decr_animation_t * decr_animation,double active_game_win_lb,double active_game_win_rb,double active_game_win_tb,double active_game_win_db)
{
	if(decr_animation->alive)
	{
		if(decr_animation->type=='H')
		{
			al_draw_filled_rectangle(active_game_win_lb, decr_animation->dy-10, active_game_win_rb, decr_animation->dy+10,al_map_rgb(255, 255, 0));
		}
		else if(decr_animation->type=='V')
		{
			al_draw_filled_rectangle(decr_animation->dx-10, active_game_win_tb, decr_animation->dx+10, active_game_win_db,al_map_rgb(255, 255, 0));
		}

	}
}

void initPlusOneAnim(plus_one_anim_t * plus_one_anim)
{
	int i;

	plus_one_anim->size=10;

	for(i=0;i<plus_one_anim->size;++i)
	{
		plus_one_anim->plus_one_unit[i].alive=0;
	}
}


void startPlusOneAnim(plus_one_anim_t plus_one_anim[],double dx,double dy)
{
	int i,count=0;

	for(i=0;i<plus_one_anim->size && count < 1;++i)
	{
		if(plus_one_anim->plus_one_unit[i].alive == 0 )
		{
			plus_one_anim->plus_one_unit[i].alive=1;
			plus_one_anim->plus_one_unit[i].dx=dx;
			plus_one_anim->plus_one_unit[i].dy=dy;
			plus_one_anim->plus_one_unit[i].transparency=1;
			count ++ ;
		}
	}
}

void updatePlusOneAnim(plus_one_anim_t * plus_one_anim)
{
	int i;

	for(i=0;i<plus_one_anim->size;++i)
	{
		if(plus_one_anim->plus_one_unit[i].alive && plus_one_anim->plus_one_unit[i].transparency <= 0)
		{
			plus_one_anim->plus_one_unit[i].alive=0;
		}
		else if(plus_one_anim->plus_one_unit[i].alive)
		{
			plus_one_anim->plus_one_unit[i].transparency-=0.04;
		}
	}
}

void drawPlusOneAmim(plus_one_anim_t * plus_one_anim,const ALLEGRO_FONT *font)
{
	int i;

	for(i=0;i<plus_one_anim->size;++i)
	{
		if(plus_one_anim->plus_one_unit[i].alive)
		{
			 	al_draw_textf(font, al_map_rgba(255*plus_one_anim->plus_one_unit[i].transparency,255*plus_one_anim->plus_one_unit[i].transparency,0,plus_one_anim->plus_one_unit[i].transparency),
			 		plus_one_anim->plus_one_unit[i].dx, plus_one_anim->plus_one_unit[i].dy, 0,"+1");
		}
	}
}

void initBackground(background_t * back,double dx,double dy,int speed,int direction_x,const char * filename)
{
	back->dx=dx;
	back->dy=dy;
	back->speed=speed;
	back->direction_x=direction_x;

	back->image=al_load_bitmap(filename);
	back->width=al_get_bitmap_width(back->image);
	back->height=al_get_bitmap_height(back->image);
}

void updatBackground(background_t * back,double from)
{
	back->dx+=back->speed*back->direction_x;

	if(back->dx+back->width <=from)
	{
		back->dx=from;
	}
}

void drawBackground(background_t * back,double window_w,double window_h,double from,double to)
{
	double prod;
	double fraction;
	double intpart;
	int i;

	prod=window_h/back->height;
	fraction=modf(prod,&intpart);

	for(i=0;i<intpart;i++)
	{
		drawExtendedBackground(0,window_w,back->width,0,0,back->width,back->height,back->dx,back->dy+back->height*i,0,back->image);
		al_draw_bitmap(back->image,to-fabs(from-back->dx),back->dy+back->height*i,0);
	}

	drawExtendedBackground(0,window_w,back->width,0,0,back->width,back->height*fraction,back->dx,back->dy+back->height*i,0,back->image);
	al_draw_bitmap_region(back->image,0,0,back->width,back->height*fraction,to-fabs(from-back->dx),back->dy+back->height*i,0);
}

void initExplosion(explosion_t * explosion)
{
	explosion->exp_num=0;
	explosion->boom = (boom_t *) realloc(explosion->boom,0);
}

void startExplosion(explosion_t * explosion,double start_x, double start_y)
{
	int i;

	(explosion->exp_num)++;
	explosion->boom=(boom_t*) realloc (explosion->boom,sizeof(boom_t)*explosion->exp_num);
	explosion->boom[explosion->exp_num-1].debris_num=70;

	for(i=0;i<explosion->boom[explosion->exp_num-1].debris_num;++i)
	{
		explosion->boom[explosion->exp_num-1].debris[i].alive=1;
		explosion->boom[explosion->exp_num-1].debris[i].dx=start_x+(rand()%60 * (rand()%2 ? -1 : 1));
		explosion->boom[explosion->exp_num-1].debris[i].dy=start_y;
		explosion->boom[explosion->exp_num-1].debris[i].size=rand()%15+1;
		explosion->boom[explosion->exp_num-1].debris[i].speed=3;
		explosion->boom[explosion->exp_num-1].debris[i].direction_x= rand()%2 ? 1 : -1;
		explosion->boom[explosion->exp_num-1].debris[i].direction_y= rand()%2 ? 1 : -1;
		explosion->boom[explosion->exp_num-1].debris[i].angle = rand()%90+1;
		explosion->boom[explosion->exp_num-1].debris[i].color=al_map_rgb(rand()%255+1,rand()%255+1,rand()%255+1);

		explosion->boom[explosion->exp_num-1].start_x=start_x;
		explosion->boom[explosion->exp_num-1].start_y=start_y;
	}
}

void updateExplosion(explosion_t * explosion)
{
	int i,j;
	int empty;


	for(i=0;i<explosion->exp_num;++i)
	{
		empty=1;

		for(j=0;j<explosion->boom[i].debris_num;++j)
		{
			if(explosion->boom[i].debris[j].alive)
			{
				explosion->boom[i].debris[j].dx += explosion->boom[i].debris[j].speed * cos(explosion->boom[i].debris[j].angle) * explosion->boom[i].debris[j].direction_x;
				explosion->boom[i].debris[j].dy += explosion->boom[i].debris[j].speed * sin(explosion->boom[i].debris[j].angle) * explosion->boom[i].debris[j].direction_y;

				if(CheckDistance(explosion->boom[i].start_x,explosion->boom[i].start_y,explosion->boom[i].debris[j].dx,explosion->boom[i].debris[j].dy) >= 300)
				{
					explosion->boom[i].debris[j
					].alive=0;
				}

				empty=0;
			}
		}

		if(empty)
		{
			sortArr(explosion->boom,i,explosion->exp_num);
			(explosion->exp_num)--;
			explosion->boom=(boom_t*) realloc (explosion->boom,sizeof(boom_t)*explosion->exp_num);
		}
	}
}

void showExplosions(explosion_t * explosion)
{
	int i,j;

	for(i=0;i<explosion->exp_num;++i)
	{
		for(j=0;j<explosion->boom[i].debris_num;++j)
		{
			if(explosion->boom[i].debris[j].alive)
			{
				al_draw_filled_rectangle(explosion->boom[i].debris[j].dx, explosion->boom[i].debris[j].dy,
					explosion->boom[i].debris[j].dx+explosion->boom[i].debris[j].size, explosion->boom[i].debris[j].dy+explosion->boom[i].debris[j].size,explosion->boom[i].debris[j].color);
			}
		}
	}
}

void sortArr(boom_t boom[],int from,int to)
{
	int i;

	for(i=from;i<to-1;++i)
	{
		boom[i]=boom[i+1];
	}
}

ALLEGRO_COLOR changeColor(ALLEGRO_COLOR old_color,int lives)
{
	unsigned char r,g,b;

	al_unmap_rgb(old_color,&r,&g,&b);

	if(lives > 0 )
	{
		if(r>b)
		{
			r=255;
			g+=60/lives;
			b=14;
		}
		else if(b>r)
		{
			b=255;
			g+=60/lives;
			r=14;
		}
	}

	return(al_map_rgb(r,g,b));
}


int getMyCoins(const char * filename)
{
	FILE * fp;
	int ans;

	if(!(fp=fopen(filename,"rb")))
	{
		return 0;
	}
	else
	{
		fread(&ans,sizeof(int),1,fp);
	}

	fclose(fp);

	return ans;
}

void saveMyCoins(const char * filename,int *coins)
{
	FILE * fp;

	fp=fopen(filename,"wb");

	fwrite(coins,sizeof(int),1,fp);
	fclose(fp);
}

int getMyTop(const char * filename)
{
	FILE * fp;
	int ans;

	if(!(fp=fopen(filename,"rb")))
	{
		return 0;
	}
	else
	{
		fread(&ans,sizeof(int),1,fp);
	}

	fclose(fp);

	return ans;
}

void saveMyTop(const char * filename,int *top_score)
{
	FILE * fp;

	fp=fopen(filename,"wb");

	fwrite(top_score,sizeof(int),1,fp);
	fclose(fp);
}



void changeGameStage(int new_state,int * old_state)
{
	*old_state=new_state;
}

void drawExtendedBackground(float window_lb,float window_rb,float image_w,
	float sx,float sy,float sw,float sh,float dx,float dy,int flag,ALLEGRO_BITMAP *bitmap)
{
	double window_w;
	double prod;
	double intpart;
	double fraction;
	int i;

	window_w = window_rb-window_lb;

	prod=window_w/image_w;

	fraction=modf(prod,&intpart);

	for(i=0;i<intpart;i++)
	{
		al_draw_bitmap_region(bitmap,sx,sy,sw,sh,dx+image_w*i,dy,flag);
	}

	al_draw_bitmap_region(bitmap,sx,sy,sw*fraction,sh,dx+image_w*i,dy,flag);

}

void initTargets(target_t *arr,int size,int raund,float cell_w,float active_game_win_lb,
	float active_game_win_tb)
{
	int i,j,k;
	int num;
	int rand1,rand2,rand3,rand4,rand5;
	int hist[7];
	int col_temp;

	if((raund%10)==0)
		num=rand()%5+1;
	else
		num=rand()%7+1;

	if(num==1)
	{
		num+=2;
	}

	for(i=0;i<7;i++)
	{
		hist[i]=0;
	}

	for(i=0,j=0;i<size && j<num;i++)
	{
		if(!(arr[i].alive))
		{
			rand1=rand()%100;

			if(rand1<10)
			{
				rand4=rand()%100;

				if(rand4<=40)
				{
					arr[i].figure_id=COIN;
				}
				else
				{
					arr[i].figure_id=BOUNCER;
				}
			}
			else if(rand1>=10 && rand1 <40)
			{
				rand2=rand()%100;

				if(rand2<20)
				{
					arr[i].figure_id=VERT_DECR;
				}
				else if(rand2>=20 && rand2<40)
				{
					arr[i].figure_id=HORIZ_DECR;
				}
				else
				{
					arr[i].figure_id=PLUS;
				}
			}
			else if(rand1>=40 && rand1<70)
			{
				rand3=rand()%100;

				if(rand3<25)
				{
					arr[i].figure_id=RIGHT_FLIPPED_TR;
				}
				else if(rand3>=25 && rand3<50)
				{
					arr[i].figure_id=LEFT_FLIPPED_TR;
				}
				else if(rand3 >= 50 && rand3 <75)
				{
					arr[i].figure_id=RIGHT_TR;
				}
				else
				{
					arr[i].figure_id=LEFT_TR;
				}
			}
			else
			{
				arr[i].figure_id=SQUARE;
			}

			arr[i].alive = 1;

			if(arr[i].figure_id == SQUARE || arr[i].figure_id == LEFT_TR || arr[i].figure_id == RIGHT_TR ||
			 arr[i].figure_id == LEFT_FLIPPED_TR || arr[i].figure_id == RIGHT_FLIPPED_TR )
			{
				arr[i].lives = !(raund%10) ? raund*2 : raund;
			}
			else
			{
				arr[i].lives=1;
			}

			col_temp=rand()%7;
			while(hist[col_temp]!=0)
			{
				col_temp=rand()%7;
			}
			hist[col_temp]=1;
			arr[i].col_num=col_temp;

			/*prevent bouncer from spowning at the 0 and 6 col-s*/
			if(arr[i].figure_id==BOUNCER && (arr[i].col_num==0 || arr[i].col_num==6))
			{
				for(k=0;k<size;k++)
				{
					if(arr[k].alive && arr[k].row_num==0 && !(arr[k].figure_id==BOUNCER) && !(arr[k].col_num==0 || arr[k].col_num==6))
					{
						col_temp=arr[i].col_num;
						arr[i].col_num=arr[k].col_num;
						arr[k].col_num=col_temp;
						arr[k].dx=active_game_win_lb+cell_w/2+cell_w*arr[k].col_num;
						k=size;
					} 
				}

				if(arr[i].col_num==0 || arr[i].col_num==6)
				{
					arr[i].alive=0;
				}
			}

			arr[i].row_num=0;

			arr[i].dx=active_game_win_lb+cell_w/2+cell_w*arr[i].col_num;
			arr[i].dy= active_game_win_tb+cell_w/2;

			arr[i].speed=2;
			arr[i].direction=1;
			arr[i].hit=0;

			if(arr[i].figure_id == SQUARE || arr[i].figure_id == LEFT_TR || arr[i].figure_id == RIGHT_TR ||
			 arr[i].figure_id == LEFT_FLIPPED_TR || arr[i].figure_id == RIGHT_FLIPPED_TR )
			{
				if(!(raund%10))
				{
					arr[i].color=al_map_rgb(0, 51, 153);
				}
				else
				{
					if((rand5=rand()%4)==0 )
						arr[i].color=al_map_rgb(255, 14, 14);
					else if(rand5==1)
						arr[i].color=al_map_rgb(255, 87, 51);
					else if(rand5==2)
						arr[i].color=al_map_rgb(199, 0, 57);
					else
						arr[i].color=al_map_rgb(144, 12, 63);
				}
			}

			j++;
		}
	}

	for(i=0;i<size;i++)
	{
		if(arr[i].alive)
		{
			arr[i].row_num++;
			if(arr[i].figure_id == HORIZ_DECR || arr[i].figure_id==VERT_DECR || arr[i].figure_id== BOUNCER)
			{
				if(rand()%2)
				{
					arr[i].alive=0;
				}
			}
		}
	}
}

int  updateTargets(target_t *arr,int size,float active_game_win_db,float cell_w,float target_r)
{
	int i;

	for(i=0;i<size;++i)
	{
		if(arr[i].alive)
		{
			arr[i].dy=arr[i].dy+arr[i].speed*arr[i].direction;

			if(arr[i].dy+cell_w/2 >= active_game_win_db)
			{
				arr[i].alive=0;

				if(arr[i].figure_id == SQUARE || arr[i].figure_id == LEFT_TR || arr[i].figure_id == RIGHT_TR ||
		 			arr[i].figure_id == LEFT_FLIPPED_TR || arr[i].figure_id == RIGHT_FLIPPED_TR)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

void drawTargets(target_t *arr,int size,float cell_w,float target_r,const ALLEGRO_FONT *font1,const ALLEGRO_FONT *font2,const ALLEGRO_FONT *font3,
	ALLEGRO_BITMAP * plus_img,ALLEGRO_BITMAP * horiz_decr_img,ALLEGRO_BITMAP * vert_decr_img,ALLEGRO_BITMAP *bouncer_img,
	ALLEGRO_BITMAP * coin_img,float space)
{
	int i;

	for(i=0;i<size;i++)
	{
		if(arr[i].alive)
		{
			switch(arr[i].figure_id)
			{
				case SQUARE:
				{
					if(!arr[i].hit)
					{
						al_draw_rectangle(arr[i].dx-cell_w/2+space,arr[i].dy-cell_w/2+space,arr[i].dx+cell_w/2-space,arr[i].dy+cell_w/2-space,arr[i].color,6);

						if(arr[i].lives <=9)
						{
							al_draw_textf(font1,arr[i].color,arr[i].dx,arr[i].dy-al_get_font_ascent(font1)/2,ALLEGRO_ALIGN_CENTRE,"%d",arr[i].lives);
						}
						else if(arr[i].lives<100)
						{
							al_draw_textf(font2,arr[i].color,arr[i].dx,arr[i].dy-al_get_font_ascent(font2)/2,ALLEGRO_ALIGN_CENTRE,"%d",arr[i].lives);
						}
						else
						{
							al_draw_textf(font3,arr[i].color,arr[i].dx,arr[i].dy-al_get_font_ascent(font3)/2,ALLEGRO_ALIGN_CENTRE,"%d",arr[i].lives);
						}
					}

				}break;

				case LEFT_TR:
				{
					if(!arr[i].hit)
					{
						al_draw_triangle(arr[i].dx-cell_w/2+space,arr[i].dy-cell_w/2+space+3,arr[i].dx+cell_w/2-space-3,arr[i].dy+cell_w/2-space,arr[i].dx-cell_w/2+space,arr[i].dy+cell_w/2-space,arr[i].color,6);

						if(arr[i].lives <= 9)
						{
							al_draw_textf(font1,arr[i].color,arr[i].dx-cell_w/2+22,arr[i].dy+cell_w/2-14-al_get_font_ascent(font1),ALLEGRO_ALIGN_CENTRE,"%d",arr[i].lives);
						}
						else if(arr[i].lives<100)
						{
							al_draw_textf(font2,arr[i].color,arr[i].dx-cell_w/2+22,arr[i].dy+cell_w/2-14-al_get_font_ascent(font2),ALLEGRO_ALIGN_CENTRE,"  %d",arr[i].lives);
						}
						else
						{
							al_draw_textf(font3,arr[i].color,arr[i].dx-cell_w/2+22,arr[i].dy+cell_w/2-14-al_get_font_ascent(font3),ALLEGRO_ALIGN_CENTRE,"   %d",arr[i].lives);
						}
					}
				}break;

				case RIGHT_TR:
				{
					if(!arr[i].hit)
					{
						al_draw_triangle(arr[i].dx+cell_w/2-space,arr[i].dy-cell_w/2+space+3,arr[i].dx+cell_w/2-space,arr[i].dy+cell_w/2-space,arr[i].dx-cell_w/2+space+3,arr[i].dy+cell_w/2-space,arr[i].color,6);
						if(arr[i].lives <= 9)
						{
							al_draw_textf(font1,arr[i].color,arr[i].dx+cell_w/2-22,arr[i].dy+cell_w/2-14-al_get_font_ascent(font1),ALLEGRO_ALIGN_CENTRE ,"%d",arr[i].lives);
						}
						else if(arr[i].lives<100)
						{
							al_draw_textf(font2,arr[i].color,arr[i].dx+cell_w/2-22,arr[i].dy+cell_w/2-14-al_get_font_ascent(font2),ALLEGRO_ALIGN_CENTRE ,"%d  ",arr[i].lives);
						}
						else
						{
							al_draw_textf(font3,arr[i].color,arr[i].dx+cell_w/2-22,arr[i].dy+cell_w/2-14-al_get_font_ascent(font3),ALLEGRO_ALIGN_CENTRE ,"%d   ",arr[i].lives);
						}
					}
				}break;

				case LEFT_FLIPPED_TR:
				{
					if(!arr[i].hit)
					{
						al_draw_triangle(arr[i].dx-cell_w/2+space,arr[i].dy-cell_w/2+space,arr[i].dx+cell_w/2-space-3,arr[i].dy-cell_w/2+space,arr[i].dx-cell_w/2+space,arr[i].dy+cell_w/2-space-3,arr[i].color,6);
						if(arr[i].lives <=9)
						{
							al_draw_textf(font1,arr[i].color,arr[i].dx-cell_w/2+22,arr[i].dy-cell_w/2-14+al_get_font_ascent(font1),ALLEGRO_ALIGN_CENTRE ,"%d",arr[i].lives);
						}
						else if(arr[i].lives<100)
						{
							al_draw_textf(font2,arr[i].color,arr[i].dx-cell_w/2+22,arr[i].dy-cell_w/2-14+ al_get_font_line_height(font2),ALLEGRO_ALIGN_CENTRE ,"  %d",arr[i].lives);
						}
						else
						{
							al_draw_textf(font3,arr[i].color,arr[i].dx-cell_w/2+22,arr[i].dy-cell_w/2-14+ al_get_font_line_height(font3),ALLEGRO_ALIGN_CENTRE ,"   %d",arr[i].lives);
						}
					}
				}break;

				case RIGHT_FLIPPED_TR:
				{
					if(!arr[i].hit)
					{
						al_draw_triangle(arr[i].dx+cell_w/2-space,arr[i].dy-cell_w/2+space,arr[i].dx+cell_w/2-space,arr[i].dy+cell_w/2-space-3,arr[i].dx-cell_w/2+space+3,arr[i].dy-cell_w/2+space,arr[i].color,6);
						if(arr[i].lives<=9)
						{
							al_draw_textf(font1,arr[i].color,arr[i].dx+cell_w/2-22,arr[i].dy-cell_w/2-14+al_get_font_ascent(font1),ALLEGRO_ALIGN_CENTRE ,"%d",arr[i].lives);
						}
						else if(arr[i].lives<100)
						{
							al_draw_textf(font2,arr[i].color,arr[i].dx+cell_w/2-22,arr[i].dy-cell_w/2-14+ al_get_font_line_height(font2),ALLEGRO_ALIGN_CENTRE ,"%d  ",arr[i].lives);
						}
						else
						{
							al_draw_textf(font3,arr[i].color,arr[i].dx+cell_w/2-22,arr[i].dy-cell_w/2-14+ al_get_font_line_height(font3),ALLEGRO_ALIGN_CENTRE ,"%d   ",arr[i].lives);
						}
					}
				}break;

				case PLUS:
				{
					al_draw_bitmap(plus_img,arr[i].dx-al_get_bitmap_width(plus_img)/2,arr[i].dy-al_get_bitmap_height(plus_img)/2,0);
				}break;

				case HORIZ_DECR:
				{
					al_draw_bitmap(horiz_decr_img,arr[i].dx-al_get_bitmap_width(horiz_decr_img)/2,arr[i].dy-al_get_bitmap_height(horiz_decr_img)/2,0);
				}break;

				case VERT_DECR:
				{
					al_draw_bitmap(vert_decr_img,arr[i].dx-al_get_bitmap_width(vert_decr_img)/2,arr[i].dy-al_get_bitmap_height(vert_decr_img)/2,0);
				}break;

				case BOUNCER:
				{
					if( arr[i].hit)
					{
						al_draw_scaled_bitmap(bouncer_img,0,0,al_get_bitmap_width(bouncer_img), al_get_bitmap_height(bouncer_img),
							arr[i].dx-al_get_bitmap_width(bouncer_img)/2,arr[i].dy-al_get_bitmap_height(bouncer_img)/2 ,58, 58, 0);
					}
					else
					{
						al_draw_bitmap(bouncer_img,arr[i].dx-al_get_bitmap_width(bouncer_img)/2,arr[i].dy-al_get_bitmap_height(bouncer_img)/2,0);
					}
				}break;

				case COIN:
				{
					al_draw_bitmap(coin_img,arr[i].dx-al_get_bitmap_width(coin_img)/2,arr[i].dy-al_get_bitmap_height(coin_img)/2,0);
				}break;

				default:break;
			}
		}
	}
}

double AngleToTarget(double x1, double y1, double x2, double y2)
{
	return atan2(x2 - x1, y2 - y1);
}

void initPlayer(player_t * player,double active_game_win_lb,double active_game_win_w,double active_game_win_db)
{
	player->dx=active_game_win_lb+active_game_win_w/2;
	player->dy=active_game_win_db;
	player->speed=8;
	player->direction=1;

	player->animation_angle=0;
	player->animation_last_time=0;

	player->balls=1;
	player->balls_left_unshot=player->balls;
	player->ball_h=NULL;
}

void updatePlayer(player_t * player,double mouse_x,double mouse_y,double new_dx,int balls_shot)
{
	double temp_angle;

	if(!balls_shot)
	{
		temp_angle= AngleToTarget(player->dx,mouse_y,mouse_x,player->dy);

		if( temp_angle*180/PI < -75)
		{
			player->angle=15*PI/180;
		}
		else if( temp_angle*180/PI > 75)
		{
			player->angle=165*PI/180;
		}
		else
		{
			player->angle=temp_angle+90*PI/180;
		}
	}

	if(balls_shot==1 && (al_current_time()-player->animation_last_time < 0.2 || player->balls_left_unshot > 0) )
	{
		player->animation_angle+=1*PI/180;
		if(player->animation_angle > 6*PI/180)
		{
			player->animation_angle=6*PI/180;
		}
	}
	else if ( balls_shot==1 && al_current_time()-player->animation_last_time > 0.2)
	{
		player->animation_angle-=1*PI/180;

		if(player->animation_angle < 0)
		{
			player->animation_angle=0;
		}
	}


	if(fabs(new_dx-player->dx) > player->speed  )
	{
		player->direction = ((new_dx > player->dx) ? 1 : -1 );

		player->dx=player->dx+player->speed*player->direction;
	}
	else if(fabs(new_dx-player->dx) <= player->speed  )
	{
		player->dx=new_dx;
	}
}

void drawPlayer(player_t * player,int draw_arrow,ALLEGRO_BITMAP * guy_img, ALLEGRO_BITMAP * arrow_img,double padding,ALLEGRO_FONT * font,double ball_rad,int balls_shot)
{

	if(balls_shot==1 )
	{
		al_draw_rotated_bitmap(guy_img, al_get_bitmap_width(guy_img)/2,al_get_bitmap_height(guy_img)/2, player->dx+al_get_bitmap_width(guy_img)/2, player->dy, player->animation_angle, 0);
	}
	else
	{
		al_draw_bitmap_region(guy_img,0, 0, al_get_bitmap_width(guy_img),al_get_bitmap_height(guy_img), player->dx, player->dy-al_get_bitmap_height(guy_img)/2, 0);
	}

	if(player->balls_left_unshot > 0 )
	{
	 	al_draw_textf(font, al_map_rgb(255,255,255),player->dx-ball_rad, player->dy-ball_rad*2 -  al_get_font_line_height(font), ALLEGRO_ALIGN_CENTRE ,"x%d",player->balls_left_unshot);
	}

	if(draw_arrow)
	{
		al_draw_rotated_bitmap(arrow_img,al_get_bitmap_width(arrow_img)/2,al_get_bitmap_height(arrow_img),
			player->dx-al_get_bitmap_width(arrow_img)/2,player->dy,player->angle-90*PI/180,0);
	}
}

void deleteBalls(ball_t	* ball_h)
{
	ball_t *current;
	ball_t *to_delete;

	current=ball_h;
	while(current!=NULL)
	{
		to_delete=current;
		current=to_delete->rest;
		free(to_delete);
	}
}


void initBalls(player_t *player,double rad)
{
	int count=0;
	int i;
	ball_t *last;
	ball_t *temp;

	if(player->ball_h==NULL)
	{
		player->ball_h=(ball_t*)malloc(sizeof(ball_t));
		player->ball_h->rest=NULL;

		player->ball_h->alive=0;
		player->ball_h->shot=0;
		player->ball_h->last_hit_target_id=-1;
		player->ball_h->dx=player->dx;
		player->ball_h->dy=player->dy-rad;
		player->ball_h->rad=rad;
		player->ball_h->speed=18;
		player->ball_h->hit=0;
	}
	else
	{
		count++;
		last=player->ball_h->rest;
		temp=player->ball_h;
		temp->shot=0;
		while(last!=NULL)
		{
			temp=last;
			count++;
			temp->shot=0;
			temp->last_hit_target_id=-1;
			last=last->rest;
		}

		last=temp;
		for(i=0;i<player->balls-count;i++)
		{
			last->rest=(ball_t*) malloc(sizeof(ball_t));
			last=last->rest;
			last->rest=NULL;

			last->alive=0;
			last->shot=0;
			last->dx=player->dx;
			last->dy=player->dy-rad;
			last->rad = rad;
			last->speed=18;
			last->hit=0;
			last->last_hit_target_id=-1;
		}
	}
}

int ballsInTheAir(ball_t *ball_h)
{
	int ans=0;
	ball_t *current;

	current=ball_h;

	while(current!=NULL)
	{
		if(current->alive)
		{
			ans=1;
		}
		current=current->rest;
	}

	return ans;
}

void startBalls(player_t *player,int balls_shot)
{
	ball_t * current;
	static double last_time;

	if(balls_shot==0)
	{
		last_time=0;
	}
	else if(balls_shot==1 && player->balls_left_unshot>0)
	{
		current=player->ball_h;
		while(current!=NULL)
		{
			if(!current->alive && !current->shot)
			{
				if(al_current_time()-last_time > 0.05)
				{
					current->alive=1;
					current->shot=1;

					if(player->angle*180/PI<90)
					{
						current->direction_x=-1;
						current->angle=player->angle;
					}
					else if(player->angle*180/PI>90)
					{
						current->direction_x=1;
						current->angle=180*PI/180-player->angle;
					}
					else
					{
						current->direction_x=0;
						current->angle=player->angle;
					}

					current ->direction_y=-1;

					last_time=al_current_time();
					(player->balls_left_unshot)--;
				}

			}

			current=current->rest;
		}
	}
}

void updateBalls(player_t * player,target_t * target,int size,int *flag,double * first_floor_hit,double cell_w,
	double target_rad,double space,double active_game_win_lb,double active_game_win_rb,double active_game_win_db,float active_game_win_tb,double new_dx,
	ALLEGRO_SAMPLE *hit_sound,ALLEGRO_SAMPLE * coin_sound,explosion_t * explosion,plus_one_anim_t * plus_one_anim,ALLEGRO_SAMPLE * plus_sound,ALLEGRO_SAMPLE * bounce_sound,decr_animation_t * decr_animation,
	ALLEGRO_SAMPLE * laser_sound,double mouse_x,double mouse_y)
{
	ball_t *current;
	int i,j;
	int collision;
	double destination_x;
	double destination_y;

	current=player->ball_h;
	while (current!=NULL)
	{
		if(current->alive)
		{
			destination_x=current->dx+current->speed * cos(current->angle) * current->direction_x;
			destination_y=current->dy+current->speed * sin(current->angle) * current->direction_y;
			collision=0;

			while(CheckDistance(current->dx,current->dy,destination_x,destination_y) >1 && !collision)
			{


				current->dy+=2*sin(current->angle) * current->direction_y;
				current->dx+=2*cos(current->angle) * current->direction_x;


				/*
				current->dx=mouse_x;
				current->dy=mouse_y;
				*/

				if(current->dx-current->rad <= active_game_win_lb)
				{
					current->direction_x=1;
					collision=1;
					current->last_hit_target_id=-1;
				}
				else if (current->dx+current->rad >= active_game_win_rb)
				{
					current->direction_x=-1;
					collision=1;
					current->last_hit_target_id=-1;
				}
				else if (current->dy-current->rad <= active_game_win_tb)
				{
					current->direction_y=1;
					collision=1;
					current->last_hit_target_id=-1;
				}
				else if (current->dy+current->rad >= active_game_win_db)
				{
					current->alive=0;
					collision=1;
					current->last_hit_target_id=-1;
					if(!(*flag) )
					{
						*first_floor_hit=current->dx;
						*flag=1;
					}
				}
				else
				{
					/*do nothing*/
				}

				for(i=0;i<size;++i)
				{
					if(target[i].alive)
					{
						switch(target[i].figure_id)
						{
							case SQUARE:
							{

								if ((current->dy-current->rad <= target[i].dy+cell_w/2-space/2 && current->dy-current->rad > target[i].dy-cell_w/2+space/2 ) ||
									(current->dy+current->rad >= target[i].dy-cell_w/2+space/2 && current->dy+current->rad < target[i].dy+cell_w/2-space/2))
								{
									if ((current->dx+current->rad >= target[i].dx-cell_w/2+space/2 && current->dx+current->rad < target[i].dx+cell_w/2-space/2) ||
									(current->dx-current->rad <= target[i].dx+cell_w/2-space/2 && current->dx-current->rad > target[i].dx-cell_w/2+space/2 ))
									{

										if (current->last_hit_target_id!=i)
										{
											if(CheckDistance(current->dx,current->dy-current->rad,target[i].dx,target[i].dy) < CheckDistance(current->dx-current->rad,current->dy,target[i].dx,target[i].dy)
											&& CheckDistance(current->dx,current->dy-current->rad,target[i].dx,target[i].dy) < CheckDistance(current->dx+current->rad,current->dy,target[i].dx,target[i].dy))
											{
												current->direction_y=1;
											}
											else if (CheckDistance(current->dx,current->dy+current->rad,target[i].dx,target[i].dy) < CheckDistance(current->dx-current->rad,current->dy,target[i].dx,target[i].dy)
												&& CheckDistance(current->dx,current->dy+current->rad,target[i].dx,target[i].dy) < CheckDistance(current->dx+current->rad,current->dy,target[i].dx,target[i].dy))
											{
												current->direction_y=-1;
											}
											else if(CheckDistance(current->dx-current->rad,current->dy,target[i].dx,target[i].dy) < CheckDistance(current->dx,current->dy-current->rad,target[i].dx,target[i].dy)
												&& CheckDistance(current->dx-current->rad,current->dy,target[i].dx,target[i].dy) < CheckDistance(current->dx,current->dy+current->rad,target[i].dx,target[i].dy))
											{
												current->direction_x=1;
											}
											else
											{
												current->direction_x=-1;
											}

											--target[i].lives;
											if(target[i].lives <=0)
											{
												target[i].alive=0;
												startExplosion(explosion,target[i].dx,target[i].dy);
											}

											target[i].hit=1;
											collision=1;
											current->last_hit_target_id=i;

											target[i].color=changeColor(target[i].color,target[i].lives);

											al_play_sample(hit_sound,1,0,1,ALLEGRO_PLAYMODE_ONCE, 0);
										}
									}
									else
									{
										target[i].hit=0;
									}
								}
								else
								{
									target[i].hit=0;
								}
							}break;

							case LEFT_TR:
							{
								if ((current->dy-current->rad <= target[i].dy+cell_w/2-space/2 && current->dy-current->rad > target[i].dy-cell_w/2+space/2 ) ||
									(current->dy+current->rad >= target[i].dy-cell_w/2+space/2 && current->dy+current->rad < target[i].dy+cell_w/2-space/2))
								{
									if( (current->dy <= target[i].dy+cell_w/2-space/2 && current->dx < target[i].dx-cell_w/2+space/2 && current->dx+current->rad >= target[i].dx-cell_w/2+space/2
										 && current->dx-current->rad < target[i].dx-cell_w/2+space/2) ||
										(current->dy > target[i].dy+cell_w/2-space/2 && ((current->dx+current->rad-2 >= target[i].dx-cell_w/2+space/2 && current->dx+current->rad < target[i].dx+cell_w/2-space/2) ||
											(current->dx-current->rad+2 <= target[i].dx+cell_w/2-space/2 && current->dx-current->rad > target[i].dx-cell_w/2+space/2))) ||
										(fabs((target[i].dx-cell_w/2+space/2) - (current->dx-current->rad-6)) <= fabs(current->dy-(target[i].dy-cell_w/2+space/2)) &&
										current->dx+current->rad > target[i].dx-cell_w/2+space/2 && current->dx >= target[i].dx-cell_w/2+space/2 && current->dy <= target[i].dy+cell_w/2-space/2 &&
										current->dy >= target[i].dy) || (current->dx+current->rad > target[i].dx-cell_w/2+space/2 && current->dx
										 >= target[i].dx-cell_w/2+space/2 && current->dy <= target[i].dy+cell_w/2-space/2 &&
										current->dy <= target[i].dy && fabs((target[i].dy+cell_w/2-space/2) - (current->dy+current->rad+6)) <= fabs(current->dx-(target[i].dx+cell_w/2-space/2)) &&
										current->dx-current->rad <= target[i].dx) )
									{

										if (current->last_hit_target_id!=i)
										{
											/*top hits first*/
											if(CheckDistance(current->dx,current->dy-current->rad,target[i].dx,target[i].dy) < CheckDistance(current->dx-current->rad,current->dy,target[i].dx,target[i].dy)
												&& CheckDistance(current->dx,current->dy-current->rad,target[i].dx,target[i].dy) < CheckDistance(current->dx+current->rad,current->dy,target[i].dx,target[i].dy))
											{
												current->direction_y=1;
											}/*right side hits first*/
											else if (CheckDistance(current->dx+current->rad,current->dy,target[i].dx,target[i].dy) < CheckDistance(current->dx,current->dy-current->rad,target[i].dx,target[i].dy)
												&& CheckDistance(current->dx+current->rad,current->dy,target[i].dx,target[i].dy) < CheckDistance(current->dx,current->dy+current->rad,target[i].dx,target[i].dy))
											{
												current->direction_x=-1;
											}/*diagonal side of the triangle is hit*/
											else
											{
												if(current->direction_y >0 && current->direction_x > 0)
												{
													current->angle-=45*PI/180;
												}
												else if(current->direction_y>0 && current->direction_x < 0 )
												{
													current->direction_y=-1;
													current->direction_x=1;

													if(current->angle*180/PI <45 )
													{
														current->angle+=45*PI/180;
													}
													else if(current->angle*180/PI > 45 )
													{
														current->angle-=45*PI/180;
													}
													else
													{
														/*do nothing*/
													}
												}
												else if(current->direction_y < 0 && current->direction_x < 0)
												{
													current->angle+=45*PI/180;
												}
											}

											--target[i].lives;
											if(target[i].lives <=0)
											{
												target[i].alive=0;
												startExplosion(explosion,target[i].dx,target[i].dy);
											}

											target[i].hit=1;
											collision=1;
											current->last_hit_target_id=i;
											target[i].color=changeColor(target[i].color,target[i].lives);

											al_play_sample(hit_sound,1,0,1,ALLEGRO_PLAYMODE_ONCE, 0);

											if(current->angle*180/PI < 15 )
											{
												current->angle=15*PI/180;
											}
											else if(current->angle*180/PI > 90)
											{
												current->angle=90*PI/180;
											}
										}
									}
									else
									{
										target[i].hit=0;
									}
								}
								else
								{
									target[i].hit=0;
								}
							}break;

							case RIGHT_TR:
							{
								if ((current->dy-current->rad <= target[i].dy+cell_w/2-space/2 && current->dy-current->rad > target[i].dy-cell_w/2+space/2 ) ||
									(current->dy+current->rad >= target[i].dy-cell_w/2+space/2 && current->dy+current->rad < target[i].dy+cell_w/2-space/2))
								{

									if( (current->dy <= target[i].dy+cell_w/2-space/2 && current->dx > target[i].dx+cell_w/2-space/2 && current->dx-current->rad <= target[i].dx+cell_w/2-space/2
										 && current->dx+current->rad > target[i].dx+cell_w/2-space/2) ||
										(current->dy > target[i].dy+cell_w/2-space/2 && ((current->dx+current->rad-2 >= target[i].dx-cell_w/2+space/2 && current->dx+current->rad < target[i].dx+cell_w/2-space/2) ||
											(current->dx-current->rad+2 <= target[i].dx+cell_w/2-space/2 && current->dx-current->rad > target[i].dx-cell_w/2+space/2))) ||
										(fabs((target[i].dx+cell_w/2-space/2) - (current->dx+current->rad+6)) <= fabs(current->dy-(target[i].dy-cell_w/2+space/2)) &&
										current->dx-current->rad < target[i].dx+cell_w/2-space/2 && current->dx <= target[i].dx+cell_w/2-space/2 && current->dy <= target[i].dy+cell_w/2-space/2 &&
										current->dy >= target[i].dy) || (current->dx-current->rad < target[i].dx+cell_w/2-space/2 && current->dx <= target[i].dx+cell_w/2-space/2 && current->dy <= target[i].dy+cell_w/2-space/2 &&
										current->dy <= target[i].dy && fabs((target[i].dy+cell_w/2-space/2) - (current->dy+current->rad+6)) <= fabs(current->dx-(target[i].dx-cell_w/2+space/2)) &&
										current->dx+current->rad >= target[i].dx) )
									{

										if (current->last_hit_target_id!=i)
										{
											/*top hits first*/
											if(CheckDistance(current->dx,current->dy-current->rad,target[i].dx,target[i].dy) < CheckDistance(current->dx-current->rad,current->dy,target[i].dx,target[i].dy)
												&& CheckDistance(current->dx,current->dy-current->rad,target[i].dx,target[i].dy) < CheckDistance(current->dx+current->rad,current->dy,target[i].dx,target[i].dy))
											{
												current->direction_y=1;
											}
											/*left side hits first*/
											else if (CheckDistance(current->dx-current->rad,current->dy,target[i].dx,target[i].dy) < CheckDistance(current->dx,current->dy-current->rad,target[i].dx,target[i].dy)
												&& CheckDistance(current->dx-current->rad,current->dy,target[i].dx,target[i].dy) < CheckDistance(current->dx,current->dy+current->rad,target[i].dx,target[i].dy))
											{
												current->direction_x=1;
											}
											else
											{
												if(current->direction_y >0 && current->direction_x < 0)
												{
													current->angle-=45*PI/180;
												}
												else if(current->direction_y>0 && current->direction_x > 0 )
												{
													current->direction_y=-1;
													current->direction_x=-1;

													if(current->angle*180/PI <45 )
													{
														current->angle+=45*PI/180;
													}
													else if(current->angle*180/PI > 45 )
													{
														current->angle-=45*PI/180;
													}
													else
													{
														/*do nothing*/
													}
												}
												else if(current->direction_y < 0 && current->direction_x > 0)
												{
													current->angle+=45*PI/180;
												}
											}

											--target[i].lives;
											if(target[i].lives <=0)
											{
												target[i].alive=0;
												startExplosion(explosion,target[i].dx,target[i].dy);
											}

											target[i].hit=1;
											collision=1;
											current->last_hit_target_id=i;
											target[i].color=changeColor(target[i].color,target[i].lives);
											al_play_sample(hit_sound,1,0,1,ALLEGRO_PLAYMODE_ONCE, 0);

											if(current->angle*180/PI < 15 )
											{
												current->angle=15*PI/180;
											}
											else if(current->angle*180/PI > 90)
											{
												current->angle=90*PI/180;
											}
										}
									}
									else
									{
										target[i].hit=0;
									}
								}
								else
								{
									target[i].hit=0;
								}
							}break;

							case LEFT_FLIPPED_TR:
							{
								if ((current->dx-current->rad <= target[i].dx+cell_w/2-space/2 && current->dx-current->rad > target[i].dx-cell_w/2+space/2 ) ||
									(current->dx+current->rad >= target[i].dx-cell_w/2+space/2 && current->dx+current->rad < target[i].dx+cell_w/2-space/2))
								{
									if( (current->dx >= target[i].dx-cell_w/2+space/2 && current->dy < target[i].dy-cell_w/2+space/2 && current->dy+current->rad >= target[i].dy-cell_w/2+space/2
										 && current->dy-current->rad < target[i].dy-cell_w/2+space/2) ||
										(current->dx < target[i].dx-cell_w/2+space/2 && ((current->dy+current->rad-2 >= target[i].dy-cell_w/2+space/2 && current->dy+current->rad < target[i].dy+cell_w/2-space/2) ||
											(current->dy-current->rad+2 <= target[i].dy+cell_w/2-space/2 && current->dy-current->rad > target[i].dy-cell_w/2+space/2))) ||
										(fabs((target[i].dy-cell_w/2+space/2) - (current->dy-current->rad-8)) <= fabs(current->dx-(target[i].dx+cell_w/2-space/2)) &&
										current->dy+current->rad > target[i].dy-cell_w/2+space/2 && current->dy >= target[i].dy-cell_w/2+space/2 &&  current->dx >= target[i].dx-cell_w/2+space/2 &&
										current->dx <= target[i].dx) || (current->dy+current->rad > target[i].dy-cell_w/2+space/2 && current->dy >= target[i].dy-cell_w/2+space/2 && current->dx >= target[i].dx-cell_w/2+space/2 &&
										current->dx >= target[i].dx && fabs((target[i].dx-cell_w/2+space/2) - (current->dx-current->rad-8)) <= fabs(current->dy-(target[i].dy+cell_w/2-space/2)) &&
										current->dy-current->rad <= target[i].dy) )
									{

										if (current->last_hit_target_id!=i)
										{
											if(CheckDistance(current->dx,current->dy+current->rad,target[i].dx,target[i].dy) < CheckDistance(current->dx-current->rad,current->dy,target[i].dx,target[i].dy)
											&& CheckDistance(current->dx,current->dy+current->rad,target[i].dx,target[i].dy) < CheckDistance(current->dx+current->rad,current->dy,target[i].dx,target[i].dy))
											{
												current->direction_y=-1;
											}
											else if (CheckDistance(current->dx+current->rad,current->dy,target[i].dx,target[i].dy) < CheckDistance(current->dx,current->dy-current->rad,target[i].dx,target[i].dy)
												&& CheckDistance(current->dx+current->rad,current->dy,target[i].dx,target[i].dy) < CheckDistance(current->dx,current->dy+current->rad,target[i].dx,target[i].dy))
											{
												current->direction_x=-1;
											}
											else
											{
												if(current->direction_y < 0 && current->direction_x > 0)
												{
													current->angle-=45*PI/180;
												}
												else if(current->direction_y < 0 && current->direction_x < 0 )
												{
													current->direction_y=1;
													current->direction_x=1;
													if(current->angle*180/PI <45 )
													{
														current->angle+=45*PI/180;
													}
													else if(current->angle*180/PI > 45 )
													{
														current->angle-=45*PI/180;
													}
													else
													{
														/*do nothing*/
													}
												}
												else if(current->direction_y > 0 && current->direction_x < 0)
												{
													current->angle+=45*PI/180;
												}
											}

											--target[i].lives;
											if(target[i].lives <=0)
											{
												target[i].alive=0;
												startExplosion(explosion,target[i].dx,target[i].dy);
											}

											target[i].hit=1;
											collision=1;
											current->last_hit_target_id=i;
											target[i].color=changeColor(target[i].color,target[i].lives);
											al_play_sample(hit_sound,1,0,1,ALLEGRO_PLAYMODE_ONCE, 0);

											if(current->angle*180/PI < 15 )
											{
												current->angle=15*PI/180;
											}
											else if(current->angle*180/PI > 90)
											{
												current->angle=90*PI/180;
											}
										}
									}
									else
									{
										target[i].hit=0;
									}
								}
								else
								{
									target[i].hit=0;
								}
							}break;

							case RIGHT_FLIPPED_TR:
							{
								if ((current->dx-current->rad <= target[i].dx+cell_w/2-space/2 && current->dx-current->rad > target[i].dx-cell_w/2+space/2 ) ||
									(current->dx+current->rad >= target[i].dx-cell_w/2+space/2 && current->dx+current->rad < target[i].dx+cell_w/2-space/2))
								{
									if( (current->dx <= target[i].dx+cell_w/2-space/2 && current->dy < target[i].dy-cell_w/2+space/2 && current->dy+current->rad >= target[i].dy-cell_w/2+space/2
										 && current->dy-current->rad < target[i].dy-cell_w/2+space/2) ||
										(current->dx > target[i].dx+cell_w/2-space/2 && ((current->dy+current->rad-2 >= target[i].dy-cell_w/2+space/2 && current->dy+current->rad < target[i].dy+cell_w/2-space/2) ||
											(current->dy-current->rad+2 <= target[i].dy+cell_w/2-space/2 && current->dy-current->rad > target[i].dy-cell_w/2+space/2))) ||
										(fabs((target[i].dy-cell_w/2+space/2) - (current->dy-current->rad-8)) <= fabs(current->dx-(target[i].dx-cell_w/2+space/2)) &&
										current->dy+current->rad > target[i].dy-cell_w/2+space/2 && current->dy >= target[i].dy-cell_w/2+space/2 &&  current->dx <= target[i].dx+cell_w/2-space/2 &&
										current->dx >= target[i].dx) || (current->dy+current->rad > target[i].dy-cell_w/2+space/2 && current->dy >= target[i].dy-cell_w/2+space/2 && current->dx <= target[i].dx+cell_w/2-space/2 &&
										current->dx <= target[i].dx && fabs((target[i].dx+cell_w/2-space/2) - (current->dx+current->rad+8)) <= fabs(current->dy-(target[i].dy+cell_w/2-space/2)) &&
										current->dy-current->rad <= target[i].dy) )
									{
										if (current->last_hit_target_id!=i)
										{
											if(CheckDistance(current->dx,current->dy+current->rad,target[i].dx,target[i].dy) < CheckDistance(current->dx-current->rad,current->dy,target[i].dx,target[i].dy)
											&& CheckDistance(current->dx,current->dy+current->rad,target[i].dx,target[i].dy) < CheckDistance(current->dx+current->rad,current->dy,target[i].dx,target[i].dy))
											{
												current->direction_y=-1;
											}
											else if (CheckDistance(current->dx-current->rad,current->dy,target[i].dx,target[i].dy) < CheckDistance(current->dx,current->dy-current->rad,target[i].dx,target[i].dy)
												&& CheckDistance(current->dx-current->rad,current->dy,target[i].dx,target[i].dy) < CheckDistance(current->dx,current->dy+current->rad,target[i].dx,target[i].dy))
											{
												current->direction_x=1;
											}
											else
											{
												if(current->direction_y < 0 && current->direction_x < 0)
												{
													current->angle-=45*PI/180;
												}
												else if(current->direction_y < 0 && current->direction_x > 0 )
												{
													current->direction_y=1;
													current->direction_x=-1;
													if(current->angle*180/PI <45 )
													{
														current->angle+=45*PI/180;
													}
													else if(current->angle*180/PI > 45 )
													{
														current->angle-=45*PI/180;
													}
													else
													{
														/*do nothing*/
													}
												}
												else if(current->direction_y > 0 && current->direction_x > 0)
												{
													current->angle+=45*PI/180;
												}
											}

											--target[i].lives;
											if(target[i].lives <=0)
											{
												target[i].alive=0;
												startExplosion(explosion,target[i].dx,target[i].dy);
											}

											target[i].hit=1;
											collision=1;
											current->last_hit_target_id=i;
											target[i].color=changeColor(target[i].color,target[i].lives);
											al_play_sample(hit_sound,1,0,1,ALLEGRO_PLAYMODE_ONCE, 0);

											if(current->angle*180/PI < 15 )
											{
												current->angle=15*PI/180;
											}
											else if(current->angle*180/PI > 90)
											{
												current->angle=90*PI/180;
											}
										}
									}
									else
									{
										target[i].hit=0;
									}
								}
								else
								{
									target[i].hit=0;
								}
							}break;

							case PLUS:
							{
								if(CheckDistance(current->dx,current->dy,target[i].dx,target[i].dy) <= current->rad+target_rad)
								{
									current->last_hit_target_id=i;
									target[i].alive=0;
									(player->balls)++;
									startPlusOneAnim(plus_one_anim,target[i].dx,target[i].dy);
									al_play_sample(plus_sound,1,0,1,ALLEGRO_PLAYMODE_ONCE, 0);
								}

							}break;

							case HORIZ_DECR:
							{
								if(CheckDistance(current->dx,current->dy,target[i].dx,target[i].dy) <= current->rad+target_rad)
								{
									if(!(target[i].hit) && !(current->hit))
									{
										for(j=0;j<size;j++)
										{
											if(target[j].alive)
											{
												if(target[j].figure_id == SQUARE || target[j].figure_id == LEFT_TR || target[j].figure_id == RIGHT_TR ||
				 									target[j].figure_id == LEFT_FLIPPED_TR || target[j].figure_id == RIGHT_FLIPPED_TR)
	                                            {
	                                                if(target[j].row_num == target[i].row_num )
													{
														target[j].lives--;
														if(target[j].lives <=0)
														{
															target[j].alive=0;
															startExplosion(explosion,target[j].dx,target[j].dy);
														}
														target[j].color=changeColor(target[j].color,target[j].lives);
													}
												}
											}
										}
										target[i].hit=1;
										current->hit=1;
										current->last_hit_target_id=i;

										startDecrAnimation(decr_animation,target[i].dx,target[i].dy,'H');
										al_play_sample(laser_sound,1,0,1,ALLEGRO_PLAYMODE_ONCE, 0);
									}
								}
								else
								{
									target[i].hit=0;
									current->hit=0;
								}
							}break;

							case VERT_DECR:
							{

								if(CheckDistance(current->dx,current->dy,target[i].dx,target[i].dy) <= current->rad+target_rad)
								{
									if(!(target[i].hit) && !(current->hit))
									{
										for(j=0;j<size;j++)
										{
											if(target[j].alive)
											{
												if(target[j].figure_id == SQUARE || target[j].figure_id == LEFT_TR || target[j].figure_id == RIGHT_TR ||
				 									target[j].figure_id == LEFT_FLIPPED_TR || target[j].figure_id == RIGHT_FLIPPED_TR)
	                                            {
	                                                if(target[j].col_num == target[i].col_num )
													{
														target[j].lives--;
														if(target[j].lives <=0)
														{
															target[j].alive=0;
															startExplosion(explosion,target[j].dx,target[j].dy);
														}
														target[j].color=changeColor(target[j].color,target[j].lives);
													}
												}
											}
										}
										target[i].hit=1;
										current->hit=1;
										current->last_hit_target_id=i;

										startDecrAnimation(decr_animation,target[i].dx,target[i].dy,'V');
										al_play_sample(laser_sound,1,0,1,ALLEGRO_PLAYMODE_ONCE, 0);
									}
								}
								else
								{
									target[i].hit=0;
									current->hit=0;
								}

							}break;

							case BOUNCER:
							{
								if(CheckDistance(current->dx,current->dy,target[i].dx,target[i].dy) <= current->rad+target_rad)
								{
									if(!(target[i].hit))
									{
										if(current->last_hit_target_id!=i)
										{
											if(current->direction_y==1)
											{
												current->direction_y=-1;
											}

											current->direction_x*=-1 ;

											current->angle+=(rand()%75*PI/180) * (rand()%2==1 ? 1 : -1);

											if(current->angle*180/PI < 15 )
											{
												current->angle=15*PI/180;
											}
											else if(current->angle*180/PI > 90)
											{
												current->angle=90*PI/180;
											}

											target[i].hit=1;
											current->last_hit_target_id=i;
											collision=1;
											al_play_sample(bounce_sound,1,0,1,ALLEGRO_PLAYMODE_ONCE, 0);
										}
									}
								}
								else
								{
									target[i].hit=0;
								}

							}break;

							case COIN:
							{
								if(CheckDistance(current->dx,current->dy,target[i].dx,target[i].dy) <= current->rad+target_rad)
								{
									target[i].alive=0;
									(player->coins)++;
									al_play_sample(coin_sound,1,0,1,ALLEGRO_PLAYMODE_ONCE, 0);
									current->last_hit_target_id=i;
								}
							}break;

							default:break;
						}
					}
				}
			}
		}
		else if(!(current->alive) && current->shot)
		{
			if(fabs(new_dx-current->dx) > current->speed )
			{
				current->direction_x = ((new_dx > current->dx) ? 1 : -1 );

				current->dx=current->dx+current->speed*current->direction_x;
			}
			else if(fabs(new_dx-current->dx) <= current->speed )
			{
				current->dx=new_dx;
				current->dy=player->dy-current->rad;
			}
		}

		current=current->rest;
	}
}

void drawBalls(ball_t *ball_h)
{
	ball_t *current;

	current=ball_h;

	while(current!=NULL)
	{
		if(!current->alive)
			al_draw_filled_circle(current->dx-current->rad, current->dy, current->rad, al_map_rgb(255,255,255));
		else
		{
			al_draw_filled_circle(current->dx, current->dy, current->rad, al_map_rgb(255,255,255));
		}

		current=current->rest;
	}
}

double CheckDistance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void force_balls_2_start_pos(ball_t * ball_h,double new_dx)
{
	ball_t * current;
	current=ball_h;

	while(current!=NULL)
	{
		current->dx=new_dx;
		current=current->rest;
	}
}