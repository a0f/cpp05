#ifndef AFORM_HPP
# define AFORM_HPP

#include <iostream>
#include <stdexcept>
#include <string>

class Bureaucrat;

class AForm
{
	private:
		const std::string	m_name;
		bool m_is_signed;
		const int m_grade_to_sign;
		const int m_grade_to_excecute;
	public:
		AForm();
		AForm(std::string name, const int grade_to_sign, const int grade_to_excecute);
		AForm(const AForm& other);
		AForm& operator=(const AForm& other);
		virtual ~AForm();
		std::string getName() const;
		bool getIsSigned() const;
		int getGradeToSign() const;
		int getGradeToExcecute() const;
		void beSigned(Bureaucrat &b);
		void beExcecuted(Bureaucrat const &b) const;
		virtual void execute(Bureaucrat const & executor) const = 0;

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

		class FormNotSignedException : public std::exception
		{
			public:
				const char* what() const throw();
		};

};

std::ostream& operator<<(std::ostream& os, const AForm& form);

#endif
