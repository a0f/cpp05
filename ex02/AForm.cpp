#include "AForm.hpp"
#include "Bureaucrat.hpp"

std::ostream& operator<<(std::ostream &os, const AForm& form)
{
    os << form.getName() 
		<< ", " << (form.getIsSigned() ? "signed" : "not signed") 
		<< ", grade to sign " << form.getGradeToSign() 
		<< ", grade to excecute " << form.getGradeToExcecute() 
		<< std::endl;
    return os;
}

AForm::AForm() : m_name("default"), m_is_signed(false), m_grade_to_sign(75), m_grade_to_excecute(75) {}

AForm::AForm(std::string name, const int grade_to_sign, const int grade_to_excecute) : m_name(name), m_is_signed(false), m_grade_to_sign(grade_to_sign), m_grade_to_excecute(grade_to_excecute)
{
    if (m_grade_to_sign < 1 || m_grade_to_excecute < 1)
    {
        throw AForm::GradeTooHighException();
    }
    else if (m_grade_to_sign > 150 || m_grade_to_excecute > 150)
    {
        throw AForm::GradeTooLowException();
    }
}

AForm::AForm(const AForm& other) : m_name(other.m_name), m_is_signed(other.m_is_signed), m_grade_to_sign(other.m_grade_to_sign), m_grade_to_excecute(other.m_grade_to_excecute) {}

AForm& AForm::operator=(const AForm& other)
{
    (void)other;
    return *this;
}

std::string AForm::getName() const
{
    return this->m_name;
}

bool AForm::getIsSigned() const
{
	return this->m_is_signed;
}

int AForm::getGradeToSign() const
{
    return this->m_grade_to_sign;
}

int AForm::getGradeToExcecute() const
{
	return this->m_grade_to_excecute;
}

const char* AForm::GradeTooLowException::what() const throw()
{
    return ("Grade too low");
}

const char* AForm::GradeTooHighException::what() const throw()
{
    return ("Grade too high");
}

const char* AForm::FormNotSignedException::what() const throw()
{
    return("Form not signed");
}

void	AForm::beSigned(Bureaucrat &b)
{
	if (b.getGrade() > this->getGradeToSign())
		throw AForm::GradeTooLowException();
	m_is_signed = true;
}

void	AForm::beExcecuted(Bureaucrat const &b) const
{
    if (this->getIsSigned() == false)
    {
        throw AForm::FormNotSignedException();
    }

	if (b.getGrade() > this->getGradeToExcecute())
    {
		throw AForm::GradeTooLowException();
    }
}

AForm::~AForm() {}
