#include "RobotomyRequestForm.hpp"
#include "Bureaucrat.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

RobotomyRequestForm::RobotomyRequestForm() : AForm("RobotomyRequestForm", 72, 45), m_target("default") {}

RobotomyRequestForm::RobotomyRequestForm(std::string target) : AForm("RobotomyRequestForm", 72, 45), m_target(target) {}

RobotomyRequestForm::RobotomyRequestForm(const RobotomyRequestForm& other) : AForm(other), m_target(other.m_target) {}

RobotomyRequestForm& RobotomyRequestForm::operator=(const RobotomyRequestForm& other)
{
	if (this != &other)
		m_target = other.m_target;
	return *this;
}

RobotomyRequestForm::~RobotomyRequestForm() {}

void RobotomyRequestForm::execute(Bureaucrat const & executor) const
{
	beExecuted(executor);
	
	std::srand(static_cast<unsigned int>(std::time(0)));
	int random_num = (std::rand() % 10) + 1;

	std::cout << "BZZZZZT... BZZZZZT... BZZZZZT..." << std::endl;
	if (random_num > 5)
	{
		std::cout << m_target << " HAS BEEN SUCCESSFULLY ROBOTOMIZED.\n";
	}
	else
	{
		std::cout << "ROBOTOMY HAS FAILED. :(\n";
	}

}
