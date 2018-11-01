#include <pump/Singleton.hpp>
#include <iostrea>

class Person : public pump::Singleton<Person> {
 public:
  Person()
      : name_("fengchao") {}

  const std::string& name() const {
    return name_;
  }
 private:
  std::string name_;
};

int main() {
  const Person* person = Person::get_instance();
  std::cout << person->name();
  return EXIT_SUCCESS;
}