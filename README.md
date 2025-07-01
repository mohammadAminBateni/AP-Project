Overview
Poffer is a multiplayer card game where players compete over 3 rounds to determine the ultimate winner. This project implements a client-server architecture using C++ with QT framework, incorporating concepts like OOP, polymorphism, threads, sockets, and STL.

Game Rules:
Card Types:
There are 4 card suits:

Diamond (highest value)

Gold

Dollar

Coin (lowest value)

Each suit contains 13 cards:

Bitcoin (highest)

King

Queen

Soldier

Number cards (2-10)

Gameplay (2-player version):
The deck is shuffled and each round proceeds as follows:

Starting player receives 7 random cards, selects 1, and passes remaining 6 to opponent

Opponent selects 1 card from the 6

This repeats 5 times per round until each player has 5 cards

Players compare their 5-card hands using the ranking patterns

First player to win 2 rounds wins the match (maximum 3 rounds)

Starting Player Determination
Each player gets one random Diamond card at round start

Player with higher-value Diamond card starts first

Starting player alternates in subsequent rounds

Special Features
Card Exchange: Players can request to exchange one card with opponent (disabled in final exchange)

Pause/Resume: Each player can pause game twice (max 20 seconds per pause)

Disconnection Handling: 60-second grace period for reconnection

Inactivity: 20-second timer for card selection with 10-second warning
Game Patterns (Rankings):
Cards are evaluated based on these patterns from strongest to weakest:

Golden Hand: Bitcoin, King, Queen, Soldier, 10 of same suit

Royal Flush: King, Queen, Soldier, 10, 9 of same suit

Four of a Kind: Four cards of same rank

Full House: Three of a kind + a pair

Flush: Five cards of same suit

Straight: Five consecutive ranked cards

Three of a Kind: Three cards of same rank

Two Pair: Two different pairs

One Pair: One pair of same rank

High Card: Highest single card when no other patterns

Tiebreaker rules:

Compare pattern ranks first

For same patterns, compare card values

If still tied, compare suit hierarchy (Diamond > Gold > Dollar > Coin)
