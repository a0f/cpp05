#include "AForm.hpp"
#include "Bureaucrat.hpp"

std::ostream& operator<<(std::ostream &os, const AForm& form)
{
    os << form.getName() 
		<< ", " << (form.getIsSigned() ? "signed" : "not signed") 
		<< ", grade to sign " << form.getGradeToSign() 
		<< ", grade to execute " << form.getGradeToExecute() 
		<< std::endl;
    return os;
}

AForm::AForm() : m_name("default"), m_is_signed(false), m_grade_to_sign(75), m_grade_to_execute(75) {}

AForm::AForm(std::string name, const int grade_to_sign, const int grade_to_execute) : m_name(name), m_is_signed(false), m_grade_to_sign(grade_to_sign), m_grade_to_execute(grade_to_execute)
{
    if (m_grade_to_sign < 1 || m_grade_to_execute < 1)
    {
        throw AForm::GradeTooHighException();
    }
    else if (m_grade_to_sign > 150 || m_grade_to_execute > 150)
    {
        throw AForm::GradeTooLowException();
    }
}

AForm::AForm(const AForm& other) : m_name(other.m_name), m_is_signed(other.m_is_signed), m_grade_to_sign(other.m_grade_to_sign), m_grade_to_execute(other.m_grade_to_execute) {}

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

int AForm::getGradeToExecute() const
{
	return this->m_grade_to_execute;
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

void	AForm::beSigned(const Bureaucrat &b)
{
	if (b.getGrade() > this->getGradeToSign())
		throw AForm::GradeTooLowException();
	m_is_signed = true;
}

void	AForm::beExecuted(Bureaucrat const &b) const
{
    if (this->getIsSigned() == false)
    {
        throw AForm::FormNotSignedException();
    }

	if (b.getGrade() > this->getGradeToExecute())
    {
		throw AForm::GradeTooLowException();
    }
}

AForm::~AForm() {}
