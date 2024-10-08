#ifndef CROSSWORD_H
#define CROSSWORD_H

#include <iostream>
#include <compare>

#define DEFAULT_EMPTY_RECT_AREA RectArea(pos_t(1, 1), pos_t(0, 0))
#define DEFAULT_CHAR '?'
#define DEFAULT_WORD Word(0, 0, orientation_t::H, "?")

// Used to represent a point on the plane (x, y)
using pos_t = std::pair<std::size_t, std::size_t>;
// Used to represent a dimension (width, height)
using dim_t = std::pair<std::size_t, std::size_t>;
// Used to represent orientation of a word (horizontal or vertical)
using orientation_t = char;

class RectArea;

enum class orientation_t;

class Word;

#endif
