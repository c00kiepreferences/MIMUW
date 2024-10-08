#ifndef COLLEGE_H
#define COLLEGE_H

#include <algorithm>
#include <cassert>
#include <map>
#include <regex>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>

class Course;
class Person;
class Student;
class Teacher;
class PhDStudent;
class College;

class Course {
public:
    explicit Course(std::string name, bool active = true)
            : name_(std::move(name)), active_(active) {}

    std::string get_name() const { return name_; }
    bool is_active() const { return active_; }

    void set_active(bool active) { active_ = active; }

private:
    std::string name_;
    bool active_;
};

class Person {
public:
    Person(std::string name, const std::string& surname)
            : name_(std::move(name)), surname_(surname) {}

    virtual ~Person() = default;

    std::string get_name() const { return name_; }
    std::string get_surname() const { return surname_; }

private:
    std::string name_;
    std::string surname_;
};

struct CourseComparator {
    bool operator()(const std::shared_ptr<Course>& a,
                    const std::shared_ptr<Course>& b) const {
        return a->get_name() < b->get_name();
    }
};

using course_set = std::set<std::shared_ptr<Course>, CourseComparator>;

struct PersonComparator {
    bool operator()(const std::shared_ptr<Person>& a,
                    const std::shared_ptr<Person>& b) const {
        return a->get_surname() == b->get_surname()
               ? a->get_name() < b->get_name()
               : a->get_surname() < b->get_surname();
    }
};

template <typename T>
using person_set = std::set<std::shared_ptr<T>, PersonComparator>;

class Student : virtual public Person {
public:
    Student(const std::string& name, const std::string& surname,
            bool active = true)
            : Person(name, surname), active_(active) {}

    void add_course(const std::shared_ptr<Course>& course) {
        courses_.insert(course);
    }

    void set_active(bool active) { active_ = active; }
    bool is_active() const { return active_; }

    const course_set& get_courses() const { return courses_; }

private:
    course_set courses_;
    bool active_;
};

class Teacher : virtual public Person {
public:
    Teacher(const std::string& name, const std::string& surname)
            : Person(name, surname) {}

    const course_set& get_courses() const { return courses_; }

    void add_course(std::shared_ptr<Course> course) { courses_.insert(course); }

private:
    course_set courses_;
};

class PhDStudent : public Student, public Teacher {
public:
    PhDStudent(const std::string& name, const std::string& surname,
               bool active = true)
            : Person(name, surname),
              Student(name, surname, active),
              Teacher(name, surname) {}
};

class College {
public:
    College() = default;

    bool add_course(const std::string& name, bool active = true) {
        auto course = std::make_shared<Course>(name, active);
        if (courses_.find(course) != courses_.end()) {
            return false;
        }
        courses_.insert(course);
        return true;
    }

    course_set find_courses(const std::string& pattern) {
        std::regex regex_pattern(glob_to_regex(pattern));
        course_set found;

        for (const auto& course : courses_) {
            if (std::regex_match(course->get_name(), regex_pattern)) {
                found.insert(course);
            }
        }

        return found;
    }

    template <typename T>
    person_set<T> find(std::shared_ptr<Course> course) {
        person_set<T> found;
        static_assert(std::is_base_of<Person, T>::value,
                      "T must be a subclass of Person.");

        auto it = courses_.find(course);
        if (it == courses_.end() || it->get() != course.get())
            return person_set<T>();

        if constexpr (std::is_same<T, Teacher>::value) {
            return attendees_[course].second;
        } else
        return attendees_[course].first;
    }

    bool change_course_activeness(const std::shared_ptr<Course>& course,
                                  bool active) {
        auto it = courses_.find(course);
        if (it != courses_.end() && it->get() == course.get()) {
            (*it)->set_active(active);
            attendees_.erase(course);
            return true;
        }
        return false;
    }

    bool remove_course(const std::shared_ptr<Course>& course) {
        auto it = courses_.find(course);
        if (it != courses_.end() && it->get() == course.get()) {
            (*it)->set_active(false);
            courses_.erase(it);
            attendees_.erase(course);
            return true;
        }
        return false;
    }

    bool change_student_activeness(std::shared_ptr<Student> student,
                                   bool active) {
        auto it = people_.find(student);
        if (it != people_.end() && it->get() == student.get()) {
            student->set_active(active);
            return true;
        }
        return false;
    }

    template <typename T>
    bool add_person(const std::string& name, const std::string& surname,
                    bool active = true) {
        static_assert(std::is_base_of<Person, T>::value,
                      "T must be a subclass of Person.");

        std::shared_ptr<T> person;

        if constexpr (std::is_same<T, Teacher>::value) {
            person = std::make_shared<T>(name, surname);
        } else {
            person = std::make_shared<T>(name, surname, active);
        }

        if (people_.find(person) != people_.end()) {
            return false;
        }

        people_.insert(person);
        return true;
    }

    template <typename T>
    person_set<T> find(const std::string& name_pattern,
                       const std::string& surname_pattern) {
        std::regex name_regex(glob_to_regex(name_pattern));
        std::regex surname_regex(glob_to_regex(surname_pattern));
        person_set<T> found;

        for (const auto& person : people_) {
            auto derived = std::dynamic_pointer_cast<T>(person);
            if (derived && std::regex_match(derived->get_name(), name_regex) &&
                std::regex_match(derived->get_surname(), surname_regex)) {
                found.insert(derived);
            }
        }

        return found;
    }

    template <typename T>
    bool assign_course(std::shared_ptr<T> person,
                       std::shared_ptr<Course> course) {
        static_assert(std::is_base_of<Person, T>::value,
                      "T must be a subclass of Person.");
        validate_course(course);
        validate_person(person);

        if constexpr (std::is_same<T, Student>::value) {
            if (!person->is_active()) {
                throw std::runtime_error(
                        "Incorrect operation for an inactive student.");
            }
            if (!check_assigned_courses(person, course)) {
                return false;
            }
            person->add_course(course);
            attendees_[course].first.insert(person);
        } else if constexpr (std::is_same<T, Teacher>::value) {
            if (!check_assigned_courses(person, course)) {
                return false;
            }
            person->add_course(course);
            attendees_[course].second.insert(person);
        } else {
            throw std::runtime_error("Invalid person type.");
        }
        return true;
    }

private:
    course_set courses_;
    person_set<Person> people_;

    using attendees_t = std::pair<person_set<Student>, person_set<Teacher>>;
    std::map<std::shared_ptr<Course>, attendees_t, CourseComparator> attendees_;

    static std::string glob_to_regex(const std::string& glob) {
        std::string regex;
        for (char c : glob) {
            switch (c) {
                case '*':
                    regex += ".*";
                    break;
                case '?':
                    regex += '.';
                    break;
                case '+':
                    regex += "\\+";
                    break;
                default:
                    regex += c;
                    break;
            }
        }
        return regex;
    }

    void validate_course(std::shared_ptr<Course> course) const {
        auto course_it = std::find(courses_.begin(), courses_.end(), course);
        if (course_it == courses_.end() || course_it->get() != course.get()) {
            throw std::runtime_error("Non-existing course.");
        }

        if (!course->is_active()) {
            throw std::runtime_error("Incorrect operation on an inactive course.");
        }
    }

    template <typename T>
    void validate_person(std::shared_ptr<T> person) const {
        auto person_it = std::find(people_.begin(), people_.end(), person);
        if (person_it == people_.end() || person_it->get() != person.get()) {
            throw std::runtime_error("Non-existing person.");
        }
    }

    template <typename T>
    bool check_assigned_courses(std::shared_ptr<T> person,
                                std::shared_ptr<Course> course) const {
        if (person->get_courses().find(course) != person->get_courses().end()) {
            return false;
        }
        return true;
    }
};

#endif  // COLLEGE_H