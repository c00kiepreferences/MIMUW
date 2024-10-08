#include "crosswords.h"

class RectArea {
private:
    pos_t top_left;
    pos_t bottom_right;

public:
    RectArea(pos_t top_left, pos_t bottom_right) : top_left(top_left),
                                                   bottom_right(bottom_right) {}

    // Copy constructor
    RectArea(const RectArea &other) : top_left(other.top_left),
                                      bottom_right(other.bottom_right) {}

    // Move constructor
    RectArea(RectArea &&rectangle) :
            top_left(std::move(rectangle.top_left)),
            bottom_right(std::move(rectangle.bottom_right)) {}

    RectArea& operator=(const RectArea& other) { // Copy assignment
        if (this != &other) {
            top_left = other.top_left;
            bottom_right = other.bottom_right;
        }
        return *this;
    }

    RectArea& operator=(RectArea&& other) { // Move assignment
        if (this != &other) {
            top_left = std::move(other.top_left);
            bottom_right = std::move(other.bottom_right);
        }
        return *this;
    }

    RectArea operator*(const RectArea& other) const {
        std::size_t new_top_left_x =
                std::max(top_left.first, other.top_left.first);
        std::size_t new_top_left_y =
                std::max(top_left.second, other.top_left.second);
        std::size_t new_bottom_right_x =
                std::min(bottom_right.first, other.bottom_right.first);
        std::size_t new_bottom_right_y =
                std::min(bottom_right.second, other.bottom_right.second);

        return RectArea(pos_t(new_top_left_x, new_top_left_y),
                        pos_t(new_bottom_right_x, new_bottom_right_y));
    }

    RectArea operator*=(const RectArea& other) {
        return *this = *this * other;
    }

    void set_left_top(pos_t top_left) {
        this->top_left = top_left;
    }

    void set_right_bottom(pos_t bottom_right) {
        this->bottom_right = bottom_right;
    }

    pos_t get_left_top() const {
        return top_left;
    }

    pos_t get_right_bottom() const {
        return bottom_right;
    }

    dim_t size() const {
        if (bottom_right.first < top_left.first ||
            bottom_right.second < top_left.second) {
            return dim_t(0, 0);
        }

        return dim_t(bottom_right.first - top_left.first + 1,
                     bottom_right.second - top_left.second + 1);
    }

    bool empty() const {
        return size() == dim_t(0, 0);
    }

    void embrace(pos_t point) {
        if (empty()) {
            set_left_top(point);
            set_right_bottom(point);
        } else {
            if (point.first < top_left.first) {
                top_left.first = point.first;
            }
            if (point.second < top_left.second) {
                top_left.second = point.second;
            }
            if (point.first > bottom_right.first) {
                bottom_right.first = point.first;
            }
            if (point.second > bottom_right.second) {
                bottom_right.second = point.second;
            }
        }
    }

    ~RectArea() = default; // Destructor
};

enum class orientation_t {
    H,
    V
};

class Word {
private:
    pos_t start_pos;
    orientation_t orientation;
    std::string word;

public:
    Word(size_t x, size_t y, orientation_t orientation, std::string word) :
            start_pos(pos_t(x, y)), orientation(orientation), word(word) {
        if (word.empty()) {
            this->word = DEFAULT_CHAR;
        }
    }

    // Copy constructor
    Word(const Word &other) : start_pos(other.start_pos),
                              orientation(other.orientation), word(other.word) {}

    // Move constructor
    Word(Word &&word) : start_pos(std::move(word.start_pos)),
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

    pos_t get_start_position() const {
        return start_pos;
    }

    pos_t get_end_position() const {
        size_t end_x = start_pos.first +
                       (orientation == orientation_t::H ? word.length() - 1 : 0);
        size_t end_y = start_pos.second +
                       (orientation == orientation_t::V ? word.length() - 1 : 0);
        return pos_t(end_x, end_y);
    }

    orientation_t get_orientation() const {
        return orientation;
    }

    char at(int index) const {
        if (index < 0 || (size_t)index >= word.length() || word.empty()) {
            return DEFAULT_CHAR;
        }
        return word.at(index);
    }

    size_t length() const {
        return word.length();
    }

    bool operator==(const Word& other) const {
        return start_pos == other.start_pos
               && orientation == other.orientation;
    }

    bool operator!=(const Word& other) const {
        return !(*this == other);
    }

    std::strong_ordering operator<=>(const Word& other) const {
        if (this->orientation == orientation_t::H
            && other.orientation == orientation_t::V) {
            return std::strong_ordering::less;
        }

        if (this->orientation == orientation_t::V
            && other.orientation == orientation_t::H) {
            return std::strong_ordering::greater;
        }

        if (this->word == other.word) {
            return std::strong_ordering::equal;
        }

        if (this->word < other.word) {
            return std::strong_ordering::less;
        }

        return std::strong_ordering::greater;
    }

    RectArea rect_area() const {
        return RectArea(start_pos, get_end_position());
    }

    ~Word() = default; // Destructor
};