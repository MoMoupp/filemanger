#include "game.h"

#define N 4

int arr[N + 2][N + 2] = {0};

int qiut = 0;
/*
{
    {0, 0, 0, 0, 0, 0, 0},

    {0,  0, 0, 0, 0, 0,  0},
    {0,  0, 0, 0, 0, 0,  0},
    {0,  0, 0, 0, 0, 0,  0},
    {0,  0, 0, 0, 0, 0,  0},
    {0,  0, 0, 0, 0, 0,  0},

    {0,  0, 0, 0, 0, 0,   0},
};
*/
char player[N + 2][N + 2];

void game_start(LcdDevice *lcd);
void print_player(LcdDevice *lcd, int i, int j);
int count_mine(int x, int y);
void show(int x, int y, LcdDevice *lcd);
void show_all(LcdDevice *lcd);
int play(LcdDevice *lcd);

void game_over()
{
    for (int i = 0; i < N + 2; i++)
    {
        for (int j = 0; j < N + 2; j++)
        {
            arr[j][i] = 0;
        }
    }
}
/// @brief 扫雷
/// @param lcd 
/// @return 
int game(LcdDevice *lcd)
{

    int x, y, j;
    while (1)
    {
        game_start(lcd);

        int w = play(lcd);

        game_over();
        if (w)
        {
            if (w == 1)
            {
                printf("WIN\n");
                show_all(lcd);
                show_font(lcd, "you win!", 665, 320, 123, 45, 28);
            }
            if (w==2)
            {
                continue;
            }
            
        }
        if (qiut)
        {
            qiut = 0;
            return 0;
        }
        touch(&x, &y, M_TOUH_, &j);
        if (x > 660 && y > 166 && x < 793&& y < 211)
        {
            continue;
        }
        if (x > 660 && y > 223 && x < 793&& y < 269)
        {

            return 0;
        }
    }
    return 0;
}

void game_start(LcdDevice *lcd)
{
    int x1, y1;
    int coun = 8;
    srand((unsigned)time(NULL));
    while (coun)
    {
        x1 = rand() % (N + 2);
        y1 = rand() % (N + 2);
        if (arr[x1][y1] == 0)
        {
            arr[x1][y1] = -1;
            coun--;
        }
    }
    for (int i = 0; i < N + 2; i++)
    {
        for (int j = 0; j < N + 2; j++)
        {
            player[i][j] = '*';
        }
    }
    list new;
    get_bmp(&new, "game.bmp");
    show_pic_inlist(lcd, &new);
    free(new.RGB_data);
}

void show_all(LcdDevice *lcd)
{

    for (int i = 0; i < N + 2; i++)
    {
        for (int j = 0; j < N + 2; j++)
        {
            char s[2];
            sprintf(s, "%c", player[i][j]);
            bitmap *bp = get_font(s, 102, 75, 70);

            char *f = bp->map;
            char *lcd_p = lcd->mptr;

            lcd_p += ((((i % 6) * (102 + 3)) + 13) * 4 + ((((j % 6) * (75 + 3)) + 11) * 800) * 4);

            for (int l = 0; l < 75; l++)
            {
                for (int m = 0; m < 102; m++)
                {
                    lcd_p[m * 4] = f[m * 4 + 2];
                    lcd_p[m * 4 + 1] = f[m * 4 + 1];
                    lcd_p[m * 4 + 2] = f[m * 4];
                }
                lcd_p += 800 * 4;
                f += bp->width * bp->byteperpixel;
            }

            destroyBitmap(bp);
        }
    }
}

void show_die(LcdDevice *lcd, int i, int j)
{
    char s[2];
    sprintf(s, "%c", player[i][j]);
    bitmap *bp = get_font(s, 102, 75, 70);

    char *f = bp->map;
    char *lcd_p = lcd->mptr;

    lcd_p += ((((i % 6) * (102 + 3)) + 13) * 4 + ((((j % 6) * (75 + 3)) + 11) * 800) * 4);

    for (int l = 0; l < 75; l++)
    {
        for (int m = 0; m < 102; m++)
        {
            lcd_p[m * 4] = f[m * 4 + 2];
            lcd_p[m * 4 + 1] = f[m * 4 + 1];
            lcd_p[m * 4 + 2] = f[m * 4];
        }
        lcd_p += 800 * 4;
        f += bp->width * bp->byteperpixel;
    }
    destroyBitmap(bp);
}

void print_player(LcdDevice *lcd, int i, int j)
{

    if (player[i][j] == '*')
    {
        return;
    }
    else if (player[i][j] == '0')
    {

        player[i][j] = ' ';
    }

    char s[2];
    sprintf(s, "%c", player[i][j]);
    bitmap *bp = get_font(s, 102, 75, 70);

    char *f = bp->map;
    char *lcd_p = lcd->mptr;

    lcd_p += ((((i % 6) * (102 + 3)) + 13) * 4 + ((((j % 6) * (75 + 3)) + 11) * 800) * 4);

    for (int l = 0; l < 75; l++)
    {
        for (int m = 0; m < 102; m++)
        {
            lcd_p[m * 4] = f[m * 4 + 2];
            lcd_p[m * 4 + 1] = f[m * 4 + 1];
            lcd_p[m * 4 + 2] = f[m * 4];
        }
        lcd_p += 800 * 4;
        f += bp->width * bp->byteperpixel;
    }

    destroyBitmap(bp);
}

int count_mine(int x, int y)
{
    int count = 0;
    if (x - 1 >= 0 && y - 1 >= 0 && arr[x - 1][y - 1] == -1)
    {
        count++;
    }
    if (x - 1 >= 0 && arr[x - 1][y] == -1)
    {
        count++;
    }
    if (x - 1 >= 0 && y + 1 < N + 2 && arr[x - 1][y + 1] == -1)
    {
        count++;
    }

    if (y - 1 >= 0 && arr[x][y - 1] == -1)
    {
        count++;
    }
    if (y + 1 < N + 2 && arr[x][y + 1] == -1)
    {
        count++;
    }

    if (x + 1 < N + 2 && y - 1 >= 0 && arr[x + 1][y - 1] == -1)
    {
        count++;
    }
    if (x + 1 < N + 2 && arr[x + 1][y] == -1)
    {
        count++;
    }
    if (x + 1 < N + 2 && y + 1 < N + 2 && arr[x + 1][y + 1] == -1)
    {
        count++;
    }

    return count;
}

void show(int x, int y, LcdDevice *lcd)
{
    if (arr[x][y] != -1)
    {
        player[x][y] = count_mine(x, y) + '0';
        print_player(lcd, x, y);
    }
    if (y - 1 >= 0 && arr[x][y - 1] != -1)
    {
        player[x][y - 1] = count_mine(x, y - 1) + '0';
        print_player(lcd, x, y - 1);
    }
    if (y + 1 < N + 2 && arr[x][y + 1] != -1)
    {
        player[x][y + 1] = count_mine(x, y + 1) + '0';
        print_player(lcd, x, y + 1);
    }

    if (y - 1 >= 0 && x - 1 >= 0 && arr[x - 1][y - 1] != -1)
    {
        player[x - 1][y - 1] = count_mine(x - 1, y - 1) + '0';
        print_player(lcd, x - 1, y - 1);
    }
    if (x - 1 >= 0 && arr[x - 1][y] != -1)
    {
        player[x - 1][y] = count_mine(x - 1, y) + '0';
        print_player(lcd, x - 1, y);
    }
    if (x - 1 >= 0 && y + 1 < N + 2 && arr[x - 1][y + 1] != -1)
    {
        player[x - 1][y + 1] = count_mine(x - 1, y + 1) + '0';
        print_player(lcd, x - 1, y + 1);
    }

    if (x + 1 < N + 2 && y - 1 >= 0 && arr[x + 1][y - 1] != -1)
    {
        player[x + 1][y - 1] = count_mine(x + 1, y - 1) + '0';
        print_player(lcd, x + 1, y - 1);
    }
    if (x + 1 < N + 2 && arr[x + 1][y] != -1)
    {
        player[x + 1][y] = count_mine(x + 1, y) + '0';
        print_player(lcd, x + 1, y);
    }
    if (x + 1 < N + 2 && y + 1 < N + 2 && arr[x + 1][y + 1] != -1)
    {
        player[x + 1][y + 1] = count_mine(x + 1, y + 1) + '0';
        print_player(lcd, x + 1, y + 1);
    }
}

void cha_q(int x, int y, LcdDevice *lcd)
{
    x -= 13;
    y -= 11;
    int a = ((x - 3) / 102) % 6;
    int b = ((y - 3) / 76) % 6;
    int i = (a % 6) * (102 + 3) + 13;
    int k = (((b % 6) * (75 + 3)) + 11);
    printf("duan\n");
    show_gif_jpeg(lcd, "qi.jpg", i , k);
}

int play(LcdDevice *lcd)
{
    int a, b, count = 0;
    int abs = 0;
    while (1)
    {

        while (1)
        {
            touch(&a, &b, M_TOUH_, &abs);
            if (a > 13 && b > 11 && a < 640 && b < 470)
            {
                break;
            }
            if (a > 683 && b > 45 && a < 773 && b < 116)
            {
                touch(&a, &b, M_TOUH_, &abs);
                cha_q(a, b, lcd);
                continue;
            }
            
            else if (a > 660 && b > 167 && a < 793 && b < 211)
            {
                return 2;
            }
            else if (a > 660 && b > 223 && a < 793 && b < 269)
            {
                qiut = 1;
                return false;
            }
        }
        printf("x:%d,y:%d\n", a, b);
        a -= 13;
        b -= 11;
        int x = ((a - 3) / 102) % 6;
        int y = ((b - 3) / 76) % 6;

        if (arr[x][y] == -1)
        {
            printf("lose\n");
            show_die(lcd, x, y);
            show_font(lcd, "you lose!", 665, 320, 123, 45, 28);
            return false;
        }

        show(x, y, lcd);

        for (int i = 0; i < N + 2; i++)
        {
            for (int j = 0; j < N + 2; j++)
            {
                if (player[i][j] == '*')
                {
                    count++;
                }
            }
        }
        if (count == 8)
        {
            return true;
        }
        count = 0;
    }
}
