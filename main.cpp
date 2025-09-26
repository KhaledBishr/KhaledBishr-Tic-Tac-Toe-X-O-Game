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
        // TODO: Implement Board constructor
    }

    // Displays the current state of the board to the console.
    void display() const
    {
        // TODO: Implement board display logic
    }

    // Places a symbol if the move is valid. Returns true on success.
    bool makeMove(int row, int col, char symbol)
    {
        // TODO: Implement move logic and validation
        return false;
    }

    // Checks if the specified coordinates are within bounds and the cell is empty.
    bool isValidMove(int row, int col) const
    {
        // TODO: Implement move validity check
        return false;
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
    Player(const std::string& name, char symbol) : name(name),
                                                   symbol(symbol)
    {
    }

    // TODO: Implement Player constructor


    // Pure virtual function to get a move. To be implemented by derived classes.
    virtual void getMove(int& row, int& col) = 0;

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
    void setName(const std::string& newName)
    {
        name = newName;
    }
};

// --- HumanPlayer Class ---
// Handles human player input.
class HumanPlayer : public Player
{
public:
    HumanPlayer(const std::string& name, char symbol) : Player(name, symbol)
    {
        // TODO: Implement HumanPlayer constructor
    }

    void getMove(int& row, int& col) override
    {
        // TODO: Implement human player move input
    }
};


// --- AIPlayer Class ---
// Implements the computer opponent.
class AIPlayer : public Player
{
public:
    enum Difficulty { EASY, HARD };

    Difficulty difficulty;

    // Constructor for the AIPlayer class.
    AIPlayer(const std::string& name, char symbol, Difficulty difficulty) : Player(name, symbol)
    {
        // TODO: Implement AIPlayer constructor
    }

    // Determines the AI's move based on the difficulty level.
    void getMove(int& row, int& col) override
    {
        // TODO: Implement getMove logic based on difficulty
    }

    // Finds a random valid move for easy difficulty.
    void getRandomMove(const Board& board, int& row, int& col) const
    {
        // TODO: Implement random move selection
    }

    // Finds the optimal move using the Minimax algorithm.
    void getBestMove(Board& board, int& row, int& col) const
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
        // TODO: Implement Minimax algorithm to find the best move
    }

    // Evaluates the board state for scoring (+10, 0, -10).
    int evaluateBoard(const Board& board) const
    {
        // TODO: Implement evaluation function
        if (board.checkWin(symbol)) return +10;
        if (board.checkWin((symbol == 'X') ? 'O' : 'X')) return -10;
        return 0;
    }

    // Sets the AI difficulty.
    void setDifficulty(Difficulty newDifficulty)
    {
        // TODO: Implement setDifficulty logic
    }
};

// --- Game Class ---
// Manages the overall game flow.
class Game
{
private:
    Board board;
    Player* player1;
    Player* player2;
    Player* currentPlayer;

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
        // TODO: Implement menu display
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
    void handleHumanMove(Player* player)
    {
        // TODO: Implement human move handling
    }

    // Handles AI player move calculation and placement.
    void handleAIMove(AIPlayer* aiPlayer)
    {
        // TODO: Implement AI move handling
    }

    // Checks for a win or draw condition.
    bool checkGameEnd()
    {
        // TODO: Implement game end check
        return false;
    }

    // Displays the game outcome message.
    void displayResult() const
    {
        // TODO: Implement result display
    }

    // Prepares the game for a new round.
    void reset()
    {
        // TODO: Implement game reset logic
    }
};

// --- Main Function ---
// The entry point of the program.
int main()
{
    Game ticTacToeGame; // Create an instance of the Game class
    ticTacToeGame.start(); // Call the start() method to begin the game

    return 0;
}
