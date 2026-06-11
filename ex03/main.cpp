#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include "Intern.hpp"

int main(void)
{


    ShrubberyCreationForm lilshrubster("lilshrubster");
    RobotomyRequestForm robotomizer("robotomizer");
    PresidentialPardonForm prespardoner("pres_pardoner");

    Bureaucrat lowgrade("lowgrade", 150);
    Bureaucrat shrubsigner("shrubsigner", 145);
    Bureaucrat shrubexec("shrubexec", 137);
    Bureaucrat robotsigner("robotsigner", 72);
    Bureaucrat robotexec("robotexec", 45);
    Bureaucrat prespardsigner("prespardsigner", 25);
    Bureaucrat prespardexec("prespardexec", 5);

    std::cout << "------------------------------------\n";
    std::cout << "Intern tests\n";
    std::cout << "------------------------------------\n";

    Intern billy;

    AForm* f1 = billy.makeForm("shrubbery", "garden");
    if (f1)
    {
        shrubsigner.signForm(*f1);
        shrubexec.executeForm(*f1);
        delete f1;
    }
    

    AForm* f2 = billy.makeForm("robotomy", "bender");
    if (f2)
    {
        robotsigner.signForm(*f2);
        robotexec.executeForm(*f2);
        delete f2;
    }
    

    AForm* f3 = billy.makeForm("pardon", "zoidberg");

    if (f3)
    {
        prespardsigner.signForm(*f3);
        prespardexec.executeForm(*f3);
        delete f3;
    }

    AForm* f4 = billy.makeForm("unknown form", "nobody");
    if (f4 == NULL)
    {
        std::cout << "Got NULL ptr" << std::endl;
    }


    return 0;
}