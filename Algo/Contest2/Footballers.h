#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <functional>

struct Player {
  int index;
  int64_t effectiveness;

  Player(const int64_t effectiveness, const int index)
      : effectiveness(effectiveness), index(index) {}
};

template<typename ForwardIterator>
bool IsEfficientTeam(ForwardIterator first,
                     ForwardIterator last) {
  if (std::distance(first, last) <= 1) {
    return true;
  }
  auto next = first + 1;
  return (first->effectiveness + next->effectiveness >= last->effectiveness);
}

int FindLeftSonIndex(const int index) {
  return 2 * index + 1;
}

int FindRightSonIndex(const int index) {
  return 2 * index + 2;
}

template<typename RandomAccessIterator,
         typename ValueType = typename std::iterator_traits<RandomAccessIterator>::value_type,
         typename Compare = std::less<ValueType>>
inline bool Heapify(const RandomAccessIterator first, const RandomAccessIterator last,
                    const int index, const Compare comparator = Compare()) {
  int len = last - first;

  if (FindLeftSonIndex(index) >= len) {
    return false;
  }

  if (FindRightSonIndex(index) >= len) {
    if (comparator(*(first + index), *(first + FindLeftSonIndex(index)))) {
      std::swap(*(first + index), *(first + FindLeftSonIndex(index)));
      return true;
    }
    return false;
  }

  if (!comparator(*(first + index), *(first + FindLeftSonIndex(index))) &&
      !comparator(*(first + index), *(first + FindRightSonIndex(index)))) {
    return false;
  }

  if (!comparator(*(first + FindLeftSonIndex(index)), *(first + FindRightSonIndex(index)))) {
    std::swap(*(first + index), *(first + FindLeftSonIndex(index)));
    Heapify(first, last, FindLeftSonIndex(index), comparator);
    return true;
  } else {
    std::swap(*(first + index), *(first + FindRightSonIndex(index)));
    Heapify(first, last, FindRightSonIndex(index), comparator);
    return true;
  }
}

template<typename ForwardIterator,
         typename ValueType = typename std::iterator_traits<ForwardIterator>::value_type,
         typename Compare = std::less<ValueType>>
inline void MakeHeap(const ForwardIterator first,
                     const ForwardIterator last,
                     const Compare comparator = Compare()) {
  int len = std::distance(first, last);
  for (int index = len - 1; index >= 0; --index) {
    Heapify(first, last, index, comparator);
  }
}

template<typename ForwardIterator,
         typename ValueType = typename std::iterator_traits<ForwardIterator>::value_type,
         typename Compare = std::less<ValueType>>
inline void HeapSort(const ForwardIterator first,
                     const ForwardIterator last,
                     const Compare comparator = Compare()) {
  MakeHeap(first, last, comparator);
  int len = std::distance(first, last);
  for (int index = len - 1; index >= 0; --index) {
    std::swap(*(first), *(std::next(first, index)));
    Heapify(first, first + index, 0, comparator);
  }
}

std::vector<Player> ReadPlayers(std::istream &in_stream = std::cin) {
  int player_count;
  in_stream >> player_count;
  std::vector<Player> players;
  players.reserve(player_count);

  for (int i = 0; i < player_count; ++i) {
    int64_t effectiveness;
    in_stream >> effectiveness;
    players.push_back({effectiveness, i});
  }
  return players;
}

template<typename InputIterator>
int64_t ComputeTotalEfficiency(const InputIterator first,
                               const InputIterator last) {
  int64_t sum = 0;
  for (auto next = first; next != last; ++next) {
    sum += next->effectiveness;
  }
  return sum;
}

bool IsEffectivenessLess(const Player& first_player, const Player& second_player) {
  return (first_player.effectiveness < second_player.effectiveness);
}

bool IsIndexLess(const Player& first_player, const Player& second_player) {
  return (first_player.index < second_player.index);
}

void PrintMostEfficientTeam(std::vector<Player> efficient_team,
                            std::ostream &out_stream = std::cout) {
  HeapSort(efficient_team.begin(), efficient_team.end(), IsIndexLess);
  int64_t sum = ComputeTotalEfficiency(efficient_team.begin(), efficient_team.end());
  out_stream << sum << "\n";
  for (const Player &player : efficient_team) {
    out_stream << player.index + 1 << " ";
  }
  out_stream << "\n";
}

template <typename ForwardIterator>
class PlayerSegment {
 public:
  PlayerSegment(const ForwardIterator& left_ptr,
                const ForwardIterator& right_ptr,
                const ForwardIterator& end)
      : left_ptr_(left_ptr), right_ptr_(right_ptr), end_(end) {
    effectiveness_ = left_ptr->effectiveness;
  }

  void PushRightPlayer() {
    ++right_ptr_;
    effectiveness_ += right_ptr_->effectiveness;
  }

  void PopRightPlayer() {
    effectiveness_ -= right_ptr_->effectiveness;
    --right_ptr_;
  }

  void PopLeftPlayer() {
    effectiveness_ -= left_ptr_->effectiveness;
    ++left_ptr_;
  }

  ForwardIterator GetLeftPtr() const {
    return left_ptr_;
  }

  ForwardIterator GetRightPtr() const {
    return right_ptr_;
  }

  int64_t GetEffectiveness() const {
    return effectiveness_;
  }

  bool operator < (const PlayerSegment& other) const {
    return this->effectiveness_ < other.effectiveness_;
  }

  bool IsSolid() const {
    return IsEfficientTeam(left_ptr_, right_ptr_);
  }

  bool IsOutOfBound() const {
    return (left_ptr_ == end_ || right_ptr_ == end_);
  }

 private:
  ForwardIterator end_;
  ForwardIterator left_ptr_;
  ForwardIterator right_ptr_;
  int64_t effectiveness_;
};

std::vector<Player> FindMostEfficientSolidTeam(std::vector<Player> players) {

  HeapSort(players.begin(), players.end(), IsEffectivenessLess);

  PlayerSegment<std::vector<Player>::const_iterator> current_segment(
    players.begin(),
    players.begin(),
    players.end());

  auto best_segment = current_segment;

  while (!current_segment.IsOutOfBound()) {
    while (!current_segment.IsOutOfBound() && current_segment.IsSolid()) {
      current_segment.PushRightPlayer();
    }
    current_segment.PopRightPlayer();

    if (best_segment < current_segment) {
      best_segment = current_segment;
    }

    current_segment.PopLeftPlayer();
  }

  return {best_segment.GetLeftPtr(), best_segment.GetRightPtr() + 1};
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  const std::vector<Player> players = ReadPlayers();
  const std::vector<Player> efficient_team = FindMostEfficientSolidTeam(players);
  PrintMostEfficientTeam(efficient_team);
  return 0;
}
