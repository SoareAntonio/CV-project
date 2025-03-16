#include <iostream>
#include <graphics.h>
#include <winbgim.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#define MAX 20
#define FUNDAL BLACK
#define FUNDAL_TABLA WHITE
#define latura 100
using namespace std;


int n, nrjucatori=1, jucator = 1, tabla_dimensiune = 9, latime, inaltime, up, dir_left, nrmutari, border_buton_dim=2;
int tabla_joc[20][20];

int directie_i[] = {0, 0, 1, -1}, directie_j[] = {1, -1, 0, 0};
const char* image_path ="./brid.png";
int imaginex, imaginey, level_bot=2;

struct buton{
    int x1, x2, y1, y2, mx, my;
    char text[40];
    bool status;
};

void game();
void butonshow();
void tabla_draw();
void afisare_tabla();
void line_draw();
void multiplayer();
bool win(int jucator);
void win_screen(int winner);
void fundal_draw();
void setings_changes();
void singleplayer();
void bot_easy(int& linie, int& coloana);
void bot_medium(int& linie, int& coloana);
void bot_hard(int ultima_linie, int ultima_coloana, int& linie, int& coloana);

int main(){
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_hight = GetSystemMetrics(SM_CYSCREEN);
    initwindow(screen_width, screen_hight, "Bridg-It", -3, -26);
    game();
    closegraph();
    return 0;
}

bool is_move_possible(int new_linie, int new_coloana) {
    return new_linie > 1 &&
            new_linie < 2 * tabla_dimensiune-1  &&
            new_coloana > 1 &&
            new_coloana < 2 * tabla_dimensiune-1  &&
            tabla_joc[new_linie][new_coloana] == 0;
}

void fundal_draw(){
    imaginex = getmaxx();
    imaginey = getmaxy();
    readimagefile(image_path, 0, 0, imaginex, imaginey);
}

void butonshow(buton& k, int dim, int color){
    ///Functia care deseneaza butoanele
    settextstyle(DEFAULT_FONT, HORIZ_DIR, dim);
    int y = textheight(k.text);
    int x = textwidth(k.text);
    k.x1 = k.mx - x/2;
    k.x2 = k.x1 + x;
    k.y1 = k.my - y/2;
    k.y2 = k.y1 + y;
    setcolor(color);
    outtextxy(k.x1, k.y1, k.text);
    int i;
    for(i=0; i<border_buton_dim; i++)
    {
        setcolor(MAGENTA);
        rectangle(k.x1-2-i, k.y1-1-i, k.x2+i, k.y2+i);
    }
}

void tabla_init(){
    ///Functia initializeaza matricea tablei;
    ///Dimensiunea matricei difera in functie de variabila tabla_dimensiune.
    int i, j;
    for(i = 0; i <= 2 * tabla_dimensiune; i++)
    {
        for(j = 0; j <= 2 * tabla_dimensiune; j++)
        {
            if((i % 2 == 1) && (j % 2 == 0))
            {
                tabla_joc[i][j]=1;
            }
            else if((i % 2 == 0) && (j % 2 == 1))
            {
                tabla_joc[i][j]=2;
            }
            else
            {
                tabla_joc[i][j]=0;
            }
        }
    }
    for(i = 0; i <= 2 * tabla_dimensiune; i++)
    {
        tabla_joc[i][0] = 2;
        tabla_joc[0][i] = 1;
        tabla_joc[i][2 * tabla_dimensiune] = 2;
        tabla_joc[2 * tabla_dimensiune][i] = 1;
    }
}

void afisare_tabla(){
    ///Functia care afiseaza matricea tablei in consola
    int i, j;
    for(i = 0; i <= 2 * tabla_dimensiune; i++)
    {
        for(j = 0; j <= 2 * tabla_dimensiune; j++)
        {
            if(tabla_joc[i][j] == 1)
            {
                cout << 1 << ' ';
            }
            else if(tabla_joc[i][j] == 2)
            {
                cout << 2 << ' ';
            }
            else
            {
                cout << 0 << ' ';
            }
        }
        cout << endl;
    }
}

void tabla_draw() {
    ///Functia care deseneaza tabla dupa ce jucatorul a apasat play;
    ///Dimensiunea tablei difera in functie de variabila tabla_dimensiune.
    setbkcolor(FUNDAL);
    clearviewport();
    fundal_draw();
    int i, j;
    latime = inaltime = latura * tabla_dimensiune;
    up = (getmaxy() - latime) / 2;
    dir_left = (getmaxx() - inaltime) / 2;
    setfillstyle(SOLID_FILL, FUNDAL_TABLA);
    bar(dir_left, up, dir_left + latime, up + inaltime);

    int x, y, ok;
    for (i = 0; i < 2 * tabla_dimensiune - 1; i++) {
        if(i%2==0)
        {
            ok = 0;
        }
        else{
            ok = 1;
        }
        for (j = 0; j < 2 * tabla_dimensiune - 1; j++) {
            if ((i == 0 && (j == 0 || j == 2 * tabla_dimensiune - 1)) ||
                (i == 2 * tabla_dimensiune - 1 && (j == 0 || j == 2 * tabla_dimensiune - 1))) {
                ok=!ok;
                continue;
            }
            if (ok == 1)
            {
                if (i % 2 == 1) {
                    setcolor(LIGHTBLUE);
                    setfillstyle(SOLID_FILL, LIGHTBLUE);
                }
                else {
                    setcolor(LIGHTGREEN);
                    setfillstyle(SOLID_FILL, LIGHTGREEN);
                }
                x = dir_left + j * (latura/2) + 50;
                y = up + i * (latura/2) + 50;
                ok = 0;
                fillellipse(x, y, 7, 7);
            }
            else
            {
                ok = 1;
            }
        }
    }

    /// Marginile de la tabla de joc
    int margine_dim = 10;

    setcolor(LIGHTBLUE);
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(dir_left - margine_dim, up, dir_left, up + inaltime);
    bar(dir_left + latime, up, dir_left + latime + margine_dim, up + inaltime);

    setcolor(LIGHTGREEN);
    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(dir_left, up - margine_dim, dir_left + latime, up);
    bar(dir_left, up + inaltime, dir_left + latime, up + inaltime + margine_dim);
}

void line_draw(int linie, int coloana){
    ///Functie care deseneaza liniile si schimba textul cu ce jucator urmeaza sa mute.
    if(jucator == 1)
    {
        settextstyle(3, HORIZ_DIR, 5);
        setcolor(LIGHTGREEN);
        outtextxy(getmaxx()/30+textwidth("Green turn... ")/2-100, getmaxy()/4, "Green turn... ");
        jucator = 2;
        setcolor(LIGHTBLUE);
        setlinestyle(SOLID_LINE, 0, 5);
    }
    else
    {

        settextstyle(3, HORIZ_DIR, 5);
        setcolor(LIGHTBLUE);
        outtextxy(getmaxx()/30+textwidth("  Blue turn...  ")/2-100, getmaxy()/4, "  Blue turn...  ");
        jucator = 1;
        setcolor(LIGHTGREEN);
        setlinestyle(SOLID_LINE, 0, 5);
    }
    tabla_joc[linie][coloana] = jucator;
    if(tabla_joc[linie][coloana-1]==jucator)
    {
        line(dir_left + (coloana - 1) * (latura / 2), up + linie * (latura / 2), dir_left + (coloana + 1) * (latura / 2), up + linie * (latura / 2));
    }
    else
    {
        line(dir_left + coloana * (latura / 2), up + (linie - 1) * (latura / 2), dir_left + coloana * (latura / 2), up + (linie + 1) * (latura / 2));
    }
}

bool win(int juc){
    ///Facem win(1/2) unu dintre jucatori si il verificam daca el castiga cu ultima mutare facuta sau nu;
    int vali[100], valj[100], cont=0, actual=0, i, j;
    int mat_bordata[100][100];

    ///Creem o noua matrice care este exact ca si cea din tabla_joc doar ca bordata cu 3 pe margini.
    for(i=0; i<=2*tabla_dimensiune; i++)
    {
        for(j=0; j<=2*tabla_dimensiune; j++)
        {
            mat_bordata[i+1][j+1]=tabla_joc[i][j];
        }
    }
    for(i=0; i<=tabla_dimensiune*2+2; i++)
    {
        mat_bordata[i][0]=mat_bordata[i][tabla_dimensiune*2+2] = 3;
    }
    for(i=0; i<=tabla_dimensiune*2+2; i++)
    {
        mat_bordata[0][i] = mat_bordata[tabla_dimensiune*2+2][i] =3;
    }

    ///Scrierea matricei bordate in consola
    /*
    for(i=0; i<=tabla_dimensiune*2+2; i++)
    {
        for(j=0; j<=tabla_dimensiune*2+2; j++)
        {
            cout << mat_bordata[i][j] << ' ';
        }
        cout << endl;
    }
    */

    ///verificam in functie de fiecare jucator.
    ///In functie de ce jucator este jerificat noi avem puncte diferite de verificare. Unele pentru jucatorul albastru si altele pentru jucatorul verde;
    ///juc == 1 -> VERDE/GREEN
    ///juc == 2 -> ALBASTRU/BLUE
    if(juc == 1)
    {
        vali[cont]=1;
        valj[cont]=2;
        cont++;
        ///finalul in 2*tabla_dimensiune, 2*tabla_dimensiune;
        while(actual<cont)
        {
            if(vali[actual] == 2*tabla_dimensiune+1 && valj[actual] == 2*tabla_dimensiune+1)
            {
                return true;
            }
            else
            {
                for(i=0; i<4; i++)
                {
                    if(mat_bordata[vali[actual]+directie_i[i]][valj[actual]+directie_j[i]] == mat_bordata[vali[actual]][valj[actual]])
                    {
                        vali[cont]=vali[actual]+directie_i[i];
                        valj[cont]=valj[actual]+directie_j[i];
                        cont++;
                    }
                }
                mat_bordata[vali[actual]][valj[actual]] = 3;
                actual++;
            }
        }
    }
    else if(juc == 2)
    {
        vali[cont] = 2;
        valj[cont] = 1;
        cont++;
        ///finalul in 1, 2*tabla_dimensiune;
        while(actual<cont)
        {
            if(vali[actual] == 2 && valj[actual] == 2*tabla_dimensiune+1)
            {
                return true;
            }
            else
            {
                for(i=0; i<4; i++)
                {
                    if(mat_bordata[vali[actual]+directie_i[i]][valj[actual]+directie_j[i]] == mat_bordata[vali[actual]][valj[actual]])
                    {
                        vali[cont]=vali[actual]+directie_i[i];
                        valj[cont]=valj[actual]+directie_j[i];
                        cont++;
                    }
                }
                mat_bordata[vali[actual]][valj[actual]] = 3;
                actual++;
            }
        }
    }
    return false;
}

void win_screen(int winner){
    ///Prima data fixam fundalul
    imaginex = getmaxx();
    imaginey = getmaxy();
    readimagefile(image_path, 0, 0, imaginex, imaginey);
    int titlu_x, titlu_y;
    if(winner == 1)
    {
        ///Funcal pentru jucatorul verde, daca catiga;
        setcolor(LIGHTGREEN);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 10);
        titlu_x = (getmaxx() - textwidth("Winner is GREEN!!!!")) / 2;
        titlu_y = (getmaxy() - textheight("Winner is GREEN!!!!")) / 4;
        outtextxy(titlu_x, titlu_y, "Winner is GREEN!!!!");
        titlu_x = (getmaxx() - textwidth("CONGRATS!")) / 2;
        titlu_y = (getmaxy() - textheight("CONGRATS!")) / 4 * 2;
        outtextxy(titlu_x, titlu_y, "CONGRATS!");
    }
    else
    {
        ///Funcal pentru jucatorul albastru, daca catiga;
        setcolor(LIGHTBLUE);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 10);
        titlu_x = (getmaxx() - textwidth("Winner is BLUE!!!!")) / 2;
        titlu_y = (getmaxy() - textheight("Winner is BLUE!!!!")) / 4;
        outtextxy(titlu_x, titlu_y, "Winner is BLUE!!!!");
        titlu_x = (getmaxx() - textwidth("CONGRATS!")) / 2;
        titlu_y = (getmaxy() - textheight("CONGRATS!")) / 4 * 2;
        outtextxy(titlu_x, titlu_y, "CONGRATS!");
    }
    buton menu;
    menu.status = false;
    menu.mx = getmaxx()/2;
    menu.my = getmaxy()/4*3;
    strcpy(menu.text, "Back to menu");
    if(winner == 2)
    {
        butonshow(menu, 5, LIGHTBLUE);
    }
    else
    {
        butonshow(menu, 5, LIGHTGREEN);
    }
    int x, y;
    while(!menu.status)
    {
        x = mousex();
        y = mousey();
        if(ismouseclick(WM_LBUTTONDBLCLK))
        {
            clearmouseclick(WM_LBUTTONDBLCLK);
            if(x>=menu.x1 && x<=menu.x2 && y>=menu.y1 && y<=menu.y2)
            {
                cout << "Back to menu after win." << endl;
                menu.status=true;
                return;
            }
        }
    }
}

void multiplayer(){
    nrmutari = 0;
    if(jucator == 1)
    {
        setcolor(LIGHTBLUE);
        settextstyle(3, HORIZ_DIR, 5);
        outtextxy(getmaxx()/30+textwidth("  Blue turn...  ")/2-100, getmaxy()/4, "  Blue turn...  ");
    }
    else
    {
        setcolor(LIGHTGREEN);
        settextstyle(3, HORIZ_DIR, 5);
        outtextxy(getmaxx()/30+textwidth("Green turn... ")/2-100, getmaxy()/4, "Green turn... ");
    }

    bool gata = false, undo = 1;
    int linie, coloana;

    buton reset, menu;
    reset.status = menu.status = false;

    settextstyle(3, HORIZ_DIR, 5);
    reset.mx = getmaxx()/20+textwidth("Green turn... ")/2;
    reset.my = getmaxy()/4*2;
    strcpy(reset.text, "Restart");
    menu.mx = getmaxx()/20+textwidth("Green turn... ")/2;
    menu.my = getmaxy()/4*3;
    strcpy(menu.text, "Menu");

    settextstyle(3, HORIZ_DIR, 5);
    butonshow(reset, 5, WHITE);
    butonshow(menu, 5, WHITE);
    /// Daca vrei sa faci cu mai mult de 6 va trebuii sa vezi cum merge cu dimensiunile de la butoane si unde sa le pui

    int x, y, winner=0;
    while(!gata)
    {
        x = mousex();
        y = mousey();
        if(ismouseclick(WM_LBUTTONDBLCLK))
        {
            clearmouseclick(WM_LBUTTONDBLCLK);
            if(x > dir_left + latura && x < dir_left + latime - latura && y > up + latura && y < up + inaltime - latura){
                reset.status = false;
                linie = (y-up+20) / (latura/2);
                coloana = (x-dir_left+20) / (latura/2);
                if(tabla_joc[linie][coloana] == 0)
                {
                    line_draw(linie, coloana);
                    nrmutari++;
                    if(nrmutari>=tabla_dimensiune)
                    {
                        if(jucator == 2)
                        {
                            if(win(2))
                            {
                                winner = 2;
                            }
                        }
                        else if(jucator == 1)
                        {
                            if(win(1))
                            {
                                winner = 1;
                            }
                        }
                    }
                    if(winner)
                    {
                        win_screen(winner);
                        gata = true;
                    }
                }
            }
            else
            {
                if(x>=reset.x1 && x<=reset.x2 && y>=reset.y1 && y<=reset.y2)
                {
                    if(reset.status == false)
                    {
                        butonshow(reset, 4, WHITE);
                        reset.status = true;
                        menu.status = true;
                        jucator=1;
                        nrmutari=0;
                        tabla_init();
                        tabla_draw();
                        multiplayer();
                        cout << "BUTON RESET APASAT" << endl;
                    }
                }
                else
                {
                    if(reset.status == true)
                    {
                        butonshow(reset, 4, WHITE);
                        reset.status = false;
                    }
                }
                if(x>=menu.x1 && x<=menu.x2 && y>=menu.y1 && y<=menu.y2)
                {
                    if(menu.status == false)
                    {
                        butonshow(menu, 4, WHITE);
                        menu.status = true;
                        jucator=1;
                        nrmutari=0;
                        cout << "BUTON MENU APASAT" << endl;
                        break;
                    }
                }
                else
                {
                    if(menu.status == true)
                    {
                        butonshow(menu, 4, WHITE);
                        menu.status = false;
                    }
                }
            }
        }
    }
}

void setings_changes(){
    setcolor(FUNDAL);
    clearviewport();
    fundal_draw();
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 10);
    int titlu_x = (getmaxx()) / 2 - textwidth("Settings")/2;
    int titlu_y = (getmaxy() - textheight("Settings")) / 8;
    outtextxy(titlu_x, titlu_y, "Settings");

    ///Numar jucatori
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 6);
    int jucatori_x = (getmaxx() - textwidth("No. Players")) / 6 - 50;
    int jucatori_y = (getmaxy() - textheight("No. Players")) / 6 * 2;
    outtextxy(jucatori_x, jucatori_y, "No. Players");

    ///Dimensiune tabla
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 6);
    int tabdim_x = (getmaxx() - textwidth("Table Dimention")) / 6*5 + 50;
    int tabdim_y = (getmaxy() - textheight("Table Dimention")) / 6 * 2;
    outtextxy(tabdim_x, tabdim_y, "Table Dimention");

    ///Dificultate - intra doar daca jucatori == 1;
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 6);
    int dificultate_x = (getmaxx() - textwidth("Difficulty Single Player")) / 2;
    int dificultate_y = (getmaxy() - textheight("Difficulty Single Player")) / 12 * 7;
    if(nrjucatori == 1)
    {
        outtextxy(dificultate_x, dificultate_y, "Difficulty Single Player");
    }

    buton easy, medium, difficult, jucatori_1, jucatori_2, dim4, dim5, dim6, dim7, dim8, dim9, menu;
    easy.status = medium.status = difficult.status = jucatori_1.status = jucatori_2.status = dim4.status = dim5.status = dim6.status = menu.status = 0;

    ///Legate de dificultate (_x, _y)
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    dificultate_x = getmaxx()/2;
    easy.mx = dificultate_x/4*2, easy.my = dificultate_y + 100;
    strcpy(easy.text, "Easy");
    if(nrjucatori == 1)
    {
        butonshow(easy, 6, WHITE);
    }

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    medium.mx = dificultate_x, medium.my = dificultate_y + 100;
    strcpy(medium.text, "Mediu");
    if(nrjucatori == 1)
    {
        butonshow(medium, 6, WHITE);
    }

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    difficult.mx = dificultate_x/4*6, difficult.my = dificultate_y + 100;
    strcpy(difficult.text, "Hard");
    if(nrjucatori == 1)
    {
        butonshow(difficult, 6, WHITE);
    }

    ///Legate de jucatori (_x, _y);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    jucatori_1.mx = jucatori_x + (textwidth("No. Players")*6/4/2) - textwidth("1")/2 - 75, jucatori_1.my = jucatori_y + 150;
    strcpy(jucatori_1.text, "1");
    butonshow(jucatori_1, 6, WHITE);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    jucatori_2.mx = jucatori_x +(textwidth("No. Players")*6/4/2) - textwidth("2")/2 + 75, jucatori_2.my = jucatori_y + 150;
    strcpy(jucatori_2.text, "2");
    butonshow(jucatori_2, 6, WHITE);

    ///Legate de tabdim (_x, _y);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    dim4.mx = tabdim_x + textwidth("Table Dimention")*6/4/2 - 100 - textwidth("4")/2, dim4.my = tabdim_y + 100;
    strcpy(dim4.text, "4");
    butonshow(dim4, 6, WHITE);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    dim5.mx = tabdim_x + textwidth("Table Dimention")*6/4/2, dim5.my = tabdim_y + 100;
    strcpy(dim5.text, "5");
    butonshow(dim5, 6, WHITE);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    dim6.mx = tabdim_x + textwidth("Table Dimention")*6/4/2 + 100 + textwidth("6")/2, dim6.my = tabdim_y + 100;
    strcpy(dim6.text, "6");
    butonshow(dim6, 6, WHITE);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    dim7.mx = tabdim_x + textwidth("Table Dimention")*6/4/2 - 100 - textwidth("7")/2, dim7.my = tabdim_y + 200;
    strcpy(dim7.text, "7");
    butonshow(dim7, 6, WHITE);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    dim8.mx = tabdim_x + textwidth("Table Dimention")*6/4/2, dim8.my = tabdim_y + 200;
    strcpy(dim8.text, "8");
    butonshow(dim8, 6, WHITE);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    dim9.mx = tabdim_x + textwidth("Table Dimention")*6/4/2 + 100 + textwidth("9")/2, dim9.my = tabdim_y + 200;
    strcpy(dim9.text, "9");
    butonshow(dim9, 6, WHITE);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    menu.mx = getmaxx()/2, menu.my = getmaxy()/12*10;
    strcpy(menu.text, "DONE");
    butonshow(menu, 6, WHITE);

    int x, y, gata_setari=0;
    while(!gata_setari)
    {
        x = mousex();
        y = mousey();
        if(ismouseclick(WM_LBUTTONDBLCLK))
        {
            clearmouseclick(WM_LBUTTONDBLCLK);
            if(x>=menu.x1 && x<=menu.x2 && y>=menu.y1 && y<=menu.y2)
            {
                if(menu.status == false)
                {
                    butonshow(menu, 5, WHITE);
                    menu.status=true;
                    gata_setari = 1;
                }
            }
            else if(x>=jucatori_1.x1 && x<=jucatori_1.x2 && y>=jucatori_1.y1 && y<=jucatori_1.y2)
            {
                nrjucatori = 1;
                cout << "SINGLEPLAYER" << endl;
            }
            else if(x>=jucatori_2.x1 && x<=jucatori_2.x2 && y>=jucatori_2.y1 && y<=jucatori_2.y2)
            {
                nrjucatori = 2;
                cout << "MULTIPLAYER" << endl;
            }
            else if(x>=dim4.x1 && x<=dim4.x2 && y>=dim4.y1 && y<=dim4.y2)
            {
                tabla_dimensiune = 4;
                cout << "Dimensiune tabla 4" << endl;
            }
            else if(x>=dim5.x1 && x<=dim5.x2 && y>=dim5.y1 && y<=dim5.y2)
            {
                tabla_dimensiune = 5;
                cout << "Dimensiune tabla 5" << endl;
            }
            else if(x>=dim6.x1 && x<=dim6.x2 && y>=dim6.y1 && y<=dim6.y2)
            {
                tabla_dimensiune = 6;
                cout << "Dimensiune tabla 6" << endl;
            }
            else if(x>=dim7.x1 && x<=dim7.x2 && y>=dim7.y1 && y<=dim7.y2)
            {
                tabla_dimensiune = 7;
                cout << "Dimensiune tabla 7" << endl;
            }
            else if(x>=dim8.x1 && x<=dim8.x2 && y>=dim8.y1 && y<=dim8.y2)
            {
                tabla_dimensiune = 8;
                cout << "Dimensiune tabla 8" << endl;
            }
            else if(x>=dim9.x1 && x<=dim9.x2 && y>=dim9.y1 && y<=dim9.y2)
            {
                tabla_dimensiune = 9;
                cout << "Dimensiune tabla 9" << endl;
            }
            else if(x>=easy.x1 && x<=easy.x2 && y>=easy.y1 && y<=easy.y2)
            {
                level_bot = 1;
                cout << "Bot Easy" << endl;
            }
            else if(x>=medium.x1 && x<=medium.x2 && y>=medium.y1 && y<=medium.y2)
            {
                level_bot = 2;
                cout << "Bot Medium" << endl;
            }
            else if(x>=difficult.x1 && x<=difficult.x2 && y>=difficult.y1 && y<=difficult.y2)
            {
                level_bot = 3;
                cout << "Bot Hard" << endl;
            }
        }
    }
}

void singleplayer(){
    nrmutari = 0;
    if(jucator == 1)
    {
        setcolor(LIGHTBLUE);
        settextstyle(3, HORIZ_DIR, 5);
        outtextxy(getmaxx()/30+textwidth(" Blue turn...  ")/2 - 100, getmaxy()/4, "  Blue turn...  ");
    }
    else
    {
        setcolor(LIGHTGREEN);
        settextstyle(3, HORIZ_DIR, 5);
        outtextxy(getmaxx()/30+textwidth("Green turn...")/2 - 100, getmaxy()/4, "Green turn...");
    }

    bool gata = false, undo = 1;
    int linie, coloana, ultima_linie, ultima_coloana;

    buton reset, menu;
    reset.status = menu.status = false;

    settextstyle(3, HORIZ_DIR, 5);
    reset.mx = getmaxx()/20+textwidth("Green turn...")/2;
    reset.my = getmaxy()/4*2;
    strcpy(reset.text, "Restart");
    menu.mx = getmaxx()/20+textwidth("Green turn...")/2;
    menu.my = getmaxy()/4*3;
    strcpy(menu.text, "Menu");

    settextstyle(3, HORIZ_DIR, 5);
    butonshow(reset, 5, WHITE);
    butonshow(menu, 5, WHITE);

    int x, y, winner=0;
    while(!gata)
    {
        x = mousex();
        y = mousey();
        if(ismouseclick(WM_LBUTTONDBLCLK))
        {
            clearmouseclick(WM_LBUTTONDBLCLK);
            if(x > dir_left + latura && x < dir_left + latime - latura && y > up + latura && y < up + inaltime - latura){
                reset.status = false;
                linie = (y-up+20) / (latura/2);
                coloana = (x-dir_left+20) / (latura/2);
                if(tabla_joc[linie][coloana] == 0)
                {
                    line_draw(linie, coloana);
                    nrmutari++;
                    if(win(jucator))
                    {
                        winner = jucator;
                    }
                    if(winner)
                    {
                        win_screen(winner);
                        gata = true;
                    }
                    else
                    {
                        if(level_bot == 1)
                        {
                            bot_easy(linie, coloana);
                            line_draw(linie, coloana);
                        }
                        else if(level_bot == 2)
                        {
                            bot_medium(linie, coloana);
                            line_draw(linie, coloana);
                        }
                        else
                        {
                            bot_hard(ultima_linie, ultima_coloana, linie, coloana);
                            line_draw(linie, coloana);
                        }
                        ultima_linie = linie;
                        ultima_coloana = coloana;
                        nrmutari++;
                        if(win(jucator))
                        {
                            winner = jucator;
                        }
                        if(winner)
                        {
                            gata=true;
                            win_screen(jucator);
                        }
                    }
                }
            }
            else
            {
                if(x>=reset.x1 && x<=reset.x2 && y>=reset.y1 && y<=reset.y2)
                {
                    if(reset.status == false)
                    {
                        settextstyle(3, HORIZ_DIR, 5);
                        butonshow(reset, 5, WHITE);
                        reset.status = true;
                        menu.status = true;
                        jucator=1;
                        nrmutari=0;
                        tabla_init();
                        tabla_draw();
                        singleplayer();
                        cout << "BUTON RESET APASAT" << endl;
                    }
                }
                else
                {
                    if(reset.status == true)
                    {
                        settextstyle(3, HORIZ_DIR, 5);
                        butonshow(reset, 5, WHITE);
                        reset.status = false;
                    }
                }
                if(x>=menu.x1 && x<=menu.x2 && y>=menu.y1 && y<=menu.y2)
                {
                    if(menu.status == false)
                    {
                        settextstyle(3, HORIZ_DIR, 5);
                        butonshow(menu, 5, WHITE);
                        menu.status = true;
                        jucator=1;
                        nrmutari=0;
                        cout << "BUTON MENU APASAT" << endl;
                        break;
                    }
                }
                else
                {
                    if(menu.status == true)
                    {
                        settextstyle(3, HORIZ_DIR, 5);
                        butonshow(menu, 5, WHITE);
                        menu.status = false;
                    }
                }
            }
        }
    }
}

void bot_easy(int& linie, int& coloana) {
    int mutare = 0;
    int x_min = dir_left + latura;
    int x_max = dir_left + latime - latura;
    int y_min = up + latura;
    int y_max = up + inaltime - latura;

    while (!mutare) {
        int x = rand() % (x_max - x_min + 1) + x_min;
        int y = rand() % (y_max - y_min + 1) + y_min;
        linie = (y - up + 20) / (latura / 2);
        coloana = (x - dir_left + 20) / (latura / 2);

        if (tabla_joc[linie][coloana] == 0) {
            tabla_joc[linie][coloana] = jucator;
            mutare = 1;
        }
    }
}

void bot_medium(int& linie, int& coloana) {
    if (nrmutari == 0)
    {
        bot_easy(linie, coloana);
    }
    else
    {
        int mutare = 0, i, d, new_linie, new_coloana;
        int directii[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        for (d = 0; d < 4 && mutare == 0; d++)
        {
            for (i = 1; i <= tabla_dimensiune && mutare == 0; i++) {
                new_linie = linie + i * directii[d][0];
                new_coloana = coloana + i * directii[d][1];
                if (is_move_possible(new_linie, new_coloana))
                {
                    tabla_joc[new_linie][new_coloana] = jucator;
                    linie = new_linie;
                    coloana = new_coloana;
                    mutare = 1;
                }
            }
        }
        if (mutare == 0)
        {
            bot_easy(linie, coloana);
        }
    }
}

void bot_hard(int ultima_linie, int ultima_coloana, int& linie, int& coloana) {
    int coadai[100] = {0}, coadaj[100] = {0}, k = 1, miscare = 0, i;
    int directie_i[4] = {0, 0, 1, -1}, directie_j[4] = {1, -1, 0, 0};
    coadai[1] = ultima_linie;
    coadaj[1] = ultima_coloana;

    if (nrmutari < 1) {
        bot_easy(linie, coloana);
    } else {
        if ((tabla_joc[ultima_linie][ultima_coloana - 1] == jucator) && (tabla_joc[ultima_linie][ultima_coloana + 1] == jucator)) {
            coadaj[++k] = ultima_coloana - 1;
            coadai[k] = ultima_linie;
            coadaj[++k] = ultima_coloana + 1;
            coadai[k] = ultima_linie;
        }
        if ((tabla_joc[ultima_linie - 1][ultima_coloana] == jucator) && (tabla_joc[ultima_linie + 1][ultima_coloana] == jucator)) {
            coadai[++k] = ultima_linie - 1;
            coadaj[k] = ultima_coloana;
            coadai[++k] = ultima_linie + 1;
            coadaj[k] = ultima_coloana;
        }
        k = 1;
        while (miscare == 0 && coadai[k] != 0 && coadaj[k] != 0) {
            for (i = 0; i < 4; ++i) {
                int new_linie = coadai[k] + directie_i[i];
                int new_coloana = coadaj[k] + directie_j[i];
                if (is_move_possible(new_linie, new_coloana)) {
                    coadai[++k] = new_linie;
                    coadaj[k] = new_coloana;
                }
            }
            for (i = 0; i < 4; ++i) {
                int new_linie = coadai[k] + directie_i[i];
                int new_coloana = coadaj[k] + directie_j[i];
                if (is_move_possible(new_linie, new_coloana)) {
                    tabla_joc[new_linie][new_coloana] = jucator;
                    linie = new_linie;
                    coloana = new_coloana;
                    miscare = 1;
                    break;
                }
            }
            k++;
        }
        if (miscare == 0) {
            bot_medium(linie, coloana);
        }
    }
}

void game(){
    setcolor(FUNDAL);
    clearviewport();
    fundal_draw();
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 9);
    int titlu_x = (getmaxx() - textwidth("Bridg-It")) / 2;
    int titlu_y = (getmaxy() - textheight("Bridg-It")) / 4;
    outtextxy(titlu_x, titlu_y, "Bridg-It");


    bool exit = 0;
    buton play, ext, setings;

    play.mx = getmaxx()/2;
    play.my = getmaxy()/12*5;
    strcpy(play.text, "Play");
    play.status = false;
    butonshow(play, 6, WHITE);

    ext.mx = getmaxx()/2;
    ext.my = getmaxy()/12*9;
    strcpy(ext.text, "Exit");
    ext.status = false;
    butonshow(ext, 6, WHITE);

    setings.mx = getmaxx()/2;
    setings.my = play.my + (ext.my-play.my)/2;
    strcpy(setings.text, "Settings");
    setings.status = false;
    butonshow(setings, 6, WHITE);

    int x, y;
    int ok=0;

    while(!exit)
    {
        x = mousex();
        y = mousey();

        if(ismouseclick(WM_LBUTTONDBLCLK))
        {
            clearmouseclick(WM_LBUTTONDBLCLK);
            if(x>=play.x1 && x<=play.x2 && y>=play.y1 && y<=play.y2)
            {
                tabla_init();
                afisare_tabla();
                tabla_draw();
                if(nrjucatori == 1)
                {
                    singleplayer();
                }
                else
                {
                    multiplayer();
                }
                setcolor(FUNDAL);
                clearviewport();
                fundal_draw();
                setcolor(WHITE);
                settextstyle(DEFAULT_FONT, HORIZ_DIR, 9);
                outtextxy(titlu_x, titlu_y, "Bridg-It");
                butonshow(play, 6, WHITE);
                butonshow(ext, 6, WHITE);
                butonshow(setings, 6, WHITE);
            }
            else if(x>=setings.x1 && x<=setings.x2 && y>=setings.y1 && y<=setings.y2)
            {
                setings.status = true;
                setings_changes();
                setcolor(FUNDAL);
                clearviewport();
                fundal_draw();
                setcolor(WHITE);
                settextstyle(DEFAULT_FONT, HORIZ_DIR, 9);
                outtextxy(titlu_x, titlu_y, "Bridg-It");
                butonshow(play, 6, WHITE);
                butonshow(ext, 6, WHITE);
                butonshow(setings, 6, WHITE);
            }
            else if(x>=ext.x1 && x<=ext.x2 && y>=ext.y1 && y<=ext.y2)
            {
                exit = 1;
            }
        }
        else
        {
            if(x>=play.x1 && x<=play.x2 && y>=play.y1 && y<=play.y2)
            {
                if(play.status == false)
                {
                    butonshow(play, 6, WHITE);
                    play.status = true;
                }
            }
            else
            {
                if(play.status == true)
                {
                    butonshow(play, 6, WHITE);
                    play.status = false;
                }
            }
            if(x>=setings.x1 && x<=setings.x2 && y>=setings.y1 && y<=setings.y2)
            {
                if(setings.status == false)
                {
                    butonshow(setings, 6, WHITE);
                    setings.status = true;
                }
            }
            else
            {
                if(setings.status == true)
                {
                    butonshow(setings, 6, WHITE);
                    setings.status = false;
                }
            }
            if(x>=ext.x1 && x<=ext.x2 && y>=ext.y1 && y<=ext.y2)
            {
                if(ext.status == false)
                {
                    butonshow(ext, 6, WHITE);
                    ext.status = true;
                }
            }
            else
            {
                if(ext.status == true)
                {
                    butonshow(ext, 6, WHITE);
                    ext.status = false;
                }
            }

        }
    }
}





