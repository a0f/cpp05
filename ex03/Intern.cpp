#include "Intern.hpp"

Intern::Intern() {}

Intern::Intern(const Intern& other) { (void)other; }

Intern& Intern::operator=(const Intern& other)
{
	(void)other;
	return *this;
}

Intern::~Intern() {}

AForm* Intern::makeForm(std::string name, std::string target) const
{
	// TODO: lookup table mapping name to form creator, return matching AForm*
	// Print "Intern creates <form>" on success
	// Print error and return NULL if name not found
	(void)name;
	(void)target;
	return NULL;
}
