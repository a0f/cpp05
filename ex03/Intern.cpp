#include "Intern.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

Intern::Intern() {}

Intern::Intern(const Intern& other) 
{ 
	(void)other; 
}

Intern& Intern::operator=(const Intern& other)
{
	(void)other;
	return *this;
}

Intern::~Intern() {}


AForm * makeShrubForm(std::string form_name)
{
	return(new ShrubberyCreationForm(form_name));
}

AForm * makeRobotomyForm(std::string form_name)
{
	return(new RobotomyRequestForm(form_name));
}

AForm * makePardonForm(std::string form_name)
{
	return new PresidentialPardonForm(form_name);
}

FromLookup functions[] =
{
	{"shrubbery", makeShrubForm},
	{"robotomy", makeRobotomyForm},
	{"pardon", makePardonForm}
};

AForm* Intern::makeForm(std::string name, std::string target) const
{
	for (size_t i = 0; i < 3; i++)
	{
		if (name == functions[i].form_name)
		{
			std::cout << "Intern creates " << name << std::endl;
			try
			{
				return (functions[i].create(target));
			}
			catch (std::bad_alloc &e)
			{
				std::cerr << e.what() << std::endl;
				return NULL;
			}
			
		}
	}
	std::cout << "Intern could not find a form by that name" << std::endl;
	return NULL;
}
