#include "Bureaucrat.hpp"

void runTryCatch_bur(std::string name, int grade, bool loop_flag)
{
    try
    {
        std::cout << "Making Bureaucrat: " << name << std::endl;
        Bureaucrat b(name, grade);
        std::cout << b;
        if (loop_flag == true)
        {
            if (b.getGrade() > 75)
            {
                std::cout << "Decrementing Grade" << std::endl;
                while(1)
                {
                    b.decrementGrade();
                    std::cout << b;
                }
            }
            else
            {
                std::cout << "Incrementing Grade" << std::endl;
                while(1)
                {
                    b.incrementGrade();
                    std::cout << b;
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
    runTryCatch_bur("increment check", 75, true);
    runTryCatch_bur("decrement check", 76, true);

    std::cout << "------------------------------------\n";
    std::cout << "Constructor checks\n";
    std::cout << "------------------------------------\n";
    
    runTryCatch_bur("below_min", 0, false);
    runTryCatch_bur("above_max", 151, false);
    runTryCatch_bur("boundary_min", 1, false);
    runTryCatch_bur("boundary_max", 150, false);

    return 0;
}