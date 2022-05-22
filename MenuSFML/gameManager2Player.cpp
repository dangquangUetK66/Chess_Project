#include "gameManager2Player.h"

Vector2f offset2(28, 28);       // vector dieu chinh vi tri cua player

int board2[8][8] =
{ -1,-2,-3,-4,-5,-3,-2,-1,
-6,-6,-6,-6,-6,-6,-6,-6,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
6, 6, 6, 6, 6, 6, 6, 6,
1, 2, 3, 4, 5, 3, 2, 1 };

/////////////// GameManager2Player //////////////////////////

// Tang so nuoc co the di
void GameManager2Player::IncreasePositive(int i, int j) {
    positiveMove[positiveCount] = Vector2f(i * Size, j * Size) + offset2;
    positiveCount++;
}


// Di chuyen quan co
void GameManager2Player::move(int n, Vector2f oldPos, Vector2f newPos)
{
    posS.push(oldPos);
    posS.push(newPos);
    nS.push(n);
    int y = int((newPos - offset2).y / 56);     //kiem tra xem co phong hau hay khong

    //phong hau cho tot
    if (y == 0 && f[n].index == 6) {
        nS.push(100);   //de ty undo xoa phong hau di
        f[n].index = 4;
        f[n].cost = 90;
        f[n].s.setTextureRect(IntRect(3 * Size, Size, Size, Size));
    }
    if (y == 7 && f[n].index == -6) {
        nS.push(-100);
        f[n].index = -4;
        f[n].cost = -90;
        f[n].s.setTextureRect(IntRect(3 * Size, 0, Size, Size));
    }

    //di chuyen em an vao vi tri moi 
    for (int i = 0; i < 32; i++) {
        if (f[i].s.getPosition() == newPos) {
            f[i].s.setPosition(-100, -100);     //di chuyen em bi an ra khoi man hinh
            posS.push(newPos);
            posS.push(Vector2f(-100, -100));
            nS.push(i);
            nBiS.push(f[i].index);
            break;      //neu ta dat f[n].s.setPosition(newPos) len truoc ma ko co break=> bi mat not con nay
        }
    }
    f[n].s.setPosition(newPos);
}


// Undo (Press Space to Undo)
void GameManager2Player::Undo()
{
    int n = nS.top();
    nS.pop();
    Vector2f p = posS.top();    //kiem tra xem co = (-100,-100) => day la con bi an
    posS.pop();
    Vector2f q = posS.top();
    posS.pop();
    if (n == 100) {
        n = nS.top();
        nS.pop();
        f[n].index = 6;
        f[n].cost = 10;
        f[n].s.setTextureRect(IntRect(5 * Size, Size, Size, Size));
    }
    if (n == -100) {
        n = nS.top();
        nS.pop();
        f[n].index = -6;
        f[n].cost = -10;
        f[n].s.setTextureRect(IntRect(5 * Size, 0, Size, Size));
    }
    f[n].s.setPosition(q);

    if (p == Vector2f(-100, -100))  Undo(); // luc nay moi dy chuyen con an
}

// Khoi tao gia tri cho quan co
void GameManager2Player::Create()   //gan gia tri can thiet vao danh sach Quan co
{
    positiveCount = 0;  //so nuoc co the di ban dau duong nhien =0(chua chon gi ca)
    int k = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
        {
            int n = board2[i][j];
            if (!n) continue;
            int x = abs(n) - 1;
            int y = n > 0 ? 1 : 0;
            f[k].index = n;
            f[k].s.setTextureRect(IntRect(Size * x, Size * y, Size, Size));
            f[k].s.setPosition(Size * j + offset2.x, Size * i + offset2.y);
            //cost
            int v = 0, g;
            g = abs(f[k].index);
            if (g == 1)  v = 50;
            else if (g == 2 || g == 3)  v = 30;
            else if (g == 4)  v = 90;
            else if (g == 5)  v = 900;
            else if (g == 6)  v = 10;
            f[k].cost = f[k].index / g * v;
            k++;
        }
    }
}

void GameManager2Player::PositivePawn(int n, int x, int y, int grid[9][9])
{
    int k = grid[x][y] / abs(grid[x][y]);// 1 hoac -1
    if ((y == 1 || y == 6) && grid[x][y - k] == 0 && grid[x][y - 2 * k] == 0 && y - 2 * k >= 0 && y - 2 * k < 8) IncreasePositive(x, y - 2 * k);
    if (grid[x][y - k] == 0 && y - k >= 0 && y - k < 8)            IncreasePositive(x, y - k);
    if (grid[x + 1][y - k] * grid[x][y] < 0 && y - k >= 0 && y - k < 8 && x + 1 < 8)       IncreasePositive(x + 1, y - k);
    if (grid[x - 1][y - k] * grid[x][y] < 0 && y - k >= 0 && y - k < 8 && x - 1 >= 0)      IncreasePositive(x - 1, y - k);
}

void GameManager2Player::PositiveKing(int n, int x, int y, int grid[9][9])//xet 8 vi tri co the di cua vua
{
    if ((grid[x + 1][y] == 0 || grid[x][y] * grid[x + 1][y] < 0) && x + 1 < 8)     IncreasePositive(x + 1, y);
    if ((grid[x - 1][y] == 0 || grid[x][y] * grid[x - 1][y] < 0) && x - 1 >= 0)     IncreasePositive(x - 1, y);
    if ((grid[x + 1][y + 1] == 0 || grid[x][y] * grid[x + 1][y + 1] < 0) && x + 1 < 8 && y + 1 < 8)  IncreasePositive(x + 1, y + 1);
    if ((grid[x - 1][y + 1] == 0 || grid[x][y] * grid[x - 1][y + 1] < 0) && x - 1 >= 0 && y + 1 < 8)  IncreasePositive(x - 1, y + 1);
    if ((grid[x][y + 1] == 0 || grid[x][y] * grid[x][y + 1] < 0) && y + 1 < 8)     IncreasePositive(x, y + 1);
    if ((grid[x - 1][y - 1] == 0 || grid[x][y] * grid[x - 1][y - 1] < 0) && x - 1 >= 0 && y - 1 >= 0) IncreasePositive(x - 1, y - 1);
    if ((grid[x + 1][y - 1] == 0 || grid[x][y] * grid[x + 1][y - 1] < 0) && y - 1 >= 0 && x + 1 < 8)  IncreasePositive(x + 1, y - 1);
    if ((grid[x][y - 1] == 0 || grid[x][y] * grid[x][y - 1] < 0) && y - 1 >= 0)     IncreasePositive(x, y - 1);
}

void GameManager2Player::PositiveKnight(int n, int x, int y, int grid[9][9])//xet 8 vi tri co the di cua ma
{
    if ((grid[x + 2][y + 1] == 0 || grid[x][y] * grid[x + 2][y + 1] < 0) && x + 2 < 8 && y + 1 < 8)  IncreasePositive(x + 2, y + 1);
    if ((grid[x + 2][y - 1] == 0 || grid[x][y] * grid[x + 2][y - 1] < 0) && y - 1 >= 0 && x + 2 < 8)  IncreasePositive(x + 2, y - 1);
    if ((grid[x - 2][y + 1] == 0 || grid[x][y] * grid[x - 2][y + 1] < 0) && x - 2 >= 0 && y + 1 < 8)  IncreasePositive(x - 2, y + 1);
    if ((grid[x - 2][y - 1] == 0 || grid[x][y] * grid[x - 2][y - 1] < 0) && x - 2 >= 0 && y - 1 >= 0) IncreasePositive(x - 2, y - 1);
    if ((grid[x + 1][y + 2] == 0 || grid[x][y] * grid[x + 1][y + 2] < 0) && x + 1 < 8 && y + 2 < 8)  IncreasePositive(x + 1, y + 2);
    if ((grid[x - 1][y + 2] == 0 || grid[x][y] * grid[x - 1][y + 2] < 0) && x - 1 >= 0 && y + 2 < 8)  IncreasePositive(x - 1, y + 2);
    if ((grid[x + 1][y - 2] == 0 || grid[x][y] * grid[x + 1][y - 2] < 0) && y - 2 >= 0 && x + 1 < 8)  IncreasePositive(x + 1, y - 2);
    if ((grid[x - 1][y - 2] == 0 || grid[x][y] * grid[x - 1][y - 2] < 0) && x - 1 >= 0 && y - 2 >= 0) IncreasePositive(x - 1, y - 2);
}

void GameManager2Player::PositiveBishop(int n, int x, int y, int grid[9][9])//tu vi tri (x,y) xet ra 4 huong cheo
{
    for (int i = x + 1, j = y + 1; (i < 8 && j < 8); i++, j++)
    {
        if (grid[i][j] != 0) {
            if (grid[i][j] * grid[x][y] < 0)  IncreasePositive(i, j);
            break;
        }
        IncreasePositive(i, j);
    }
    for (int i = x + 1, j = y - 1; (i < 8 && j >= 0); i++, j--)
    {
        if (grid[i][j] != 0) {
            if (grid[i][j] * grid[x][y] < 0)  IncreasePositive(i, j);
            break;
        }
        IncreasePositive(i, j);
    }
    for (int i = x - 1, j = y + 1; (i >= 0 && j < 8); i--, j++)
    {
        if (grid[i][j] != 0) {
            if (grid[i][j] * grid[x][y] < 0)  IncreasePositive(i, j);
            break;
        }
        IncreasePositive(i, j);
    }
    for (int i = x - 1, j = y - 1; (i >= 0 && j >= 0); i--, j--)
    {
        if (grid[i][j] != 0) {
            if (grid[i][j] * grid[x][y] < 0)  IncreasePositive(i, j);
            break;
        }
        IncreasePositive(i, j);
    }
}

void GameManager2Player::PositiveRook(int n, int x, int y, int grid[9][9])//tu vi tri (x,y) xet ra 4 huong ngang
{
    for (int i = x + 1; i < 8; i++)
    {
        if (grid[i][y] != 0) {
            if (grid[i][y] * grid[x][y] < 0)  IncreasePositive(i, y);
            break;
        }
        IncreasePositive(i, y);
    }
    for (int i = x - 1; i >= 0; i--)
    {
        if (grid[i][y] != 0) {
            if (grid[i][y] * grid[x][y] < 0)  IncreasePositive(i, y);
            break;
        }
        IncreasePositive(i, y);
    }
    for (int j = y + 1; j < 8; j++)
    {
        if (grid[x][j] != 0) {
            if (grid[x][j] * grid[x][y] < 0)  IncreasePositive(x, j);
            break;
        }
        IncreasePositive(x, j);
    }
    for (int j = y - 1; j >= 0; j--)
    {
        if (grid[x][j] != 0) {
            if (grid[x][j] * grid[x][y] < 0)  IncreasePositive(x, j);
            break;
        }
        IncreasePositive(x, j);
    }
}

// Cac nuoc co the di cua quan co
void GameManager2Player::PositiveMoving(int n)
{
    Vector2f pos = f[n].s.getPosition() - offset2;
    int x = pos.x / 56;
    int y = pos.y / 56;

    int grid[9][9];//mang luoi(8x8) luu lai cac vi tri ban co:
    Vector2i vtri;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            grid[i][j] = 0;// neu khong co quan co nao o O nay thi =0
    for (int j = 0; j < 32; j++)
    {
        vtri = Vector2i(f[j].s.getPosition() - offset2);
        grid[vtri.x / Size][vtri.y / Size] = f[j].index;// neu co = index cua quan co
    }

    if (abs(f[n].index) == 1)  PositiveRook(n, x, y, grid);//xe
    else if (abs(f[n].index) == 2)  PositiveKnight(n, x, y, grid);//ma
    else if (abs(f[n].index) == 3)  PositiveBishop(n, x, y, grid);//tuong 
    else if (abs(f[n].index) == 4)//hau: hop lai cac nuoc cua ca xe va tuong
    {
        PositiveRook(n, x, y, grid);
        PositiveBishop(n, x, y, grid);
    }
    else if (abs(f[n].index) == 5)  PositiveKing(n, x, y, grid);//vua
    else   PositivePawn(n, x, y, grid); //tot
}

// Game 2 player loop
void GameManager2Player::Play(RenderWindow& window)
{
    Texture t1, t2, t3;
    t1.loadFromFile("images//figures.png");
    t2.loadFromFile("images//board.png");
    t3.loadFromFile("images//no.png");

    for (int i = 0; i < 32; i++) f[i].s.setTexture(t1);
    Sprite sBoard(t2);
    Sprite sPositive(t3);

    Create();//khoi tao

    bool turn = true;// turn : = true => white ... false => yellow
    Vector2f oldPos, newPos;    // luu vi tri click lan1 va lan2
    int n = 0, click = 0, count = 0, checkEndGame = 0;
    Vector2i pos;   //vitri chuot khi click
    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
            ////move back//////
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Space)
                {
                    Undo();
                }
            /////click///////
            if (e.type == Event::MouseButtonPressed)
                if (e.key.code == Mouse::Left)
                {
                    pos = Mouse::getPosition(window) - Vector2i(offset2);
                    click++;
                }
        }
        // Player1 move
        if (turn == true)
        {
			// Check if end game //////////////////////////
            if (nBiS.size() != 0) {
                if (nBiS.top() == 5 || nBiS.top() == -5) {
                    window.close();

                }

                for (int i = 0; i < nBiS.size() - 1; i++) {
                    nBiS.pop();
                }
            }
            ////////////////////////////////////////
            // CLick chon quan co
            if (click == 1) {
                bool isMove = false;
                for (int i = 16; i < 32; i++)
                {
                    if (f[i].s.getGlobalBounds().contains(pos.x + offset2.x, pos.y + offset2.y))
                    {
                        isMove = true;
                        n = i;
                        f[n].s.setColor(Color::Green);
                        oldPos = f[n].s.getPosition();
                    }
                }
                if (!isMove)  click = 0;
                else {
                    PositiveMoving(n);
                    count = positiveCount;
                    positiveCount = 0;
                }
            }
            ///////////////////////////////

            // Click chon nuoc di
            if (click == 2)
            {
                f[n].s.setColor(Color::White);
                int x = pos.x / 56;   int y = pos.y / 56;
                newPos = Vector2f(x * 56, y * 56) + offset2;
                //chi di chuyen trong vung positiveMove
                for (int i = 0; i < count; i++)
                {
                    if (positiveMove[i] == newPos) {
                        move(n, oldPos, newPos);
                        turn = !turn;
                    }
                }
                //reset
                count = 0;
                click = 0;
            }

        }
        else  //Player2 moving
        {
            // Check if end game //////////////////////////
            if (nBiS.size() != 0) {
                if (nBiS.top() == 5 || nBiS.top() == -5) {
                    window.close();
                }

                for (int i = 0; i < nBiS.size() - 1; i++) {
                    nBiS.pop();
                }
            }
            ///////////////////////////////////////////////////
            if (click == 1) {
                bool isMove = false;
                for (int i = 0; i < 16; i++)
                {
                    if (f[i].s.getGlobalBounds().contains(pos.x + offset2.x, pos.y + offset2.y))
                    {
                        isMove = true;
                        n = i;
                        f[n].s.setColor(Color::Green);
                        oldPos = f[n].s.getPosition();
                    }
                }
                if (!isMove)  click = 0;
                else {
                    PositiveMoving(n);
                    count = positiveCount;
                    positiveCount = 0;
                }
            }
            if (click == 2)
            {
                f[n].s.setColor(Color::White);
                int x = pos.x / 56;   int y = pos.y / 56;
                newPos = Vector2f(x * 56, y * 56) + offset2;
                //chi di chuyen trong vung positiveMove
                for (int i = 0; i < count; i++)
                {
                    if (positiveMove[i] == newPos) {
                        move(n, oldPos, newPos);
                        turn = !turn;
                    }
                }
                //reset
                count = 0;
                click = 0;
            }
        }
        ////// draw  ///////
        window.draw(sBoard);
        for (int i = 0; i < count; i++) {
            sPositive.setPosition(positiveMove[i]);
            window.draw(sPositive);
        }
        for (int i = 0; i < 32; i++) {
            window.draw(f[i].s);
        }
        window.display();
    }
}