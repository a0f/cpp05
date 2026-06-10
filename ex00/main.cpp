#include "Bureaucrat.hpp"

void reportMemberVaraibles(const Bureaucrat &b)
{
    std::cout << "Name: " << b.getName() << " Grade: " << b.getGrade() << std::endl;
}

void runTryCatch(std::string name, int grade, bool loop_flag)
{
    try
    {
        std::cout << "Making Bureaucrat: " << name << std::endl;
        Bureaucrat b(name, grade);
        reportMemberVaraibles(b);
        if (loop_flag == true)
        {
            if (b.getGrade() > 75)
            {
                std::cout << "Decrementing Grade" << std::endl;
                while(1)
                {
                    b.decrementGrade();
                    reportMemberVaraibles(b);
                }
            }
            else
            {
                std::cout << "Incrementing Grade" << std::endl;
                while(1)
                {
                    b.incrementGrade();
                    reportMemberVaraibles(b);
                }
            }
        }
    }
    catch(Bureaucrat::GradeTooLowException &e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch(Bureaucrat::GradeTooHighException &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

int main(void)
{
    std::cout << "Increment/Decrement checks\n";
    runTryCatch("increment check", 75, true);
    runTryCatch("decrement check", 76, true);

    std::cout << "------------------------------------\n";
    std::cout << "Contructor checks\n";
    std::cout << "------------------------------------\n";
    
    runTryCatch("below_min", 0, false);
    runTryCatch("above_max", 151, false);
    runTryCatch("boundary_min", 1, false);
    runTryCatch("boundary_max", 150, false);

    return 0;
}