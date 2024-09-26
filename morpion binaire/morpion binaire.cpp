// morpion binaire.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <bitset>

void PrintGrid(unsigned int* game);
void UpdateGrid(unsigned int* game, int place);
void ChangeTurn(unsigned int* game);
void Game(unsigned int* game);
void ResetMask();
bool CheckEmpty(unsigned int* game, int place);

unsigned int maskFull = 0xffffffff;
unsigned int maskEmpty = 0;
unsigned int currentTurn = 0;

//std::cout << "binaire : " << std::bitset<32>(*game) << std::endl;

int main()
{
    unsigned int game = 0;
    
    Game(&game);
   
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

bool CheckEmpty(unsigned int* game, int place)
{
    if ((*game & (0b11 << place * 2))) 
        return true;

    return false;
}

void Game(unsigned int* game)
{
    PrintGrid(game);
    unsigned int place;
    while (true)
    {
        std::cout << "Ou voulez vous jouer ?";
        std::cin >> place;

        if (CheckEmpty(game, place - 1)) {
            std::cout << "La case est deja prise \n";
            continue;
        }

        UpdateGrid(game, place - 1);

        PrintGrid(game);
    }
}