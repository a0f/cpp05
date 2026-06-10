#include "Bureaucrat.hpp"
#include "Form.hpp"

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

void runTryCatch_form(Bureaucrat& b, std::string name, int grade_to_sign, int grade_to_excecute)
{

    Bureaucrat robert("Robert", 75);

    try
    {
        std::cout << "Making Form: " << name << std::endl;
        Form f(name, grade_to_sign, grade_to_excecute);
        std::cout << f;
        b.signForm(f);
    }
    catch(Form::GradeTooLowException &e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch(Form::GradeTooHighException &e)
    {
        std::cerr << e.what() << std::endl;
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
    std::cout << "Contructor checks\n";
    std::cout << "------------------------------------\n";
    
    runTryCatch_bur("below_min", 0, false);
    runTryCatch_bur("above_max", 151, false);
    runTryCatch_bur("boundary_min", 1, false);
    runTryCatch_bur("boundary_max", 150, false);

    std::cout << "------------------------------------\n";
    std::cout << "ex01 normal testing\n";
    std::cout << "------------------------------------\n";

    Bureaucrat robert("Robert", 75);
 

    runTryCatch_form(robert, "Strict Form", 74, 100);
    runTryCatch_form(robert, "Easy Form", 100, 100);
    runTryCatch_form(robert, "Same Grade Form", 75, 75);
    runTryCatch_form(robert, "One Less Form", 74, 75);
    runTryCatch_form(robert, "One More Form", 76, 75);

    std::cout << "------------------------------------\n";
    std::cout << "ex01 boundary testing\n";
    std::cout << "------------------------------------\n";

    runTryCatch_form(robert, "Min possible value sign", 1, 75);
    runTryCatch_form(robert, "Max possible value sign", 150, 75);
    runTryCatch_form(robert, "Min possible value sign", 75, 1);
    runTryCatch_form(robert, "Max possible value sign", 75, 150);
    runTryCatch_form(robert, "Invalid form min sign", 0, 75);
    runTryCatch_form(robert, "Invalid form max sign", 151, 75);
    runTryCatch_form(robert, "Invalid form min excecute", 75, 0);
    runTryCatch_form(robert, "Invalid form max excecute", 75, 150);




    return 0;
}