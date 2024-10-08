#ifndef CROSSWORD_H
#define CROSSWORD_H

#include <cassert>
#include <cctype>
#include <compare>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <vector>

#define SIZE_T_MAX std::numeric_limits<size_t>::max()

inline char const DEFAULT_CHAR = '?';
inline char CROSSWORD_BACKGROUND = '.';

// Used to represent a point on the plane (x, y)
using pos_t = std::pair<std::size_t, std::size_t>;
// Used to represent a dimension (width, height)
using dim_t = std::pair<std::size_t, std::size_t>;

class RectArea {
 private:
  pos_t top_left;
  pos_t bottom_right;

 public:
  RectArea(pos_t top_left, pos_t bottom_right)
      : top_left(top_left), bottom_right(bottom_right) {}

  // Copy constructor
  RectArea(const RectArea& other)
      : top_left(other.top_left), bottom_right(other.bottom_right) {}

  // Move constructor
  RectArea(RectArea&& rectangle)
      : top_left(std::move(rectangle.top_left)),
        bottom_right(std::move(rectangle.bottom_right)) {}

  // Copy assignment
  RectArea& operator=(const RectArea& other) {
    if (this != &other) {
      top_left = other.top_left;
      bottom_right = other.bottom_right;
    }
    return *this;
  }

  // Move assignment
  RectArea& operator=(RectArea&& other) {
    if (this != &other) {
      top_left = std::move(other.top_left);
      bottom_right = std::move(other.bottom_right);
    }
    return *this;
  }

  RectArea operator*=(const RectArea& other) {
    top_left.first = std::max(top_left.first, other.top_left.first);
    top_left.second = std::max(top_left.second, other.top_left.second);
    bottom_right.first = std::min(bottom_right.first, other.bottom_right.first);
    bottom_right.second =
        std::min(bottom_right.second, other.bottom_right.second);

    return *this;
  }

  void set_left_top(pos_t top_left) { this->top_left = top_left; }

  void set_right_bottom(pos_t bottom_right) {
    this->bottom_right = bottom_right;
  }

  pos_t get_left_top() const { return top_left; }

  pos_t get_right_bottom() const { return bottom_right; }

  dim_t size() const {
    if (bottom_right.first < top_left.first ||
        bottom_right.second < top_left.second) {
      return dim_t(0, 0);
    }

    assert((bottom_right.first != SIZE_T_MAX) || (top_left.first != 0));
    assert((bottom_right.second != SIZE_T_MAX) || (top_left.second != 0));

    return dim_t(bottom_right.first - top_left.first + 1,
                 bottom_right.second - top_left.second + 1);
  }

  bool empty() const { 
    return bottom_right.first < top_left.first ||
           bottom_right.second < top_left.second;
  }

  void embrace(const pos_t point) {
    if (empty()) {
      set_left_top(point);
      set_right_bottom(point);
    } else {
      if (point.first < top_left.first) top_left.first = point.first;

      if (point.second < top_left.second) top_left.second = point.second;

      if (point.first > bottom_right.first) bottom_right.first = point.first;

      if (point.second > bottom_right.second)
        bottom_right.second = point.second;
    }
  }

  // Destructor
  ~RectArea() = default;
};

RectArea const operator*(const RectArea& a, const RectArea& b) {
  return RectArea(a) *= b;
}

inline RectArea const DEFAULT_EMPTY_RECT_AREA =
    RectArea(pos_t(1, 1), pos_t(0, 0));

enum class orientation_t : bool { H = true, V = false };

class Word {
 private:
  pos_t start_pos;
  orientation_t orientation;
  std::string word;

 public:
  Word(size_t x, size_t y, orientation_t orientation, std::string word)
      : start_pos(pos_t(x, y)), orientation(orientation), word(word) {
    if (word.empty()) this->word = DEFAULT_CHAR;

    if (orientation == orientation_t::H &&
        start_pos.first + word.length() < start_pos.first)
      this->word = word.substr(0, SIZE_T_MAX - start_pos.first + 1);

    if (orientation == orientation_t::V &&
        start_pos.second + word.length() < start_pos.second)
      this->word = word.substr(0, SIZE_T_MAX - start_pos.second + 1);
  }

  // Copy constructor
  Word(const Word& other)
      : start_pos(other.start_pos),
        orientation(other.orientation),
        word(other.word) {}

  // Move constructor
  Word(Word&& word)
      : start_pos(std::move(word.start_pos)),
        orientation(std::move(word.orientation)),
        word(std::move(word.word)) {}

  // Copy assignment
  Word& operator=(const Word& other) {
    if (this != &other) {
      start_pos = other.start_pos;
      orientation = other.orientation;
      word = other.word;
    }
    return *this;
  }

  // Move assignment
  Word& operator=(Word&& other) {
    if (this != &other) {
      start_pos = std::move(other.start_pos);
      orientation = std::move(other.orientation);
      word = std::move(other.word);
    }
    return *this;
  }

  pos_t get_start_position() const { return start_pos; }

  pos_t get_end_position() const {
    size_t end_x = start_pos.first +
                   (orientation == orientation_t::H ? word.length() - 1 : 0);

    size_t end_y = start_pos.second +
                   (orientation == orientation_t::V ? word.length() - 1 : 0);

    return pos_t(end_x, end_y);
  }

  orientation_t get_orientation() const { return orientation; }

  char at(const long long index) const {
    if (index < 0 || (size_t)index >= word.length() || word.empty())
      return DEFAULT_CHAR;

    return word.at(index);
  }

  size_t length() const { return word.length(); }

  bool operator==(const Word& other) const {
    return start_pos == other.start_pos && orientation == other.orientation;
  }

  bool operator!=(const Word& other) const { return !(*this == other); }

  std::strong_ordering operator<=>(const Word& other) const {
    if (this->orientation == orientation_t::H &&
        other.orientation == orientation_t::V) {
      return std::strong_ordering::less;
    }

    if (this->orientation == orientation_t::V &&
        other.orientation == orientation_t::H) {
      return std::strong_ordering::greater;
    }

    if (this->word == other.word) return std::strong_ordering::equal;

    if (this->word < other.word) return std::strong_ordering::less;

    return std::strong_ordering::greater;
  }

  RectArea rect_area() const { return RectArea(start_pos, get_end_position()); }

  // Destructor
  ~Word() = default;
};

inline Word const DEFAULT_WORD = Word(0, 0, orientation_t::H, "?");

class Crossword {
 private:
  std::vector<Word> words;
  RectArea rect_area;
  std::pair<size_t, size_t> orientation_count;

  struct Letter {
    char value;
    bool H = false;
    bool V = false;
  };

  std::map<pos_t, Letter> letters;

  bool letter_word_orientation_equal(const Letter l, Word w) const {
    if (l.H && w.get_orientation() == orientation_t::H) return true;
    if (l.V && w.get_orientation() == orientation_t::V) return true;
    return false;
  }

  // converts symbol to upper case or to deafult char
  // if the symbol is non-alphabetical
  char convert_symbol(char x) const {
    x = std::toupper(x);
    if (!isalpha(x)) x = DEFAULT_CHAR;

    return x;
  }

  // checks if size_t variable won't overflow
  bool check_add(size_t x, int y) {
    assert(-1 <= y && y <= 1);

    if (x == 0 && y == -1) return false;

    if (x == SIZE_T_MAX && y == +1) return false;

    return true;
  }

  // moves point along or perpendicularly to the given orientation
  // along, perp \in {-1, 0, 1}
  // returns nullopt if moving the point is not possible
  std::optional<pos_t> move_into(pos_t pos, const int along, const int perp,
                                 const orientation_t orient) {
    if (orient == orientation_t::H) {
      if (!check_add(pos.first, along)) return std::nullopt;
      if (!check_add(pos.second, perp)) return std::nullopt;
      return pos_t{pos.first + along, pos.second + perp};
    } else {
      if (!check_add(pos.first, perp)) return std::nullopt;
      if (!check_add(pos.second, along)) return std::nullopt;
      return pos_t{pos.first + perp, pos.second + along};
    }
  }

  // checks if the word will fit into the crossword
  bool book_space(const Word word) {
    orientation_t orient = word.get_orientation();

    // check if word fits into the grid
    pos_t pos = word.get_start_position();
    for (size_t i = 1; i < word.length(); i++) {
      auto res = move_into(pos, +1, 0, orient);
      if (!res) return false;
      pos = *res;
    }

    // checking collisions on the edges of the word
    auto check_bound = [&](pos_t bound) {
      for (int sign : {-1, 0, +1}) {
        auto res = move_into(bound, 0, sign, orient);
        if (res && letters.count(*res)) {
          return false;
        }
      }
      return true;
    };

    auto bound_l = move_into(word.get_start_position(), -1, 0, orient);
    if (bound_l && !check_bound(*bound_l)) return false;

    auto bound_r = move_into(pos, +1, 0, orient);
    if (bound_r && !check_bound(*bound_r)) return false;

    // checking collisions in the middle parts of the word
    pos = word.get_start_position();
    for (size_t i = 0; i < word.length(); i++) {
      if (i) pos = *move_into(pos, +1, 0, orient);

      if (letters.contains(pos)) {
        Letter l = letters[pos];
        
        if (std::toupper(l.value) != std::toupper(word.at(i)) &&
            (std::isalpha(l.value) || std::isalpha(word.at(i))))
          return false;

        if (letter_word_orientation_equal(l, word))
          return false;

        for (int sign : {-1, 1}) {
          auto res = move_into(pos, 0, sign, orient);
          if (res && letters.count(*res)){
            Letter m = letters[*res];
            if (letter_word_orientation_equal(m, word))
              return false;
          }
        }
      } else {
        for (int sign : {-1, 1}) {
          auto res = move_into(pos, 0, sign, orient);
          if (res && letters.count(*res)) return false;
        }
      }
    }

    return true;
  }

  // adds word to the crossword
  // assumes the word can be added
  void add_word(const Word word) {
    words.push_back(word);
    rect_area.embrace(word.get_start_position());
    rect_area.embrace(word.get_end_position());
    if (word.get_orientation() == orientation_t::H) {
      orientation_count.first++;
    } else {
      orientation_count.second++;
    }
    pos_t curr_pos = word.get_start_position();
    for (size_t i = 0; i < word.length(); i++) {
      if (i) curr_pos = *move_into(curr_pos, 1, 0, word.get_orientation());
      Letter& letter = letters[curr_pos];
      letter.value = word.at(i);
      if (word.get_orientation() == orientation_t::H)
        letter.H = true;
      else
        letter.V = true;
    }
  }

 public:
  Crossword(Word word, std::initializer_list<Word> additional_words)
      : rect_area(DEFAULT_EMPTY_RECT_AREA), orientation_count(0, 0) {
    add_word(word);
    for (Word i : additional_words) {
      if (book_space(i)) add_word(i);
    }
  }

  // Copy constructor
  Crossword(const Crossword& other)
      : words(other.words),
        rect_area(other.rect_area),
        orientation_count(other.orientation_count),
        letters(other.letters) {}

  // Move constructor
  Crossword(Crossword&& other)
      : words(std::move(other.words)),
        rect_area(std::move(other.rect_area)),
        orientation_count(std::move(other.orientation_count)),
        letters(std::move(other.letters)) {}

  // Copy assignment
  Crossword& operator=(const Crossword& other) {
    if (this != &other) {
      words = other.words;
      rect_area = other.rect_area;
      orientation_count = other.orientation_count;
      letters = other.letters;
    }
    return *this;
  }

  // Move assignment
  Crossword& operator=(Crossword&& other) {
    if (this != &other) {
      words = std::move(other.words);
      rect_area = std::move(other.rect_area);
      orientation_count = std::move(other.orientation_count);
      letters = std::move(other.letters);
    }
    return *this;
  }

  bool insert_word(const Word word) {
    if (!book_space(word)) return false;
    add_word(word);

    return true;
  }

  dim_t size() const {
    if (words.empty()) return dim_t(0, 0);
    return rect_area.size();
  }

  dim_t word_count() const {
    if (words.empty()) return dim_t(0, 0);
    return orientation_count;
  }

  Crossword& operator+=(const Crossword& other) {
    for (Word i : other.words) insert_word(i);
    return *this;
  }

  friend std::ostream& operator<<(std::ostream& o, const Crossword& c) {
    long long start_V = (long long)c.rect_area.get_left_top().second - 1;
    long long end_V = (long long)c.rect_area.get_right_bottom().second + 1;
    long long start_H = (long long)c.rect_area.get_left_top().first - 1;
    long long end_H = (long long)c.rect_area.get_right_bottom().first + 1;

    for (long long i = start_V; i <= end_V; i++) {
      for (long long j = start_H; j <= end_H; j++) {
        pos_t position(j, i);
        auto it = c.letters.find(position);

        if (it != c.letters.end())
          o << c.convert_symbol(it->second.value);
        else
          o << CROSSWORD_BACKGROUND;

        if (j < end_H) o << ' ';
      }

      o << '\n';
    }
    return o;
  }

  // Destructor
  ~Crossword() = default;
};

Crossword operator+(const Crossword& a, const Crossword& b) {
  return Crossword(a) += b;
}

#endif  