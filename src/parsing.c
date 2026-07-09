#include "codexion.h"

/*
*   compare both the string for scheduler
*   if both string are not equal it will return non zero
 
*/
static int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

/*
*   Contains only digits:
*   Check if a string contains only digis 0 - 9.
*   Returns true if the string only contains digits.
*   Reurns false if the string contains a character that is not a digit.
 
 */

 static bool contains_only_digits(char *str)
 {
    int i;

    i = 0;
    while(str[i])
    {
        if (str[i] <'0' || str[i] > '9')
            return (false);
        i++;

    }
    return (true);
 }

 /* integer_atoi:
*	Converts a digit-only string into a positive integer.
*	Returns the converted number between 0 and INT MAX.
*	Returns -1 if the converted number exceeds INT MAX.
*/

int integer_atoi(char *str)
{
    unsigned long int nb;
    int               i;

    i = 0;
    nb = 0;
    while(str[i] && (str[i] >= '0' && str[i] <= '9'))
    {
        nb = nb * 10 + (str[i] - '0');
        i++;
    }
    if  (nb > INT_MAX)
        return (-1);
    return ((int)nb);
}

/* is_valid_input:
*	Checks if all required arguments are valid, i.e. is a string of
*	digits only, which does not exceed INT MAX. Also checks if the number
*	of philosophers is valid (between 1 and MAX_PHILOS).
*	Returns true if all arguments are valid, false if one of them is invalid.
*/


bool    is_valid_input(int ac, char **av)
{

    int i;
    int parsed_args[8];
    i = 1;
    bool flag;
    flag = false;
    while(i != 8)
    {
        if (!contains_only_digits(av[i]))
            return (print_error(STR_ERR_INPUT_DIGIT, av[i]));
        parsed_args[i - 1] = integer_atoi(av[i]);
        if(i == 1 &&( parsed_args[i - 1] > MAX_CODERS ))
            return (print_error(STR_ERR_INPUT_COFLOW, STR_MAX_CODERS));
        if (parsed_args[i - 1] == -1)
            return (print_error(STR_ERR_INPUT_DIGIT, av[i]));
        i++;   
        
    }
    if (strcmp(FIFO, av[i]) == 0 || strcmp(EDF, av[i]) == 0)
        flag = true;
    return (flag);

    
}
