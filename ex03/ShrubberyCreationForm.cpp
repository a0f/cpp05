#include "ShrubberyCreationForm.hpp"
#include "Bureaucrat.hpp"
#include <iostream>
#include <fstream>

ShrubberyCreationForm::ShrubberyCreationForm() : AForm("ShrubberyCreationForm", 145, 137), m_target("default") {}

ShrubberyCreationForm::ShrubberyCreationForm(std::string target) : AForm("ShrubberyCreationForm", 145, 137), m_target(target) {}

ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm& other) : AForm(other), m_target(other.m_target) {}

ShrubberyCreationForm& ShrubberyCreationForm::operator=(const ShrubberyCreationForm& other)
{
	(void)other;
	return *this;
}

ShrubberyCreationForm::~ShrubberyCreationForm() {}

void ShrubberyCreationForm::execute(Bureaucrat const & executor) const
{
	beExcecuted(executor);
	
	std::string filename = m_target + "_shrubbery";
	std::ofstream file(filename.c_str());

	if (file.is_open())
	{
		file << "    *\n"
			 << "   ***\n"
			 << "  *****\n"
			 << " *******\n"
			 << "    |\n";

		file.close();
	}
	else
	{
		std::cerr << "Failed to create file " << filename << std::endl;
	}
}
