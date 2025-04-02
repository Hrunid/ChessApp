# Chess App

This project is fully designed and implemented by me, except for the Zobrist hashing algorithm, an efficient method used to represent chess positions uniquely as 64-bit numbers. Zobrist hashing optimizes the process of detecting repeated positions.

A distinctive aspect of this project is its logical structure. Instead of representing the board as an 8x8 array of pointers to pieces, I decided to create a custom `Square` class. The conventional approach, recommended by AI, calculates available moves for every piece after each turn or validates moves immediately, leading to many unnecessary calculations. To streamline this, the `Square` class contains key information such as the ID of the current piece and IDs of pieces with access to it. This design simplifies finding valid moves since only pieces interacting with the starting or ending squares of a move require recalculating potential moves—except in the unique case of the king.

## Technologies
The project is developed primarily using C++ and leverages Git for version control and collaboration. CMake is utilized to streamline the build process, ensuring compatibility across different platforms. SFML is used to create an intuitive and functional user interface, while Stockfish serves as the engine for computer-generated moves and analysis.



## End Goal

The goal is to develop a desktop chess application featuring three different game modes—two-player, player versus computer, and an analysis mode—with a functional user interface created using external libraries like SFML.

### Player vs. Computer

To implement the CPU opponent, the application will utilize Stockfish, an open-source chess engine. A dedicated method will run Stockfish as a background process, with a child class managing the communication with the engine and adjusting gameplay execution based on the current player.

### Analysis Mode

Analysis mode will support game review by loading PGN files, starting from custom positions, or analyzing finished games. Move branches will be stored in a tree structure, with each move referencing the previous one. To minimize computation, each node will store the best move suggested by the engine and the position in FEN format or as a sequence of moves. Stockfish uses either the current position in FEN or a list of moves from the starting position to evaluate the best moves.

### UI

The user interface will be created using SFML to ensure both functionality and simplicity. Although the main focus of the project is on game logic, the UI will offer customizable options such as square colors and piece styles.


## Commit History and Branch Management
    I acknowledge that my commit history and branch management are somewhat chaotic and can lack clarity. In a collaborative team environment, I would prioritize clearer commit messages and ensure that commits are properly organized within their respective branches.

## Learing Experience
    Developing this project significantly improved my programming and software design skills. Driven by curiosity and a desire to deeply understand the rationale behind choosing specific solutions, I explored various aspects of application design and implementation. This process provided me with valuable insights into structuring classes, making informed decisions about type usage, effectively handling parameter passing, and correctly utilizing different pointer types. Overall, this experience greatly enhanced my ability to write efficient, clear, and maintainable code. Obviously there is much room for improvement but whole the proccess was very engaging and simply fun for me to learn and understand more.