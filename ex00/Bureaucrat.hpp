#ifndef BUREAUCRAT_HPP
# define BUREAUCRAT_HPP

#include <iostream>
#include <stdexcept>
#include <string>


class Bureaucrat
{
    private:
        const std::string    m_name;
        int                 m_grade;
    public:
        Bureaucrat();
        Bureaucrat(std::string name, int grade);
        Bureaucrat(const Bureaucrat& other);
        Bureaucrat& operator=(const Bureaucrat& other);
        ~Bureaucrat();
        std::string getName() const;
        int getGrade() const;
        void incrementGrade();
        void decrementGrade();

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

std::ostream& operator<<(std::ostream& os, const Bureaucrat& bureaucrat);

#endif