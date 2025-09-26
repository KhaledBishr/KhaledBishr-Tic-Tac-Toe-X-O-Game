#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <bits/locale_facets_nonio.h>

// --- Board Class ---
// Manages the game board state and validates moves.
class Board
{
private:
    std::vector<std::vector<char>> grid;
    const int size = 3;

public:
    // Initializes an empty grid of the given size.
    Board()
    {
        grid = std::vector<std::vector<char>>(size, std::vector<char>(size, ' '));
    }

    // Displays the current state of the board to the console.
    void display() const
    {
        std::cout << "\n";
        for (int i = 0; i < size; ++i)
        {
            std::cout << " ";
            for (int j = 0; j < size; ++j)
            {
                std::cout << (grid[i][j] == ' ' ? ' ' : grid[i][j]);
                if (j < size - 1)
                    std::cout << " | ";
            }
            std::cout << "\n";
            if (i < size - 1)
            {
                std::cout << "---+---+---\n";
            }
        }
        std::cout << "\n";
    }

    // Places a symbol if the move is valid. Returns true on success.
    bool makeMove(int row, int col, char symbol)
    {
        if (!isValidMove(row, col))
            return false;
        grid[row][col] = symbol;
        return true;
    }

    // Checks if the specified coordinates are within bounds and the cell is empty.
    bool isValidMove(int row, int col) const
    {
        if (row < 0 || row >= size || col < 0 || col >= size)
            return false;
        return grid[row][col] == ' ';
    }

    // Checks all win conditions (rows, columns, diagonals) for a given symbol.
    bool checkWin(char symbol) const
    {
        for (int i = 0; i < size; i++)
        {
            bool roWin = true;
            bool colWin = true;
            for (int j = 0; j < size; j++)
            {
                if (grid[i][j] != symbol)
                    roWin = false;
                if (grid[j][i] != symbol)
                    colWin = false;
            }
            if (roWin || colWin)
                return true;
        }
        bool diagonalWin = true;
        for (int i = 0; i < size; i++)
        {
            if (grid[i][size - 1 - i] != symbol)
            {
                diagonalWin = false;
                break;
            }
        }
        if (diagonalWin)
            return true;

        bool antiDiagonalWin = true;
        for (int i = 0; i < size; i++)
        {
            if (grid[i][size - 1 - i] != symbol)
            {
                antiDiagonalWin = false;
                break;
            }
        }
        if (antiDiagonalWin)
            return true;

        return false;
    }

    // Checks if all cells are occupied.
    bool isFull() const
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (grid[i][j] == ' ')
                    return false;
            }
        }
        return false;
    }

    // Returns the character at the specified coordinates.
    char getCell(int row, int col) const
    {
        // Check if the coordinates are within the valid range of the grid.
        if (row >= 0 && row < size && col >= 0 && col < size)
            return grid[row][col];

        return '';
    }

    // Clears all cells to an empty state for a new game.
    void reset()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                grid[i][j] = ' ';
            }
        }
    }

    int getSize() const
    {
        return size;
    }
};

// --- Player Class (Abstract) ---
// Base class for player entities.
class Player
{
protected:
    std::string name;
    char symbol;

public:
    // Constructor for the Player class.
    Player(const std::string &name, char symbol) : name(name),
                                                   symbol(symbol)
    {
    }

    // Pure virtual function to get a move. To be implemented by derived classes.
    virtual void getMove(int &row, int &col) = 0;

    // Returns the player's name.
    std::string getName() const
    {
        return name;
    }

    // Returns the player's symbol.
    char getSymbol() const
    {
        return symbol;
    }

    // Sets a new name for the player.
    void setName(const std::string &newName)
    {
        name = newName;
    }
};

// --- HumanPlayer Class ---
// Handles human player input.
class HumanPlayer : public Player
{
public:
    HumanPlayer(const std::string &name, char symbol) : Player(name, symbol)
    {
        // TODO: Implement HumanPlayer constructor
    }

    void getMove(int &row, int &col) override
    {
        // TODO: Implement human player move input
    }
};

// --- AIPlayer Class ---
// Implements the computer opponent.
class AIPlayer : public Player
{
private:
    Board *board;
    enum Difficulty
    {
        EASY,
        HARD
    };

public:
    Difficulty difficulty;

    // Constructor for the AIPlayer class.
    AIPlayer(const std::string &name, char symbol, Difficulty difficulty, Board *board)
        : Player(name, symbol), difficulty(difficulty), board(board)
    {
    }

    // Determines the AI's move based on the difficulty level.
    void getMove(int &row, int &col) override
    {
        Board board;
        if (difficulty == EASY)
        {
            getRandomMove(board, row, col);
        }
        else
        {
            getBestMove(board, row, col);
        }
    }

    // Finds a random valid move for easy difficulty.
    void getRandomMove(const Board &board, int &row, int &col) const
    {
        int boardSize = board.getSize();

        if (board.isFull())
        {
            row = -1; // Indicate no move is possible
            col = -1;
            return;
        }
        row = rand() % boardSize;
        col = rand() % boardSize;
        while (!board.isValidMove(row, col))
        {
            row = rand() % boardSize;
            col = rand() % boardSize;
        }
    }

    // Finds the optimal move using the Minimax algorithm.
    void getBestMove(Board &board, int &row, int &col) const
    {
        int bestScore = -1000;
        row = -1;
        col = -1;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board.isValidMove(i, j))
                {
                    Board test = board;
                    // int score = minimax(board,0,false);
                    test.makeMove(i, j, this->symbol);
                    int score = minimax(test, 0, false);
                    if (score > bestScore)
                    {
                        bestScore = score;
                        row = i;
                        col = j;
                    }
                }
            }
        }
        row = row;
        col = col;
    }

    // Evaluates the board state for scoring (+10, 0, -10).
    int evaluateBoard(const Board &board) const
    {
        if (board.checkWin(symbol))
            return +10;
        if (board.checkWin((symbol == 'X') ? 'O' : 'X'))
            return -10;
        return 0;
    }

    // Sets the AI difficulty.
    void setDifficulty(Difficulty newDifficulty)
    {
        difficulty = newDifficulty;
    }
};

// --- Game Class ---
// Manages the overall game flow.
class Game
{
private:
    Board board;
    Player *player1;
    Player *player2;
    Player *currentPlayer;
    bool gameOver = false;
    Player *winner = nullptr;

public:
    // Constructor for the Game class.
    Game()
    {
        // TODO: Implement Game constructor
    }

    // Main game entry point.
    void start()
    {
        // TODO: Implement main game loop and menu
    }

    // Displays the game mode selection menu.
    void showMenu()
    {
        std::cout << "===== Tic Tac Toe =====\n";
        std::cout << "1. Player vs Player\n";
        std::cout << "2. Player vs Computer (Easy)\n";
        std::cout << "3. Player vs Computer (Hard)\n";
        std::cout << "Select mode: ";
    }

    // Sets up Player vs Player mode.
    void setupPvP()
    {
        // TODO: Implement PvP setup
    }

    // Sets up Player vs Computer mode.
    void setupPvC(AIPlayer::Difficulty difficulty)
    {
        // TODO: Implement PvC setup
    }

    // Alternates the current player.
    void switchPlayer()
    {
        // TODO: Implement player switch logic
    }

    // Handles human player input and move validation.
    void handleHumanMove(Player *player)
    {
        // TODO: Implement human move handling
    }

    // Handles AI player move calculation and placement.
    void handleAIMove(AIPlayer *aiPlayer)
    {
        // TODO: Implement AI move handling
    }

    // Checks for a win or draw condition.
    bool checkGameEnd()
    {
        if (board.checkWin(currentPlayer->getSymbol()))
        {
            gameOver = true;
            winner = currentPlayer;
            return true;
        }
        if (board.isFull())
        {
            gameOver = true;
            winner = nullptr; // Draw
            return true;
        }
        return false;
    }

    // Displays the game outcome message.
    void displayResult() const
    {
        if (winner)
        {
            std::cout << winner->getName() << " (" << winner->getSymbol() << ") wins!\n";
        }
        else
        {
            std::cout << "It's a draw!\n";
        }
    }

    // Prepares the game for a new round.
    void reset()
    {
        board.reset();
        currentPlayer = player1;
        gameOver = false;
        winner = nullptr;
    }
};

// --- Main Function ---
// The entry point of the program.
int main()
{
    Game ticTacToeGame;    // Create an instance of the Game class
    ticTacToeGame.start(); // Call the start() method to begin the game

    return 0;
}
