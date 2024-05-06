#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <Windows.h>
#include <algorithm>

using namespace std;

class player
{
public:
	int index;
	char sign;
	int wins;
	int draws;
	int loses;
	int points;
	player() {
        index = 0;
        sign;
        wins = 0;
        draws = 0;
        loses = 0;
        points = 0;
    }
	player(int id, char sig) {
        index = id;
        sign = sig;
        wins = 0;
        draws = 0;
        loses = 0;
        points = 0;
    }
};

struct TicTacToe {
    vector<vector<char>> board;
};

pair<char, char> checkWinner(const vector<vector<char>>& board, char player1, char player2) {
    char winner = '.';
    char loser = '.';
    //poziome
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != '.' && board[i][0] == board[i][1] && board[i][0] == board[i][2]) {
            winner = board[i][0];
            loser = (winner == player1) ? player2 : player1;
            return { winner, loser };
        }
    }
    //pionowe
    for (int j = 0; j < 3; ++j) {
        if (board[0][j] != '.' && board[0][j] == board[1][j] && board[0][j] == board[2][j]) {
            winner = board[0][j];
            loser = (winner == player1) ? player2 : player1;
            return { winner, loser };
        }
    }
    //ukos
    if (board[0][0] != '.' && board[0][0] == board[1][1] && board[0][0] == board[2][2]) {
        winner = board[0][0];
        loser = (winner == player1) ? player2 : player1;
        return { winner, loser };
    }
    if (board[0][2] != '.' && board[0][2] == board[1][1] && board[0][2] == board[2][0]) {
        winner = board[0][2];
        loser = (winner == player1) ? player2 : player1;
        return { winner, loser };
    }
    bool draw = true;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == '.') {
                draw = false;
                break;
            }
        }
        if (!draw) break;
    }
    if (draw) {
        return { '.', '.' };
    }
    return { winner, loser };
}

pair<char, char> extractPlayerSymbols(const vector<vector<char>>& board) {
    char player1 = '.', player2 = '.';
    bool foundPlayer1 = false, foundPlayer2 = false;
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell != '.' && !foundPlayer1) {
                player1 = cell;
                foundPlayer1 = true;
            }
            else if (cell != '.' && cell != player1 && !foundPlayer2) {
                player2 = cell;
                foundPlayer2 = true;
            }
            if (foundPlayer1 && foundPlayer2) {
                break;
            }
        }
        if (foundPlayer1 && foundPlayer2) {
            break;
        }
    }
    return { player1, player2 };
}

void printGames(const vector<TicTacToe>& games) {
    for (int i = 0; i < games.size(); ++i) {
        cout << "Game " << i + 1 << ":" << endl;
        for (const auto& row : games[i].board) {
            for (char cell : row) {
                cout << cell << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

bool comparePlayersByPoints(const player& a, const player& b) {
    return a.points > b.points;
}

void sortPlayersByPoints(std::vector<player>& players) {
    std::sort(players.begin(), players.end(), comparePlayersByPoints);
}

int main() {
    vector<player> players;
    ifstream plik("players.txt");
    if (!plik.is_open()) {
        cout << "Nie można otworzyć pliku." << endl;
        return 1;
    }
    int index;
    char sign;
    while (plik >> index >> sign) {
        player newplayer(index, sign);
        players.push_back(newplayer);
    }
    plik.close();

    vector<TicTacToe> games;
    ifstream gamesFile("tictactoe.txt");
    if (!gamesFile.is_open()) {
        cout << "Nie można otworzyć pliku z grami." << endl;
        return 1;
    }
    string line;
    TicTacToe currentGame;
    while (getline(gamesFile, line)) {
        if (line.empty()) {
            if (!currentGame.board.empty()) {
                games.push_back(currentGame);
            }
            currentGame = TicTacToe();
        }
        else {
            vector<char> row;
            for (char c : line) {
                row.push_back(c);
            }
            currentGame.board.push_back(row);
        }
    }
    if (!currentGame.board.empty()) {
        games.push_back(currentGame);
    }
    gamesFile.close();

    for (int i = 0; i < games.size(); ++i) {
        pair<char, char> playerSymbols = extractPlayerSymbols(games[i].board);
        pair<char, char> result = checkWinner(games[i].board, playerSymbols.first, playerSymbols.second);
        if (result.first != '.') {
            for (auto& el : players) {
                if (el.sign == result.first) {
                    el.wins+=1;
                    el.points += 3;
                }
            }
            for (auto& el : players) {
                if (el.sign == result.second) {
                    el.loses += 1;
                }
            }
        }
        else {
            for (auto& el : players) {
                if (el.sign == playerSymbols.first) {
                    el.draws += 1;
                    el.points += 1;
                }
                if (el.sign == playerSymbols.second) {
                    el.draws += 1;
                    el.points += 1;
                }
            }
        }

    }
    int option;
    cout << "1 - show table" << endl;
    cout << "2 - show results" << endl;
    cout << "3 - show player stats" << endl;
    cout << "4 - exit" << endl;
    cout << "Choose option: ";
    cin >> option;
    switch (option) {
    case 1:
        sortPlayersByPoints(players);
        cout << "Index" << "\t" << "Sign" << "\t" << "Wins" << "\t" << "Draws" << "\t" << "Loses" << "\t" << "Points" << endl;
        for (auto& el : players) {
            cout << el.index << "\t" << el.sign << "\t" << el.wins << "\t" << el.draws << "\t" << el.loses << "\t" << el.points << endl;
        }
        break;
    case 2: {
        printGames(games);
        break;
    }
    case 3:
        int id;
        cout << "Enter player id: ";
        cin >> id;
        for (auto& el : players) {
            if (id == el.index) {
                cout << "Index" << "\t" << "Sign" << "\t" << "Wins" << "\t" << "Draws" << "\t" << "Loses" << "\t" << "Points" << endl;
                cout << el.index << "\t" << el.sign << "\t" << el.wins << "\t" << el.draws << "\t" << el.loses << "\t" << el.points << endl;
            }
        }
        break;
    case 4:
        cout << "closing...";
        Sleep(1000);
        exit(0);
        break;
    default:
        cout << "Wrong option!" << endl;
        break;
    }
    return 0;
}
