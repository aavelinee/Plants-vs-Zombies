#include <vector>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <algorithm>
#include "rsdl.hpp"
using namespace std;

const int length_of_board = 800;
const int width_of_board = 600;
const int initial_x_position = 40;
const int initial_y_position = 80;
const int size_of_plants = 60;
const int size_of_peas = 20;
const int size_of_zombie = 100;
const int size_of_suns1 = 30;
const int size_of_suns2 = 70;
const int move = 5;
const int zombie_move =1;
const int peashooter_price = 100;
const int sunflower_price = 50;
const int wallnut_price = 50;
const int sun_price = 25;
const int chomper_price = 125;

struct sun
{
    int x,y;
    int sunRand;
    int time_counter_for_removing;
};

struct sun_flower
{
    int x,y;
    int life_time;
    int time_counter_for_making;
};

struct pea_shooter
{
    int x,y;
    int life_time;
};

struct wallnut
{
    int x,y;
    int life_time;    
};

struct zombie
{
    int x,y;
    int life_time;
    int comming_time;
};

struct pea
{
    int x,y;
};

struct chompers
{
    int x,y;
    int life_time;
};

struct level
{
    int wave_length;
    int zombies_sequence;
};

void read_levels(int &waves, vector <int> &zombies_sequence, vector <int> &wave_length)
{
    int temp;
    string a;
    ifstream levels("1.level");
    levels>>a;
    levels>>waves;
    waves++;
    for(int i=0; i<3; i++)
        levels>>a;

    for(int i=0; i<waves; i++)
    {
        levels >> temp; 
        zombies_sequence.push_back(temp);
    }
    for(int i=0; i<2; i++)
        levels>>a;

    for(int i=0; i<waves; i++)
    {
        levels>>temp;
        wave_length.push_back(temp);
    }

    levels.close();
}

void demo_startup(window &win)
{
    win.draw_png("./startup.png",0,0,length_of_board,width_of_board);
    while(true)
    {
    HANDLE(
        LCLICK(
            if(mouse_x>length_of_board-550 && mouse_x<length_of_board-250 && mouse_y>width_of_board-80 && mouse_y<width_of_board-20)
                return;)
        win.update_screen();
        DELAY(15);
        )
    }
}

void demo_background(window &win)
{
    for(int i=0 ;i<230;i++)
    {
        win.draw_bg("./frontyard.png",i,0);
        DELAY(5);
        win.update_screen();
    }
    return;
}

bool check_zombie_life(zombie* &zom)
{
    if(zom->life_time<10)
        return true;
    return false;
}

bool check_sunflower_life(sun_flower* sunFlower)
{
    if(sunFlower->life_time<5*40)
        return true;
    return false;
}

bool check_wallnut_life(wallnut* wallnuts)
{
    if(wallnuts->life_time<72*40)
        return true;
    return false;
}

bool check_peashooter_life(pea_shooter* peashooter)
{
    if(peashooter->life_time<5*40)
        return true;
    return false;
}

bool check_chomper_life(chompers* &chomps)
{
    if(chomps->life_time<7*40)
        return true;
    return false;
}

void initialize_Pea(int x, int y, vector<pea*> &peas ,vector<pea_shooter*> &peashooter )
{
    pea* n = new pea();
    n->x=peashooter.back()->x+50;
    n->y=peashooter.back()->y+5;
    peas.push_back(n);
}

void initialize_suns_of_flowers(int i, vector<sun*> &suns_of_flowers, vector<sun_flower*> &sunFlower)
{
    sun* s= new sun();
    s->x = sunFlower[i]-> x+10;
    s->y = sunFlower[i]-> y+5;
    s-> time_counter_for_removing = 0;
    suns_of_flowers.push_back(s);
}

void initialize_Pea_Shooter(int x, int y, vector<pea_shooter*> &peashooter, vector<pea*> &peas, int &money)
{
    if(money >= peashooter_price)
    {
        pea_shooter* p_s = new pea_shooter();
        p_s->x = x;
        p_s->y = y;
        //-------------------------
        money-=peashooter_price;
        peashooter.push_back(p_s);
        initialize_Pea(x, y, peas, peashooter);
    }
}

void initialize_Sun_Flower(int x, int y, vector<sun_flower*> &sunFlower, vector<sun*> &suns_of_flowers, int &money)
{
    if(money >= sunflower_price)
    {
        sun_flower* s_f = new sun_flower();
        s_f->x = x;
        s_f->y = y;
        s_f->time_counter_for_making = 0;
        money-=sunflower_price;
        sunFlower.push_back(s_f);
    }
}

void initialize_suns_of_sky(int x, int y, vector<sun*> &suns_of_sky, vector<sun_flower*> &sunFlower)
{
    sun* s= new sun();
    s->x = x+10;
    s->y = y+5;
    s->sunRand = rand()%5;
    s->time_counter_for_removing = 0;
    suns_of_sky.push_back(s);
}

void initialize_suns_with_time(vector <sun_flower*> &sunFlower, vector<sun*> &suns_of_flowers)
{
    for(int i=0; i<sunFlower.size(); i++)
        if(sunFlower[i]->time_counter_for_making % 1000 == 100)
            initialize_suns_of_flowers(i, suns_of_flowers, sunFlower);
}

void initialize_Wallnut(int x, int y, vector<wallnut*> &wallnuts, int &money)
{
    if(money >= wallnut_price)
    {
        wallnut* w = new wallnut();
        w->x = x;
        w->y = y;
        //-------------------
        money-=wallnut_price;
        wallnuts.push_back(w);
    }
}

void initialize_chomper(int x, int y, vector<chompers*> &chomps, int &money)
{
    if(money >= chomper_price)
    {
        chompers* ch = new chompers();
        ch->x = x;
        ch->y = y;
        //-----ch->life_time = 0;
        money-= chomper_price;
        chomps.push_back(ch);
    }
}

void initialize_Zombie(int x, int y, vector<zombie*> &zom)
{
    zombie* z = new zombie();
    z->x = x;
    z->y = y;
    zom.push_back(z);
}

bool zombie_vs_peashooter_movement(zombie* &zom , vector <pea_shooter*> &peashooter)
{
    for(int i=0;i<peashooter.size();i++)
        if(zom->x == peashooter[i]->x +40 && peashooter[i]->y < zom->y +50 && peashooter[i]->y > zom->y -50)
          {
            peashooter[i]->life_time++;
            return true;
         }
      return false;     
}

bool zombie_vs_wallnut_movement(zombie* &zom , vector <wallnut*> &wallnuts)
{
    for(int i=0;i<wallnuts.size();i++)
        if(zom->x == wallnuts[i]->x +40 && wallnuts[i]->y < zom->y +50 && wallnuts[i]->y > zom->y -50)
        {
            wallnuts[i]->life_time++;
            return true;
        }  
    return false;
}

bool zombie_vs_chomper_movement(zombie* &zom, vector <chompers*> &chomps)
{
    for(int i=0;i<chomps.size();i++)
        if(zom->x == chomps[i]->x +40 && chomps[i]->y < zom->y +50 && chomps[i]->y > zom->y -50)
        {
            chomps[i]->life_time++;
            return true;
        }
    return false;
}

bool zombie_vs_sunflower_movement(zombie* &zom , vector <sun_flower*> &sunFlower)
{
    for(int i=0;i<sunFlower.size();i++)
        if(zom->x == sunFlower[i]->x +40 && sunFlower[i]->y < zom->y +50 && sunFlower[i]->y > zom->y -50)
            {
            sunFlower[i]->life_time++;
            return true;
          }

    return false;      
}

void draw_wallnut(window &win, wallnut* wallnuts)
{
  if(wallnuts->life_time<=24*40)
    win.draw_png("./Wallnut_body.png",wallnuts->x,wallnuts->y,size_of_plants,size_of_plants);
  else if(wallnuts->life_time>24*40 && wallnuts->life_time<=48*40)
    win.draw_png("./Wallnut_cracked1.png",wallnuts->x,wallnuts->y,size_of_plants,size_of_plants);
  else
    win.draw_png("./Wallnut_cracked2.png",wallnuts->x,wallnuts->y,size_of_plants,size_of_plants);
}

void draw_pea_shooter(window &win, pea_shooter* peashooter)
{
  win.draw_png("./Peashooter.png",peashooter->x,peashooter->y,size_of_plants,size_of_plants);
}

void draw_chomper(window &win, chompers* chomps)
{
    win.draw_png("./HDChomperPvZ2.png", chomps->x, chomps->y, size_of_plants, size_of_plants);
}

void draw_sun_flower(window &win, sun_flower* sunFlower)
{
  sunFlower->time_counter_for_making++;
  win.draw_png("./Sunflower.png",sunFlower->x,sunFlower->y,size_of_plants,size_of_plants);  
}

void removing_suns_with_time(vector <sun*> &all_suns,int i)
{
    if(all_suns[i]->time_counter_for_removing > 200)
      all_suns.erase(all_suns.begin()+i);
}

void draw_sun_of_sky(window &win, vector<sun*> &suns_of_sky, int tick, int i)
{
    int x = suns_of_sky[i]->x;
    int y = suns_of_sky[i]->y;
    suns_of_sky[i]->time_counter_for_removing++;
    win.draw_png(string("./Sun3.png"), x, y, size_of_suns2, size_of_suns2, tick);
    win.draw_png(string("./Sun2.png"), x, y, size_of_suns2, size_of_suns2, -1*tick%360);
    win.draw_png(string("./Sun1.png"), x+20, y+20, size_of_suns1, size_of_suns1);
    removing_suns_with_time (suns_of_sky,i);
}

void draw_suns_of_flowers(window &win, vector <sun*> &suns_of_flowers, int tick, int i)
{
  int x = suns_of_flowers[i]->x;
  int y = suns_of_flowers[i]->y;
  suns_of_flowers[i]->time_counter_for_removing++;
  win.draw_png(string("./Sun3.png"), x, y, size_of_suns2, size_of_suns2, tick);
  win.draw_png(string("./Sun2.png"), x, y, size_of_suns2, size_of_suns2, -1*tick%360);
  win.draw_png(string("./Sun1.png"), x+20, y+20, size_of_suns1, size_of_suns1);
  removing_suns_with_time (suns_of_flowers,i);
}

void draw_zombie(window &win, zombie* &zom, vector<sun_flower*> &sunFlower,
                 vector<wallnut*> &wallnuts, vector<pea_shooter*> &peashooter)
{
    if(zombie_vs_peashooter_movement(zom, peashooter))
    {}
    else if(zombie_vs_wallnut_movement(zom, wallnuts))
    {}
    else if(zombie_vs_sunflower_movement(zom, sunFlower))
    {}
    else
        zom->x-=zombie_move;
    win.draw_png("./Zombie.png",zom->x,zom->y,size_of_zombie,size_of_zombie);
}

void draw_pea(window &win, pea* peas, vector <zombie*> &zom , pea_shooter* peashooter, int &counter)
{
    for(int i=0; i<zom.size(); i++)
        if(peas->x > zom[i]->x && peas->y < zom[i]->y +55 && peas->y > zom[i]->y -55)
        {
            if(counter%60 == 59)
            {
                peas->x = peashooter->x+50;
                zom[i]->life_time++;
            }
            else
                peas->x = 1000;
        }

    for(int i=0;i<zom.size();i++)
    {
        if(peas->x < zom[i]->x && peas->y < zom[i]->y +55 && peas->y > zom[i]->y -55)
        {
            peas->x+= move;
            win.draw_png("./pea.png",peas->x,peas->y,size_of_peas,size_of_peas);
            break;
        }
   }
}

void draw_rect(window &win, int x, int y)
{
   win.fill_rect(x+25, y+25, 15, 15, RED);
}

void pick_suns(int x, int y, vector<sun*> &suns, int &money)
{
   for(int i = suns.size()-1; i >= 0 ;i--)
   {
        if(x > suns[i]->x && x < suns[i]->x + size_of_suns1+10 && y > suns[i]->y && y < suns[i]->y +size_of_suns1+10)
        {
            suns.erase(suns.begin()+i);
            money+=sun_price;
            //win.show_text(money,300,300,BLACK,"FreeSans.ttf",23);
            return;
        } 
    }
}

void sun_movement(sun* &suns_of_sky)
{
    if(suns_of_sky->y < suns_of_sky->sunRand*100 )
        suns_of_sky->y+=move;
}

void deleting_plants(int x, int y, vector<sun_flower*> &sunFlower, vector<pea_shooter*> &peashooter, vector<wallnut*> &wallnuts)
{
    for(int i=0; i<sunFlower.size();i++)
    {
        if(x>sunFlower[i]->x-30 && x<sunFlower[i]->x+30 && y>sunFlower[i]->y-30 && y<sunFlower[i]->y+30)
        {
            sunFlower.erase(sunFlower.begin()+i);
            return;
        }  
    }
    for(int i=0;i<peashooter.size();i++)
    {
        if(x>peashooter[i]->x-30 && x<peashooter[i]->x+30 && y>peashooter[i]->y-30 && y<peashooter[i]->y+30)
        {
            peashooter.erase(peashooter.begin()+i);
            return;
        }  
    }
    for(int i=0;i<wallnuts.size();i++)
    {
        if(x>wallnuts[i]->x-30 && x<wallnuts[i]->x+30 && y>wallnuts[i]->y-30 && y<wallnuts[i]->y+30)
        {
            wallnuts.erase(wallnuts.begin()+i);
            return;
        }  
    }
}

bool End(window &win, vector<zombie*> zom)
{
    for(int i=0;i<zom.size();i++)
        if(zom[i]->x<-100)
        {
            while(true)
            {
                win.show_text("ZOMBIES ARE EATING YOUR BRAIN!!",300,300,BLACK,"FreeSans.ttf",30);
                HANDLE({
                    QUIT({return true;})
                    KEY_PRESS(q , {return true;})
                    })
                win.update_screen();
                DELAY(20);
            }
        }
}

int main()
{
    vector <int> zombies_sequence;
    vector <int> wave_length;
    vector <int> times;
    vector <pea_shooter*> peashooter;
    vector <wallnut*> wallnuts;
    vector <sun_flower*> sunFlower;
    vector <zombie*> zom; 
    vector <pea*> peas;
    vector <sun*> suns_of_sky;
    vector <sun*> suns_of_flowers;
    vector <chompers*> chomps;

    int waves;
    int x = initial_x_position, y = initial_y_position;
    int velocity_of_zombie = 0;
    int tick = 1;
    int money=500;
    int counter = 1;

    window win(length_of_board, width_of_board);
    read_levels(waves, zombies_sequence, wave_length);
    demo_startup(win);
    demo_background(win);

    int k = 0;
    for(int i=0; i< zombies_sequence[0]; i++)
        times.push_back(rand() % wave_length[0] + 1);

    while (true)
   {
        win.draw_bg("./frontyard.png", 230, 0);
        velocity_of_zombie++;
        tick++;

        draw_rect(win, x, y);

        int zombie_rand = rand() % 5 +1;
        int suns_rand = rand() % 9 + 1;
        int probability_of_suns_of_sky = rand() % 5;
    //----------------------------------------------------------------------
        /*for(int i=0; i<times.size(); i++)
            if(counter == times[i])
                initialize_Zombie(length_of_board-100, (zombie_rand*85), zom);*/
    //-----------------------------------------------------------------------
        if (velocity_of_zombie%100 == 0)
            initialize_Zombie (length_of_board-100, (zombie_rand*85)  ,zom);

        if(velocity_of_zombie%170 == 0) 
            if(probability_of_suns_of_sky == 2)
                initialize_suns_of_sky(suns_rand*80 -30, 0, suns_of_sky, sunFlower);

        initialize_suns_with_time(sunFlower, suns_of_flowers);
      
        for(int i=0; i<sunFlower.size(); i++)
            if(check_sunflower_life(sunFlower[i]))
                draw_sun_flower(win, sunFlower[i]);
            else
                sunFlower.erase(sunFlower.begin()+i);

        for(int i=0; i<peas.size(); i++)
            draw_pea(win, peas[i], zom, peashooter[i], tick);

        for(int i=0; i<suns_of_flowers.size(); i++)
            draw_suns_of_flowers(win, suns_of_flowers, tick , i);

        for(int i=0; i<chomps.size(); i++)
            if(check_chomper_life(chomps[i]))
                draw_chomper(win, chomps[i]);

        for(int i=0; i<zom.size(); i++)
            if(check_zombie_life(zom[i]))
                draw_zombie(win, zom[i], sunFlower, wallnuts, peashooter);
            else
                zom.erase(zom.begin()+i);

        for(int i=0; i<peashooter.size(); i++)
            if(check_peashooter_life(peashooter[i]))
                draw_pea_shooter(win, peashooter[i]);
            else
            {
                peashooter.erase(peashooter.begin()+i);
                peas.erase(peas.begin()+i);
            }

        for(int i=0; i<wallnuts.size(); i++)
            if (check_wallnut_life(wallnuts[i]))
                draw_wallnut(win,wallnuts[i]);
            else
                wallnuts.erase(wallnuts.begin()+i);

        for (int i=0; i<suns_of_sky.size(); i++)
        {
            draw_sun_of_sky (win, suns_of_sky, tick, i);
            sun_movement (suns_of_sky[i]);
        }

        HANDLE({
            QUIT (return 0;)
            KEY_PRESS (q,return 0;)
            KEY_PRESS (UP, {y-=95; draw_rect(win, x, y);})
            KEY_PRESS (DOWN, {y+=95; draw_rect(win, x, y);})
            KEY_PRESS (LEFT, {x-=80; draw_rect(win, x, y);})
            KEY_PRESS (RIGHT, {x+=80; draw_rect(win, x, y);})
            KEY_PRESS (p, {initialize_Pea_Shooter(x, y, peashooter, peas, money);})
            KEY_PRESS (s, {initialize_Sun_Flower(x, y, sunFlower, suns_of_flowers, money);})
            KEY_PRESS (w, {initialize_Wallnut(x, y, wallnuts, money);})
            KEY_PRESS(c, {initialize_chomper(x, y, chomps, money);})
            LCLICK ({pick_suns(mouse_x, mouse_y, suns_of_sky, money);
                    pick_suns(mouse_x, mouse_y, suns_of_flowers, money);})
            KEY_PRESS (d , {deleting_plants(x, y, sunFlower, peashooter, wallnuts);})
        });

      win.update_screen();
      counter++;
      /*if (counter % wave_length[k] == 0)
      {
        if (k>waves)
          break;
        times.clear();
        k++;
        for (int i=0; i< zombies_sequence[k]; i++)
          times.push_back(rand()%wave_length[k] + 1);
        counter = 1;
      }*/
      if(End(win,zom))
         break;
      DELAY(25);
   }
}