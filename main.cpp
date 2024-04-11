#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>

using namespace std;

const int BOARD_SIZE = 6;
const int NUM_SHIPS = 3;
const int NUM_PLAYERS = 2;
const int MIN_SIZE_BOAT = 2;
const int MAX_SIZE_BOAT = 5;

class Ship
{
private :
    int size;
    int x_position;
    int y_position;
    bool vertical_rotation;

public :
    Ship()
    {
        size = (rand() % MAX_SIZE_BOAT-1) + MIN_SIZE_BOAT; // la taille est generee aleatoirement entre 2 et 5 d'apres les regles de la bataille navale
        x_position = rand() % BOARD_SIZE;
        y_position = rand() % BOARD_SIZE;
        vertical_rotation = rand() % 2;
    }

    Ship(int chosen_size) // dans la vraie bataille navale, on aura besoin de placer des bateaux de taille prédéfinie.
    {
        size = chosen_size;
        x_position = rand() % BOARD_SIZE;
        y_position = rand() % BOARD_SIZE;
        vertical_rotation = rand() % 2;
    }

    int getSize()
    {
        return size;
    }

    int getXposition()
    {
        return x_position;
    }

    int getYposition()
    {
        return y_position;
    }

    bool getVerticalRotationFlag()
    {
        return vertical_rotation;
    }

};

class Board
{
private:
    char board[BOARD_SIZE][BOARD_SIZE];

public:
    Board()
    {
        char **board = new char*[BOARD_SIZE];

        for(int i = 0; i < BOARD_SIZE; ++i)
        {
            board[i] = new char[BOARD_SIZE];
        }
    }

    char getCoordinates(int line, int row)
    {
        return board[line][row];
    }

    void setCoordinates(int line, int row, char newValue)
    {
        board[line][row] = newValue;
    }

    void initializeBoard()
    {
        for (int length = 0; length<BOARD_SIZE; length++)
        {
            for(int width = 0; width<BOARD_SIZE; width++)
            {
                board[length][width] = '-';
            }
        }
    }

    void printBoard()
    {

        system("cls");

        cout<<' ';
        cout<<' ';

        for (int colonne=0; colonne<BOARD_SIZE; colonne++) //afficher l'indice de colonne
        {
            cout<<colonne+1;
            cout<<' ';
        }

        cout<<endl;

        for(int ligne=0; ligne<BOARD_SIZE; ligne++) //afficher l'indice de ligne...
        {
            cout<<ligne+1;

            if(ligne+1<10) //...en evitant un decalage si le plateau est de taille superieure a 10
            {
                cout<<' ';
            }

            for(int colonne=0; colonne<BOARD_SIZE; colonne++)
            {
                if(board[ligne][colonne]=='0') //si une case bateau est present
                {
                    cout<<'-'; //on cache cette case au joueur
                }
                else
                {
                    cout<<board[ligne][colonne];
                }

                cout<<' ';
            }

            cout<<endl;
        }
    }

    void placeShips()
    {
        for (int i=0; i<NUM_SHIPS; i++)
        {
            int ligne, colonne;


            bool valid_position = false;

            //on repete qu'on a pas genere un bateau ayant un placement valide sur la grille

            while(!valid_position)
            {

                Ship temp_ship;

                // si le bateau s'etend sur la longueur

                if(temp_ship.getVerticalRotationFlag()==false)
                {
                    // si le bord du bateau sort de la grille, on sait deja que ce bateau ne peut pas être place

                    if(temp_ship.getXposition() + temp_ship.getSize() >= BOARD_SIZE)
                    {
                        valid_position = false;
                    }
                    else
                    {
                    // on est surs que le bateau peut etre place sur la grille, c'est exactement ce qu'on fait

                        ligne = temp_ship.getXposition();
                        colonne = temp_ship.getYposition();

                        for(int repetitions=0; repetitions<temp_ship.getSize(); repetitions++) // on place case a case le bateau
                        {
                            if(board[ligne][colonne] == '-')
                            {
                                board[ligne][colonne] = '0';
                                ligne++;
                            }
                        }

                        valid_position = true;
                    }

                }

                // au contraire, si le bateau s'etend dans le sens de la largeur

                if(temp_ship.getVerticalRotationFlag()==true)
                {
                    // meme verification de si le bateau rentre : si oui on le place sinon on retourne false

                    if(temp_ship.getYposition() + temp_ship.getSize() >= BOARD_SIZE)
                    {
                        valid_position = false;
                    }
                    else
                    {

                        ligne = temp_ship.getXposition();
                        colonne = temp_ship.getYposition();

                        for(int repetitions=0; repetitions<temp_ship.getSize(); repetitions++) // on place case a case le bateau
                        {
                            if(board[ligne][colonne] == '-')
                            {
                                board[ligne][colonne] = '0';
                                colonne++;
                            }
                        }

                    valid_position = true;
                    }
                }

            }


        }
    }

    bool isValidInput(int row, int col)
    {
        if(row <= BOARD_SIZE && row > 0 && col <= BOARD_SIZE && col > 0) // si le joueur est dans les bornes du plateau
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool areShipsSunk()
    {
        for (int ligne=0; ligne<BOARD_SIZE; ligne++)
        {
            for(int colonne=0; colonne<BOARD_SIZE; colonne++)
            {
                if(board[ligne][colonne] == '0') // si on tombe sur une case bateau, alors ils n'ont pas tous coule
                {
                    return false;
                }
            }
        }

        //ici, on n'a trouve aucune case bateau, on peut donc affirmer qu'ils ont tous coule
        return true;
    }
};

class Player
{
private:
    string name;
    int score;
    Board board; // chaque joueur a son propre plateau

public:
    Player(string playerName)
    {
        name = playerName;
        score = 0;
        board.initializeBoard();
        board.placeShips();
    }

    string getName()
    {
        return name;
    }

    int getScore()
    {
        return score;
    }

    Board& getBoard() // on passe par reference ici pour modifier le tableau et non sa copie
    {
        return board;
    }

    void incrementScore()
    {
        score = score + 1;
    }
};

class Game
{
private:
    vector<Player> players;
    int currentPlayerIndex;

public:
    Game(const vector<string>& playerNames)
    {
        for(string name : playerNames)
        {
            Player newPlayer(name);
            players.push_back(newPlayer);
        }
    }

    void start()
    {
        cout<<"Bienvenue a la Bataille Navale !!!"<<endl;
        sleep(1);
        cout<<"Vous devez detruire "<<NUM_SHIPS<<" bateaux, pouvant aller d'une longueur de "<<MIN_SIZE_BOAT<<" a "<<MAX_SIZE_BOAT<<" cases."<<endl;
        sleep(5);
        cout<<"Etes vous prets?..."<<endl;
        sleep(1);
        cout<<"C'est parti!"<<endl;
        sleep(1);
        system("cls");

        currentPlayerIndex = 0;
    }

    int play()
    {

        bool gameContinues = true;

        while(gameContinues)
        {

            if(!(players[currentPlayerIndex].getBoard().areShipsSunk()))
            {

                players[currentPlayerIndex].getBoard().printBoard();

                cout<<players[currentPlayerIndex].getName()<<"! a toi de jouer!!!"<<endl;

                bool hasPlayed = false;
                int line;
                int row;

                while(!hasPlayed)
                {
                    cout<<"Ou veux tu tirer?"<<endl;
                    cout<<"Saisis la ligne puis la colonne"<<endl;
                    cin>>line;
                    cin>>row;

                    //si sur le plateau du joueur actuel, la saisie dus coordonnees est correcte
                    if(players[currentPlayerIndex].getBoard().isValidInput(line, row))
                    {
                        hasPlayed = true;

                        if (players[currentPlayerIndex].getBoard().getCoordinates(line-1, row-1)=='0')
                        {
                            players[currentPlayerIndex].getBoard().setCoordinates(line-1,row-1,'X');
                            players[currentPlayerIndex].getBoard().printBoard();
                            cout<<"Vous avez touche un bateau!"<<endl;
                            players[currentPlayerIndex].incrementScore();
                            sleep(2);
                            cout<<players[currentPlayerIndex].getName()<<", votre score actuel est de "
                            <<players[currentPlayerIndex].getScore()<<" points"<<endl;
                            sleep(2);
                        }
                        else
                        {
                            players[currentPlayerIndex].getBoard().setCoordinates(line-1,row-1,'#');
                            players[currentPlayerIndex].getBoard().printBoard();
                            cout<<"Dommage... il n'y a pas de bateau"<<endl;
                            sleep(3);
                        }
                    }
                    else
                    {
                        cout<<"Les coordonnees sont incorrectes"<<endl;
                        sleep(2);
                    }

                }

                if (!(players[currentPlayerIndex].getBoard().areShipsSunk()))
                {
                    currentPlayerIndex = (currentPlayerIndex+1)%NUM_PLAYERS;
                }

            }
            else
            {
                gameContinues = false;
            }
        }

    return currentPlayerIndex;
    }

    void announceWinner(int winnerIndex)
    {
        system("cls");
        cout<<"Et le gagnant est... "<<players[winnerIndex].getName()<<"!!!!!"<<endl;
        cout<<"Liste des scores :"<<endl;

        for(Player onePlayer: players)
        {
            cout<<onePlayer.getName()<<" : "<<onePlayer.getScore()<<" points"<<endl;
        }
    }
};

int main()
{
    srand(time(nullptr));

    vector<string> playerNames;
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        string name;
        cout << "Joueur numero " << i + 1 << ", entrez votre nom : ";
        cin >> name;
        playerNames.push_back(name);
    }

    Game game(playerNames);

    system("cls");

    game.start();
    int winner_id = game.play();
    game.announceWinner(winner_id);

    return 0;
}
