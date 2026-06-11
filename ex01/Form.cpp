#include "Form.hpp"
#include "Bureaucrat.hpp"

std::ostream& operator<<(std::ostream &os, const Form& form)
{
    os << form.getName() 
		<< ", " << (form.getIsSigned() ? "signed" : "not signed") 
		<< ", grade to sign " << form.getGradeToSign() 
		<< ", grade to execute " << form.getGradeToExecute() 
		<< std::endl;
    return os;
}

Form::Form() : m_name("default"), m_is_signed(false), m_grade_to_sign(75), m_grade_to_execute(75) {}

Form::Form(std::string name, const int grade_to_sign, const int grade_to_execute) : m_name(name), m_is_signed(false), m_grade_to_sign(grade_to_sign), m_grade_to_execute(grade_to_execute)
{
    if (m_grade_to_sign < 1 || m_grade_to_execute < 1)
    {
        throw Form::GradeTooHighException();
    }
    else if (m_grade_to_sign > 150 || m_grade_to_execute > 150)
    {
        throw Form::GradeTooLowException();
    }
}

Form::Form(const Form& other) : m_name(other.m_name), m_is_signed(other.m_is_signed), m_grade_to_sign(other.m_grade_to_sign), m_grade_to_execute(other.m_grade_to_execute) {}

Form& Form::operator=(const Form& other)
{
    (void)other;
    return *this;
}

std::string Form::getName() const
{
    return this->m_name;
}

bool Form::getIsSigned() const
{
	return this->m_is_signed;
}

int Form::getGradeToSign() const
{
    return this->m_grade_to_sign;
}

int Form::getGradeToExecute() const
{
	return this->m_grade_to_execute;
}

const char* Form::GradeTooLowException::what() const throw()
{
    return ("Grade too low");
}

const char* Form::GradeTooHighException::what() const throw()
{
    return ("Grade too high");
}

void	Form::beSigned(const Bureaucrat &b)
{
	int bur_grade = b.getGrade();

	if (bur_grade > this->getGradeToSign())
		throw Form::GradeTooLowException();
	m_is_signed = true;
}

Form::~Form() {}