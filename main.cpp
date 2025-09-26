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
    // Checks all win conditions (rows, columns, diagonals) for a given symbol.
    bool checkWin(char symbol) const
    {
        const int size = 3;

        // Check Rows and Columns (Correctly implemented in original code)
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

        // Check Main Diagonal (Top-Left to Bottom-Right)
        bool mainDiagonalWin = true;
        for (int i = 0; i < size; i++)
        {
            if (grid[i][i] != symbol) // FIX: Must check grid[i][i] for the main diagonal
            {
                mainDiagonalWin = false;
                break;
            }
        }
        if (mainDiagonalWin)
            return true;

        // Check Anti-Diagonal (Top-Right to Bottom-Left)
        bool antiDiagonalWin = true;
        for (int i = 0; i < size; i++)
        {
            if (grid[i][size - 1 - i] != symbol) // This is correct for the anti-diagonal
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

        return ' ';
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
    }

    void getMove(int& row, int& col) override
    {
        std::cout << name << " (" << symbol << "), enter your move (row and column 1-3): ";
        std::cin >> row >> col;

        // Convert to 0-based indexing
        row--;
        col--;
    }
};

// --- AIPlayer Class ---
// Implements the computer opponent.
class AIPlayer : public Player
{
private:
    Board* board;

public:
    enum Difficulty
    {
        EASY,
        HARD
    };

    Difficulty difficulty;

    // Constructor for the AIPlayer class.
    AIPlayer(const std::string& name, char symbol, Difficulty difficulty, Board* board)
        : Player(name, symbol), difficulty(difficulty), board(board)
    {
    }

    // Determines the AI's move based on the difficulty level.
    // Determines the AI's move based on the difficulty level.
    void getMove(int& row, int& col) override
    {
        // FIX: Remove 'Board board;' and use '*this->board' to access the actual game board.
        if (difficulty == EASY)
        {
            getRandomMove(*this->board, row, col); // Pass the actual board state
        }
        else
        {
            getBestMove(*this->board, row, col);   // Pass the actual board state
        }
    }

    // Finds a random valid move for easy difficulty.
    void getRandomMove(const Board& board, int& row, int& col) const
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

    int minimax(Board board, int depth, bool isMaximizing) const
    {
        int score = evaluateBoard(board);
        if (score == 10 || score == -10)
            return score;
        if (board.isFull())
            return 0;

        if (isMaximizing)
        {
            int best = -1000;
            for (int i = 0; i < board.getSize(); i++)
            {
                for (int j = 0; j < board.getSize(); j++)
                {
                    if (board.isValidMove(i, j))
                    {
                        board.makeMove(i, j, symbol);
                        best = std::max(best, minimax(board, depth + 1, false));
                        // REMOVE: board.makeMove(i, j, ' '); // undo is not needed for pass-by-value
                    }
                }
            }
            return best;
        }
        else
        {
            int best = 1000;
            char opponent = (symbol == 'X') ? 'O' : 'X';
            for (int i = 0; i < board.getSize(); i++)
            {
                for (int j = 0; j < board.getSize(); j++)
                {
                    if (board.isValidMove(i, j))
                    {
                        board.makeMove(i, j, opponent);
                        best = std::min(best, minimax(board, depth + 1, true));
                        // REMOVE: board.makeMove(i, j, ' '); // undo is not needed for pass-by-value
                    }
                }
            }
            return best;
        }
    }
    // Finds the optimal move using the Minimax algorithm.
    void getBestMove(Board& board, int& row, int& col) const
    {
        int bestScore = -1000;
        row = -1;
        col = -1;
        for (int i = 0; i < board.getSize(); i++) // FIX: Use board.getSize() for loops
        {
            for (int j = 0; j < board.getSize(); j++) // FIX: Use board.getSize() for loops
            {
                if (board.isValidMove(i, j))
                {
                    // Create a temporary copy of the board state for the Minimax calculation
                    Board tempBoard = board;

                    // Make the move on the temporary board
                    tempBoard.makeMove(i, j, this->symbol);

                    // Call minimax with the copied board (which is passed by value)
                    int score = minimax(tempBoard, 0, false);

                    // The 'tempBoard' is destroyed at the end of the loop, so no undo is needed here.

                    if (score > bestScore)
                    {
                        bestScore = score;
                        row = i;
                        col = j;
                    }
                }
            }
        }
    }
    // Evaluates the board state for scoring (+10, 0, -10).
    int evaluateBoard(const Board& board) const
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
    Player* player1;
    Player* player2;
    Player* currentPlayer;
    bool gameOver = false;
    Player* winner = nullptr;

public:
    Game() : player1(nullptr), player2(nullptr), currentPlayer(nullptr), gameOver(false) {}

    // Destructor to clean up allocated players
    ~Game()
    {
        delete player1;
        delete player2;
    }

    // Main game entry point.
    void start()
    {
        srand(static_cast<unsigned>(time(0)));

        int choice;
        showMenu();
        std::cin >> choice;

        if (choice == 1)
            setupPvP();
        else if (choice == 2)
            setupPvC(AIPlayer::EASY);
        else
            setupPvC(AIPlayer::HARD);

        board.display();

        while (!gameOver)
        {
            if (dynamic_cast<HumanPlayer*>(currentPlayer))
            {
                handleHumanMove(currentPlayer);
            }
            else
            {
                handleAIMove(dynamic_cast<AIPlayer*>(currentPlayer));
            }

            board.display();

            if (checkGameEnd())
                break;

            switchPlayer();
        }

        displayResult();
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
        std::string name1, name2;
        std::cout << "Enter name for Player 1 (X): ";
        std::cin >> name1;
        std::cout << "Enter name for Player 2 (O): ";
        std::cin >> name2;

        player1 = new HumanPlayer(name1, 'X');
        player2 = new HumanPlayer(name2, 'O');
        currentPlayer = player1;
    }

    // Sets up Player vs Computer mode.
    void setupPvC(AIPlayer::Difficulty difficulty)
    {
        std::string name;
        std::cout << "Enter your name (X): ";
        std::cin >> name;

        player1 = new HumanPlayer(name, 'X');
        player2 = new AIPlayer("Computer", 'O', difficulty, &board);
        currentPlayer = player1;
    }

    // Alternates the current player.
    void switchPlayer()
    {
        currentPlayer = (currentPlayer == player1) ? player2 : player1;
    }

    // Handles human player input and move validation.
    void handleHumanMove(Player* player)
    {
        int row, col;
        player->getMove(row, col);
        while (!board.isValidMove(row, col))
        {
            std::cout << "Invalid move, try again.\n";
            player->getMove(row, col);
        }
        board.makeMove(row, col, player->getSymbol());
    }

    // Handles AI player move calculation and placement.
    void handleAIMove(AIPlayer* aiPlayer)
    {
        int row, col;
        aiPlayer->getMove(row, col);
        if (row != -1 && col != -1)
        {
            board.makeMove(row, col, aiPlayer->getSymbol());
            std::cout << aiPlayer->getName() << " chose (" << row+1 << "," << col+1 << ")\n";
        }
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
    Game ticTacToeGame; // Create an instance of the Game class
    ticTacToeGame.start(); // Call the start() method to begin the game

    return 0;
}
