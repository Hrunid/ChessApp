# ChessApp

ChessApp is a desktop chess application written in modern C++ with a custom rules engine, a graphical interface built on SFML and TGUI, and planned UCI integration with Stockfish 17.1. The project emphasizes clear separation of concerns, incremental state updates, and idiomatic use of the C++ Standard Library up to C++23.

## Overview

The engine models a complete chess rule set, including legal move generation, pins and check resolution, castling, promotion, and en passant. It supports PGN and UCI notations, FEN export, and uses Zobrist hashing for efficient position identity tracking and repetition detection. The UI provides a clickable board with move highlighting and a promotion flow. A `VsComputer` mode is scaffolded; UCI wiring with Stockfish 17.1 is the next milestone.

## Architecture

The application layer (`App`, `UI`) handles rendering and input. The game controller (`Game`) manages selection, turns, move execution, end-of-game detection, and history. The model centers on `Board`, `Square`, `Move`, `Player`, and a `Piece` hierarchy (`King`, `Queen`, `Rook`, `Bishop`, `Knight`, `Pawn`). `Board` owns the authoritative state, updates pins and attack data, maintains Zobrist keys, and serializes positions and moves. `UI` communicates user intent to `Game`, which delegates to `Board` and exposes read-only views back to the renderer.

## Unique approach to move calculation

Instead of recomputing all piece moves every turn, each `Square` records which pieces have access to it. After a move, the engine updates only the affected local neighborhoods (source and destination regions plus king checks) and then recomputes pins. This incremental, access-map strategy minimizes redundant computation and keeps the interface responsive as complexity grows.

## Technologies

ChessApp uses C++20/23 features and the STL (smart pointers, containers, algorithms, utility types), is configured with CMake, renders with SFML, composes widgets with TGUI, serializes via PGN/FEN/UCI, and fingerprints positions with Zobrist hashing. UCI integration targets Stockfish 17.1 for analysis and engine play.

## End Goal

The goal is a fully featured desktop chess application with integrated computer analysis (Stockfish 17.1), multiple modes (human–human, human–engine, analysis), PGN import, FEN starts, a browsable variations tree, and customizable visuals (piece sets, board themes, highlights). The incremental engine design underpins smooth UX by reducing unnecessary calculations.

## External sources

The GUI is implemented with SFML and TGUI. Engine analysis and computer play are planned via the UCI protocol using Stockfish 17.1 (distributed separately).

