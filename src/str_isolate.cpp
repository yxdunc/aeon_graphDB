#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <whitedb/dbapi.h>
#include <ctype.h>


char    **add_element(char **tab, char *sub_str)
{
        int     old;

        old = 0;
	if (tab)
	{
		while (tab[old] != NULL)
        		old++;
	}
        tab = (char **)realloc(tab, sizeof(char*) * (old + 2));
        tab[old] = sub_str;
        tab[old + 1] = NULL;
        return(tab);
}

char    *get_sub_str(int y, int i, char *str)
{
        int     x;
        char    *sub;

        x = 0;
        sub = (char *)malloc(sizeof(char) * (i + 1 - y));
        while (y < i)
        {
                sub[x] = str[y];
                x++;
                y++;
        }
        sub[x] = '\0';
        return (sub);
}

char    *char_to_str(char a)
{
        char    *a_str;

        a_str = (char *)malloc(sizeof(char) * 2);
        a_str[0] = a;
        a_str[1] = '\0';
        return (a_str);
}

char    **str_isolate(char *str)
{
        int     i;  
        int     y;  
        char    *sub_str;
        char    **tab = NULL;

        if (str == NULL)
                return (NULL);
        i = 0;
        while (str[i] != '\0')
        {
                y = i;
                if (isalpha(str[i]))
                {
                        while (isalpha(str[i]))
                                i++;
                        sub_str = get_sub_str(y, i, str);
                        tab = add_element(tab, sub_str);
                }
		if (str[i] != '\0')
		{
                	tab = add_element(tab, char_to_str(str[i]));
                	i++;
		}
        }
        return (tab);
}
