#include <string>
#include <unordered_map>
#include <unordered_set>

#include "poset.h"

using std::cerr;
using std::iostream;
using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;

using ver_to_id_t = unordered_map<string, size_t>;
using neighbours_t = unordered_set<size_t>;
using vertex_t = pair<neighbours_t, neighbours_t>;
using vertices_t = unordered_map<size_t, vertex_t>;
using poset_t = pair<ver_to_id_t, vertices_t>;
using posets_t = unordered_map<size_t, poset_t>;

#define IFDEBUG if constexpr (debug)

// ZADANIE POSET - PAULINA KUBERA I ŁUKASZ PIEKUTOWSKI
//
// Struktura przechowująca posety, to hashmapa, której kluczami są
// id posetów, a wartościami - struktury posetów.
//
// Struktura posetu to para przechowująca "słownik" nazw wierzchołków
// oraz strukturę wierzchołków.
//
// "Słownik" to hashmapa, której kluczami są nazwy wierzchołków,
// a wartościami - odpowiadające im id. Id jest unikalne dla
// wierzchołków również między posetami.
//
// Struktura wierzchołków to hashmapa, której kluczami są id wierzchołków,
// a wartościami - struktura pojedynczego wierzchołka.
//
// Struktura wierzchołka o id x, to para hashsetów, gdzie .first jest
// zbiorem wierzchołków, które mają bezpośrednią ścieżkę wejściową do x, a
// .second - tych, do których istnieje bezpośrednia ścieżka wejściowa z x.
//
// Niezmiennikiem tego rozwiązania jest to, że każdy wierzchołek trzyma w swoich
// hashsetach wierzchołek wtw jest on jego bezpośrednim sąsiadem
// (np. dla relacji a->b->c, hashset .second wierzchołka a ma postać {b},
// a para hashsetów b - <{a}, {c}>).

// Stałe, zmienne i funkcje pomocnicze.

namespace {

#ifdef NDEBUG
  bool constexpr debug = false;
#else
  bool constexpr debug = true;
#endif

posets_t &posets() {
  static posets_t posets;
  return posets;
}

size_t &num_of_posets() {
  static size_t num_of_posets = 0;
  return num_of_posets;
}

size_t &num_of_all_vertices() {
  static size_t num_of_all_vertices = 0;
  return num_of_all_vertices;
}

// Sprawdza, czy poset o danym id istnieje.
bool poset_exists(size_t id) { return posets().count(id); }

// Sprawdza, czy w danym posecie istnieje wierzchołek o danym id.
bool vertex_exists(size_t poset_id, size_t vertex_id) {
  return posets()[poset_id].second.count(vertex_id);
}

// Zwraca id wierzchołka o danej nazwie.
size_t name_to_id(size_t poset_id, string &name) {
  if (posets()[poset_id].first.count(name) == 0)
    return num_of_all_vertices() + 1;
  return posets()[poset_id].first[name];
}

// Zwraca wskaźnik na poset o danym id.
poset_t *get_poset(size_t id) { return &posets()[id]; }

// Zwraca wskaźnik na set wyjściowych krawędzi z danego wierzchołka.
neighbours_t *get_out(size_t poset_id, size_t vertex_id) {
  return &posets()[poset_id].second[vertex_id].second;
}

// Zwraca wskaźnik na set wejściowych krawędzi do danego wierzchołka.
neighbours_t *get_in(size_t poset_id, size_t vertex_id) {
  return &posets()[poset_id].second[vertex_id].first;
}

// Sprawdza, czy istnieje relacja między dwoma danymi wierzchołkami.
// Zwraca true, jeśli relacja istnieje.
bool connection_exists(size_t poset_id, size_t current_vertex_id,
                       size_t end_vertex_id) {
  if (current_vertex_id == end_vertex_id) return true;
  for (size_t neighbour : *get_out(poset_id, current_vertex_id)) {
    if (connection_exists(poset_id, neighbour, end_vertex_id)) return true;
  }
  return false;
}

// Dodaje relację między wierzchołkami id1, id2.
// Funkcja zakłada, że dodanie tej relacji ta jest zgodna z założeniami posetu.
void add_connection(size_t poset_id, size_t id1, size_t id2) {
  get_in(poset_id, id2)->insert(id1);
  get_out(poset_id, id1)->insert(id2);
}

// Usuwa relację między wierzchołkami id1, id2.
// Funkcja zakłada, że usunięcie tej relacji jest zgodne z założeniami posetu.
void delete_connection(size_t poset_id, size_t id1, size_t id2) {
  get_in(poset_id, id2)->erase(id1);
  get_out(poset_id, id1)->erase(id2);
}

// Przy usuwaniu wierzchołka z posetu,
// funkcja "przepina" relacje usuwanego wierzchołka tak,
// by zachować niezmiennik tego rozwiązania.
void reconnect(size_t p_id, size_t v_id) {
  for (auto i : *get_in(p_id, v_id)) {
    get_out(p_id, i)->erase(v_id);
    get_out(p_id, i)->insert(get_out(p_id, v_id)->begin(),
                             get_out(p_id, v_id)->end());
  }
  for (auto i : *get_out(p_id, v_id)) {
    get_in(p_id, i)->erase(v_id);
    get_in(p_id, i)->insert(get_in(p_id, v_id)->begin(),
                            get_in(p_id, v_id)->end());
  }
}

// Sprawdza, czy wierzchołki o danych nazwach należą do danego posetu
// oraz, czy ich nazwy nie są nullpointerami.
bool check_two_names(size_t id, char const *value1, char const *value2) {
  if (value1 == nullptr || value2 == nullptr) return false;
  string name1 = value1;
  string name2 = value2;
  if (!poset_exists(id) || !vertex_exists(id, name_to_id(id, name1)) ||
      !vertex_exists(id, name_to_id(id, name2)))
    return false;
  return true;
}

// Sprawdza, czy wierzchołek o danej nazwie należy do danego posetu
// oraz, czy jego nazwa nie jest nullpointerem.
bool check_name(size_t id, char const *value, bool mode) {
  if (value == nullptr) return false;
  string name = value;
  if (!poset_exists(id) || !(vertex_exists(id, name_to_id(id, name)) ^ mode)) {
    return false;
  }
  return true;
}

string s_to_out(char const *value) {
  if (value == nullptr) return "NULL";
  string ret = value;
  ret = "\"" + ret + "\"";
  return ret;
}

void error_two_names(size_t id, char const *value1, char const *value2,
                     string func_name) {
  bool poset_ex = poset_exists(id);
  if (!poset_ex) cerr << func_name << ": poset " << id << " does not exist\n";
  if (value1 == nullptr)
    cerr << func_name << ": invalid value1 (NULL)\n";
  else if (poset_ex) {
    string name1 = value1;
    if (!vertex_exists(id, name_to_id(id, name1)))
      cerr << func_name << ": poset " << id << ", element \"" << name1
           << "\" does not exist\n";
  }
  if (value2 == nullptr)
    cerr << func_name << ": invalid value2 (NULL)\n";
  else if (poset_ex) {
    string name2 = value2;
    if (!vertex_exists(id, name_to_id(id, name2)))
      cerr << func_name << ": poset " << id << ", element \"" << name2
           << "\" does not exist\n";
  }
}

void error_name(size_t id, char const *value, string func_name, bool mode) {
  bool poset_ex = poset_exists(id);
  if (!poset_ex) cerr << func_name << ": poset " << id << " does not exist\n";
  if (value == nullptr)
    cerr << func_name << ": invalid value (NULL)\n";
  else if (poset_ex) {
    string name = value;
    if (!(vertex_exists(id, name_to_id(id, name)) ^ mode)) {
      if (mode == 0)
        cerr << func_name << ": poset " << id << ", element \"" << name
             << "\" does not exist\n";
      else
        cerr << func_name << ": poset " << id << ", element \"" << name
             << "\" already exists\n";
    }
  }
}
}  // namespace

// Właściwa treść biblioteki poset.

namespace cxx {

// Tworzy nowy poset.
unsigned long poset_new(void) {
  IFDEBUG cerr << "poset_new()\n";
  posets()[num_of_posets()];
  num_of_posets()++;
  IFDEBUG cerr << "poset_new: poset " << num_of_posets() - 1 << " created\n";
  return num_of_posets() - 1;
}

// Usuwa dany poset.
void poset_delete(unsigned long id) {
  IFDEBUG cerr << "poset_delete(" << id << ")\n";
  if (poset_exists(id)) {
    IFDEBUG cerr << "poset_delete: poset " << id << " deleted\n";
    posets().erase(id);
    return;
  }
  IFDEBUG cerr << "poset_delete: poset " << id << " does not exist\n";
}

// Zwraca wielkość posetu, jeśli dany poset istnieje.
// W przeciwnym wypadku, zwraca 0.
size_t poset_size(unsigned long id) {
  IFDEBUG cerr << "poset_size(" << id << ")\n";
  if (poset_exists(id)) {
    size_t ret = posets()[id].first.size();
    IFDEBUG cerr << "poset_size: poset " << id << " contains " << ret
                 << " element(s)\n";
    return ret;
  }
  IFDEBUG cerr << "poset_size: poset " << id << " does not exist\n";
  return 0;
}

// Dodaje wierzchołek do posetu, nadając mu unikalne id.
bool poset_insert(unsigned long id, char const *value) {
  IFDEBUG cerr << "poset_insert(" << id << ", " << s_to_out(value) << ")\n";
  if (!check_name(id, value, 1)) {
    IFDEBUG error_name(id, value, "poset_insert", 1);
    return false;
  }
  string name = value;
  poset_t *poset = get_poset(id);
  poset->first[name] = num_of_all_vertices();
  poset->second[num_of_all_vertices()];
  num_of_all_vertices()++;
  IFDEBUG cerr << "poset_insert: poset " << id << ", element \"" << name
               << "\" inserted\n";
  return true;
}

// Usuwa wierzchołek z posetu, "przepinając" jego
// relacje przy użyciu funkcji reconnect.
bool poset_remove(unsigned long id, char const *value) {
  IFDEBUG cerr << "poset_remove(" << id << ", " << s_to_out(value) << ")\n";
  if (!check_name(id, value, 0)) {
    IFDEBUG error_name(id, value, "poset_remove", 0);
    return false;
  }
  string name = value;
  size_t v_id = name_to_id(id, name);
  poset_t *poset = get_poset(id);
  reconnect(id, v_id);
  poset->second.erase(v_id);
  poset->first.erase(name);
  IFDEBUG cerr << "poset_remove: poset " << id << ", element \"" << name
               << "\" removed\n";
  return true;
}

// Dodaje relację do posetu.
bool poset_add(unsigned long id, char const *value1, char const *value2) {
  IFDEBUG cerr << "poset_add(" << id << ", " << s_to_out(value1) << ", "
               << s_to_out(value2) << ")\n";
  if (!check_two_names(id, value1, value2)) {
    IFDEBUG error_two_names(id, value1, value2, "poset_add");
    return false;
  }
  string name1 = value1;
  string name2 = value2;
  size_t id1 = name_to_id(id, name1);
  size_t id2 = name_to_id(id, name2);
  if (connection_exists(id, id2, id1) || connection_exists(id, id1, id2)) {
    IFDEBUG cerr << "poset_add: poset " << id << ", relation (\"" << name1
                 << "\", \"" << name2 << "\") cannot be added\n";
    return false;
  }
  add_connection(id, id1, id2);
  IFDEBUG cerr << "poset_add: poset " << id << ", relation (\"" << name1
               << "\", \"" << name2 << "\") added\n";
  return true;
}

// Usuwa relację pod warunkiem, że nie zaburzy ona niezmiennika posetu.
bool poset_del(unsigned long id, char const *value1, char const *value2) {
  IFDEBUG cerr << "poset_del(" << id << ", " << s_to_out(value1) << ", "
               << s_to_out(value2) << ")\n";
  if (!check_two_names(id, value1, value2)) return false;
  string name1 = value1;
  string name2 = value2;
  size_t id1 = name_to_id(id, name1);
  size_t id2 = name_to_id(id, name2);
  if (get_out(id, id1)->count(id2) == 0) return false;
  delete_connection(id, id1, id2);
  if (connection_exists(id, id1, id2)) return false;
  for (auto i : *get_out(id, id2)) {
    add_connection(id, id1, i);
  }
  for (auto i : *get_in(id, id1)) {
    add_connection(id, i, id2);
  }
  return true;
}

// Sprawdza, czy istnieje relacja między danymi wierzchołkami.
bool poset_test(unsigned long id, char const *value1, char const *value2) {
  IFDEBUG cerr << "poset_test(" << id << ", " << s_to_out(value1) << ", "
               << s_to_out(value2) << ")\n";
  if (!check_two_names(id, value1, value2)) {
    IFDEBUG error_two_names(id, value1, value2, "poset_test");
    return false;
  }
  string name1 = value1;
  string name2 = value2;
  size_t id1 = name_to_id(id, name1);
  size_t id2 = name_to_id(id, name2);
  if (connection_exists(id, id1, id2)) {
    IFDEBUG cerr << "poset_test: poset " << id << ", relation (\"" << name1
                 << "\", \"" << name2 << "\") exists\n";
    return true;
  }
  IFDEBUG cerr << "poset_test: poset " << id << ", relation (\"" << name1
               << "\", \"" << name2 << "\") does not exist\n";
  return false;
}

// Usuwa wszystkie wierzchołki z posetu.
void poset_clear(unsigned long id) {
  IFDEBUG cerr << "poset_clear(" << id << ")\n";
  if (!poset_exists(id)) {
    IFDEBUG cerr << "poset_clear: poset " << id << " does not exist\n";
    return;
  }
  poset_t *poset = get_poset(id);
  poset->first.clear();
  poset->second.clear();
  IFDEBUG cerr << "poset_clear: poset " << id << " cleared\n";
}
}  // namespace cxx