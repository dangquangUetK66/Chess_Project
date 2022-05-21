#include "gameManager1Player.h"

Vector2f offset(28, 28);

int board[8][8] =
{ -1,-2,-3,-4,-5,-3,-2,-1,
-6,-6,-6,-6,-6,-6,-6,-6,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
6, 6, 6, 6, 6, 6, 6, 6,
1, 2, 3, 4, 5, 3, 2, 1 };

///////////////////// AI Alpha - Beta //////////////////////////////////////////
int GameManager::Alpha_Beta(int depth, bool luot, int alpha, int beta)
{
    if (depth == 0) {
        return CostMove();
    }
    Vector2f positiveMovetemp[32];//luu lai vi tri cac nuoc co the di
    if (luot == true) {
        int bestMove = -10000;//gia cua bestMove ban dau
        for (int j = 16; j < 32; j++)//cac quan cua nguoi choi
        {
            if (f[j].s.getPosition() == Vector2f(-100, -100)) continue;
            PositiveMoving(j);
            int coun = positiveCount;//ta khong the dung PositiveCount vi no thay doi lien tuc khi ta de quy
            positiveCount = 0;
            for (int i = 0; i < coun; i++)  positiveMovetemp[i] = positiveMove[i];
            for (int i = 0; i < coun; i++)
            {
                move(j, f[j].s.getPosition(), positiveMovetemp[i]);
                bestMove = max(bestMove, Alpha_Beta(depth - 1, !luot, alpha, beta));
                //undo
                Undo();
                alpha = max(alpha, bestMove);
                if (beta <= alpha)  return bestMove;
            }
        }
        return bestMove;
    }
    else {
        int bestMove = 10000;//gia cua bestMove ban dau
        for (int j = 0; j < 16; j++)//quan cua may
        {
            if (f[j].s.getPosition() == Vector2f(-100, -100)) continue;
            PositiveMoving(j);
            int coun = positiveCount;//ta khong the dung PositiveCount vi no thay doi lien tuc khi ta de quy
            positiveCount = 0;
            for (int i = 0; i < coun; i++)  positiveMovetemp[i] = positiveMove[i];
            for (int i = 0; i < coun; i++)
            {
                move(j, f[j].s.getPosition(), positiveMovetemp[i]);
                bestMove = min(bestMove, Alpha_Beta(depth - 1, !luot, alpha, beta));
                //undo
                Undo();
                beta = min(beta, bestMove);
                if (beta <= alpha)  return bestMove;
            }
        }
        return bestMove;
    }
}

// tim vi tri nuoc co the di
void GameManager::IncreasePositive(int i, int j) {
    positiveMove[positiveCount] = Vector2f(i * Size, j * Size) + offset;
    positiveCount++;
}

// Di chuyen quan co
void GameManager::move(int n, Vector2f oldPos, Vector2f newPos)
{
    posS.push(oldPos);
    posS.push(newPos);
    nS.push(n);
    int y = int((newPos - offset).y / 56);//kiem tra xem co phong hau hay khong
    //phong hau cho tot
    if (y == 0 && f[n].index == 6) {
        nS.push(100);//de ty undo xoa phong hau di
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
            f[i].s.setPosition(-100, -100);//di chuyen em bi an ra khoi man hinh
            posS.push(newPos);
            posS.push(Vector2f(-100, -100));
            nS.push(i);
            nBiS.push(f[i].index);
            break;//neu ta dat f[n].s.setPosition(newPos) len truoc ma ko co break=> bi mat not con nay
        }
    }
    f[n].s.setPosition(newPos);
}


// Undo
void GameManager::Undo()
{
    int n = nS.top();
    nS.pop();
    Vector2f p = posS.top();//kiem tra xem co = (-100,-100) => day la con bi an
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

    if (p == Vector2f(-100, -100))  Undo();// luc nay moi di chuyen con an
}


// Create board and value for piece
void GameManager::Create()//gan gia tri can thiet vao danh sach Quan co
{
    positiveCount = 0;//so nuoc co the di ban dau duong nhien =0(chua chon gi ca)
    int k = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
        {
            int n = board[i][j];
            if (!n) continue;
            int x = abs(n) - 1;
            int y = n > 0 ? 1 : 0;
            f[k].index = n;
            f[k].s.setTextureRect(IntRect(Size * x, Size * y, Size, Size));
            f[k].s.setPosition(Size * j + offset.x, Size * i + offset.y);
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

// get move for AI
Vector2f GameManager::getNextMove(bool luot)
{
    Vector2f oldPos, newPos, oldPostemp, newPostemp;// ta can tim vi tri co minimax nho nhat de ung voi may( quan den)
    int minimaxtemp = 10000, minimax = 10000;
    int count1, n;
    Vector2f positiveMovetemp[32];

    for (int i = 0; i < 16; i++)
    {
        if (f[i].s.getPosition() == Vector2f(-100, -100)) continue;
        //////
        PositiveMoving(i);
        count1 = positiveCount;//khong the dung PositiveCount vi no thay doi lien tuc khi ta de quy
        positiveCount = 0;
        ///set///
        for (int k = 0; k < count1; k++) positiveMovetemp[k] = positiveMove[k];
        //set oldPos va newPos  tam thoi
        oldPostemp = f[i].s.getPosition();
        //newPostemp=positiveMove[0];
        for (int j = 0; j < count1; j++)
        {
            move(i, oldPostemp, positiveMovetemp[j]);
            int alpha = -9999, beta = 9999;
            int temp = Alpha_Beta(3, !luot, alpha, beta);
            if (minimaxtemp > temp) {
                newPostemp = positiveMovetemp[j];
                minimaxtemp = temp;
            }
            Undo();
        }
        if (minimax > minimaxtemp) {
            minimax = minimaxtemp;
            oldPos = oldPostemp;
            newPos = newPostemp;
            n = i;
        }
    }
    //lay cac thong tin nuoc di
    posS.push(oldPos);//luu tam o trong stack ty ra ngoai xoa di
    nS.push(n);
    return newPos;
}

// Alpha-Beta
int GameManager::CostMove()// don gian con nao bi chet thi khong tinh diem cua con day
{
    int s = 0;
    for (int i = 0; i < 32; i++)
    {
        if (f[i].s.getPosition() == Vector2f(-100, -100)) continue;//neu no da bi out-> ko tinh diem
        s += f[i].cost;
    }
    return s;
}

void GameManager::PositivePawn(int n, int x, int y, int grid[9][9])
{
    int k = grid[x][y] / abs(grid[x][y]);// 1 hoac -1
    if ((y == 1 || y == 6) && grid[x][y - k] == 0 && grid[x][y - 2 * k] == 0 && y - 2 * k >= 0 && y - 2 * k < 8) IncreasePositive(x, y - 2 * k);
    if (grid[x][y - k] == 0 && y - k >= 0 && y - k < 8)            IncreasePositive(x, y - k);
    if (grid[x + 1][y - k] * grid[x][y] < 0 && y - k >= 0 && y - k < 8 && x + 1 < 8)       IncreasePositive(x + 1, y - k);
    if (grid[x - 1][y - k] * grid[x][y] < 0 && y - k >= 0 && y - k < 8 && x - 1 >= 0)      IncreasePositive(x - 1, y - k);
}

void GameManager::PositiveKing(int n, int x, int y, int grid[9][9])//xet 8 vi tri co the di cua vua
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

void GameManager::PositiveKnight(int n, int x, int y, int grid[9][9])//xet 8 vi tri co the di cua ma
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

void GameManager::PositiveBishop(int n, int x, int y, int grid[9][9])//tu vi tri (x,y) xet ra 4 huong cheo
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

void GameManager::PositiveRook(int n, int x, int y, int grid[9][9])//tu vi tri (x,y) xet ra 4 huong ngang
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

// Tim vi tri co the di cua quan co
void GameManager::PositiveMoving(int n)
{
    Vector2f pos = f[n].s.getPosition() - offset;
    int x = pos.x / 56;
    int y = pos.y / 56;

    int grid[9][9];//mang luoi(8x8) luu lai cac vi tri ban co:
    Vector2i vtri;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            grid[i][j] = 0;// neu khong co quan co nao o O nay thi =0
    for (int j = 0; j < 32; j++)
    {
        vtri = Vector2i(f[j].s.getPosition() - offset);
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

// Main game loop
void GameManager::Play(RenderWindow& window)
{
    Texture t1, t2, t3;
    t1.loadFromFile("images//figures.png");
    t2.loadFromFile("images//board.png");
    t3.loadFromFile("images//no.png");

    Time delay = milliseconds(500);

    Clock clock;
    for (int i = 0; i < 32; i++) f[i].s.setTexture(t1);
    Sprite sBoard(t2);
    Sprite sPositive(t3);

    Create();//khoi tao

    bool turn = true;//luot choi : = true=> nguoi ... =false => may
    Vector2f oldPos, newPos;// luu vi tri click lan1 va lan2
    int n = 0, click = 0, count = 0;
    Vector2i pos;//vitri chuot khi click
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
                    pos = Mouse::getPosition(window) - Vector2i(offset);
                    click++;
                }
        }
        // Player move
        if (turn == true)
        {
            // Check if end game //////
            if (nBiS.size() != 0) {
                if (nBiS.top() == 5 || nBiS.top() == -5) {
                    window.close();
                }

                for (int i = 0; i < nBiS.size() - 1; i++) {
                    nBiS.pop();
                }
            }
            /// ///////////////////////

			// Click chon quan co
            if (click == 1) {
                bool isMove = false;
                for (int i = 16; i < 32; i++)
                {
                    if (f[i].s.getGlobalBounds().contains(pos.x + offset.x, pos.y + offset.y))
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
			
			// Click di chuyen quan co
            if (click == 2)
            {
                f[n].s.setColor(Color::White);
                int x = pos.x / Size;
                int y = pos.y / Size;
                newPos = Vector2f(x * Size, y * Size) + offset;
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
        else  //computer moving
        {
            if (nBiS.size() != 0) {
                if (nBiS.top() == 5 || nBiS.top() == -5) {
                    window.close();
                }

                for (int i = 0; i < nBiS.size() - 1; i++) {
                    nBiS.pop();
                }
            }
            newPos = getNextMove(turn);
            int c = nS.top();   nS.pop();//lay dk thong tin roi xoa di
            oldPos = posS.top();  posS.pop();//vi ham move tu nhet trong stack roi
            move(c, oldPos, newPos);
            this_thread::sleep_for(chrono::milliseconds(500));
            turn = !turn;
            //reset 
            click = 0;
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
