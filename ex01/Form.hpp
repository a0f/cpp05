#ifndef FORM_HPP
# define FORM_HPP

#include <iostream>
#include <stdexcept>
#include <string>

class Bureaucrat;

class Form
{
	private:
		const std::string	m_name;
		bool m_is_signed;
		const int m_grade_to_sign;
		const int m_grade_to_execute;
	public:
		Form();
		Form(std::string name, const int grade_to_sign, const int grade_to_execute);
		Form(const Form& other);
		Form& operator=(const Form& other);
		~Form();
		std::string getName() const;
		bool getIsSigned() const;
		int getGradeToSign() const;
		int getGradeToExecute() const;
		void beSigned(const Bureaucrat &b);

		class GradeTooLowException : public std::exception
        {
            public:
                const char* what() const throw();
        };

        class GradeTooHighException : public std::exception
        {
            public:
                const char* what() const throw();
        };

};

std::ostream& operator<<(std::ostream& os, const Form& form);

#endif