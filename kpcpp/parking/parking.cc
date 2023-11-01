#include <iostream>
#include <set>
#include <unordered_map>
#include <regex>
#include <string>
#include <sstream>

#define Hour pair<int8_t, int8_t>
#define CarSet set<pair<Hour, string>>
#define CarMap unordered_map<string, pair<Hour, Hour>>

using namespace std;

// creating needed regular expressions
const string id_pattern = "([A-Z])([A-Z]|[0-9]){2,10}";
const string time_pattern = "((0?[89]|1[0-9])\\.([0-5][0-9])|(20\\.00))";
const string parking_pattern = "^(\\s*)" + id_pattern + "(\\s+)" + time_pattern
                               + "(\\s+)" + time_pattern + "(\\s*)$";
const string query_pattern = "^(\\s*)" + id_pattern + "(\\s+)" + time_pattern + "(\\s*)$";

// calculates and returns minute difference between tho given hours
int16_t hour_difference(Hour start, Hour stop) {
    stop.second -= start.second;
    if (stop.second < 0) {
        stop.second += 60;
        stop.first--;
    }
    stop.first -= start.first;
    return (stop.first * 60 + stop.second);
}

// checks if given parking hours are valid
bool valid_parking(Hour start, Hour stop) {
    if (hour_difference(start, stop) > 11 * 60 + 59)
        return 0;
    if (hour_difference(start, stop) < 10 && hour_difference(start, stop) >= 0)
        return 0;
    if (hour_difference(start, stop) < 0) {
        int16_t act_difference = 0;
        act_difference += hour_difference(start, make_pair(20, 00));
        act_difference += hour_difference(make_pair(8, 00), stop);
        if (act_difference <= 11 * 60 + 59 && act_difference >= 10)
            return 1;
        return 0;
    }
    return 1;
}

// matches input with regular expression and returns its type
// 1 - invalid input
// 0 - no further input
// 1 - arrival of another car in the parking
// 2 - payment query
int8_t get_info(string &line) {
    static regex const parking_line(parking_pattern);
    static regex const query_line(query_pattern);
    if (!getline(cin, line)) return 0;
    if (regex_match(line, parking_line)) return 1;
    if (regex_match(line, query_line)) return 2;
    return -1;
}

// casts string time format to pair<int, int> data format
Hour str_to_hour(string a) {
    if (a.size() == 4) a = "0" + a;
    return make_pair(stoi(a.substr(0, 2)), stoi(a.substr(3, 2)));
}

// updates current time, erasing from memory redundant data
//
// as some cars are stored in set "tomorrow", checks if the day had changed
// if so, erases set "today" (which now actually contains cars from yesterday)
// and swaps it with set "tomorrow"
void update_hour(CarSet &today, CarSet &tomorrow, CarMap &cars,
                 Hour &clock, const Hour now) {
    if (hour_difference(clock, now) < 0) {
        for (const auto &i: today) cars.erase(i.second);
        today.clear();
        today.swap(tomorrow);
    }
    clock = now;

    while (!today.empty() && today.begin()->first < clock) {
        cars.erase(today.begin()->second);
        today.erase(today.begin());
    }
}

// checks if paid time of car that is already on the parking
// is being extended
bool is_extended(CarSet &today, CarMap &cars, const string &id,
                 const Hour start, const Hour stop) {
    if (today.contains(make_pair(cars[id].second, id))) {
        if (hour_difference(start, stop) < 0) return 1;
        if (hour_difference(cars[id].second, stop) >= 0) return 1;
        return 0;
    } else {
        if (hour_difference(start, stop) > 0) return 0;
        if (hour_difference(cars[id].second, stop) <= 0) return 0;
        return 1;
    }
}

// handles arrival of another car, adding it to the proper set
// and updating the map of parked cars
//
// sometimes, a car which has already parked may want to extend its paid time
// if so, function erases previous data about the car and then adds new data
void park_a_car(CarSet &today, CarSet &tomorrow, CarMap &cars,
                const string &line, const int32_t order_number, Hour &clock) {
    // normalizing entry line
    string id, start_s, stop_s;
    stringstream ss;
    ss << line;
    ss >> id >> start_s >> stop_s;
    Hour start = str_to_hour(start_s), stop = str_to_hour(stop_s);

    // checking if parking hours are valid
    if (!valid_parking(start, stop)) {
        cerr << "ERROR " << order_number << '\n';
        return;
    }

    // updating current time
    update_hour(today, tomorrow, cars, clock, start);

    // checking if the car extends it's paid time
    if (cars.contains(id)) {
        if (is_extended(today, cars, id, start, stop)) {
            pair<Hour, string> car_data = make_pair(cars[id].second, id);
            today.erase(car_data);
            tomorrow.erase(car_data);
            cars.erase(id);
        } else {
            cout << "OK " << order_number << '\n';
            return;
        }
    }

    // adding the car to the proper set
    if (hour_difference(start, stop) < 0)
        tomorrow.insert(make_pair(stop, id));
    else
        today.insert(make_pair(stop, id));

    // adding the car to the map of cars currently on the parking
    cars[id] = make_pair(start, stop);
    cout << "OK " << order_number << '\n';
}

// checks if a car has paid for given hour
void answer_query(CarSet &today, CarSet &tomorrow, CarMap &cars,
                  const string &line, const int32_t order_number, Hour &clock) {
    // normalizing entry line
    string id, q_time_s;
    stringstream ss;
    ss << line;
    ss >> id >> q_time_s;
    Hour q_time = str_to_hour(q_time_s);

    // updating current time
    update_hour(today, tomorrow, cars, clock, q_time);

    // after updating the hour, only cars that have parked before/during current hour
    // and are still in the time range of their payment, are contained in the map
    if (cars.count(id) == 0)
        cout << "NO " << order_number << '\n';
    else
        cout << "YES " << order_number << '\n';
}

int main() {
    CarSet today, tomorrow;         // sets containing cars parked till today and till tomorrow
    CarMap cars;                    // map with a key-value id-parking_hours
    Hour clock = make_pair(8, 0);   // current time
    string line;                    // loaded line
    int32_t order_number = 1;       // number of line

    while (true) {
        int8_t out = get_info(line);
        if (out == -1) cerr << "ERROR " << order_number << '\n';
        else if (out == 0) break;
        else if (out == 1) park_a_car(today, tomorrow, cars, line, order_number, clock);
        else answer_query(today, tomorrow, cars, line, order_number, clock);
        order_number++;
    }
    return 0;
}
