#include <pump/Singleton.hpp>
#include <iostream>

class Person : public pump::Singleton<Person> {
public:
	Person()
		: name_("fengchao") {
	}

	std::string name() const {
		return name_;
	}

private:
	std::string name_;
};

int main() {
	Person* person = Person::get_instance();
	std::cout << person->name().c_str();
	return EXIT_SUCCESS;
}
