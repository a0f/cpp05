#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

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
    std::cout << "ShrubberyCreationForm tests\n";
    std::cout << "------------------------------------\n";
    lowgrade.signForm(lilshrubster);
    lowgrade.executeForm(lilshrubster);
    shrubsigner.signForm(lilshrubster);
    lowgrade.executeForm(lilshrubster);
    shrubexec.executeForm(lilshrubster);

    std::cout << "------------------------------------\n";
    std::cout << "RobotomyRequestForm tests\n";
    std::cout << "------------------------------------\n";
    lowgrade.signForm(robotomizer);
    lowgrade.executeForm(robotomizer);
    robotsigner.signForm(robotomizer);
    lowgrade.executeForm(robotomizer);
    robotexec.executeForm(robotomizer);

    std::cout << "------------------------------------\n";
    std::cout << "PresidentialPardonForm tests\n";
    std::cout << "------------------------------------\n";
    lowgrade.signForm(prespardoner);
    lowgrade.executeForm(prespardoner);
    prespardsigner.signForm(prespardoner);
    lowgrade.executeForm(prespardoner);
    prespardexec.executeForm(prespardoner);



    return 0;
}