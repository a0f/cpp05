#include "Bureaucrat.hpp"

std::ostream& operator<<(std::ostream &os, const Bureaucrat& bureaucrat)
{
    os << bureaucrat.getName() << ", bureaucrat grade " << bureaucrat.getGrade() << std::endl;
    return os;
}

Bureaucrat::Bureaucrat() : m_name("default"), m_grade(75) {}

Bureaucrat::Bureaucrat(std::string name, int grade) : m_name(name), m_grade(grade)
{
    if (m_grade < 1)
    {
        throw Bureaucrat::GradeTooHighException();
    }
    else if (m_grade > 150)
    {
        throw Bureaucrat::GradeTooLowException();
    }
}

Bureaucrat::Bureaucrat(const Bureaucrat& other) : m_name(other.m_name), m_grade(other.m_grade) {}

Bureaucrat& Bureaucrat::operator=(const Bureaucrat& other)
{
    (void)other;
    return *this;
}

std::string Bureaucrat::getName() const
{
    return this->m_name;
}

int Bureaucrat::getGrade() const
{
    return this->m_grade;
}

void Bureaucrat::incrementGrade()
{
    if ((m_grade - 1) < 1)
    {
        throw Bureaucrat::GradeTooHighException();
    }
    m_grade--;
}

void Bureaucrat::decrementGrade()
{
    if ((m_grade + 1) > 150)
    {
        throw Bureaucrat::GradeTooLowException();
    }
    m_grade++;
}

void Bureaucrat::signForm(AForm& form)
{
    try
    {
        form.beSigned(*this);
        std::cout << this->m_name << " signed " << form.getName() << std::endl;
    }
    
    catch(const AForm::GradeTooLowException & e)
    {
        std::cerr << this->getName() << " couldn't sign " << form.getName() << " because " << e.what() << std::endl;
    }
    
}

const char* Bureaucrat::GradeTooLowException::what() const throw()
{
    return ("Grade too low");
}

const char* Bureaucrat::GradeTooHighException::what() const throw()
{
    return ("Grade too high");
}

Bureaucrat::~Bureaucrat() {}