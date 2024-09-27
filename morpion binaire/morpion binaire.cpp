// morpion binaire.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <bitset>

enum GameState { InGame, Tie, Player1Win, Player2Win };

void PrintGrid(unsigned int* game);
void ResetMask();
void UpdateTie(unsigned int* game);
void UpdateGrid(unsigned int* game, int place);
void ChangeTurn(unsigned int* game);
void ChangeState(unsigned int* game, GameState newState);
void Game(unsigned int* game);
bool CheckEmpty(unsigned int* game, int place);
bool CheckTie(unsigned int* game);
int CheckWin(unsigned int* game);

unsigned int maskFull = 0xffffffff;
unsigned int maskEmpty = 0;
unsigned int currentTurn = 0;

//std::cout << "binaire : " << std::bitset<32>(*game) << std::endl;

int main()
{
    unsigned int game = 0;
    int test = 0;
    bool run = true;
    unsigned int state = (game & (0b11 << 19));

    while (run)
    {
        state = (game & (0b11 << 19));
        switch (state)
        {
        case 0b00 << 19:
            Game(&game);
            break;
        case 0b11 << 19:
            std::cout << "Tie \n";
            break;
        case 0b10 << 19:
            std::cout << "Player 1 win \n";
            break;
        case 0b01 << 19:
            std::cout << "Player 2 win ? \n";
            break;
        default:
            std::cout << "C'est pas bon du tout \n";
            break;
        }

    }
}

void PrintGrid(unsigned int* game) 
{
    system("cls");
    for (int i = 0; i < 18; i += 2) 
    {
        if (i % 6 == 0)
            std::cout << "\n";

        if ((*game & (0b11 << i)) == 3 || (*game & (0b11 << i)) == 0) 
        { 
            // si les bits sont égaux a 11 ou 00
            std::cout << " . ";
        }

        else if (*game & (0b01 << i)) {
            // C'est une croix
            std::cout << " x ";
        }
        else if (*game & (0b10 << i)) {
            // C'est un cercle
            std::cout << " o ";
        }
    }
    std::cout << "\n";
}

void ChangeTurn(unsigned int* game)
{
    unsigned int turn = *game & 0b1 << 18;
    turn ^= 0b1 << 18;

    maskFull ^= 0b1 << 18;
    *game &= maskFull;

    maskEmpty |= turn;

    *game |= maskEmpty;

    ResetMask();
}

void ResetMask() 
{
    maskFull = 0xffffffff;
    maskEmpty = 0;

}

void UpdateGrid(unsigned int* game, int place)
{
    currentTurn = *game & 0b1 << 18;

    maskFull ^= 0b11 << place * 2;
    *game &= maskFull;

    if (currentTurn)
        maskEmpty |= 0b01 << place * 2;
    else
        maskEmpty |= 0b10 << place * 2;


    *game |= maskEmpty;

    ResetMask();

    ChangeTurn(game);
}

void UpdateTie(unsigned int* game)
{
}

bool CheckEmpty(unsigned int* game, int place)
{
    if ((*game & (0b11 << place * 2))) 
        return true;

    return false;
}

bool CheckTie(unsigned int* game) 
{
    for (int i = 0; i < 18; i += 2)
    {

        if ((*game & (0b11 << i)) == 3 || (*game & (0b11 << i)) == 0)
            return false;
            
    }

    return true;
}

int CheckWin(unsigned int* game)
{
    for (int i = 0; i < 18; i += 6)
    {
        if ((*game & (0b111111 << i)) == 0b101010 << i)
            return 0;     
        if ((*game & (0b111111 << i)) == 0b010101 << i)
            return 1;
    }
    for (int i = 0; i < 6; i += 2)
    {
        if ((*game & (0b11 << i)) == 0b10 << i && (*game & (0b11 << i + 6)) == 0b10 << i + 6 && (*game & (0b11 << i + 12)) == 0b10 << i + 12)
            return 0;
        if ((*game & (0b11 << i)) == 0b01 << i && (*game & (0b11 << i + 6)) == 0b01 << i + 6 && (*game & (0b11 << i + 12)) == 0b01 << i + 12)
            return 1;
    }

    if ((*game & (0b11)) == 0b10 && (*game & (0b11 << 8)) == 0b10 << 8 && (*game & (0b11 << 16)) == 0b10 << 16)
        return 0;
    if ((*game & (0b11)) == 0b01 && (*game & (0b11 << 8)) == 0b01 << 8 && (*game & (0b11 << 16)) == 0b01 << 16)
        return 1;

    if ((*game & (0b11 << 4)) == 0b10 << 4 && (*game & (0b11 << 8)) == 0b10 << 8 && (*game & (0b11 << 12)) == 0b10 << 12)
        return 0;
    if ((*game & (0b11 << 4)) == 0b01 << 4 && (*game & (0b11 << 8)) == 0b01 << 8 && (*game & (0b11 << 12)) == 0b01 << 12)
        return 1;

    return -1;
}

void ChangeState(unsigned int* game, GameState newState)
{
    maskFull ^= 0b11 << 19;
    *game &= maskFull;

    switch (newState)
    {
    case InGame:
        maskEmpty |= 0b00 << 19;
        break;
    case Tie:
        maskEmpty |= 0b11 << 19;
        break;
    case Player1Win:
        maskEmpty |= 0b10 << 19;
        break;
    case Player2Win:
        maskEmpty |= 0b01 << 19;
        break;

    default:
        break;
    };

    *game |= maskEmpty;
    ResetMask();
}

void Game(unsigned int* game)
{

    PrintGrid(game);
    unsigned int place;
   
    std::cout << "Ou voulez vous jouer ?";
    std::cin >> place;

    if (CheckEmpty(game, place - 1)) {
        std::cout << "La case est deja prise \n";
    }

    UpdateGrid(game, place - 1);

    PrintGrid(game);

    if (CheckWin(game) != -1)
    {
        std::cout << "Victoire";

        if (CheckWin(game) == 0)
            ChangeState(game, Player1Win);
        else
            ChangeState(game, Player2Win);

        return;
    }

    if (CheckTie(game)) 
    {
        std::cout << "Egalite";
        ChangeState(game, Tie);
        return;
    }
}