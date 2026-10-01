// Leaderboard.cpp
#include "core/Leaderboard.h"

namespace rps {

Leaderboard::~Leaderboard() {
    clear();
}

Leaderboard::Leaderboard(Leaderboard&& other) noexcept
    : head_(other.head_), size_(other.size_) {
    other.head_ = nullptr;
    other.size_ = 0;
}

Leaderboard& Leaderboard::operator=(Leaderboard&& other) noexcept {
    if (this != &other) {
        clear();
        head_ = other.head_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

bool Leaderboard::removeNode(const std::string& name) {
    LeaderboardNode* current = head_;
    LeaderboardNode* previous = nullptr;

    while (current != nullptr) {
        if (current->entry.playerName == name) {
            if (previous == nullptr) {
                head_ = current->next;
            } else {
                previous->next = current->next;
            }
            delete current;
            size_--;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}

void Leaderboard::insertSorted(const LeaderboardEntry& entry) {
    LeaderboardNode* node = new LeaderboardNode{entry, nullptr};

    // Empty list, or the new entry belongs at the very front.
    if (head_ == nullptr ||
        entry.wins > head_->entry.wins ||
        (entry.wins == head_->entry.wins && entry.winRate > head_->entry.winRate)) {
        node->next = head_;
        head_ = node;
        size_++;
        return;
    }

    // Walk the list until we find the first node that should come after
    // the new entry (fewer wins, or equal wins but a lower win rate).
    LeaderboardNode* current = head_;
    while (current->next != nullptr &&
           (current->next->entry.wins > entry.wins ||
            (current->next->entry.wins == entry.wins &&
             current->next->entry.winRate >= entry.winRate))) {
        current = current->next;
    }

    node->next = current->next;
    current->next = node;
    size_++;
}

void Leaderboard::insertOrUpdate(const std::string& name, int wins, int losses, int draws) {
    // If the player already has a node, remove it first so we can
    // reinsert it in the correct sorted position for its new score.
    removeNode(name);

    LeaderboardEntry entry;
    entry.playerName = name;
    entry.wins = wins;
    entry.losses = losses;
    entry.draws = draws;
    int totalMatches = wins + losses; // draws don't count as a match in this game
    entry.winRate = totalMatches == 0 ? 0.0
                  : (100.0 * static_cast<double>(wins) / static_cast<double>(totalMatches));

    insertSorted(entry);
}

void Leaderboard::clear() {
    LeaderboardNode* current = head_;
    while (current != nullptr) {
        LeaderboardNode* toDelete = current;
        current = current->next;
        delete toDelete;
    }
    head_ = nullptr;
    size_ = 0;
}

std::vector<LeaderboardEntry> Leaderboard::toSortedVector() const {
    std::vector<LeaderboardEntry> result;
    result.reserve(size_);
    for (LeaderboardNode* current = head_; current != nullptr; current = current->next) {
        result.push_back(current->entry);
    }
    return result;
}

} // namespace rps
