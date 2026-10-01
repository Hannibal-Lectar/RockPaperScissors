// Leaderboard.h
// A hand-written singly linked list that keeps players ranked by number
// of match wins (descending). Implemented manually (rather than using
// std::list) to satisfy the project's requirement for a custom linked
// list data structure, and to make the ranking/insertion logic explicit.
#pragma once

#include <string>
#include <vector>

namespace rps {

// One player's entry on the leaderboard.
struct LeaderboardEntry {
    std::string playerName;
    int wins = 0;
    int losses = 0;
    int draws = 0;
    double winRate = 0.0;
};

// Internal node of the linked list. Not exposed outside this class other
// than through LeaderboardEntry copies returned by toSortedVector().
struct LeaderboardNode {
    LeaderboardEntry entry;
    LeaderboardNode* next = nullptr;
};

class Leaderboard {
public:
    Leaderboard() = default;
    ~Leaderboard();

    // Manual memory management means copying must be handled carefully.
    // The project has no need to copy a Leaderboard, so copying is
    // disabled to avoid accidental double frees; moving is allowed.
    Leaderboard(const Leaderboard&) = delete;
    Leaderboard& operator=(const Leaderboard&) = delete;
    Leaderboard(Leaderboard&& other) noexcept;
    Leaderboard& operator=(Leaderboard&& other) noexcept;

    // Inserts a brand new player node, or updates an existing player's
    // record if the name is already present. Rebuilds this player's
    // position in the list so it stays sorted by win count descending.
    void insertOrUpdate(const std::string& name, int wins, int losses, int draws);

    // Removes every node from the list.
    void clear();

    bool empty() const { return head_ == nullptr; }
    int size() const { return size_; }

    // Walks the linked list and returns its contents as a vector,
    // already sorted highest wins first (ties broken by win rate),
    // ready for the GUI to render as a ranked table.
    std::vector<LeaderboardEntry> toSortedVector() const;

private:
    LeaderboardNode* head_ = nullptr;
    int size_ = 0;

    // Removes the node for `name` if present, without deleting the
    // entry data (caller is expected to have copied it out first, or
    // is about to reinsert it). Returns true if a node was removed.
    bool removeNode(const std::string& name);

    // Inserts a node in sorted position (descending by wins).
    void insertSorted(const LeaderboardEntry& entry);
};

} // namespace rps
