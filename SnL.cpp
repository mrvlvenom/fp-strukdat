#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>
#include <unordered_map>

// Forward declaration of classes
class Square;

// Abstract class representing a square on the board
class Square {
public:
    virtual void performAction(int& playerPosition) = 0;
    virtual ~Square() {}
};

// Concrete class representing a normal square
class NormalSquare : public Square {
public:
    void performAction(int& playerPosition) override {
        std::cout << "Nothing special here. Move forward." << std::endl;
    }
};

// Concrete class representing a snake square
class SnakeSquare : public Square {
public:
    void performAction(int& playerPosition) override {
        std::cout << "Oh no! You landed on a snake. Going back to position 2." << std::endl;
        playerPosition = 2;
    }
};

// Concrete class representing a ladder square
class LadderSquare : public Square {
public:
    void performAction(int& playerPosition) override {
        std::cout << "You found a ladder! Climbing up to position 63." << std::endl;
        playerPosition = 63;
    }
};

// Concrete class representing a special ladder square
class SpecialLadderSquare : public Square {
public:
    void performAction(int& playerPosition) override {
        std::cout << "You found a special ladder! Climbing up to position 15." << std::endl;
        playerPosition = 15;
    }
};

// Forward declaration of classes
class SpecialSnakeSquare;

// Concrete class representing a special snake square
class SpecialSnakeSquare : public Square {
public:
    void performAction(int& playerPosition) override {
        std::cout << "Oops! You landed on a special snake. Moving back to position 10." << std::endl;
        playerPosition = 10;
    }
};

class SnakeAndLadderGame {
private:
    int numPlayers;
    std::vector<int> playerPositions;
    std::vector<Square*> board;
    const int boardSize = 10; // Assuming a square board for simplicity

    // Map to store special positions and their corresponding destinations
    std::unordered_map<int, int> specialPositions;

public:
    // Constructor
    SnakeAndLadderGame(int players) : numPlayers(players) {
        // Initialize the board with Normal, Snake, and Ladder squares
        for (int i = 0; i < 100; ++i) {
            if (i == 16) {
                board.push_back(new SnakeSquare());
            } else if (i == 31) {
                board.push_back(new LadderSquare());
            } else if (i == 5 || i == 8 || i == 12 || i == 18 || i == 24 || i == 28 || i == 33 || i == 40 || i == 45 || i == 50) {
                board.push_back(new SpecialLadderSquare());
                specialPositions[i] = i + 10; // Landing on these positions moves forward by 10
            } else if (i == 22 || i == 36 || i == 42 || i == 52 || i == 63 || i == 72 || i == 84 || i == 91 || i == 94 || i == 98) {
                board.push_back(new SpecialSnakeSquare());
                specialPositions[i] = i - 10; // Landing on these positions moves back by 10
            } else {
                board.push_back(new NormalSquare());
            }
        }

        // Initialize player positions
        playerPositions.resize(numPlayers, 1);
    }

    // Destructor
    ~SnakeAndLadderGame() {
        // Clean up allocated memory for the board
        for (Square* square : board) {
            delete square;
        }
    }

    void playGame() {
        int currentPlayer = 0;

        while (true) {
            std::cout << "Player " << currentPlayer + 1 << "'s turn. Press Enter to roll the dice...";
            std::cin.ignore(); // Wait for Enter key

            rollDice(currentPlayer);
            displayBoard();

            if (playerPositions[currentPlayer] >= 100) {
                std::cout << "Congratulations! Player " << currentPlayer + 1 << " reached position 100. Player " << currentPlayer + 1 << " wins!" << std::endl;
                break;
            }

            currentPlayer = (currentPlayer + 1) % numPlayers; // Move to the next player
        }
    }

    void rollDice(int currentPlayer) {
        int diceRoll = 1 + rand() % 6;
        std::cout << "Player " << currentPlayer + 1 << " rolled a " << diceRoll << "." << std::endl;

        playerPositions[currentPlayer] += diceRoll;
        handleSnakeAndLadder(currentPlayer);
    }

    void handleSnakeAndLadder(int currentPlayer) {
        int currentPosition = playerPositions[currentPlayer];

        // Check if the current position is a special position
        auto it = specialPositions.find(currentPosition);
        if (it != specialPositions.end()) {
            std::cout << "Special position! Moving to " << it->second << "." << std::endl;
            playerPositions[currentPlayer] = it->second;
        } else if (currentPosition >= 0 && currentPosition < 100) {
            // Apply normal square or ladder/special ladder action
            board[currentPosition]->performAction(playerPositions[currentPlayer]);
        }
    }

    void displayBoard() const {
        std::cout << "Current Board:" << std::endl;

        for (int i = boardSize - 1; i >= 0; --i) {
            for (int j = 0; j < boardSize; ++j) {
                int position = i * boardSize + j + 1;
                std::cout << std::setw(3); // Adjusted setw for proper alignment

                bool playerOnSquare = false;
                for (int k = 0; k < numPlayers; ++k) {
                    if (position == playerPositions[k]) {
                        std::cout << " P" << k + 1;
                        playerOnSquare = true;
                        break;
                    }
                }

                if (!playerOnSquare) {
                    std::cout << std::setw(3) << position;
                }
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
};

int main() {
    srand(time(0)); // Seed for random number generation

    int numPlayers;
    std::cout << "Enter the number of players (2-4): ";
    std::cin >> numPlayers;

    if (numPlayers < 2 || numPlayers > 4) {
        std::cout << "Invalid number of players. Exiting." << std::endl;
        return 1;
    }

    SnakeAndLadderGame game(numPlayers);
    game.playGame();

    return 0;
}